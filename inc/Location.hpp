#ifndef LOCATION_H
#define LOCATION_H
#include <iostream>
#include <string>
#include <map>
#include "webserv.h"

enum e_method
{
	GET,
	POST,
	DELETE,
	PUT,
	PATCH
};

using namespace std;
// Class definition 
class Location
{
private: 
	string _index;
	bool _autoIndex;
	map<e_method, bool> _method;
	string _upload;
 
public: 
// Constructor 
	Location(); 
	Location( const Location &rhs); 
	Location(string newIndex, bool newAutoIndex, map<e_method, bool> newMethod, string newUpload);
	~Location(); 
	Location& operator=( const Location &rhs); 
// Getters 
	string getIndex(); 
	bool getAutoIndex(); 
	map<e_method, bool> getMethod();
	string getUpload(); 
// Setters 
	void setIndex(string _index); 
	void setAutoIndex(bool _autoIndex);
	void setDefaultMethod(void);
	void setMethod(map<e_method, bool> _method);
	void setUpload(string _upload); 
// Output 
	void output(); 
}; 
 
#endif
