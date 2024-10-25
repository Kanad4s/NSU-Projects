CC=gcc
RM=rm
CFLAGS= -g -Wall
LIBS=-lpthread
INCLUDE_DIR="."

all: thread_e thread_b thread_c thread_d thread_f

thread_e: thread_e.c
	${CC} ${CFLAGS} -I${INCLUDE_DIR} thread_e.c ${LIBS} -o thread_e

thread_b: thread_b.c
	${CC} ${CFLAGS} -I${INCLUDE_DIR} thread_b.c ${LIBS} -o thread_b

thread_c: thread_c.c
	${CC} ${CFLAGS} -I${INCLUDE_DIR} thread_c.c ${LIBS} -o thread_c

thread_d: thread_d.c
	${CC} ${CFLAGS} -I${INCLUDE_DIR} thread_d.c ${LIBS} -o thread_d
	
thread_f: thread_f.c
	${CC} ${CFLAGS} -I${INCLUDE_DIR} thread_f.c ${LIBS} -o thread_f

clean:
	${RM} -f *.o ${TARGET}
	${RM} thread_b
	${RM} thread_c
	${RM} thread_d
	${RM} thread_e
	${RM} thread_f