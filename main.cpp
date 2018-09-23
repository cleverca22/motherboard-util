#include <usb.h>
#include <stdio.h>
#include <stdint.h>

void getValue(struct usb_dev_handle *hmouse, int value);
void readThings(struct usb_dev_handle *hmouse);
void readThing(struct usb_dev_handle *hmouse, char *data);

int main(int argc, char **argv) {
  int ret;
  char data[500];
  struct usb_bus *bus;
  struct usb_device *mouse=NULL;
  struct usb_dev_handle *hmouse;

  usb_set_debug(4);
  usb_init();
  ret = usb_find_busses();
  if (ret < 0) {
    printf("fail1\n");
    return ret;
  }

  ret = usb_find_devices();
  if (ret < 0) {
    printf("fail2\n");
    return ret;
  }

  for (bus = usb_get_busses(); bus; bus = bus->next) {
    struct usb_device *dev;
    for(dev = bus->devices; dev; dev = dev->next) {
      if(dev->descriptor.idVendor == 0x1770 && dev->descriptor.idProduct == 0xff00) {
        mouse = dev;
      }
    }
  }

  if (!mouse) {
    printf("failed to find usb device\n");
    return -1;
  }
  
  hmouse = usb_open(mouse);
  if (!hmouse) {
    printf("unable to open usb device\n");
    return 2;
  }

  printf("Mouse opened: %s\n", mouse->filename);

  ret = usb_detach_kernel_driver_np(hmouse, 0);
  if (ret < 0) {
    printf("fail3\n");
  }

  if(usb_claim_interface(hmouse, 0) < 0) {
    printf("Could not claim interface\n");
    usb_close(hmouse); 
    return 3;
  }


  //getValue(hmouse, 0x0302);

  readThings(hmouse);

  usb_release_interface(hmouse, 0);
  usb_close(hmouse);

  return 0;
}

void readThings(struct usb_dev_handle *hmouse) {
  char data[8];
  data[0] = 0x01;
  data[1] = 0x40;
  data[2] = 0;
  data[3] = 0;
  data[4] = 0;
  data[5] = 0;
  data[6] = 0;
  data[7] = 0;
  readThing(hmouse, data);
  data[3] = 1;
  readThing(hmouse, data);
  data[3] = 2;
  readThing(hmouse, data);
  data[3] = 3;
  readThing(hmouse, data);
  data[3] = 4;
  readThing(hmouse, data);
  data[3] = 5;
  readThing(hmouse, data);
  data[3] = 6;
  readThing(hmouse, data);
}

void readThing(struct usb_dev_handle *hmouse, char *data) {
#define SET_REPORT 0x09
#define SET_REPORT_VALUE 0x0301
  int ret;
  char dataout[257];
  usb_control_msg(hmouse, 0x21, SET_REPORT, SET_REPORT_VALUE, 0, data, 8, 0);

  ret = usb_control_msg(hmouse, 0xa1, 0x01, 0x0302, 0, dataout, 257, 0);
    for (int i=0; i<ret; i++) {
      if ((dataout[i] == '\r') || (dataout[i] == 0)) {}
      else if ((dataout[i] >= 0x20 && dataout[i] <= 0x7e) || (dataout[i] == '\n')) printf("%c", dataout[i]);
      else printf("_");
    }
}

void getValue(struct usb_dev_handle *hmouse, int value) {
  int ret;
  char data[500];
    ret = usb_control_msg(hmouse, 0xa1, 0x01, 0x0302, 0, data, 257, 0);
    printf("%i: ", ret);
    for(int i=0; i<ret; i++) {
      printf("%02x ", (unsigned char)(data[i]));
    }
    printf("\n");
    for (int i=0; i<ret; i++) {
      if ((data[i] >= 0x20 && data[i] <= 0x7e) || (data[i] == '\n')) printf("%c", data[i]);
      else printf("_");
    }
    printf("\n");
}
