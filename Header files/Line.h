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
	friend class Network;

	Line(string name, int start, int end, int time_difference, vector<Station*>& line_stations);
	~Line();

	string getName() const;

	vector<Path*> findPaths(int current_station, int t, Path& current_path);

	void print(ofstream& output) const;
	void printStatistics(ofstream& output);
	void printPath(ofstream& output, int l, int r);

	int nextArrivalTime(bool direction, int position, int t);

private:
	static const int TIMEBETWEENSTATIONS = 3; //Odredjuje broj minuta izmedju 2 stanice
	static const int MINUTESPERDAY = 60 * 24; //Odredjuje broj minuta u danu

	string name_;
	int start_, end_, reverse_start_, reverse_end_, time_difference_;
	vector<Station*> line_stations_; //Vektor svih stanica kroz koje prolazi ova linija
	set<string> intersecting_lines_; //Skup svih linija koje dele stajaliste sa ovom linijom
};

#endif // line_h
