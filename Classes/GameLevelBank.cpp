#include "GameLevelBank.h"
#include "Config.h"


GameLevelBank::GameLevelBank(void)
{
	_initialized = false;
}


GameLevelBank::~GameLevelBank(void)
{
}

GameLevelBank* GameLevelBank::getInstance()
{
	if (_instance == nullptr) {
		_instance = new	GameLevelBank();
		_instance->lazy_initialize();
	}
	return _instance;
}


void GameLevelBank::lazy_initialize()
{
	if (_initialized)
		return;
	/*for (int i = 0; i < 10; i++)
	{
		float time = 60;
		vector<Point> terrain = genarateTerrain();
		GameLevelData game;
		game.terrain = terrain;
		game.time = time;
		_levelData.push_back(game);
	}*/
	float time = 60;
	vector<Point> terrain;

	{ // map xanh
		terrain = genarateTerrainWithHeight(100, 20, 60, 80, 200);
		GameLevelData* game1 = new GameLevelData();;
		game1->terrain = terrain;
		game1->time = time;
		game1->colorStrip1 = Color4F(Color4B(85, 179, 65, 255));
		game1->colorStrip2 = Color4F(Color4B(169, 223, 22, 255));
		game1->colorBg = Color4F(Color4B(132, 194, 215, 255));
		game1->nStrips = 6;
		game1->terrainY = -80;
		_levelData.push_back(game1);
	}
	{// map tim
		terrain = genarateTerrainWithHeight(100, 40);
		GameLevelData* game1 = new GameLevelData();
		game1->terrain = terrain;
		game1->time = time - 10;
		game1->colorStrip1 = Color4F(Color4B(168, 45, 60, 255));
		game1->colorStrip2 = Color4F(Color4B(124, 86, 118, 255));
		game1->colorBg = Color4F(Color4B(205, 165, 166, 255));
		game1->nStrips = 10;
		game1->terrainY = -70;
		_levelData.push_back(game1);
	}
	{// map vang
		terrain = genarateTerrainWithHeight(120, 60);
		GameLevelData* game1 = new GameLevelData();
		game1->terrain = terrain;
		game1->time = time - 30;
		game1->colorStrip1 = Color4F(Color4B(82, 152, 143, 255));
		game1->colorStrip2 = Color4F(Color4B(226, 181, 58, 255));
		game1->colorBg = Color4F(Color4B(143, 211, 190, 255));
		game1->nStrips = 6;
		game1->terrainY = -30;
		_levelData.push_back(game1);
	}

	_initialized = true;
}

vector<Point> GameLevelBank::genarateTerrain(int numPoint)
{
	vector<Point> result;
	
	Size winSize = Director::getInstance()->getWinSize();
	float minDX = 160;
	float minDY = 60;
	int rangeDX = 80;
	int rangeDY = 40;

	float x = -minDX;
	float y = winSize.height/2;

	float dy, ny;
	float sign = 1; // +1 - going up, -1 - going  down
	float paddingTop = 20;
	float paddingBottom = 20;

	for (int i = 0; i < numPoint; i++)
	{
		result.push_back(Point(x, y));
		if (i == 0) {
			x = 0;
			y = winSize.height/2;
		} else {
			x += rand()%rangeDX+minDX;
			while(true) {
				dy = rand()%rangeDY+minDY;
				ny = y + dy*sign;
				if(ny < winSize.height-paddingTop && ny > paddingBottom) {
					break;
				}
			}
			y = ny;
		}
		sign *= -1;
	}
	// add first and last point
	result.insert(result.begin(), Point(result[0].x - 150, MIN_TERRAIN));
	result.push_back(Point(result[result.size() - 1].x + 150, MIN_TERRAIN));

	return result;
}

GameLevelData* GameLevelBank::getGameLevelData( int levelId )
{
	CCASSERT(levelId >= 0 && levelId < _levelData.size(), "Invalid level id input");
	return _levelData.at(levelId);
}

void GameLevelBank::initAllStrip()
{
	for (int i = 0; i < _levelData.size(); i++)
	{
		_levelData[i]->initStrip();
	}
}

vector<Point> GameLevelBank::genarateTerrainWithHeight( int numPoint, float height, float minDy, float width, float minX )
{
	vector<Point> result;

	Size winSize = Director::getInstance()->getWinSize();
	float minDX = minX;
	float minDY = minDy;
	int rangeDX = width;
	int rangeDY = height;

	float x = -minDX;
	float y = winSize.height/2;

	float dy, ny;
	float sign = 1; // +1 - going up, -1 - going  down
	float paddingTop = 20;
	float paddingBottom = 20;

	for (int i = 0; i < numPoint; i++)
	{
		result.push_back(Point(x, y));
		if (i == 0) {
			x = 0;
			y = winSize.height/2;
		} else {
			x += rand()%rangeDX+minDX;
			while(true) {
				dy = rand()%rangeDY+minDY;
				ny = y + dy*sign;
				if(ny < winSize.height-paddingTop && ny > paddingBottom) {
					break;
				}
			}
			y = ny;
		}
		sign *= -1;
	}
	// add first and last point
	result.insert(result.begin(), Point(result[0].x - 150, MIN_TERRAIN));
	result.push_back(Point(result[result.size() - 1].x + 150, MIN_TERRAIN));

	return result;
}

GameLevelBank* GameLevelBank::_instance = nullptr;
