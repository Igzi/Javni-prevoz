#ifndef path_h
#define path_h

#include <string>

using namespace std;

enum PathType {
	FASTEST,
	SHORTEST,
};

struct Path {
public:
	friend struct ComparePath;
	friend class Line;
	friend class Network;

	Path();
	Path(int time, int start, int end, int bus_cnt, string line, PathType type);

private:
	int time_, start_, end_, bus_cnt_;
	string line_;
	PathType type_;
};

struct ComparePath {
	bool operator()(Path const& p1, Path const& p2);
};

#endif // path_h