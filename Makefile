# GNU Makefile for Animal War game
#
# @author: Tanasescu Darius, Ivan Corina
# @desc: build rules for cpp targets including flags for compiling and linking
#

FILES = main.cpp
BIN_NAME = game

CPP_FLAGS = -w \
						-lSDL2

clean:
	rm -rf build

all:
	mkdir build
	g++ $(CPP_FLAGS) ./$(FILES) -o ./build/$(BIN_NAME)
	chmod +x ./build/$(BIN_NAME)
	./build/$(BIN_NAME)

