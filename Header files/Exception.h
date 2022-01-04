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

class FileError : public Error {
public:
	FileError(const wstring& msg, int line) : Error(msg){
		line_ = line;
	};

	int getLine() {
		return line_;
	};

private:
	int line_; //Linija unutar fajla na kojoj je doslo do greske
};

#endif // exceptions_h
