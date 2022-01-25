#include "Exception.h"
#include "UserInstructions.h"


int main() {
	UserInstructions* instructions = nullptr;

	try {
		instructions = new UserInstructions();
	}
	catch (Error& e) {
		//Objekat je nastao ali su njegove vrednosti neodredjene pa ga moramo obrisati i prekinuti izvrsavanje programa
		delete instructions;
		return 0;
	}

	if (instructions->loadNetwork() == CANCEL) {
		delete instructions;
		return 0;
	}

	while (true) {
		int user_input = instructions->loadInstruction();

		if (instructions->executeInstruction(user_input) == CANCEL) break;
	}

	delete instructions;
	return 0;
}