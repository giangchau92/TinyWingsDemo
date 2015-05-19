#pragma once
#include "cocos2d.h"
#include <vector>
#include "GameLevelData.h"

using namespace cocos2d;
using namespace std;

class GameLevelBank
{
	static GameLevelBank* _instance;
	vector<GameLevelData*> _levelData;
	vector<Point> genarateTerrain(int numPoint);
	vector<Point> genarateTerrainWithHeight(int numPoint, float height = 40, float minDy = 60, float width = 80, float minX = 160);
	bool _initialized;
public:
	GameLevelBank(void);
	~GameLevelBank(void);

	/**
	 * Init game data level
	 */
	void lazy_initialize();
	GameLevelData* getGameLevelData(int levelId);
	void initAllStrip();
	static GameLevelBank* getInstance();
};

