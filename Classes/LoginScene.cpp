#include "LoginScene.h"
#include "cocostudio/CocoStudio.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "Global.h"
#include "SelectScene.h"
#include <regex>
using std::to_string;
using std::regex;
using std::match_results;
using std::regex_match;
using std::cmatch;
using namespace rapidjson;
USING_NS_CC;

#pragma execution_character_set("UTF-8")

using namespace cocostudio::timeline;

using namespace  rapidjson;

#define database UserDefault::getInstance()

Scene* LoginScene::createScene() {
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = LoginScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool LoginScene::init() {
	// 1. super init first
	if (!Layer::init()) {
		return false;
	}

	Size size = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	visibleHeight = size.height;
	visibleWidth = size.width;

	auto bg = Sprite::create("login.jpg");
	bg->setPosition(Vec2(size.width / 2 + origin.x, size.height / 2 + origin.y));
	this->addChild(bg, 0);

	usernameField = TextField::create("Player     Name", "Arial", 20);
	//usernameField->setColor(ccc3(150, 100, 80));
	usernameField->setColor(ccc3(250, 250, 250));
	usernameField->setPosition(Size(visibleWidth / 2, visibleHeight / 8 * 6));
	
	this->addChild(usernameField, 2);
	// -------
	auto borderUp = Sprite::create("border1.png");
	borderUp->setPosition(Vec2(visibleWidth / 2, visibleHeight / 8 * 6 + 13));
	this->addChild(borderUp, 2);
	auto borderDown = Sprite::create("border1.png");
	borderDown->setPosition(Vec2(visibleWidth / 2, visibleHeight / 8 * 6 - 17));
	this->addChild(borderDown, 2);
	auto borderL = Sprite::create("border2.png");
	borderL->setPosition(Vec2(visibleWidth / 2 - 86, visibleHeight / 8 * 6 - 2 ));
	this->addChild(borderL, 2);
	auto borderR = Sprite::create("border2.png");
	borderR->setPosition(Vec2(visibleWidth / 2 + 86, visibleHeight / 8 * 6 - 2));
	this->addChild(borderR, 2);

	auto label = Label::createWithTTF("Name", "fonts/Arial.ttf", 30);
	label->setPosition(Vec2(visibleWidth / 2 - 130, visibleHeight / 8 * 6));
	this->addChild(label, 2);
	//--------
	passwordField = TextField::create("Player Password", "Arial", 20);
	passwordField->setColor(ccc3(250, 250, 250));
	passwordField->setPosition(Size(visibleWidth / 2, visibleHeight / 8 * 5));
	this->addChild(passwordField, 2);

	auto borderUp2 = Sprite::create("border1.png");
	borderUp2->setPosition(Vec2(visibleWidth / 2, visibleHeight / 8 * 5 + 13));
	this->addChild(borderUp2, 2);
	auto borderDown2 = Sprite::create("border1.png");
	borderDown2->setPosition(Vec2(visibleWidth / 2, visibleHeight / 8 * 5 - 17));
	this->addChild(borderDown2, 2);
	auto borderL2 = Sprite::create("border2.png");
	borderL2->setPosition(Vec2(visibleWidth / 2 - 86, visibleHeight / 8 * 5 - 2));
	this->addChild(borderL2, 2);
	auto borderR2 = Sprite::create("border2.png");
	borderR2->setPosition(Vec2(visibleWidth / 2 + 86, visibleHeight / 8 * 5 - 2));
	this->addChild(borderR2, 2);

	//----
	auto label2 = Label::createWithTTF("PassWord", "fonts/Arial.ttf", 20);
	label2->setPosition(Vec2(visibleWidth / 2 - 135, visibleHeight / 8 * 5));
	this->addChild(label2, 2);
	//----
	errorField = TextField::create("", "Arial", 18);
	errorField->setColor(ccc3(150, 100, 80));
	errorField->setPosition(Size(visibleWidth / 2, visibleHeight / 10 * 9));
	this->addChild(errorField, 2);

	auto registButton = Button::create();
	registButton->setTitleText("Regist");
	registButton->setTitleFontSize(30);
	registButton->setTitleFontName("fonts/Marker Felt.TTF");
	registButton->setPosition(Size(visibleWidth / 2, visibleHeight / 8 * 3));
	registButton->addClickEventListener(CC_CALLBACK_0(LoginScene::registEvent, this));
	this->addChild(registButton, 2);

	auto loginButton = Button::create();
	loginButton->setTitleText("Login");
	loginButton->setTitleFontSize(30);
	loginButton->setTitleFontName("fonts/Marker Felt.TTF");
	loginButton->setPosition(Size(visibleWidth / 2, visibleHeight / 8 * 2));
	loginButton->addClickEventListener(CC_CALLBACK_0(LoginScene::loginEvent, this));
	this->addChild(loginButton, 2);

	auto autoLoginButton = Button::create();
	autoLoginButton->setTitleText("Login Automatically");
	autoLoginButton->setTitleFontSize(30);
	autoLoginButton->setTitleFontName("fonts/Marker Felt.TTF");
	autoLoginButton->setPosition(Size(visibleWidth / 2, visibleHeight / 8));
	autoLoginButton->addClickEventListener(CC_CALLBACK_0(LoginScene::autoLoginEvent, this));
	this->addChild(autoLoginButton, 2);

	return true;
}

// 注册事件函数
void LoginScene::registEvent() {
	if (usernameField->getString() == "Player Name" || usernameField->getString() == ""
		|| passwordField->getString() == "Player Password" || passwordField->getString() == "") {
		return;
	}

	HttpRequest* request = new HttpRequest();
	request->setUrl((string() + "http://" + Global::ip + ":11900/regist").c_str());
	request->setRequestType(HttpRequest::Type::POST);
	request->setResponseCallback(CC_CALLBACK_2(LoginScene::onRegistHttpCompleted, this));

	// 写入post请求数据
	string tmp = "username=" + usernameField->getString() + "&password=" + passwordField->getString();
	const char* postData = tmp.c_str();
	request->setRequestData(postData, strlen(postData));
	request->setTag("POST test");
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();

}

// 注册请求结束回调函数
void LoginScene::onRegistHttpCompleted(HttpClient* sender, HttpResponse* response) {
	if (!response) {
		return;
	}

	std::vector<char>* headBuffer = response->getResponseHeader();
	std::vector<char>* bodyBuffer = response->getResponseData();
	Global::loginHead = string(Global::toString(headBuffer));
	Global::loginBody = string(Global::toString(bodyBuffer));

	errorField->setString(Global::loginBody);

	usernameField->setString("");
	passwordField->setString("");
}



// 登录事件函数
void LoginScene::loginEvent() {
	if (usernameField->getString() == "Player Name" || usernameField->getString() == ""
		|| passwordField->getString() == "Player Password" || passwordField->getString() == "") {
		return;
	}

	HttpRequest* request = new HttpRequest();
	request->setUrl((string() + "http://" + Global::ip + ":11900/login").c_str());
	request->setRequestType(HttpRequest::Type::POST);
	request->setResponseCallback(CC_CALLBACK_2(LoginScene::onLoginHttpCompleted, this));

	// 写入post请求数据
	string tmp = "username=" + usernameField->getString() + "&password=" + passwordField->getString();
	const char* postData = tmp.c_str();
	request->setRequestData(postData, strlen(postData));
	request->setTag("POST test");
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();

}

// 登录请求结束回调函数
void LoginScene::onLoginHttpCompleted(HttpClient *sender, HttpResponse* response) {
	if (!response) {
		return;
	}

	std::vector<char>* headBuffer = response->getResponseHeader();
	std::vector<char>* bodyBuffer = response->getResponseData();
	Global::loginHead = string(Global::toString(headBuffer));
	Global::loginBody = string(Global::toString(bodyBuffer));

	if (!response->isSucceed()) {
		errorField->setString(Global::loginBody);
		return;
	}

	// 获取username和密码并保存在本地文件中
	Global::username = usernameField->getString();
	Global::maxscore = atoi(Global::loginBody.c_str());
	database->setStringForKey("username", usernameField->getString());
	database->setStringForKey("password", passwordField->getString());
	database->setIntegerForKey("maxscore", Global::maxscore);

	auto selectScene = SelectScene::createScene();
	Director::getInstance()->replaceScene(selectScene);

	usernameField->setString("");
	passwordField->setString("");
}

// 自动登录事件函数
void LoginScene::autoLoginEvent() {
	if (database->getStringForKey("username") == "")
		return;

	HttpRequest* request = new HttpRequest();
	request->setUrl((string() + "http://" + Global::ip + ":11900/login").c_str());
	request->setRequestType(HttpRequest::Type::POST);
	request->setResponseCallback(CC_CALLBACK_2(LoginScene::onAutoLoginHttpCompleted, this));

	// 写入POST请求数据
	string tmp = "username=" + database->getStringForKey("username") + "&password=" + database->getStringForKey("password");
	const char* postData = tmp.c_str();
	request->setRequestData(postData, strlen(postData));
	request->setTag("POST test");

	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
	
}

// 自动登录请求结束回调函数
void LoginScene::onAutoLoginHttpCompleted(HttpClient *sender, HttpResponse* response) {
	if (!response) {
		return;
	}
	
	std::vector<char>* headBuffer = response->getResponseHeader();
	std::vector<char>* bodyBuffer = response->getResponseData();
	Global::loginHead = string(Global::toString(headBuffer));
	Global::loginBody = string(Global::toString(bodyBuffer));

	if (!response->isSucceed()) {
		errorField->setString(Global::loginBody);
		return;
	}

	// 获取文件中的用户名
	Global::username = database->getStringForKey("username");
	Global::maxscore = database->getIntegerForKey("maxscore");
	database->setStringForKey("maxscore", Global::loginBody);

	auto selectScene = SelectScene::createScene();
	Director::getInstance()->replaceScene(selectScene);

	usernameField->setString("");
	passwordField->setString("");
}
