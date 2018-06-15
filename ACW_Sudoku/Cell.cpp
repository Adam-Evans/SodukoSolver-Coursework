#include "Cell.h"
#include <algorithm>


Cell::Cell(void)
{
	m_preset = false;
	m_value = 0;
}

Cell::Cell(int value, bool preset)
{
	m_value = value;
	m_preset = preset;
}

void Cell::addPossible(const int value)
{
	m_possibles.push_back(value);
}

void Cell::setPossibles(const vector<int> values)
{
	m_possibles = values;
}

vector<int> Cell::getPossibles() const
{
	return m_possibles;
}

void Cell::removePossible(const int value)
{
	m_possibles.erase(std::remove(m_possibles.begin(), m_possibles.end(), value), m_possibles.end()); // erase-remove idiom.
}

void Cell::clearPossible()
{
	m_possibles.clear();
}

void Cell::setValue(const int value)
{
	m_value = value;
}

int Cell::getValue(void) const
{
	return m_value;
}

void Cell::setPreset(const bool preset)
{
	m_preset = preset;
}

bool Cell::isPreset(void) const
{
	return m_preset;
}

Cell::~Cell()
{
}
