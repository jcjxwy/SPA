#include "PQL.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <regex>
#include <sstream>

using namespace std;

// unable to inclue PKB.h
//#include "PKB.h"
#include <unordered_map> 

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

string PQL::getResult() {
	return queryResult;
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
	smatch matchSelect, matchSuchThat, matchPattern, findPrefix;
	try {
		regex selectR("Select\\s(stmt|read|print|call|while|if|assign|variable|constant|procedure)");
		regex_search(body, matchSelect, selectR);
		string tempAns = matchSelect.str(0);


		if (!matchSelect.empty()) {
			string suffixR = matchSelect.suffix().str();

			if (suffixR.empty()) {
				// case 1, Select *arugment* without *such that* and *pattern*
				// eg - Select *arugment*

				string word_select, arugment;
				istringstream iss(matchSelect.str(0));
				iss >> word_select;
				iss >> arugment;

				// unable to inclue PKB.h
				/*
				if (arugment.compare("variable")) {
					unordered_map <std::string, int > VarTable = PKB.getVariables();
					queryResult = "";
					for (auto const& element : VarTable)
					{
						queryResult = queryResult + VarTable.first + ", ";

					}
				}
				*/
			}
			else {
				regex selectEmpty("\\s(such that\\s(Follow|Parent|Uses|Modifies|Follow\W|Parent\W)|pattern\\sassign)");
				regex_search(suffixR, findPrefix, selectEmpty);
				string prefixString = findPrefix.prefix().str();

				if (prefixString.empty() && !findPrefix.empty()) {
					//case 2, Select* arugment* with *such that* and/or *pattern*

					/* TBC
					regex selectSuchThat("such that\\s(Follow|Parent|Uses|Modifies|Follow\W|Parent\W)");
					regex_search(suffixR, matchSuchThat, selectSuchThat);
					regex selectPattern("pattern\\sassign");
					regex_search(suffixR, matchPattern, selectPattern);
					*/
					queryResult = "TBC";
				}
				else {
					//case 2.2, Invaild syntex after Select *arugment*
					queryResult = "INVAILD SYNTEX";
				}
			}
		}
		else {
			queryResult = "INVAILD ARGUMENT AFTER 'Select'.";
		}
	}
	catch (std::regex_error& e) {
		// Syntax error in the regular expression
		queryResult = "Syntax error in the regular expression.";
	}

}