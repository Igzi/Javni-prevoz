#include "Exception.h"
#include "Network.h"

#include <queue>
#include <codecvt>

#include <iostream>
#include <sstream>
#include <fstream>

const string Network::SEPARATOR = "_";
const string Network::FILETYPETXT = ".txt";
const string Network::FOLDERPATH = "izlazi/";
const string Network::STATIONFILEPREFIX = "stajaliste";
const string Network::LINEFILEPREFIX = "linija";
const string Network::STATISTICSFILEPREFIX = "statistika";
const string Network::PATHFILEPREFIX = "putanja";

Network::Network()
{
}

Network::~Network()
{
	clearStations();
	clearLines();
}

void Network::loadStations(const string& filepath)
{
	clearStations();

	ifstream input(filepath);

	if (!input.is_open()) {
		std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
		wstring msg = L"Nije moguće otvoriti fajl " + myconv.from_bytes(filepath) + L".";
		throw Error(msg);
		return;
	}

	string code, name;
	int int_code;

	while (getline(input, code, ' ')) {
		if (!isNumber(code)) {
			throw Error(L"Šifra stanice mora biti ceo broj.");
			return;
		}

		getline(input, name);
		int_code = stoi(code);

		if (stations_.find(int_code) != stations_.end()) {
			throw Error(L"Šifra stanice mora biti jedinstvena.");
			return;
		}
		
		Station* new_station = new Station(int_code, name);
		stations_[int_code] = new_station;
	}
}

void Network::loadLines(const string& filepath)
{
	clearLines();

	ifstream input(filepath);
	string line;

	if (!input.is_open()) {
		std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
		wstring msg = L"Nije moguće otvoriti fajl " + myconv.from_bytes(filepath) + L".";
		throw Error(msg);
		return;
	}

	while (getline(input, line)) {
		Line* new_line = makeLine(line);
		lines_[new_line->getName()] = new_line;
	}

	sortStationLines();
}

void Network::printStation(int code)
{
	string filepath = FOLDERPATH + STATIONFILEPREFIX + SEPARATOR + to_string(code) + FILETYPETXT;

	stations_[code]->print(filepath);
}

void Network::printLine(string& name)
{
	string filepath = FOLDERPATH + LINEFILEPREFIX + SEPARATOR + name + FILETYPETXT;

	lines_[name]->print(filepath);
}

void Network::printStatistics(string& name)
{
	string filepath = FOLDERPATH + STATISTICSFILEPREFIX + SEPARATOR + name + FILETYPETXT;

	lines_[name]->printStatistics(filepath);
}

void Network::findFastestPath(int start, int end, string clock, PathType type)
{
	string filepath = FOLDERPATH + PATHFILEPREFIX + SEPARATOR + to_string(start) + SEPARATOR + to_string(end) +  FILETYPETXT;

	//Najkraci put pronalazimo pomocu dajkstrinog algoritma
	unordered_map<int, Path> paths;
	priority_queue<Path, vector<Path>, ComparePath> pq;

	int t = parseTime(clock);
	pq.push(Path(t, start, start, 0, "", type));

	while (!pq.empty()) {
		Path current_path = pq.top();
		pq.pop();

		if (paths.find(current_path.end_) != paths.end()) continue;
		paths[current_path.end_] = current_path;
		if (current_path.end_ == end) break;

		Station* current_station = stations_[current_path.end_];
		int path_time = current_path.time_;

		//Prolazak kroz sve linije koje staju na ovoj stanici
		for (int i = 0; i < current_station->station_lines_.size(); i++) {
			Line* current_line = lines_[current_station->station_lines_[i]];
			vector<Path*> line_paths = current_line->findPaths(current_station, path_time, current_path);

			for (int j = 0; j < line_paths.size(); j++) {
				pq.push(Path(*line_paths[j]));
				delete line_paths[j];
			}
			line_paths.clear();
		}
	}

	while (!pq.empty()) {
		pq.pop();
	}

	printPaths(start, end, filepath, paths);
}

void Network::clearStations()
{
	for (auto& station : stations_) {
		delete station.second;
	}
	stations_.clear();
}

void Network::clearLines()
{
	for (auto& line : lines_) {
		delete line.second;
	}
	lines_.clear();
}

Line* Network::makeLine(string& line)
{
	stringstream sstream(line);
	string input_word, name;
	int start, end, time_difference;
	vector<Station*> line_stations;

	getline(sstream, name, ' ');

	sstream.get(); // '['
	getline(sstream, input_word, '-');
	start = parseTime(input_word);

	getline(sstream, input_word, '#');
	end = parseTime(input_word);

	getline(sstream, input_word, ']');
	time_difference = stoi(input_word);

	sstream.get(); // ' '

	while (getline(sstream, input_word, ' ')) {
		Station* station = stations_[stoi(input_word)];
		line_stations.push_back(station);
		station->addLine(name);
	}

	return new Line(name, start, end, time_difference, line_stations);
}

bool Network::isNumber(const string& num) const
{
	for (int i = 0; i < num.size(); i++) {
		if (i == 0 && num[i] == '-') continue;
		if (!isdigit(num[i])) return false;
	}
	return true;
}

bool Network::checkTimeFormat(const string& time) const
{
	if (time.size() != 5) return false;
	for (int i = 0; i < 5; i++) {
		if (i == 2 && time[i] != ':') return false;
		if (i == 2) continue;
		if (!isdigit(time[i])) return false;
	}
	return true;
}

int Network::parseTime(const string& time) const
{
	if (!checkTimeFormat(time)) {
		throw Error(L"Vreme mora biti u formatu mm:ss.");
		return 0;
	}
	string mm, ss;
	mm = time[0];
	mm += time[1];
	ss = time[3];
	ss += time[4];

	return 60 * stoi(mm) + stoi(ss);
}

void Network::sortStationLines()
{
	for (auto& station : stations_) {
		station.second->sortLines();
	}
}

void Network::printPaths(int start, int end, const string& filepath, unordered_map<int, Path>& stations)
{
	ofstream output(filepath);
	vector<Path> paths;
	int current_station = end;

	while (current_station != start) {
		Path current_path = stations[current_station];
		paths.push_back(current_path);
		current_station = current_path.start_;
	}

	stations.clear();
	reverse(paths.begin(), paths.end());

	output << "->" << paths[0].line_ << endl;

	for (int i = 0; i < paths.size(); i++) {
		Line* current_line = lines_[paths[i].line_];

		current_line->printPath(output, stations_[paths[i].start_], stations_[paths[i].end_]);
		if (i != paths.size() - 1) {
			output << "\n" << paths[i].line_ << "->" << paths[i + 1].line_ << "\n";
		}
	}
}
