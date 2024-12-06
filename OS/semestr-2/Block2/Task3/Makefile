TARGET_1 = rwlock
SRCS_1 = rwlock.c 

TARGET_2 = spinlock
SRCS_2 = spinlock.c

TARGET_3 = mutex
SRCS_3 = mutex.c

CC=gcc
RM=rm
CFLAGS= -g -Wall
LIBS=-lpthread
INCLUDE_DIR="."

all: ${TARGET_1} ${TARGET_2} ${TARGET_3}

${TARGET_1}: ${SRCS_1}
	${CC} ${CFLAGS} -I${INCLUDE_DIR} ${SRCS_1} ${LIBS} -o ${TARGET_1}

${TARGET_2}: ${SRCS_2}
	${CC} ${CFLAGS} -I${INCLUDE_DIR} ${SRCS_2} ${LIBS} -o ${TARGET_2}
	
${TARGET_3}: ${SRCS_3}
	${CC} ${CFLAGS} -I${INCLUDE_DIR} ${SRCS_3} ${LIBS} -o ${TARGET_3}

clean:
	${RM} -f *.o ${TARGET_1} ${TARGET_2} ${TARGET_3}
