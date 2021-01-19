CC := g++
INCLUDE_DIR := ./includes
SRC_DIR := ./impl
OBJ_DIR := ./build/obj
BIN_DIR := ./build/bin
SRC_FILES := $(wildcard ${INCLUDE_DIR}/*.cpp)
OBJ_FILES := ${OBJ_DIR}/$(patsubst ${SRC_DIR}/%.cpp,${OBJ_DIR}/%.o,${SRC_FILES})
CCFLAGS := 
LDFLAGS :=
ICFLAGS := -I $(INCLUDE_DIR)
TARGET := ${BIN_DIR}/masas

build:
	mkdir -p ${OBJ_DIR}
	mkdir -p ${BIN_DIR}

$(OBJ_FILES): 

${TARGET}: $(OBJ_FILES)
	CC $(LDFLAGS) -o $@ $^ $(CCFLAGS) $(ICFLAGS)
	chmod +x $@
