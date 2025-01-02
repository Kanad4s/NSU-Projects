CC=gcc
RM=rm
CFLAGS= -g
LIBS=-lpthread
INCLUDE_DIR="."

all: uthread

myThreads: uthread.c
	${CC} ${CFLAGS} -I${INCLUDE_DIR} uthread.c ${LIBS} -o uthread

clean:
	${RM} -f *.o ${TARGET}
	${RM} uthread