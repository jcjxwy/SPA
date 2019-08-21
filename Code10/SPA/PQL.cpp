#include "PQL.h"
#include<stdio.h>
#include <iostream>
#include <string>
#include <regex>

using namespace std;

PQL::PQL(string query) {
	regex r1("(((stmt|read|print|call|while|if|assign|variable|constant|procedure)\\s[a-zA-Z];(\\s)*)*(?=Select))");
	smatch match1;
	regex_search(query, match1, r1);
	declaration = match1.str(0);

	regex r2("(Select.*)");
	smatch match2;
	regex_search(query, match2, r2);
	body = match2.str(0);

}

void PQL::parseDeclaration(string declaration) {

}

void PQL::parseBody(string body) {

}