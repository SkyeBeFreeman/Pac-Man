#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <string>
#include "Global.h"
using namespace cocos2d;
using namespace cocos2d::ui;

#include "network/HttpClient.h"
using namespace cocos2d::network;

using std::string;

class GameScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);

	void addMap();							// 加载地图
	void addPlayer();						// 加入玩家
	void addBeans();						// 加入大小豆子
	Sprite* addBigBeans(Vec2 loaction);		// 加入大豆子
	void addEnemy();						// 加入怪物
	Sprite *addEnemyByColor(int color, Vec2 locaion);	// 加入不同颜色怪物

	void preloadMusic();					// 加载音乐
	void playBgm();							// 播放背景音乐

	void movePlayer();						// 通过按键移动玩家
	void enemyMove();						// 怪物移动----不同颜色的怪物移动方式不一样
	void pinkEnemyMove(float time);			
	void blueEnemyMove(float time);
	void orangeEnemyMove(float time);
	void redEnemyMove(float time);

	void addKeyboardListener();				// 添加键盘事件监听器
	void onKeyPressed(EventKeyboard::KeyCode code, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode code, Event* event);

	void getReward();		// 玩家获得所有怪物静止三秒的奖励
	void stillEnermys();	// 怪物静止
	void darkenEnermys();	// 所有怪物颜色变深
	void largenPlayer();	// 把玩家变大一倍
	void recoverSprites();	// 恢复所有精灵
	void recoverEnermys();	// 怪物颜色恢复
	void playerRecover();	// 玩家恢复正常大小
	

	bool collide(Sprite *s1, Sprite *s2);	// 精灵之间的碰撞
	bool collide(Sprite *s1, TMXObjectGroup *w);	// 玩家和墙的碰撞

	void update(float f);

	// 跳转到结算页面
	void toEndScene(cocos2d::Ref *pSender, bool isWin);
	void unscheduleAll();					// 取消所有调度器
	void quitEvent(cocos2d::Ref* pSender);
	void submitEvent();
	void onSubmitHttpCompleted(HttpClient *sender, HttpResponse* response);

private:
	Vec2 origin;				// 游戏界面原点
	Size visibleSize;			// 界面大小
	Label * score_field;		// 得分板
	///////////////////////////

	TMXTiledMap *map;			// 地图
	TMXObjectGroup *wall;		// 地图周围的墙
	// 地图左下角坐标
	int map_x;
	int map_y;
	// 地图的高宽
	int width;
	int height;
	///////////////////////////

	// 玩家
	Sprite *player;
	// 玩家移动的四个方向
	enum MOVE { UP, DOWN, LEFT, RIGHT };
	MOVE move = MOVE::LEFT;		// 玩家最开始的方向
	bool isMove;				// 玩家是否移动
	bool isRewarded;			// 玩家是否获得奖励
	///////////////////////////

	// 怪物
	Sprite *enemys[4];
	// 怪物颜色
	enum ENERMYCOLOR { BLUE, RED, ORANGE, PINK };
	string enermy_color[4] = { "blue", "red", "orange", "pink" };
	Sprite *darkEnermys[4];
	//////////////////////////

	// 所有小型豆子
	std::vector<Sprite*> beans;
	// 大豆子
	Sprite *bigBeans[4];

	
	
};

