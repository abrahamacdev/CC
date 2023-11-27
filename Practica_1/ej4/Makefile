CXX = g++
CXXFLAGS = -std=c++11 -Wall -O3

EXES = exercise_2
OBJS = ${EXES:=.o}

%: %.o
	${CXX} ${LDFLAGS} -o $@ $^

all: ${EXES}

${OBJS}: ram.h

clean:
	${RM} ${EXES} ${OBJS} core *~
