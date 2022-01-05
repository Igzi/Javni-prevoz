#include "Line.h"

#include <iostream>
#include <sstream>
#include <fstream>

Line::Line(string name, int start, int end, int time_difference, vector<Station*>& line_stations)
{
	name_ = name;
	start_ = start;
	end_ = end;
	time_difference_ = time_difference;
	reverse_start_ = start + (line_stations.size() - 1) * TIMEBETWEENSTATIONS;
	reverse_end_ = end + (line_stations.size() - 1) * TIMEBETWEENSTATIONS;
	line_stations_ = line_stations;
}

Line::~Line()
{
	//Ne zelimo da obrisemo objekte iz vektora line_stations_ jer se oni koriste i na drugim mestima
	//Brisanje ovih objekata treba uraditi u klasi Network
	line_stations_.clear();
	intersecting_lines_.clear();
}

string Line::getName() const
{
	return name_;
}

//Generise sve putanje ove linije koje pocinju sa stanice station u trenutku t i nastavljaju se na current_path
vector<Path*> Line::findPaths(int current_station, int t, Path& current_path)
{
	vector<Path*> line_paths;
	int direct_arrival_time, reverse_arrival_time;
	int station_position = -1;

	//Prolazak kroz sve stanice kroz koje prolazi current_line
	for (int i = 0; i < line_stations_.size(); i++) {
		if (current_station == line_stations_[i]->getCode()) {
			station_position = i;
			direct_arrival_time = nextArrivalTime(true, station_position, t);
			continue;
		}

		if (station_position == -1) continue;

		int time = direct_arrival_time + (i - station_position) * Line::TIMEBETWEENSTATIONS;
		int start = current_station;
		int end = line_stations_[i]->getCode();
		int bus_cnt = current_path.bus_cnt_ + 1;

		line_paths.push_back(new Path(time, start, end, station_position, i, bus_cnt, name_, current_path.type_));
	}

	station_position = -1;
	for (int i = line_stations_.size() - 1; i >= 0; i--) {
		if (current_station == line_stations_[i]->getCode()) {
			station_position = i;
			reverse_arrival_time = nextArrivalTime(false, station_position, t);
			continue;
		}

		if (station_position == -1) continue;

		int time = reverse_arrival_time + (station_position - i) * Line::TIMEBETWEENSTATIONS;
		int start = current_station;
		int end = line_stations_[i]->getCode();
		int bus_cnt = current_path.bus_cnt_ + 1;

		line_paths.push_back(new Path(time, start, end, station_position, i, bus_cnt, name_, current_path.type_));
	}

	return line_paths;
}

//Ispisuje osnovne podatke o liniji u trazenom formatu
void Line::print(const string& filepath) const
{
	ofstream output(filepath);

	output << name_ << " " << line_stations_[0]->name_ << "->" << line_stations_.back()->name_;
	for (int i = 0; i < line_stations_.size(); i++) {
		output << "\n" << line_stations_[i]->code_ << " " << line_stations_[i]->name_;
	}
}

//Ispisuje statistiku o liniji u trazenom formatu
void Line::printStatistics(const string& filepath)
{
	if (intersecting_lines_.empty()) {
		//Ako do sad nismo odredili skup svih linija koje dele stajaliste sa ovom linijom potrebno je to da uradimo ovde
		for (int i = 0; i < line_stations_.size(); i++) {

			for (int j = 0; j < line_stations_[i]->station_lines_.size(); j++) {
				intersecting_lines_.insert(line_stations_[i]->station_lines_[j]);
			}

		}
	}

	ofstream output(filepath);
	set<string>::iterator it = intersecting_lines_.begin();

	output << name_ << "\n";

	bool first = true;
	while (it != intersecting_lines_.end())
	{
		if ((*it) == name_) {
			it++;
			continue;
		}
		if (!first) output << " ";
		first = false;
		output << (*it);
		it++;
	}

	output << "\n" << (end_ - start_) / time_difference_ + 1;
}

//Ispisuje sve stanice ove linije izmedju start i end
void Line::printPath(ofstream& output, int l, int r)
{
	for (int i = l; i <= r; i++) {
		if (i != l) output << " ";
		output << line_stations_[i]->getCode();
	}

	for (int i = l; i >= r; i--) {
		if (l == r) continue;
		if (i != l) output << " ";
		output << line_stations_[i]->getCode();
	}
}

int Line::nextArrivalTime(bool direction, int position, int t)
{
	int s, e; //Vreme prolaska prvog i poslednjeg busa ove linije kroz tu stanicu

	//direction odredjuje smer u kome zelimo da idemo (true za uskladjen smer, a false za suprotan smer)
	if (direction) {
		int n = position;
		s = start_ + n * TIMEBETWEENSTATIONS;
		e = end_ + n * TIMEBETWEENSTATIONS;
	}
	else {
		int n = line_stations_.size() - 1 - position;
		s = reverse_start_ + n * TIMEBETWEENSTATIONS;
		e = reverse_end_ + n * TIMEBETWEENSTATIONS;
	}

	int time_from_start = (t - s) % MINUTESPERDAY;
	if (time_from_start < 0) time_from_start += MINUTESPERDAY;

	if (time_from_start > e - s) {
		return t + MINUTESPERDAY - time_from_start;
	}

	if (time_from_start % time_difference_ == 0) {
		return t;
	}

	int wait_time;
	wait_time = time_difference_ - time_from_start % time_difference_;
	if (wait_time >= time_difference_) wait_time -= time_difference_;

	return t + wait_time;
}
