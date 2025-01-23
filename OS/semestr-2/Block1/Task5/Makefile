CC=gcc
RM=rm
CFLAGS= -g -Wall
LIBS=-lpthread
INCLUDE_DIR="."

all: thread_a thread_b

thread_a: thread_a.c
	${CC} ${CFLAGS} -I${INCLUDE_DIR} thread_a.c ${LIBS} -o thread_a

thread_b: thread_b.c
	${CC} ${CFLAGS} -I${INCLUDE_DIR} thread_b.c ${LIBS} -o thread_b

clean:
	${RM} -f *.o ${TARGET}
	${RM} thread_a thread_b
