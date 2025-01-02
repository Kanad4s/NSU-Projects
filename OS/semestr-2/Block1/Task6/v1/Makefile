CC=gcc
RM=rm
CFLAGS= -g
LIBS=-lpthread
INCLUDE_DIR="."

all: myThreads

myThreads: myThreads.c
	${CC} ${CFLAGS} -I${INCLUDE_DIR} myThreads.c ${LIBS} -o myThreads

clean:
	${RM} -f *.o ${TARGET}
	${RM} myThreads