main: main.o
	g++ -o $@ $^ -lusb
main.o: main.cpp
	g++ -c -o $@ $^
