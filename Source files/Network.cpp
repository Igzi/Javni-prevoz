﻿#include "Exception.h"
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

Network::Network(const Network& obj)
{
	for (auto& line : obj.lines_) {
		lines_[line.first] = new Line(*line.second); //Generise se plitka kopija
	}

	for (auto& station : obj.stations_) {
		Station* current_station = new Station(*station.second);
		stations_[station.first] = current_station;

		for (int i = 0; i < current_station->station_lines_.size(); i++) {
			Line* current_line = lines_[current_station->station_lines_[i]];
			current_line->addStation(current_station); //Ovde od plitke kopije pravimo duboku
		}
	}
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
		std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
		wstring msg = L"Nije moguće otvoriti fajl " + conv.from_bytes(filepath) + L".";
		throw FileError(msg, 0);
		return;
	}

	string code, name;
	int int_code, file_line = 0;

	while (getline(input, code, ' ')) {
		file_line++;
		if (!isNumber(code)) {
			throw FileError(L"Šifra stanice mora biti ceo broj.", file_line);
			return;
		}

		name = "";
		getline(input, name);
		int_code = stoi(code);

		if (stations_.find(int_code) != stations_.end()) {
			throw FileError(L"Šifra stanice mora biti jedinstvena.", file_line);
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
		std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
		wstring msg = L"Nije moguće otvoriti fajl " + conv.from_bytes(filepath) + L".";
		throw FileError(msg, 0);
		return;
	}

	int file_line = 0;
	try {
		while (getline(input, line)) {
			file_line++;
			Line* new_line = makeLine(line, file_line);
			lines_[new_line->getName()] = new_line;
		}
	}
	catch (FileError e) {
		throw e;
		return;
	}

	sortStationLines();
}

void Network::printStation(int code)
{
	if (stations_.find(code) == stations_.end()) {
		throw Error(L"Stanica sa šifrom " + to_wstring(code) + L" ne postoji.");
		return;
	}

	string filepath = FOLDERPATH + STATIONFILEPREFIX + SEPARATOR + to_string(code) + FILETYPETXT;

	stations_[code]->print(filepath);
}

void Network::printLine(string& name)
{
	if (lines_.find(name) == lines_.end()) {
		std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
		throw Error(L"Linija sa imenom " + conv.from_bytes(name) + L" ne postoji.");
		return;
	}

	string filepath = FOLDERPATH + LINEFILEPREFIX + SEPARATOR + name + FILETYPETXT;

	lines_[name]->print(filepath);
}

void Network::printStatistics(string& name)
{
	if (lines_.find(name) == lines_.end()) {
		std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
		throw Error(L"Linija sa imenom " + conv.from_bytes(name) + L" ne postoji.");
		return;
	}

	string filepath = FOLDERPATH + STATISTICSFILEPREFIX + SEPARATOR + name + FILETYPETXT;

	lines_[name]->printStatistics(filepath);
}

void Network::findPath(int start, int end, string clock, PathType type)
{
	if (stations_.find(start) == stations_.end()) {
		throw Error(L"Stanica sa šifrom " + to_wstring(start) + L" ne postoji.");
		return;
	}
	if (stations_.find(end) == stations_.end()) {
		throw Error(L"Stanica sa šifrom " + to_wstring(end) + L" ne postoji.");
		return;
	}
	int t;
	try {
		t = parseTime(clock);
	}
	catch (Error e) {
		throw e;
		return;
	}

	string filepath = FOLDERPATH + PATHFILEPREFIX + SEPARATOR + to_string(start) + SEPARATOR + to_string(end) +  FILETYPETXT;

	//Najkraci put pronalazimo pomocu dajkstrinog algoritma
	unordered_map<int, Path> paths;
	priority_queue<Path, vector<Path>, ComparePath> pq;

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

Line* Network::makeLine(string& line, int& file_line)
{
	stringstream sstream(line);
	string input_word, name;
	int start, end, time_difference;
	vector<Station*> line_stations;
	
	name = "";
	getline(sstream, name, ' ');

	if (name == "") {
		throw FileError(L"Nedostaje ime linije", file_line);
		return nullptr;
	}
	if (lines_.find(name) != lines_.end()) {
		throw FileError(L"Oznaka linije mora biti jedinstvena", file_line);
		return nullptr;
	}

	try {
		sstream.get(); // '['

		input_word = "";
		getline(sstream, input_word, '-');
		start = parseTime(input_word);

		input_word = "";
		getline(sstream, input_word, '#');
		end = parseTime(input_word);
	}
	catch (Error e) {
		throw FileError(e.what(), file_line);
		return nullptr;
	}

	input_word = "";
	getline(sstream, input_word, ']');
	if (!isNumber(input_word) || stoi(input_word) <= 0) {
		throw FileError(L"Razmak između polazaka mora biti pozitivan ceo broj.", file_line);
		return nullptr;
	}
		
	time_difference = stoi(input_word);

	sstream.get(); // ' '

	while (getline(sstream, input_word, ' ')) {
		if (!isNumber(input_word)) {
			throw FileError(L"Šifra stanice mora biti ceo broj.", file_line);
			return nullptr;
		}
		if (stations_.find(stoi(input_word)) == stations_.end()) {
			throw FileError(L"Linija prolazi kroz nepostojeću stanicu", file_line);
			return nullptr;
		}

		Station* station = stations_[stoi(input_word)];
		line_stations.push_back(station);
		station->addLine(name);
	}

	return new Line(name, start, end, time_difference, line_stations);
}

bool Network::isNumber(const string& num) const
{
	if (num.size() == 0) return false;

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
	if (stations.find(end) == stations.end()) {
		throw Error(L"Putanja od stanice " + to_wstring(start) + L" do stanice " + to_wstring(end) + L" ne postoji.");
		return;
	}

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
