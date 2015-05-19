#pragma once
#include "cocos2d.h"
#include <vector>

using namespace cocos2d;
using namespace std;

class Background : public Node
{
	float _time;
	float _currentTime;
	Color4F _colorBg;
	Color4F _colorBgTarget;
	Color4F _colorNight;
	Sprite* _sun;
	Sprite* _noise;
	bool _isRuning;
	Point _sunStart;
	Point _sunEnd;
	CustomCommand _customCommand;
	CustomCommand _customCommandNight;
	vector<Point> _vertices;
	vector<Color4F> _colors;
	vector<Color4F> _colorsNight;

	Sprite* createBackground(ccColor4F color, int width, int height);
	void updateColorTex(Color4F color);
public:
	Background(void);
	~Background(void);
	virtual void update(float delta);
	virtual void draw(Renderer *renderer, const Mat4& transform, uint32_t flags);
	bool initWithColor(Color4F color);
	void changeColor(Color4F color);
	void changeColorWithAnimation(Color4F color, float time);
	void start(float time);
	void reset();
	static Background* createWithColor(Color4F color);
};

