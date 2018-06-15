#include "CellGroup.h"
#include "Cell.h"


CellGroup::CellGroup()
{
}


CellGroup::~CellGroup()
{
}

void CellGroup::setCell(const int index, Cell &cell)
{
	m_cells[index] = &cell;
	return;
}

Cell* CellGroup::getCell(const int index) const
{
	return m_cells[index];
}

int CellGroup::getCellValue(const int index) const
{
	Cell *cell = getCell(index);
	int i = cell->getValue();
	return i;
}

void CellGroup::setCellValue(const int index, const int value)
{
	Cell *cell = getCell(index);
	cell->setValue(value);
	return;
}

void CellGroup::setCellValue(Cell &cell, const int value)
{
	cell.setValue(value);
	return;
}
