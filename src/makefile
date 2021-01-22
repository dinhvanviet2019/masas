CC := g++
LDFLASG := -DINFO
SRC_DIR := ./impl
INC_DIR := ./includes
OBJ_DIR := ./build
BIN_DIR := ./build

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
BINS := $(BIN_DIR)/main

.PHONY: build compile clean test

all: clean build compile
build:
	echo $(SRCS)
	mkdir -p ${OBJ_DIR}
	mkdir -p ${BIN_DIR}

compile: $(BINS)
test: $(BINS)
	reset
	$^


$(BINS): $(SRCS)
	$(CC) -o $@ $^ -I $(INC_DIR) $(LDFLASG)

clean:
	rm -rf ${OBJ_DIR}
	rm -rf ${BIN_DIR}



