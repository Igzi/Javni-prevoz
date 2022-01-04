#include "Exception.h"
#include "UserInstructions.h"

#include <iostream>
#include <sstream>
#include <fstream>

#include <io.h>
#include <fcntl.h>

using namespace std;


int main() {
	_setmode(_fileno(stdout), _O_U16TEXT);

	wcout << L"Dobrodošli u simulator mreže gradskog prevoza. Molimo Vas, odaberite opciju:" << endl;
	wcout << L"1. Učitavanje podataka o mreži gradskog prevoza" << endl;
	wcout << L"0. Kraj rada" << endl;

	int user_input;
	cin >> user_input;

	if (user_input == 0) return 0;
	
	UserInstructions* instructions = new UserInstructions();

	if (instructions->loadStations() == 0) {
		delete instructions;
		return 0;
	}

	if (instructions->loadLines() == 0) {
		delete instructions;
		return 0;
	}

	wcout << L"Mreža gradskog prevoza je uspešno učitana" << endl;

	while (user_input != 0) {
		wcout << L"Molimo Vas, odaberite opciju:" << endl;
		wcout << L"1. Prikaz informacija o stajalištu" << endl;
		wcout << L"2. Prikaz osnovnih informacija o liniji gradskog prevoza" << endl;
		wcout << L"3. Prikaz statističkih informacija o liniji gradskog prevoza" << endl;
		wcout << L"4. Pronalazak putanje između dva stajališta" << endl;
		wcout << L"0. Kraj rada" << endl;

		cin >> user_input;

		if (user_input == 1) {
			instructions->printStation();
		}

		if (user_input == 2) {
			instructions->printLine();
		}

		if (user_input == 3) {
			instructions->printStatistics();
		}

		if (user_input == 4) {
			if (instructions->printPath() == 0) {
				delete instructions;
				return 0;
			}
		}
	}

	delete instructions;
	return 0;
}