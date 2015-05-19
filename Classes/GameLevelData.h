#pragma once
#include "cocos2d.h"
#include "vector"

using namespace cocos2d;
using namespace std;

class StripCreateNode;

class GameLevelData : public Ref
{
public:
	GameLevelData();
	~GameLevelData();
	vector<Point> terrain;
	float time;
	Color4F colorBg;
	Color4F colorStrip1;
	Color4F colorStrip2;
	Sprite* stripsSprite;
	int nStrips;
	StripCreateNode* stripCreate;
	int terrainY;

	void initStrip();
};