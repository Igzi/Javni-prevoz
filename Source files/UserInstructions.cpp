#include "Exception.h"
#include "UserInstructions.h"

#include <iostream>
#include <sstream>
#include <fstream>

UserInstructions::UserInstructions()
{
	network = new Network();
}

UserInstructions::UserInstructions(const UserInstructions& obj)
{
	network = new Network(*obj.network);
}

UserInstructions::~UserInstructions()
{
	delete network;
}

UserResponse UserInstructions::loadStations()
{
	while (true) {
		wcout << L"Molimo Vas, unesite putanju do fajla sa stajalištima:" << endl;

		try {
			string filepath;
			cin >> filepath;

			network->loadStations(filepath);
			break;
		}
		catch (FileError e) {
			if (e.getLine() != 0) wcout << L"Linija " << e.getLine() << ": ";
			wcout << e.what() << endl;

			wcout << L"1. Pokušajte ponovo" << endl;
			wcout << L"0. Kraj rada" << endl;

			while (true) {
				int user_input;
				cin >> user_input;
				if (user_input == CANCEL) return CANCEL;
				if (user_input == CONTINUE) break;
				wcout << L"Neispravan unos, pokušajte ponovo." << endl;
			}
		}
	}
	return UserResponse::CONTINUE;
}

UserResponse UserInstructions::loadLines()
{
	while (true) {
		wcout << L"Molimo Vas, unesite putanju do fajla sa linijama gradskog prevoza:" << endl;

		try {
			string filepath;
			cin >> filepath;

			network->loadLines(filepath);
			break;
		}
		catch (FileError e) {
			if (e.getLine() != 0) wcout << L"Linija " << e.getLine() << ": ";
			wcout << e.what() << endl;

			wcout << L"1. Pokušajte ponovo" << endl;
			wcout << L"0. Kraj rada" << endl;

			while (true) {
				int user_input;
				cin >> user_input;
				if (user_input == CANCEL) return CANCEL;
				if (user_input == CONTINUE) break;
				wcout << L"Neispravan unos, pokušajte ponovo." << endl;
			}
		}
	}
	return CONTINUE;
}

int UserInstructions::loadInstruction()
{
	wcout << L"Molimo Vas, odaberite opciju:" << endl;
	wcout << L"1. Prikaz informacija o stajalištu" << endl;
	wcout << L"2. Prikaz osnovnih informacija o liniji gradskog prevoza" << endl;
	wcout << L"3. Prikaz statističkih informacija o liniji gradskog prevoza" << endl;
	wcout << L"4. Pronalazak putanje između dva stajališta" << endl;
	wcout << L"0. Kraj rada" << endl;

	int user_input;
	cin >> user_input;

	return user_input;
}

UserResponse UserInstructions::executeInstruction(int user_input)
{
	switch (user_input) {
		case PRINTSTATION:
			printStation();
			break;

		case PRINTLINE:
			printLine();
			break;

		case PRINTSTATISTICS:
			printStatistics();
			break;

		case PRINTPATH:
			printPath();
			break;

		case INSTRUCTIONCANCEL:
			return CANCEL;
			break;

		default:
			wcout << L"Neispravan unos, pokušajte ponovo." << endl;
	}

	return CONTINUE;
}

void UserInstructions::printStation()
{
	wcout << L"Molimo Vas, unesite oznaku stajališta čije informacije želite da prikažete." << endl;

	int station;
	cin >> station;

	try {
		network->printStation(station);

		wcout << L"Generisan je fajl stajaliste_" + to_wstring(station) + L".txt sa osnovnim informacijama za stajalište " + to_wstring(station) + L"." << endl;
	}
	catch (Error e) {
		wcout << e.what() << endl;
	}
}

void UserInstructions::printLine()
{
	wcout << L"Molimo Vas, unesite oznaku linije čije osnovne informacije želite da prikažete." << endl;

	string line;
	cin >> line;

	try {
		network->printLine(line);

		wcout << L"Generisan je fajl linija_" + wstring(line.begin(), line.end()) + L".txt sa osnovnim informacijama o liniji " + wstring(line.begin(), line.end()) + L"." << endl;
	}
	catch (Error e) {
		wcout << e.what() << endl;
	}
}

void UserInstructions::printStatistics()
{
	wcout << L"Molimo Vas, unesite oznaku linije čije statističke informacije želite da prikažete." << endl;

	string line;
	cin >> line;

	try {
		network->printStatistics(line);

		wcout << L"Generisan je fajl statistika_" + wstring(line.begin(), line.end()) + L".txt sa statističkim informacijama o liniji " + wstring(line.begin(), line.end()) + L"." << endl;
	}
	catch (Error e) {
		wcout << e.what() << endl;
	}
}

void UserInstructions::printPath()
{
	wcout << L"Molimo Vas, unesite početnu stanicu, krajnu stanicu i vreme polaska." << endl;

	string t;
	int start, end, type;
	cin >> start >> end >> t;

	wcout << L"Molimo Vas, odaberite opciju:" << endl;
	wcout << L"1. Najbrži put" << endl;
	wcout << L"2. Put sa najmanje presedanja" << endl;
	wcout << L"0. Odustanite" << endl;

	while (true) {
		cin >> type;
		if (type == CANCEL) return;
		if (type == Path::FASTEST || type == Path::SHORTEST) break;
		wcout << L"Neispravan unos, pokušajte ponovo." << endl;
	}

	try {
		network->findPath(start, end, t, static_cast<Path::PathType>(type));

		wcout << L"Generisan je fajl putanja_" + to_wstring(start) + L"_" + to_wstring(end) + L".txt sa traženom putanjom." << endl;
	}
	catch (Error e) {
		wcout << e.what() << endl;
	}
}
