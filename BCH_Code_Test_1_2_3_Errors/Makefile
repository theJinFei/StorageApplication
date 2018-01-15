bchcode : BCHCode.o main.o
	g++ BCHCode.o main.o -o bchcode
BCHCode.o:BCHCode.cpp BCHCode.h
	g++ -c -std=c++11 BCHCode.cpp -o BCHCode.o
main.o:main.cpp
	g++ -c -std=c++11 main.cpp -o main.o
clean:
	rm -rf *.o bchcode