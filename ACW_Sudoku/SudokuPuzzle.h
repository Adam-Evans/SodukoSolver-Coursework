#pragma once

#include <iostream>
#include <fstream>
#include "Cell.h"
#include "CellGroup.h"
#include <algorithm>

using namespace std;

class SudokuPuzzle
{
public:
	SudokuPuzzle(void);
	~SudokuPuzzle(void);

	void Solve(char filenameIn[]);
	int blockStartColumn(const int blockValue) const;
	int blockStartRow(const int blockValue) const;
	int getRowFromBlock(const int blockValue, const int cellValue) const;
	int getColumnFromBlock(const int blockValue, const int cellValue) const;
	int getBlockFromRow(const int rowValue, const int cellValue) const;
	int getBlockFromColumn(const int colValue, const int cellValue) const;
	int gridToRow(const int gridValue) const;
	int gridToCol(const int gridValue) const;
	int gridToBlock(const int gridValue) const;


	void checkSingleMissing();
	void eliminateDigits();
	void hiddenPairs();
	void hiddenSingles();
	void nakedSingles(Cell grid[]);
	void removePossible(const int row, const int col, const int block, const int value);

	int findTotalPossible() const; // for testing 

private:
	void Output(Cell grid[]) const;
	void assignGroups(Cell grid[]);
	CellGroup m_rows[9];
	CellGroup m_columns[9];
	CellGroup m_blocks[9];

	int cellsSolved;
	int gridPasses;
	int candidatesConsidered;
};

