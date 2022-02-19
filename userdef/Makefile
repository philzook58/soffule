
functors.o: functors.cpp
	g++ functors.cpp -c -fPIC -o functors.o 
libfunctors.so: functors.o
	g++ -shared -o libfunctors.so functors.o 
run: libfunctors.so
	souffle test.dl -lfunctors
