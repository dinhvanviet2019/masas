CC := g++
SRC_DIR := ./impl
INC_DIR := ./includes
OBJ_DIR := ./build
BIN_DIR := ./build

SRCS := $(SRC_DIR)/Graph.cpp $(SRC_DIR)/SmallGraph.cpp $(SRC_DIR)/MassiveGraph.cpp $(SRC_DIR)/GraphFactory.cpp $(SRC_DIR)/main.cpp
BINS := $(BIN_DIR)/main

.PHONY: build compile clean test

all: clean build compile
build:
	mkdir -p ${OBJ_DIR}
	mkdir -p ${BIN_DIR}

compile: $(BINS)
test: $(BINS)
	$^


$(BINS): $(SRCS)
	$(CC) -o $@ $^ -I $(INC_DIR)

clean:
	rm -rf ${OBJ_DIR}
	rm -rf ${BIN_DIR}



