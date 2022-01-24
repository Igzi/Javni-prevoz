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

	if (user_input == CANCEL) return 0;
	
	UserInstructions* instructions = new UserInstructions();

	if (instructions->loadStations() == CANCEL) {
		delete instructions;
		return 0;
	}

	if (instructions->loadLines() == CANCEL) {
		delete instructions;
		return 0;
	}

	wcout << L"Mreža gradskog prevoza je uspešno učitana" << endl;

	while (true) {
		user_input = instructions->loadInstruction();

		if (instructions->executeInstruction(user_input) == CANCEL) break;
	}

	delete instructions;
	return 0;
}