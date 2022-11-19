CPP=g++
LD_FLAGS=-lglfw3 -lm -lGL -Lglad -lglad -static-libgcc -static-libstdc++
CPP_FLAGS=-O2 -Iglad/include -Iglm
SRC=$(wildcard *.cpp)
OBJ=$(SRC:%.cpp=%.o)
BIN_NAME=surface-of-revolution

output: ${OBJ} glad/libglad.a
	${CPP} ${OBJ} ${LD_FLAGS} ${CPP_FLAGS} -o ${BIN_NAME}

%.o: %.cpp
	${CPP} -c $^ -o $@

glad/libglad.a:
	cd glad && cmake . && make

run:
	./${BIN_NAME}

clean:
	rm -f ${OBJ} ${BIN_NAME}

clean_all:
	rm -f ${OBJ} ${BIN_NAME}
	rm -f glad/libglad.a
