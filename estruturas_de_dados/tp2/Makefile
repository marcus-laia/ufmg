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

test: $(EXE)
	$(EXE) -i input/1.txt -m 3 -s 4 -o output.txt
	./compare 1
	$(EXE) -i input/2.txt -m 3 -s 4 -o output.txt
	./compare 2
	$(EXE) -i input/3.txt -m 3 -s 4 -o output.txt
	./compare 3
	$(EXE) -i input/4.txt -m 3 -s 4 -o output.txt
	./compare 4
	$(EXE) -i input/5.txt -m 3 -s 4 -o output.txt
	./compare 5
	$(EXE) -i input/6.txt -m 3 -s 4 -o output.txt
	./compare 6
	$(EXE) -i input/7.txt -m 3 -s 4 -o output.txt
	./compare 7
	$(EXE) -i input/8.txt -m 3 -s 4 -o output.txt
	./compare 8
	$(EXE) -i input/9.txt -m 3 -s 4 -o output.txt
	./compare 9
	$(EXE) -i input/10.txt -m 3 -s 4 -o output.txt
	./compare 10

clean:
	@rm -rf $(OBJ_FOLDER)* 

# $(BIN_FOLDER)*
