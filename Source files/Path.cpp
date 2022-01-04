#include "Path.h"

Path::Path()
{
	time_ = start_ = end_ = bus_cnt_ = 0;
	line_ = "";
}

Path::Path(int time, int start, int end, int bus_cnt, string line, PathType type)
{
	time_ = time;
	start_ = start;
	end_ = end;
	bus_cnt_ = bus_cnt;
	line_ = line;
	type_ = type;
}

bool ComparePath::operator()(Path const& p1, Path const& p2)
{
	if (p1.type_ == FASTEST) {
		if (p1.time_ != p2.time_) return p1.time_ > p2.time_;
		return p1.bus_cnt_ > p2.bus_cnt_;
	}

	if (p1.type_ == SHORTEST) {
		if (p1.bus_cnt_ != p2.bus_cnt_) return p1.bus_cnt_ > p2.bus_cnt_; 
		return p1.time_ > p2.time_;
	}
}
