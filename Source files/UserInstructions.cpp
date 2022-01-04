#include "Exception.h"
#include "UserInstructions.h"

#include <iostream>
#include <sstream>
#include <fstream>

UserInstructions::UserInstructions()
{
	network = new Network();
}

UserInstructions::~UserInstructions()
{
	delete network;
}

int UserInstructions::loadStations()
{
	while (true) {
		try {
			wcout << L"Molimo Vas, unesite putanju do fajla sa stajalištima:" << endl;

			string filepath;
			cin >> filepath;

			network->loadStations(filepath);
			break;
		}
		catch (Error e) {
			wcout << e.what() << endl;
			wcout << L"1. Pokušajte ponovo" << endl;
			wcout << L"0. Kraj rada" << endl;

			while (true) {
				int user_input;
				cin >> user_input;
				if (user_input == 0) return 0; //0 je signal da korisnik zeli da zavrsi izvrsavanje programa
				if (user_input == 1) break;
				wcout << L"Neispravan unos, pokušajte ponovo." << endl;
			}
		}
	}
	return 1; //1 je signal da je izvrsavanje uspesno izvrseno
}

int UserInstructions::loadLines()
{
	while (true) {
		try {
			wcout << L"Molimo Vas, unesite putanju do fajla sa linijama gradskog prevoza:" << endl;

			string filepath;
			cin >> filepath;

			network->loadLines(filepath);
			break;
		}
		catch (Error e) {
			wcout << e.what() << endl;
			wcout << L"1. Pokušajte ponovo" << endl;
			wcout << L"0. Kraj rada" << endl;

			while (true) {
				int user_input;
				cin >> user_input;
				if (user_input == 0) return 0; //0 je signal da korisnik zeli da zavrsi izvrsavanje programa
				if (user_input == 1) break;
				wcout << L"Neispravan unos, pokušajte ponovo." << endl;
			}
		}
	}
	return 1; //1 je signal da je izvrsavanje uspesno izvrseno
}

void UserInstructions::printStation()
{
	wcout << L"Molimo Vas, unesite oznaku stajališta čije informacije želite da prikažete." << endl;

	int station;
	cin >> station;

	network->printStation(station);

	wcout << L"Generisan je fajl stajaliste_" + to_wstring(station) + L".txt sa osnovnim informacijama za stajalište " + to_wstring(station) + L"." << endl;
}

void UserInstructions::printLine()
{
	wcout << L"Molimo Vas, unesite oznaku linije čije osnovne informacije želite da prikažete." << endl;

	string line;
	cin >> line;

	network->printLine(line);

	wcout << L"Generisan je fajl linija_" + wstring(line.begin(), line.end()) + L".txt sa osnovnim informacijama o liniji " + wstring(line.begin(), line.end()) + L"." << endl;
}

void UserInstructions::printStatistics()
{
	wcout << L"Molimo Vas, unesite oznaku linije čije statističke informacije želite da prikažete." << endl;

	string line;
	cin >> line;

	network->printStatistics(line);

	wcout << L"Generisan je fajl statistika_" + wstring(line.begin(), line.end()) + L".txt sa statističkim informacijama o liniji " + wstring(line.begin(), line.end()) + L"." << endl;
}

int UserInstructions::printPath()
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
		if (type == 0) return 0; //0 je signal da korisnik zeli da zavrsi izvrsavanje programa
		if (type == 1 || type == 2) break;
		wcout << L"Neispravan unos, pokušajte ponovo." << endl;
	}

	network->findFastestPath(start, end, t, static_cast<PathType>(type - 1));

	wcout << L"Generisan je fajl putanja_" + to_wstring(start) + L"_" + to_wstring(end) + L".txt sa traženom putanjom." << endl;
	return 1;
}
