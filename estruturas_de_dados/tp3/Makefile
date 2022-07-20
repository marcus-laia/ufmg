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
	$(EXE) -i input_tp3/entrada_1.txt -o out/output_1.txt
	./compare 1
	$(EXE) -i input_tp3/entrada_2.txt -o out/output_2.txt
	./compare 2
	$(EXE) -i input_tp3/entrada_3.txt -o out/output_3.txt
	./compare 3
	$(EXE) -i input_tp3/entrada_4.txt -o out/output_4.txt
	./compare 4
	$(EXE) -i input_tp3/entrada_test.txt -o out/output_test.txt
	./compare test

clean:
	@rm -rf $(OBJ_FOLDER)* 

# $(BIN_FOLDER)*
