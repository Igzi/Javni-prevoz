#ifndef userinstructions_h
#define userinstructions_h

#include "Network.h"

class UserInstructions {
public:
	UserInstructions();
	UserInstructions(const UserInstructions& obj);
	~UserInstructions();

	int loadStations();
	int loadLines();

	void printStation();
	void printLine();
	void printStatistics();
	int printPath();

private:
	Network* network;
};

#endif // userinstructions_h