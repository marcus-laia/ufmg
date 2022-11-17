# cc and flags
CC = g++
CXXFLAGS = -std=c++11 -g -Wall
# CXXFLAGS = -std=c++11 -O3 -Wall

# folders
INCLUDE_FOLDER = ./include/
BIN_FOLDER = ./bin/
OBJ_FOLDER = ./obj/
SRC_FOLDER = ./src/

# all sources, objs, and header files
TARGET = main
SRC = $(wildcard $(SRC_FOLDER)*.cpp)
OBJ = $(patsubst $(SRC_FOLDER)%.cpp, $(OBJ_FOLDER)%.o, $(SRC))

$(OBJ_FOLDER)%.o: $(SRC_FOLDER)%.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@ -I$(INCLUDE_FOLDER)

EXE = $(BIN_FOLDER)/main

all:  build clean

build: $(OBJ)
	$(CC) $(CXXFLAGS) -o $(BIN_FOLDER)$(TARGET) $(OBJ)
	$(CC) $(CXXFLAGS) ./tests/check/compareFiles.cpp -o ./tests/check/compare

test: $(EXE)
	$(EXE) -i ./tests/input/input_1.txt -o tests/output/output_1.txt
	./tests/check/compare 1
	$(EXE) -i ./tests/input/input_2.txt -o tests/output/output_2.txt
	./tests/check/compare 2
	$(EXE) -i ./tests/input/input_3.txt -o tests/output/output_3.txt
	./tests/check/compare 3
	$(EXE) -i ./tests/input/input_4.txt -o tests/output/output_4.txt
	./tests/check/compare 4
	$(EXE) -i ./tests/input/input_test.txt -o tests/output/output_test.txt
	./tests/check/compare test

clean:
	@rm -rf $(OBJ_FOLDER)* 

# $(BIN_FOLDER)*
