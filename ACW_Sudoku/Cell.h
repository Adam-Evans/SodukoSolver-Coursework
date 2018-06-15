#pragma once

#include <vector>
using namespace std;

class Cell
{
public:
	Cell();
	Cell(int value, bool preset);
	~Cell();

	void addPossible(const int value);
	void setPossibles(const vector<int> values);
	vector<int> getPossibles() const;
	void removePossible(const int value);
	void clearPossible();
	void setValue(const int value);
	int getValue(void) const;
	void setPreset(const bool preset);
	bool isPreset(void) const;

private:
	int m_value;
	bool m_preset;
	std::vector<int> m_possibles;
};

