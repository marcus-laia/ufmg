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
	$(EXE) -i ./tests/input/e1.txt -m 3 -s 4 -o ./tests/output/out_e1.txt
	./tests/check/compare e1
	$(EXE) -i ./tests/input/e2.txt -m 3 -s 4 -o ./tests/output/out_e2.txt
	./tests/check/compare e2
	$(EXE) -i ./tests/input/e3.txt -m 3 -s 4 -o ./tests/output/out_e3.txt
	./tests/check/compare e3
	$(EXE) -i ./tests/input/e4.txt -m 3 -s 4 -o ./tests/output/out_e4.txt
	./tests/check/compare e4
	$(EXE) -i ./tests/input/e5.txt -m 3 -s 4 -o ./tests/output/out_e5.txt
	./tests/check/compare e5
	$(EXE) -i ./tests/input/e6.txt -m 3 -s 4 -o ./tests/output/out_e6.txt
	./tests/check/compare e6
	$(EXE) -i ./tests/input/e7.txt -m 3 -s 4 -o ./tests/output/out_e7.txt
	./tests/check/compare e7
	$(EXE) -i ./tests/input/e8.txt -m 3 -s 4 -o ./tests/output/out_e8.txt
	./tests/check/compare e8
	$(EXE) -i ./tests/input/e9.txt -m 3 -s 4 -o ./tests/output/out_e9.txt
	./tests/check/compare e9
	$(EXE) -i ./tests/input/e10.txt -m 3 -s 4 -o ./tests/output/out_e10.txt
	./tests/check/compare e10
	$(EXE) -i ./tests/input/t1.txt -m 3 -s 4 -o ./tests/output/out_t1.txt
	./tests/check/compare t1
	$(EXE) -i ./tests/input/t2.txt -m 3 -s 4 -o ./tests/output/out_t2.txt
	./tests/check/compare t2
	$(EXE) -i ./tests/input/t3.txt -m 3 -s 4 -o ./tests/output/out_t3.txt
	./tests/check/compare t3
	$(EXE) -i ./tests/input/t4.txt -m 3 -s 4 -o ./tests/output/out_t4.txt
	./tests/check/compare t4
	$(EXE) -i ./tests/input/t5.txt -m 3 -s 4 -o ./tests/output/out_t5.txt
	./tests/check/compare t5
	$(EXE) -i ./tests/input/t6.txt -m 3 -s 4 -o ./tests/output/out_t6.txt
	./tests/check/compare t6
	$(EXE) -i ./tests/input/t7.txt -m 3 -s 4 -o ./tests/output/out_t7.txt
	./tests/check/compare t7
	$(EXE) -i ./tests/input/t8.txt -m 3 -s 4 -o ./tests/output/out_t8.txt
	./tests/check/compare t8

clean:
	@rm -rf $(OBJ_FOLDER)* 

