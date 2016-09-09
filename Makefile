CXX = g++
CPP_FLAGS = -fpermissive -g
CPP_SOURCES = generated/mips8asm_grammar.cpp $(wildcard *.cpp)
HEADERS = $(wildcard *.h) 
OBJ = ${CPP_SOURCES:.cpp=.o}
INCLUDE = .
LIBS = 
TARGET = mips8asm

all: ${TARGET}

${TARGET}: ${OBJ}
	${CXX} -o $@ $^ ${LIBS}

%.o: %.cpp
	${CXX} -c -I ${INCLUDE} ${CPP_FLAGS} -o $@ $<

generated/mips8asm_grammar.cpp: mips8asm_grammar.y
	lemon -Tlemon/lempar.c $<
	mkdir -p generated
	mv mips8asm_grammar.c $@

clean:
	rm -f *.o
	rm -f *.out
	rm -f mips8asm_grammar.h
	rm -fr generated/