libmonte.so:monte.o
	gcc -shared -fPIC -L ~/gsl/lib -o libmonte.so monte.o -lgsl -lgslcblas -lm
monte.o:monte.c
	gcc -c -fPIC -g -W -I ~/gsl/include -c monte.c
clean:
	rm -rf *.o libmonte.so
