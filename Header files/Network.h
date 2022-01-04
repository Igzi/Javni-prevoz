#ifndef network_h
#define network_h

#include "Line.h"
#include "Station.h"
#include "Path.h"

class Network {
public:
	Network();
	~Network();

	void loadStations(const string& filepath);
	void loadLines(const string& filepath);

	void printStation(int code);
	void printLine(string& name);
	void printStatistics(string& name);

	void findFastestPath(int start, int end, string clock, PathType type);

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

	Line* makeLine(string& line);

	bool isNumber(const string& num) const;
	bool checkTimeFormat(const string& time) const ;
	int parseTime(const string& time) const;

	void sortStationLines();

	void printPaths(int start, int end, const string& filepath, unordered_map<int, Path>& stations);

	unordered_map<int, Station*> stations_;
	unordered_map<string, Line*> lines_;
};

#endif // network_h
