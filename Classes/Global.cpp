#include "Global.h"

#include <regex>
using std::regex;
using std::match_results;
using std::regex_match;
using std::cmatch;

string Global::username = "";
long Global::score = 0;
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

// 正则表达式提取session
string Global::getSessionIdFromHeader(string head) {
	regex nlp("\\r\\n");
	string header = regex_replace(head, nlp, " ");
	regex pattern(".*SESSION=(.*) Content-Type.*");
	//match_results<std::string::const_iterator> result;
	cmatch result;
	bool valid = regex_match(header.c_str(), result, pattern);

	if (!valid) {
		return "";
	}
	return result[1];
}