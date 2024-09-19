TARGET = ces-prog
SRCS = solver.cpp main.cpp inputParser.cpp outputParser.cpp
PREFIX ?= /home/kanades/NSU-Projects-Cplusplus/CM/CubicEquationSolution
SRC_DIR = src
SRC_SUBDIRS = .
INCLD_DIR = include
RLTV_SRC_DIRS = $(addprefix ./$(SRC_DIR)/, $(SRC_SUBDIRS))
RLTV_INCLD_DIRS = $(addprefix ./, $(INCLD_DIR))
OBJS_DIRS = $(addprefix ./$(SRC_DIR)/, $(SRC_SUBDIRS))
RLTV_OBJS = $(patsubst ./%, %, $(wildcard $(addsuffix /*.cpp*, $(RLTV_SRC_DIRS))))
OBJS = $(SRCS:.cpp=.o)


CXX=g++
RM=rm

.PHONY: all clean print

all: $(TARGET)

$(TARGET): obj_dirs $(RLTV_OBJS)
	$(CXX) -o $@ $(RLTV_OBJS)

obj_dirs:
	mkdir -p $(OBJS_DIRS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo 'useless $(CXX) -o $@ $(addprefix $(OBJS_DIRS), $(OBJS)) $(CXXFLAGS)'

clean:
	$(RM) -rf $(TARGET) $(addprefix $(OBJS_DIRS)/, $(OBJS))

print:
	@echo 'RLTV_SRC_DIRS: "$(RLTV_SRC_DIRS)"'
	@echo 'OBJS: "$(RLTV_OBJS)"'
	@echo 'OBJS_DIRS: $(OBJS_DIRS)'
	@echo 'TARGET: $(TARGET)'
	@echo '.c.o: $(OBJS)'
	@echo 'include dirs: $(RLTV_INCLD_DIRS)'
