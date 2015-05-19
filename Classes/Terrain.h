#pragma once
#include <cocos2d.h>
#include "Box2D/Box2D.h"
#include <vector>

using namespace cocos2d;
using namespace std;

#define MAX_POINT 200;

class GameLevelData;

class Terrain : public Node
{
	vector<Point> _listPoint;
	vector<Point> _borderVertices;
	vector<Point> _hillVertices;
	vector<Point> _hillTexCoord;
	Sprite* _stripes;
	int _formIndexPoint;
	int _toIndexPoint;
	int _offsetX;

	static int preFromIndex;
	static int preToIndex;

	b2World* _world;
	b2Body* _body;

	CustomCommand _customCommand;
	CustomCommand _customCommand2;

	void generatePoint();
	void resetPointVertices();
	void resetBox2dBody();
	void draw(Renderer *renderer, const Mat4& transform, uint32_t flags);
public:
	Terrain(void);
	~Terrain(void);

	void parseTerrain(GameLevelData* gameData);
	bool initWithWorld(b2World* world);
	void setOffset(int offset);
	int getOffset();
	Point getStartPoint();
	Point getEndPoint();
	static Terrain* createWithWorld(b2World* world);
	Sprite* createSpriteWithStrip(ccColor4F color1, ccColor4F color2, int with, int height, int nStrips);
};

