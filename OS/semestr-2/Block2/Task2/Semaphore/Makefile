TARGET_1 = queue-semaphore-thread
SRCS_1 = queue-semaphore-thread.c queue-semaphore.c

CC=gcc
RM=rm
CFLAGS= -g -Wall
LIBS=-lpthread
INCLUDE_DIR="."

all: ${TARGET_1} 

${TARGET_1}: queue-semaphore.h ${SRCS_1}
	${CC} ${CFLAGS} -I${INCLUDE_DIR} ${SRCS_1} ${LIBS} -o ${TARGET_1}

clean:
	${RM} -f *.o ${TARGET_1} 
