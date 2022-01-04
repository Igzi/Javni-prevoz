#ifndef station_h
#define station_h

#include <string>
#include <vector>

using namespace std;

class Station {
public:
	friend class Line;
	friend class Network;

	Station(int code, string& name);
	~Station();

	int getCode() const;
	string getName() const;

	void addLine(string& new_line);
	void sortLines();

	void print(const string& filepath) const;

private:
	int code_;
	string name_;
	vector<string> station_lines_;
};

#endif // station_h
