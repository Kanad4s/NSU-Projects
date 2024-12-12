TARGET_1 = counter-myspinlock
SRCS_1 = counter-myspinlock.c mySpinlock.c

CC=gcc
RM=rm
CFLAGS=

all: ${TARGET_1} 

${TARGET_1}: ${SRCS_1}
	${CC} ${CFLAGS} ${SRCS_1} -o ${TARGET_1}

clean:
	${RM} -f *.o ${TARGET_1} 
