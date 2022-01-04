#include "Station.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>

Station::Station(int code, string& name)
{
	code_ = code;
	name_ = name;
}

Station::~Station()
{
	station_lines_.clear();
}

int Station::getCode() const
{
	return code_;
}

string Station::getName() const
{
	return name_;
}

void Station::addLine(string& new_line)
{
	station_lines_.push_back(new_line);
}

void Station::sortLines()
{
	sort(station_lines_.begin(), station_lines_.end());
}

//Ispisuje podatke o stanici u trazenom formatu
void Station::print(const string& filepath) const
{
	ofstream output(filepath);

	output << code_ << " " << name_<<" [";
	for (int i = 0; i < station_lines_.size(); i++) {
		if (i != 0) output << " ";
		output << station_lines_[i];
	}
	output << "]";
}
