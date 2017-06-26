#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <string>
#include "Global.h"
using namespace cocos2d::ui;

#include "network/HttpClient.h"
using namespace cocos2d::network;

using std::string;

class RankScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(RankScene);

	void submitEvent();

	void onSubmitHttpCompleted(HttpClient *sender, HttpResponse* response);

	void rankEvent();

	void onRankHttpCompleted(HttpClient *sender, HttpResponse* response);


private:
    float visibleHeight;
    float visibleWidth;
	TextField * username_field;
    TextField * score_field;
	TextField * ranking_field;
    TextField * rank_field;
	TextField * head_field;
	TextField * body_field;
    Button *submit_button;
    Button *rank_button;
};

