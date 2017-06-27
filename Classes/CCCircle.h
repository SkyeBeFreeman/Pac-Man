#pragma once
#include "cocos2d.h"

USING_NS_CC;

class CCCircle {
public:
	CCCircle(void);
	CCCircle(cocos2d::CCPoint& center, float radius); //提供圆心与半径 初始化圆
	~CCCircle(void);

	bool intersectsRect(cocos2d::CCRect& rect) const;//是否与长方形区域相交
	bool intersectsCircle(CCCircle& circle) const;//是否与圆形区域相交

private:
	CC_SYNTHESIZE(cocos2d::CCPoint, m_center, MCenter);//圆心
	CC_SYNTHESIZE(float, m_radius, MRadius);//半径
};

#define CCCircMake(ccPoint, radius) CCCircle::CCCircle(ccPoint, (float)(radius))