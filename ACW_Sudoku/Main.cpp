using namespace std;
#include "SudokuPuzzle.h"
#include "Cell.h"
#include "CellGroup.h"

// This is a basic starting point for you to use to implement your Sudoku solver
int main(int argc, char **) {
	SudokuPuzzle puzzle;
	puzzle.Solve("sudoku_puzzle.txt"); 

	system("pause");

	return 0;
}
