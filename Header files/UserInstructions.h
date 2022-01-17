#ifndef userinstructions_h
#define userinstructions_h

#include "Network.h"

enum UserResponse {
	CONTINUE = 1,
	CANCEL = 0,
};

enum Instruction {
	PRINTSTATION = 1,
	PRINTLINE = 2,
	PRINTSTATISTICS = 3,
	PRINTPATH = 4,
	INSTRUCTIONCANCEL = 0,
};

class UserInstructions {
public:
	UserInstructions();
	UserInstructions(const UserInstructions& obj);
	~UserInstructions();

	UserResponse loadStations();
	UserResponse loadLines();

	void printStation();
	void printLine();
	void printStatistics();
	UserResponse printPath();

private:
	Network* network;
};

#endif // userinstructions_h