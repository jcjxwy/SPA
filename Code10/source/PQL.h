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
		string body_select;
		string body_such_that;
		string body_pattern;
		string queryResult;
		const string string_false = "false";
		bool syntaxCheck(string declaration, string body);
		void parseDeclaration();
		void replace_body(string replacement);
		void parseBody();
		string query_algorithm();
		string check_clause(string clause, string relref);
		bool check_extra_char(string select_clause, string such_that_clause, string pattern_clause);
};

#endif
