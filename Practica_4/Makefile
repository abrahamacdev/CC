CXX = g++
CXXFLAGS = -std=c++11 -Wall -O3

EXES = main
OBJS = ${EXES:=.o}

%: %.o
	${CXX} ${LDFLAGS} -o $@ $^

${OBJS}: GrafoP.hpp translator.hpp

all: ${EXES}

clean:
	${RM} ${EXES} ${OBJS} core *~
