#ifndef network_h
#define network_h

#include "Line.h"
#include "Station.h"
#include "Path.h"

#include <unordered_map>

using namespace std;

class Network {
public:
	Network();
	Network(const Network& obj);
	~Network();

	void loadStations(const string& filepath);
	void loadLines(const string& filepath);

	void printStation(int code);
	void printLine(string& name);
	void printStatistics(string& name);

	void findPath(int start, int end, string clock, PathType type);

private: 
	
	static const string SEPARATOR;
	static const string FILETYPETXT;
	static const string FOLDERPATH;
	static const string STATIONFILEPREFIX;
	static const string LINEFILEPREFIX;
	static const string STATISTICSFILEPREFIX;
	static const string PATHFILEPREFIX;

	void clearStations();
	void clearLines();

	Line* makeLine(string& line, int& file_line);

	bool isNumber(const string& num) const;
	bool checkTimeFormat(const string& time) const;
	int parseTime(const string& time) const;

	void sortStationLines();

	void printPaths(int start, int end, const string& filepath, unordered_map<int, Path>& stations);

	unordered_map<int, Station*> stations_; //Mapa svih stanica unedjena po sifri stanica
	unordered_map<string, Line*> lines_; //Mapa svih linija uredjena po nazivu linije
};

#endif // network_h
