#include<stdio.h>
#include <iostream>
#include <string>

using namespace std;

class PQL
{
public:
	PQL(string query);
private:
	string declaration;
	string body;
	void parseDeclaration(string declaration);
	void parseBody(string body);
};

