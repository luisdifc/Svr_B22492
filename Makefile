all: build run clean

build:
	g++ -std=c++11 -O3 -o main *.cpp
	
run:
	./main

clean:
	rm main
