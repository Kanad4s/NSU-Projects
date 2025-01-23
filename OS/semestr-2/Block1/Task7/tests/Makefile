all:
	gcc -c -fPIC uthread.c -o uthread.o
	gcc -shared -o libuthread.so uthread.o
	gcc main.c -L. -luthread -Wl,-rpath,. -o main
clean: 
	rm libuthread.so main uthread.o