#include "SudokuPuzzle.h"
#include "Vector2.h"
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <set>
#include <vector>

using namespace std;

SudokuPuzzle::SudokuPuzzle(void)
{
}

SudokuPuzzle::~SudokuPuzzle(void)
{
}

void SudokuPuzzle::Solve(char filenameIn[])
{
	// You will need to read the values of the Sudoku puzzle into your data structure
	std::ifstream myfile(filenameIn);
	char c;
	int count = candidatesConsidered = gridPasses = cellsSolved = 0; // some disgusting maths going on here
	Cell *grid = new Cell[81];
	if (myfile.is_open())
	{
		while (myfile >> c)
		{
			if (c != ' ')
			{
				grid[count].setValue(c - '0');
				if (c != '0')
					grid[count].setPreset(true);
				count++;
			}
		}
		myfile.close();
	}

	LARGE_INTEGER start, end, frequency;
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&start);

	// Solve the puzzle -- find possibles, try values, backtrace. 
#pragma region Solution


#pragma region pencil Marks
	//first we need to assign groups.
	for (int i = 0; i < 9; i++) // row
	{
		for (int j = 0; j < 9; j++)
		{
			m_rows[i].setCell(j, grid[j + (9 * i)]);
			m_columns[j].setCell(i, grid[j + (9 * i)]);
			//switch for assigning blocks.
			switch (i)
			{
			case(0):
			case(1):
			case(2):
				switch (j)
				{
				case(0):
				case(1):
				case(2):
					m_blocks[0].setCell(j + (3 * i), grid[j + (9 * i)]);
					break;
				case(3):
				case(4):
				case(5):
					m_blocks[1].setCell((j - 3) + (3 * i), grid[j + (9 * i)]);
					break;
				case(6):
				case(7):
				case(8):
					m_blocks[2].setCell((j - 6) + (3 * i), grid[j + (9 * i)]);
					break;
				default:
					break;
				}
				break;
			case(3):
			case(4):
			case(5):
				switch (j)
				{
				case(0):
				case(1):
				case(2):
					m_blocks[3].setCell(j + (3 * (i - 3)), grid[j + (9 * i)]);
					break;
				case(3):
				case(4):
				case(5):
					m_blocks[4].setCell((j - 3) + (3 * (i - 3)), grid[j + (9 * i)]);
					break;
				case(6):
				case(7):
				case(8):
					m_blocks[5].setCell((j - 6) + (3 * (i - 3)), grid[j + (9 * i)]);
					break;
				default:
					break;
				}
				break;
			case(6):
			case(7):
			case(8):
				switch (j)
				{
				case(0):
				case(1):
				case(2):
					m_blocks[6].setCell(j + (3 * (i - 6)), grid[j + (9 * i)]);
					break;
				case(3):
				case(4):
				case(5):
					m_blocks[7].setCell((j - 3) + (3 * (i - 6)), grid[j + (9 * i)]);
					break;
				case(6):
				case(7):
				case(8):
					m_blocks[8].setCell((j - 6) + (3 * (i - 6)), grid[j + (9 * i)]);
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}
		}
	}
	gridPasses++;
	int block, col, row;
	for (int i = 0; i < 81; i++)
	{
		block = gridToBlock(i);
		col = gridToCol(i);
		row = gridToRow(i);

		vector<int> possible = vector<int>{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };

		if (!grid[i].isPreset())
		{
			for (int j = 0; j < 9; j++)
			{
				possible.erase(std::remove(possible.begin(), possible.end(), m_rows[row].getCellValue(j)), possible.end());
				possible.erase(std::remove(possible.begin(), possible.end(), m_columns[col].getCellValue(j)), possible.end());
				possible.erase(std::remove(possible.begin(), possible.end(), m_blocks[block].getCellValue(j)), possible.end());
				candidatesConsidered++;
			}
			grid[i].setPossibles(possible);
		}
	}
	gridPasses++;
#pragma endregion

	nakedSingles(grid);

#pragma endregion

	QueryPerformanceCounter(&end);
	float time = (end.QuadPart - start.QuadPart) / (static_cast<float> (frequency.QuadPart));

	Output(grid);  // Output the solved puzzle
	delete[] grid;
	grid = NULL;
}

//giving sketchy results. check again...
void SudokuPuzzle::hiddenPairs()
{
	// the idea is to find pairs of numbers that appear in a house only twice. This means one of each number must be in one of the two cells housing them.

	// first step, we need to increment through each cell and each house.
	Cell *tempcell;
	for (int i = 0; i < 9; i++) //grid/col/block index
	{
		int offset = 0;
		for (int x = 1; x < 10; x++) //values 1-9 to be checked
		{
			for (int y = 2 + offset; y < 10; y++) //second value of the pair. Offset to prevent cases such as 1,1 and 1,2 2,1
			{
				int count = 0;
				int index1, index2;
				for (int j = 0; j < 9; j++) //cell index within the current house must be stpped through last
				{
					if (m_rows[i].getCellValue(j) == 0)
					{
						vector<int> possible = m_rows[i].getCell(j)->getPossibles();
						if (count > 2)
							break;
						if (std::find(possible.begin(), possible.end(), x) != possible.end())
						{
							if (std::find(possible.begin(), possible.end(), y) != possible.end())
							{
								count++;	
								if (count == 1)
									index1 = j;
								else
									index2 = j;
							}
						}
					}
				}
				if (count == 2)
				{
					vector<int> newPos = vector<int>{ x, y };
					tempcell = m_rows[i].getCell(index1);
					tempcell->setPossibles(newPos);
					tempcell = m_rows[i].getCell(index2);
					tempcell->setPossibles(newPos);
				}
				//now the same for columns
				count = 0;
				for (int j = 0; j < 9; j++) //cell index within the column must be stpped through last
				{
					if (m_columns[i].getCellValue(j) == 0)
					{
						vector<int> possible = m_columns[i].getCell(j)->getPossibles();
						if (count > 2)
							break;
						if (std::find(possible.begin(), possible.end(), x) != possible.end())
						{
							if (std::find(possible.begin(), possible.end(), y) != possible.end())
							{
								count++;
								if (count == 1)
									index1 = j;
								else
									index2 = j;
							}
						}
					}
				}
				if (count == 2)
				{
					vector<int> newPos = vector<int>{ x, y };
					tempcell = m_columns[i].getCell(index1);
					tempcell->setPossibles(newPos);
					tempcell = m_columns[i].getCell(index2);
					tempcell->setPossibles(newPos);
				}
				//and finally blocks
				count = 0;
				for (int j = 0; j < 9; j++) //cell index within the column must be stpped through last
				{
					if (m_blocks[i].getCellValue(j) == 0)
					{
						vector<int> possible = m_blocks[i].getCell(j)->getPossibles();
						if (count > 2)
							break;
						if (std::find(possible.begin(), possible.end(), x) != possible.end())
						{
							if (std::find(possible.begin(), possible.end(), y) != possible.end())
							{
								count++;
								if (count == 1)
									index1 = j;
								else
									index2 = j;
							}
						}
					}
				}
				if (count == 2)
				{
					vector<int> newPos = vector<int>{ x, y };
					tempcell = m_blocks[i].getCell(index1);
					tempcell->setPossibles(newPos);
					tempcell = m_blocks[i].getCell(index2);
					tempcell->setPossibles(newPos);
				}
			}
		}
	}
}

//producing some error and some correct!!!!!
void SudokuPuzzle::hiddenSingles()
{
	// we want to check each row/column/block for digits that only appear once in possible. 

	Cell *tempCell;
	vector<int> remaining;
	for (int i = 0; i < 9; i++) // index of each grid/row/col 0-8
	{
		vector<int> possible = vector<int>{ 1, 2, 3, 4, 5, 6, 7, 8, 9 }; // reset possible to a full vector of ints 1-9 
		remaining.clear();
		for (int j = 0; j < 9; j++)// each cell 1-9 within its group, first ROWS
		{
			tempCell = m_rows[i].getCell(j);// find cell at row[i] and index [j]
			if (tempCell->getValue() != 0)
			{
				possible.erase(std::remove(possible.begin(), possible.end(), tempCell->getValue()), possible.end()); // erase remove idiom will look for value of the cell a
			}
			else  // the value is 0? add its index to remaining.
			{
				remaining.push_back(j);
			}
		}
		if (remaining.size() == 1) //we must have a single cell left empty in remaining, by elimination of other vlaues it must be the last value in possible
		{
			m_rows[i].getCell(remaining.front())->setValue(possible.front());
			m_rows[i].getCell(remaining.front())->clearPossible();
			removePossible(i, remaining.front(), getBlockFromRow(i, remaining.front()), possible.front());
			continue;
		}
		else
		{
			for (size_t p = 0; p < possible.size(); p++) // we now have to check each remaining cell for the possible values..
			{
				int pCount = 0;
				int lastIndex = 0;
				for (size_t r = 0; r < remaining.size(); r++) //one for each remaining cell. Check how many times possible[p] appears in this range. 
				{
					if (pCount > 1)
						break;
					tempCell = m_rows[i].getCell(remaining[r]);
					vector<int> tempPossibles = tempCell->getPossibles();
					if (std::find(tempPossibles.begin(), tempPossibles.end(), possible[p]) != tempPossibles.end())
					{
						pCount++;
						lastIndex = remaining[r];
					}
				}
				if (pCount == 1)	// if the value only appears once, set the cell value to equal the remainder.
				{
					m_rows[i].getCell(lastIndex)->setValue(possible[p]);
					//now we need to clear this row/column/block of this value in possibles.
					removePossible(i, lastIndex, getBlockFromRow(i, lastIndex), possible[p]);
					int pp = possible[p];
					possible.erase(std::remove(possible.begin(), possible.end(), pp), possible.end());
					remaining.erase(std::remove(remaining.begin(), remaining.end(), lastIndex), remaining.end());
					p = 0;
				}
			}

		}
		possible = vector<int>{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		remaining.clear();
		for (int j = 0; j < 9; j++)// each cell 0-9 within its group, first ROWS
		{
			tempCell = m_columns[i].getCell(j);
			if (tempCell->getValue() != 0)
			{
				possible.erase(std::remove(possible.begin(), possible.end(), tempCell->getValue()), possible.end());
			}
			else
			{
				remaining.push_back(j);
			}
		}
		if (remaining.size() == 1) //we must have a single value..
		{
			m_columns[i].getCell(remaining.front())->setValue(possible.front());
			m_columns[i].getCell(remaining.front())->clearPossible();
			removePossible(i, remaining.front(), getBlockFromColumn(i, remaining.front()), possible.front());
			continue;
		}
		else
		{
			for (size_t p = 0; p < possible.size(); p++) // we now have to check each remaining cell for the possible values..
			{
				int pCount = 0;
				int lastIndex = 0;
				for (size_t r = 0; r < remaining.size(); r++)//one for each remaining cell.
				{
					if (pCount > 1)
						break;
					tempCell = m_columns[i].getCell(remaining[r]);
					vector<int> tempPossibles = tempCell->getPossibles();
					if (std::find(tempPossibles.begin(), tempPossibles.end(), possible[p]) != tempPossibles.end())
					{
						pCount++;
						lastIndex = remaining[r];
					}
				}
				if (pCount == 1)	// if the value only appears once, set the cell value to equal the remainder.
				{
					m_columns[i].getCell(lastIndex)->setValue(possible[p]);
					//now we need to clear this row/column/block of this value in possibles.
					removePossible(lastIndex, i, getBlockFromColumn(i, lastIndex), possible[p]);
					int pp = possible[p];
					possible.erase(std::remove(possible.begin(), possible.end(), pp), possible.end());
					remaining.erase(std::remove(remaining.begin(), remaining.end(), lastIndex), remaining.end());
					p = 0;
				}

			}

		}
		possible = vector<int>{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		remaining.clear();
		for (int j = 0; j < 9; j++)// each cell 0-9 within its group, first ROWS
		{
			tempCell = m_blocks[i].getCell(j);
			if (tempCell->getValue() != 0)
			{
				possible.erase(std::remove(possible.begin(), possible.end(), tempCell->getValue()), possible.end());
			}
			else
			{
				remaining.push_back(j);
			}
		}
		if (remaining.size() == 1) //we must have a single value..
		{
			m_blocks[i].getCell(remaining.front())->setValue(possible.front());
			m_blocks[i].getCell(remaining.front())->clearPossible();
			removePossible(getRowFromBlock(i, remaining.front()), getColumnFromBlock(i, remaining.front()), i, possible.front());
			continue;
		}
		else
		{
			for (size_t p = 0; p < possible.size(); p++) // we now have to check each remaining cell for the possible values..
			{
				int pCount = 0;
				int lastIndex = 0;
				for (size_t r = 0; r < remaining.size(); r++)//one for each remaining cell.
				{
					if (pCount > 1)
						break;
					tempCell = m_blocks[i].getCell(remaining[r]);
					vector<int> tempPossibles = tempCell->getPossibles();
					if (std::find(tempPossibles.begin(), tempPossibles.end(), possible[p]) != tempPossibles.end())
					{
						pCount++;
						lastIndex = remaining[r];
					}
				}
				if (pCount == 1)	// if the value only appears once, set the cell value to equal the remainder.
				{
					m_blocks[i].getCell(lastIndex)->setValue(possible[p]);
					//now we need to clear this row/column/block of this value in possibles.
					removePossible(getRowFromBlock(i, lastIndex), getColumnFromBlock(i, lastIndex), i, possible[p]);
					int pp = possible[p];
					possible.erase(std::remove(possible.begin(), possible.end(), pp), possible.end());
					remaining.erase(std::remove(remaining.begin(), remaining.end(), lastIndex), remaining.end());
					p = 0;
				}
			}

		}
	}
}

void SudokuPuzzle::nakedSingles(Cell grid[])
{
	for (int i = 0; i < 81; i++)
	{
		if (!grid[i].isPreset())
		{
			if (grid[i].getValue() == 0)
			{
				candidatesConsidered++;
				if (grid[i].getPossibles().size() == 1)
				{
					grid[i].setValue(grid[i].getPossibles().front());
					grid[i].clearPossible();
					removePossible(gridToRow(i), gridToCol(i), gridToBlock(i), grid[i].getValue());
					cellsSolved++;
					gridPasses++;
					i = -1;
				}
			}
		}
	}
}

void SudokuPuzzle::removePossible(const int row, const int col, const int block, const int value)
{
	for (int x = 0; x < 9; x++)
	{
		Cell *tempCell;
		tempCell = m_rows[row].getCell(x);
		tempCell->removePossible(value);
		tempCell = m_columns[col].getCell(x);
		tempCell->removePossible(value);
		tempCell = m_blocks[block].getCell(x);
		tempCell->removePossible(value);
	}
}

int SudokuPuzzle::getRowFromBlock(const int blockValue, const int cellValue) const
{
	switch (blockValue)
	{
	case(0):
	case(1):
	case(2):
		if (cellValue < 3)
			return 0;
		else if (cellValue < 6)
			return 1;
		else if (cellValue < 9)
			return 2;
	case(3):
	case(4):
	case(5):
		if (cellValue < 3)
			return 3;
		else if (cellValue < 6)
			return 4;
		else if (cellValue < 9)
			return 5;
	case(6):
	case(7):
	case(8):
		if (cellValue < 3)
			return 6;
		else if (cellValue < 6)
			return 7;
		else if (cellValue < 9)
			return 8;
	default: return 0;
	}
}

int SudokuPuzzle::getColumnFromBlock(const int blockValue, const int cellValue) const
{
	switch (blockValue)
	{
	case(0):
	case(3):
	case(6):
		switch (cellValue)
		{
		case(0):
		case(3):
		case(6):
			return 0;
		case(1):
		case(4):
		case(7):
			return 1;
		case(2):
		case(5):
		case(8):
			return 2;
		default:
			break;
		}
	case(1):
	case(4):
	case(7):
		switch (cellValue)
		{
		case(0):
		case(3):
		case(6):
			return 3;
		case(1):
		case(4):
		case(7):
			return 4;
		case(2):
		case(5):
		case(8):
			return 5;
		default:
			break;
		}
	case(2):
	case(5):
	case(8):
		switch (cellValue)
		{
		case(0):
		case(3):
		case(6):
			return 6;
		case(1):
		case(4):
		case(7):
			return 7;
		case(2):
		case(5):
		case(8):
			return 8;
		default:
			break;
		}
	default: return 0;
	}
}

int SudokuPuzzle::getBlockFromRow(const int rowValue, const int cellValue) const
{
	switch (rowValue)
	{
	case(0):
	case(1):
	case(2):
		switch (cellValue)
		{
		case(0):
		case(1):
		case(2):
			return 0;
		case(3):
		case(4):
		case(5):
			return 1;
		case(6):
		case(7):
		case(8):
			return 2;
		}
	case(3):
	case(4):
	case(5):
		switch (cellValue)
		{
		case(0):
		case(1):
		case(2):
			return 3;
		case(3):
		case(4):
		case(5):
			return 4;
		case(6):
		case(7):
		case(8):
			return 5;
		}
	case(6):
	case(7):
	case(8):
		switch (cellValue)
		{
		case(0):
		case(1):
		case(2):
			return 6;
		case(3):
		case(4):
		case(5):
			return 7;
		case(6):
		case(7):
		case(8):
			return 8;
		}
	default: return 0;
	}
}

int SudokuPuzzle::getBlockFromColumn(const int colValue, const int cellValue) const
{
	switch (colValue)
	{
	case(0):
	case(1):
	case(2):
		switch (cellValue)
		{
		case(0):
		case(1):
		case(2):
			return 0;
		case(3):
		case(4):
		case(5):
			return 3;
		case(6):
		case(7):
		case(8):
			return 6;
		}
	case(3):
	case(4):
	case(5):
		switch (cellValue)
		{
		case(0):
		case(1):
		case(2):
			return 1;
		case(3):
		case(4):
		case(5):
			return 4;
		case(6):
		case(7):
		case(8):
			return 7;
		}
	case(6):
	case(7):
	case(8):
		switch (cellValue)
		{
		case(0):
		case(1):
		case(2):
			return 2;
		case(3):
		case(4):
		case(5):
			return 5;
		case(6):
		case(7):
		case(8):
			return 8;
		}
	default: return 0;
	}
}

int SudokuPuzzle::gridToRow(const int gridValue) const
{
	std::div_t divResult;
	divResult = div(gridValue, 9);
	return divResult.quot;
}

int SudokuPuzzle::gridToCol(const int gridValue) const
{
	std::div_t divResult;
	divResult = div(gridValue, 9);
	int col = divResult.rem;
	return col;
}

int SudokuPuzzle::gridToBlock(const int gridValue) const
{
	std::div_t divResult;
	divResult = div(gridValue, 9);
	int row, col, block = 0;
	row = divResult.quot;
	col = divResult.rem;
	switch (row)
	{
	case(0):
	case(1):
	case(2):
		switch (col)
		{
		case(0):
		case(1):
		case(2):
			block = 0;
			break;
		case(3):
		case(4):
		case(5):
			block = 1;
			break;
		case(6):
		case(7):
		case(8):
			block = 2;
			break;
		default:
			break;
		}
		break;
	case(3):
	case(4):
	case(5):
		switch (col)
		{
		case(0):
		case(1):
		case(2):
			block = 3;
			break;
		case(3):
		case(4):
		case(5):
			block = 4;
			break;
		case(6):
		case(7):
		case(8):
			block = 5;
			break;
		default:
			break;
		}
		break;
	case(6):
	case(7):
	case(8):
		switch (col)
		{
		case(0):
		case(1):
		case(2):
			block = 6;
			break;
		case(3):
		case(4):
		case(5):
			block = 7;
			break;
		case(6):
		case(7):
		case(8):
			block = 8;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	return block;
}

// This is an example of how you may output the solved puzzle
void SudokuPuzzle::Output(Cell grid[]) const
{
	ofstream fout("sudoku_solution.txt"); // DO NOT CHANGE THE NAME OF THIS FILE
	if (fout.is_open())
	{
		for (int y = 0; y < 9; ++y)
		{
			for (int x = 0; x < 9; ++x)
			{
				int v = grid[x + (9 * y)].getValue();
				fout << v << ' ';
			}

			fout << endl;
		}
		fout.close();
	}
}