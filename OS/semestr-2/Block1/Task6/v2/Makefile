all:
	gcc -c -fPIC mythread.c -o mythread.o
	gcc -shared -o libmythread.so mythread.o
	gcc test_main.c -L. -lmythread -Wl,-rpath,. -o main