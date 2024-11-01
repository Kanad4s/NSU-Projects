CC=gcc
RM=rm
CFLAGS= -g -Wall
LIBS=-lpthread
INCLUDE_DIR="."

all: thread_a thread_b thread_c

thread_a: thread_a.c
	${CC} ${CFLAGS} -I${INCLUDE_DIR} thread_a.c ${LIBS} -o thread_a

thread_b: thread_b.c
	${CC} ${CFLAGS} -I${INCLUDE_DIR} thread_b.c ${LIBS} -o thread_b
	
thread_c: thread_c.c
	${CC} ${CFLAGS} -I${INCLUDE_DIR} thread_c.c ${LIBS} -o thread_c

clean:
	${RM} -f *.o ${TARGET}
	${RM} thread_b
	${RM} thread_a
	${RM} thread_c
