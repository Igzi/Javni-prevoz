#ifndef userinstructions_h
#define userinstructions_h

#include "Network.h"

enum UserResponse {
	CONTINUE = 1,
	CANCEL = 0,
};

class UserInstructions {
public:
	UserInstructions();
	UserInstructions(const UserInstructions& obj);
	~UserInstructions();

	UserResponse loadStations();
	UserResponse loadLines();

	int loadInstruction();
	UserResponse executeInstruction(int user_input);

	void printStation();
	void printLine();
	void printStatistics();
	void printPath();

private:
	enum Instruction {
		PRINTSTATION = 1,
		PRINTLINE = 2,
		PRINTSTATISTICS = 3,
		PRINTPATH = 4,
		INSTRUCTIONCANCEL = 0,
	};

	Network* network;
};

#endif // userinstructions_h