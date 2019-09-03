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
		string getResult();
	private:
		string declaration;
		string body;
		string queryResult;
		bool syntaxCheck(string declaration, string body);
		void parseDeclaration();
		void replace_body(string replacement);
		void parseBody();
		string query_algorithm(string select_value, string such_that_value = "", string pattern_value = "");
};

#endif
