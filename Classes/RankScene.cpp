#include "RankScene.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include <regex>
#include <string>
#include <sstream>
using std::regex;
using std::match_results;
using std::regex_match;
using std::cmatch;
using namespace rapidjson;

#pragma execution_character_set("UTF-8")

USING_NS_CC;

cocos2d::Scene* RankScene::createScene() {
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = RankScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool RankScene::init() {
	if (!Layer::init()) {
		return false;
	}

	Size size = Director::getInstance()->getVisibleSize();
	visibleHeight = size.height;
	visibleWidth = size.width;

	username_field = TextField::create("", "Arial", 30);
	username_field->setPosition(Size(visibleWidth / 2, visibleHeight / 10 * 9));
	this->addChild(username_field, 2);

	score_field = TextField::create("", "Arial", 30);
	score_field->setPosition(Size(visibleWidth / 2, visibleHeight / 10 * 8));
	this->addChild(score_field, 2);

	ranking_field = TextField::create("", "Arial", 30);
	ranking_field->setPosition(Size(visibleWidth / 2, visibleHeight / 10 * 7));
	this->addChild(ranking_field, 2);

	rank_field = TextField::create("", "Arial", 30);
	rank_field->setPosition(Size(visibleWidth / 2, visibleHeight / 10 * 3));
	this->addChild(rank_field, 2);

	rankEvent();

	return true;
}

// 查看排名事件函数
void RankScene::rankEvent() {

	HttpRequest* request = new HttpRequest();

	// 发送GET请求即可
	std::string url = "http://localhost:11900/rank/";
	request->setUrl((url + Global::username).c_str());
	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback(CC_CALLBACK_2(RankScene::onRankHttpCompleted, this));
	request->setTag("GET test");

	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
}

// 查看排名请求结束回调函数
void RankScene::onRankHttpCompleted(HttpClient *sender, HttpResponse* response) {
	if (!response) {
		return;
	}
	if (!response->isSucceed()) {
		log("response failed");
		log("error buffer: %s", response->getErrorBuffer());
		return;
	}

	std::vector<char>* bodyBuffer = response->getResponseData();
	rapidjson::Document d;
	d.Parse<0>(Global::toString(bodyBuffer).c_str());
	if (d.HasParseError()) {
		log("GetParseError " + d.GetParseError());
		return;
	}
	if (d.IsObject() && d.HasMember("username") && d.HasMember("maxscore") && d.HasMember("ranking") && d.HasMember("result")) {
		username_field->setString(string("玩家名称：") + d["username"].GetString());
		int maxscore = d["maxscore"].GetInt();
		int ranking = d["ranking"].GetInt();
		std::stringstream ss;
		ss << maxscore;
		string temp;
		ss >> temp;
		ss.clear();
		ss.str("");
		score_field->setString(string("最高分数：") + temp);
		ss << ranking;
		ss >> temp;
		ranking_field->setString(string("当前排名：") + temp);
		temp = "";
		temp = d["result"].GetString();
		string result = "";
		for (int i = 1; i < temp.length(); i++) {
			if (temp[i] == '|') {
				result += "\n";
			} else {
				result += temp[i];
			}
		}
		rank_field->setString(result);
	}
}


