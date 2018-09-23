with import <nixpkgs> {};

stdenv.mkDerivation {
  name = "motherboard-util";
  src = ./.;
  buildInputs = [ libusb ];
  installPhase = ''
    mkdir $out/bin -pv
    cp main $out/bin/
  '';
}
