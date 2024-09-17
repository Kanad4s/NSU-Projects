TARGET = ces-prog
SRCS = solver.cpp main.cpp inputParser.cpp outputParser.cpp
PREFIX ?= /home/kanades/NSU-Projects-Cplusplus/CM/CubicEquationSolution
SRC = src
OBJS = $(SRCS:.c=.o)

CXX=g++
RM=rm

.PHONY: all clean

all: $(TAGRET)
$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) (CXXFLAGS) 

.c.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) -rf $(TARGET) $(OBJS)

