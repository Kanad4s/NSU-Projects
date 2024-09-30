TARGET = netDiscover
SRCS = netDiscover.c inputParser.c
PREFIX ?= /home/kanades/OS/semestr-2/netDiscover
SRC_DIR = src
SRC_SUBDIRS = .
INCLD_DIR = include
RLTV_SRC_DIRS = $(addprefix ./$(SRC_DIR)/, $(SRC_SUBDIRS))
RLTV_INCLD_DIRS = $(addprefix ./, $(INCLD_DIR))
OBJS_DIRS = $(addprefix ./$(SRC_DIR)/, $(SRC_SUBDIRS))
RLTV_OBJS = $(patsubst ./%, %, $(wildcard $(addsuffix /*.c*, $(RLTV_SRC_DIRS))))
OBJS = $(SRCS:.c=.o)

CC=gcc
RM=rm

.PHONY: all clean print

all: $(TARGET)

$(TARGET): obj_dirs $(RLTV_OBJS)
	$(CC) -o $@ $(RLTV_OBJS)

obj_dirs:
	mkdir -p $(OBJS_DIRS)

.c.o:
	$(CC) -c $< -o $@
	@echo 'useless $(CC) -o $@ $(addprefix $(OBJS_DIRS), $(OBJS))'

clean:
	$(RM) -rf $(TARGET) $(addprefix $(OBJS_DIRS)/, $(OBJS))

print:
	@echo 'RLTV_SRC_DIRS: "$(RLTV_SRC_DIRS)"'
	@echo 'OBJS: "$(RLTV_OBJS)"'
	@echo 'OBJS_DIRS: $(OBJS_DIRS)'
	@echo 'TARGET: $(TARGET)'
	@echo '.c.o: $(OBJS)'
	@echo 'include dirs: $(RLTV_INCLD_DIRS)'
