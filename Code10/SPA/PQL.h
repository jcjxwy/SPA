#ifndef PQL_H
#define PQL_H

#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;

class PQL
{
	public:
		PQL(string query);
		string getDeclaration();
		string getBody();
	private:
		string declaration;
		string body;
		void parseDeclaration();
		void replace_body(string replacement);
		void parseBody();
};

#endif

