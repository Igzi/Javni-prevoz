#ifndef exceptions_h
#define exceptions_h

#include <exception>
#include <string>

using namespace std;

class Error : public exception {
public:
	Error(const wstring& msg){
		msg_ = msg;
	};

	wstring what() {
		return msg_;
	};

private:
	wstring msg_;
};

#endif // exceptions_h
