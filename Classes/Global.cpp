#include "Global.h"

#include <regex>
using std::regex;
using std::match_results;
using std::regex_match;
using std::cmatch;

string Global::username = "";
int Global::maxscore = 0;
int Global::score = 0;
string Global::ip = "119.29.163.132";
string Global::status = "";
string Global::loginHead = "";
string Global::loginBody = "";

Global::Global() {
}


Global::~Global() {
}

string Global::toString(vector<char> *buffer) {
	string rst;
	for (char ch : *buffer) {
		rst.push_back(ch);
	}
	return rst;
}
