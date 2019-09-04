#include "PQL.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <regex>
#include <sstream>

using namespace std;

#include "PKB.h"
#include <unordered_map> 

PQL::PQL(string query) {

	//test case 1

	// wrong expected result
	//assign a; variable g; read v; Select v pattern a (g, _) such that Parent (v, _);

	//correct expected result
	//assign a; variable g; read v; Select v pattern a (g, _) such that Uses (v, _);
	//assign a; variable g; read v; Select v pattern a (g, _)  such that Uses (v, _) such that Uses (a, _) ;


	//Parse and store the query into "declaration" and "body" using regular expression
	regex r1("(((stmt|read|print|call|while|if|assign|variable|constant|procedure)\\s[a-zA-Z][a-zA-Z0-9]*;(\\s)*)*(?=Select))");
	smatch match1;
	regex_search(query, match1, r1);
	declaration = match1.str(0);

	regex r2("(Select.*)");
	smatch match2;
	regex_search(query, match2, r2);
	body = match2.str(0);

	bool isValidSyntax = syntaxCheck(declaration, body);
	if (!isValidSyntax) {
		return;
	}

	parseDeclaration();
	
	//parseBody(); i think i no longer need parseBody()
	string result = query_algorithm();

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

bool PQL::syntaxCheck(string declaration, string body) {
	//Check for declaration
	if (declaration.empty()) {
		queryResult = "Syntax Error. Missing Declaration.";
		return false;
	}

	//Check for body
	if (body.empty()) {
		queryResult = "Syntax Error. Missing Body.";
		return false;
	}


	string select_cl = "(Select [a-zA-z][a-zA-Z0-9]*)";
	string relref_such_that = "(such that ((Uses|Modifies) \\(([0-9]+|[a-zA-Z][a-zA-Z0-9]*), (_|\"[a-zA-Z][A-Za-z0-9]*\"|[a-zA-Z][A-Za-z0-9]*)\\)|((Parent|Parent\\*|Follows|Follows\\*) \\(([0-9]+|[a-zA-Z][a-zA-Z0-9]*|_), (_|[0-9]+|[a-zA-Z][a-zA-Z0-9]*\\)))))";
	string relref_pattern("(pattern [a-zA-Z][a-zA-Z0-9]* \\(((\\s)?_(\\s)?|\"[a-zA-Z][a-zA-Z0-9]*\"|[a-zA-Z][a-zA-Z0-9]*), ((\\s)?_(\\s)?|_\"([a-zA-Z][a-zA-Z0-9]*|[0-9]+)((\\s)?[+\\-*%/](\\s)?([a-zA-Z][a-zA-Z0-9]*|[0-9]+))*\"_|\"([a-zA-Z][a-zA-Z0-9]*|[0-9]+)((\\s)?[+\\-/*%](\\s)?([a-zA-Z][a-zA-Z0-9]*|[0-9]+))*\")\\))");
	
	body_select = check_clause("Select", select_cl);
	body_such_that = check_clause("such that", relref_such_that);
	body_pattern = check_clause("pattern", relref_pattern);


	cout << body_select << " body_select ________________" << endl;
	cout << body_such_that << " body_such_that ________________" << endl;
	cout << body_pattern << " body_pattern ________________" << endl;

	if (body_select.compare(string_false) == 0 || body_such_that.compare(string_false) == 0 || body_pattern.compare(string_false) == 0) {
		return false;
	}
	else {
		bool extra_char = check_extra_char(select_cl, relref_such_that, relref_pattern);
		return !extra_char;
	}
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

// answer user query
string PQL::query_algorithm()
{
	string query_value = "";
	if (body_such_that.empty() && body_pattern.empty()) {

		string word_select, arugment;
		istringstream iss(body_select);
		iss >> word_select;
		iss >> arugment;


		if (arugment.compare("variable")) {
			unordered_map <std::string, int > VarTable = PKB().getVariables();
			queryResult = "";
			for (auto const& element : VarTable)
			{
				queryResult = queryResult + element.first + ", ";
				//queryResult = "from varTable";
			}
		}
		else {
			queryResult = "Select some variable: " + body_select;
		}
	}
	else if (!body_such_that.empty() && body_pattern.empty()) {
		queryResult = "Select some variable: " + body_select + "\n";
		queryResult += "Such that: " + body_such_that + "\n";

	}
	else if (body_such_that.empty() && !body_pattern.empty()) {
		queryResult = "Only Select some variable: " + body_select + "\n";
		queryResult += "Pattern: " + body_pattern + "\n";
	}
	else {
		queryResult = "Select some variable: " + body_select + "\n";
		queryResult += "Such that: " + body_such_that + "\n";
		queryResult += "Pattern: " + body_pattern + "\n";
	}


	return query_value;
}

//check if clauses fit the string relref , and no repeated clauses
string PQL::check_clause(string clause, string relref)
{
	int counter = 0;
	regex regex_clause("(" + clause + " .*)");
	regex regex_relref(relref);
	smatch match_clause, match_relref;

	string body_copy = body;
	string clause_result;
	string clause_syntax_result;

	//regex_search(body_copy, match_clause, regex_clause);
	regex_search(body_copy, match_clause, regex_clause);
	clause_result = match_clause.str();

	while (regex_search(body_copy, match_relref, regex_relref)) {
		
		clause_syntax_result = match_relref.str();
		body_copy = match_relref.suffix().str();
		counter++;
	}

	if (counter > 1) {
		queryResult = "Syntax Error. Repeated *" + clause + "* Clause.";
		return string_false;
	}
	
	if (!clause_result.empty() && clause_syntax_result.empty()) {
		queryResult = "Syntax Error. Wrong *" + clause +  "* Clause Syntax.";
		return string_false;
	}
	
	return clause_syntax_result;
}

// check for extra charaters outside select, such that, pattern clauses.
bool PQL::check_extra_char(string select_clause, string such_that_clause, string pattern_clause)
{
	
	string body_copy = body;
	string empty_string = "";
	regex regex_select(select_clause);
	body_copy = regex_replace(body_copy, regex_select, empty_string);
	regex regex_such_that(such_that_clause);
	body_copy = regex_replace(body_copy, regex_such_that, empty_string);
	regex regex_pattern(pattern_clause);
	body_copy = regex_replace(body_copy, regex_pattern, empty_string);

	smatch match;
	regex expected_str("\\s*;");
	regex_search(body_copy, match, expected_str);


	if (match.prefix().compare(empty_string) != 0 || match.suffix().compare(empty_string) != 0) {
		queryResult = "Syntax Error. Unexpected string in query.";
		return true;
	}

	return false;
}
