#ifndef line_h
#define line_h

#include "Station.h"
#include "Path.h"

#include <unordered_map>
#include <vector>
#include <set>

using namespace std;

class Line {
public:
	Line(string name, int start, int end, int time_difference, vector<Station*>& line_stations);
	~Line();

	string getName() const;
	int getPosition(Station* station);

	vector<Path*> findPaths(Station* station, int t, Path& current_path);

	void print(const string& filepath) const;
	void printStatistics(const string& filepath);
	void printPath(ofstream& output, Station* start, Station* end);

	int nextArrivalTime(bool direction, Station* station, int t);

private:
	static const int TIMEBETWEENSTATIONS = 3; //Odredjuje broj minuta izmedju 2 stanice
	static const int MINUTESPERDAY = 60 * 24; //Odredjuje broj minuta u danu

	string name_;
	int start_, end_, reverse_start_, reverse_end_, time_difference_;
	vector<Station*> line_stations_;
	set<string> intersecting_lines_;
	unordered_map<Station*, int> station_position_; //Mapa za efikasnu pretragu stanica u vektoru line_stations_
};

#endif // line_h
