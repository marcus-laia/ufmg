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
	$(EXE) -i input/1.txt -m 3 -s 4 -o out/output_1.txt
	./compare 1
	$(EXE) -i input/2.txt -m 3 -s 4 -o out/output_2.txt
	./compare 2
	$(EXE) -i input/3.txt -m 3 -s 4 -o out/output_3.txt
	./compare 3
	$(EXE) -i input/4.txt -m 3 -s 4 -o out/output_4.txt
	./compare 4
	$(EXE) -i input/5.txt -m 3 -s 4 -o out/output_5.txt
	./compare 5
	$(EXE) -i input/6.txt -m 3 -s 4 -o out/output_6.txt
	./compare 6
	$(EXE) -i input/7.txt -m 3 -s 4 -o out/output_7.txt
	./compare 7
	$(EXE) -i input/8.txt -m 3 -s 4 -o out/output_8.txt
	./compare 8
	$(EXE) -i input/9.txt -m 3 -s 4 -o out/output_9.txt
	./compare 9
	$(EXE) -i input/10.txt -m 3 -s 4 -o out/output_10.txt
	./compare 10

cases: $(EXE)
	$(EXE) -i test_cases_tp2/e1.txt -m 3 -s 4 -o out/output_1.txt
	./compare 1
	$(EXE) -i test_cases_tp2/e2.txt -m 3 -s 4 -o out/output_2.txt
	./compare 2
	$(EXE) -i test_cases_tp2/e3.txt -m 3 -s 4 -o out/output_3.txt
	./compare 3
	$(EXE) -i test_cases_tp2/e4.txt -m 3 -s 4 -o out/output_4.txt
	./compare 4
	$(EXE) -i test_cases_tp2/e5.txt -m 3 -s 4 -o out/output_5.txt
	./compare 5
	$(EXE) -i test_cases_tp2/e6.txt -m 3 -s 4 -o out/output_6.txt
	./compare 6
	$(EXE) -i test_cases_tp2/e7.txt -m 3 -s 4 -o out/output_7.txt
	./compare 7
	$(EXE) -i test_cases_tp2/e8.txt -m 3 -s 4 -o out/output_8.txt
	./compare 8

clean:
	@rm -rf $(OBJ_FOLDER)* 

# $(BIN_FOLDER)*
