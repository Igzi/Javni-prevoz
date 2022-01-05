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
	Path(int time, int start, int end, int l, int r, int bus_cnt, string line, PathType type);

private:
	int time_; //Trenutak dolaska na cilj
	int start_, end_; //Sifra pocetne i krajnje stanice
	int l_, r_; //Redni broj pocetne i krajnje stanice u liniji line_
	int bus_cnt_; //Broj presedanja zajedno sa ovom putanjom
	string line_; //Linija koja vozi ovu putanju
	PathType type_; //Tip putanje koji je potrebno odrediti
};

//Struktura za poredjenje putanja unutar prioritetnog reda
struct ComparePath {
	bool operator()(Path const& p1, Path const& p2);
};

#endif // path_h