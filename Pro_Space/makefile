Pro_Space : main.o BasicClass.o BCHCode.o BCHCode_XOR.o Hamming_XOR.o
	g++ -o2 main.o BasicClass.o BCHCode.o BCHCode_XOR.o Hamming_XOR.o -o Pro_Space
main.o: BasicClass.h BCHCode_XOR.h BCHCode.h Hamming_XOR.h main.h
	g++ -o2 -c -std=c++11 main.cpp -o main.o
BasicClass.o:BasicClass.h
	g++ -o2 -mavx -c -std=c++11 BasicClass.cpp -o BasicClass.o
BCHCode.o:main.h BCHCode.h
	g++ -o2 -c -std=c++11 BCHCode.cpp -o BCHCode.o
BCHCode_XOR.o:main.h BCHCode_XOR.h
	g++ -o2 -mavx -c -std=c++11 BCHCode_XOR.cpp -o BCHCode_XOR.o
Hamming_XOR.o:main.h Hamming_XOR.h
	g++ -o2 -c -std=c++11 Hamming_XOR.cpp -o Hamming_XOR.o
clean:
	rm -rf *.o Pro_Space