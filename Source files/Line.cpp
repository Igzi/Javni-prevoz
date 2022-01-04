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

	for (int i = 0; i < line_stations.size(); i++) {
		line_stations_.push_back(line_stations[i]);
		station_position_[line_stations[i]] = i;
	}
}

Line::~Line()
{
	//Ne zelimo da obrisemo objekte iz ovog vektora jer se oni koriste i na drugim mestima
	//Brisanje ovih objekata treba uraditi u klasi Network
	line_stations_.clear();
	station_position_.clear();
}

string Line::getName() const
{
	return name_;
}

int Line::getPosition(Station* station)
{
	return station_position_[station];
}

//Denerise sve putanje ove linije koje pocinju sa stanice station u trenutku t i nastavljaju se na current_path
vector<Path*> Line::findPaths(Station* station, int t, Path& current_path)
{
	vector<Path*> line_paths;
	int direct_arrival_time = nextArrivalTime(true, station, t);
	int reverse_arrival_time = nextArrivalTime(false, station, t);
	int station_position = getPosition(station);

	//Prolazak kroz sve stanice kroz koje prolazi current_line
	for (int j = station_position + 1; j < line_stations_.size(); j++) {
		int time = direct_arrival_time + (j - station_position) * Line::TIMEBETWEENSTATIONS;
		int start = station->getCode();
		int end = line_stations_[j]->getCode();
		int bus_cnt = current_path.bus_cnt_ + 1;

		line_paths.push_back(new Path(time, start, end, bus_cnt, name_, current_path.type_));
	}

	for (int j = station_position - 1; j >= 0; j--) {
		int time = reverse_arrival_time + (station_position - j) * Line::TIMEBETWEENSTATIONS;
		int start = station->getCode();
		int end = line_stations_[j]->getCode();
		int bus_cnt = current_path.bus_cnt_ + 1;

		line_paths.push_back(new Path(time, start, end, bus_cnt, name_, current_path.type_));
	}

	return line_paths;
}

void Line::print(const string& filepath) const
{
	ofstream output(filepath);

	output << name_ << " " << line_stations_[0]->name_ << " " << line_stations_.back()->name_;
	for (int i = 0; i < line_stations_.size(); i++) {
		output << "\n" << line_stations_[i]->code_ << " " << line_stations_[i]->name_;
	}
}

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
	while (it != intersecting_lines_.end())
	{
		if (it != intersecting_lines_.begin()) output << " ";
		output << (*it);
		it++;
	}
	output << "\n" << (end_ - start_) / time_difference_ + 1;
}

void Line::printPath(ofstream& output, Station* start, Station* end)
{
	int l, r;
	l = getPosition(start);
	r = getPosition(end);

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

int Line::nextArrivalTime(bool direction, Station* station, int t)
{
	int s, e; //vreme prolaska prvog i poslednjeg busa ove linije kroz tu stanicu
	if (direction) {
		int n = station_position_[station];
		s = start_ + n * TIMEBETWEENSTATIONS;
		e = end_ + n * TIMEBETWEENSTATIONS;
	}
	else {
		int n = line_stations_.size() - 1 - station_position_[station];
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
