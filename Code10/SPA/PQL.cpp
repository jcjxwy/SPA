#include "PQL.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <regex>
#include <sstream>

using namespace std;

PQL::PQL(string query) {

	//Parse and store the query into "declaration" and "body" using regular expression
	regex r1("(((stmt|read|print|call|while|if|assign|variable|constant|procedure)\\s[a-zA-Z][a-zA-Z0-9]*;(\\s)*)*(?=Select))");
	smatch match1;
	regex_search(query, match1, r1);
	declaration = match1.str(0);

	regex r2("(Select.*)");
	smatch match2;
	regex_search(query, match2, r2);
	body = match2.str(0);

	parseDeclaration();
	parseBody();
}

string PQL::getDeclaration() {
	return declaration;
}

string PQL::getBody() {
	return body;
}

void PQL::parseDeclaration() {
	//Separate the multiple declaration and replace them in "body" accordingly
	regex r("((stmt|read|print|call|while|if|assign|variable|constant|procedure)\\s[a-zA-Z][a-zA-Z0-9]*(?=;))");
	smatch match;
	while (regex_search(declaration, match, r)) {
		replace_body(match.str(0));
		declaration = match.suffix().str();
	}	
}

void PQL::replace_body(string replacement) {
	//Separate a single declaration into its design entity and synonym
	string designEntity, synonym;
	istringstream iss(replacement);
	iss >> designEntity;
	iss >> synonym;

	//Replace all the synonym with actual design entity
	regex r("(\\b" + synonym + "\\b)");
	body = regex_replace(body, r, designEntity);
}

void PQL::parseBody() {

}