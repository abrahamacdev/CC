CXX = g++
CXXFLAGS = -std=c++11 -Wall -O3

EXES = main
OBJS = ${EXES:=.o}

%: %.o
	${CXX} ${LDFLAGS} -o $@ $^

${OBJS}: generator.h grafoMA.h solver.h translator.h types.h

all: ${EXES}

clean:
	${RM} ${EXES} ${OBJS} core *~
