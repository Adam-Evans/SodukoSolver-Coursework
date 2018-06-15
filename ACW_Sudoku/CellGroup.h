#pragma once

#include <vector>
#include "Cell.h"
using namespace std;

class CellGroup
{
public:
	CellGroup();
	~CellGroup();

	void setCell(const int index, Cell &cell);
	Cell* getCell(const int index) const;
	int getCellValue(const int index) const;
	void setCellValue(const int index, const int value);
	void setCellValue(Cell &cell, const int value);

private: 
	Cell *m_cells[9];
};

