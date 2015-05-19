#include "Background.h"
#include "ChangeColor.h"


Background::Background(void)
	: _sun(NULL)
{
	_isRuning = false;
	_time = -1;
}


Background::~Background(void)
{
	CC_SAFE_RELEASE(_sun);
}

bool Background::initWithColor( Color4F color )
{
	if (!Node::init())
		return false;
	setGLProgram(GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_COLOR));

	Size winSize = Director::sharedDirector()->getWinSize();
	// init vertices color
	// 
	_vertices.push_back(Point(0, 0));
	_vertices.push_back(Point(0, winSize.height));
	_vertices.push_back(Point(winSize.width, 0));
	_vertices.push_back(Point(winSize.width, winSize.height));

	_sunStart = Point(- winSize.width * 1 / 3, winSize.height * 3 / 2);
	_sunEnd = Point(winSize.width * 2 / 3, - winSize.height * 1 /3);

	// color
	_colorBg = _colorBgTarget = color;
	_colorNight = Color4F::WHITE;
	for (int i = 0; i < 4; i++)
	{
		_colors.push_back(_colorBg);
	}

	_colorsNight.clear();
	for (int i = 0; i < 4; i++)
	{
		Color4F color = Color4F(1, 1, 1, 1);
		_colorsNight.push_back(color);
	}
	/*Sprite* bg = createBackground(color, winSize.width, winSize.height);
	bg->setAnchorPoint(Point::ZERO);
	addChild(bg);*/
	_sun = Sprite::create("sun.png");
	_sun->setPosition(_sunStart);
	_sun->setScale(4);
	_sun->retain();
	addChild(_sun);

	_noise = Sprite::create("Noise.png");
	_noise->setScaleX(float(winSize.width) / _noise->getTexture()->getPixelsWide());
	_noise->setScaleY(float(winSize.height) / _noise->getTexture()->getPixelsHigh());
	BlendFunc blenFunc;
	blenFunc.src = GL_ZERO;
	blenFunc.dst = GL_ONE;
	_noise->setBlendFunc(blenFunc);
	_noise->setAnchorPoint(ccp(0, 0));
	_noise->retain();

	scheduleUpdate();
	return true;
}

Background* Background::createWithColor( Color4F color )
{
	Background* result = new Background();
	if (result && result->initWithColor(color))
	{
		result->autorelease();
		return result;
	} else {
		delete result;
		result = NULL;
		return NULL;
	}
}

void Background::start(float time)
{
	if (_isRuning)
		return;
	_time = time;
	_currentTime = 0.0f;

	Size winSize = Director::sharedDirector()->getWinSize();
	ccBezierConfig c;
	c.controlPoint_1 = Point(winSize.width / 2, winSize.height);
	c.controlPoint_2 = Point(winSize.width / 2, winSize.height);
	c.endPosition = _sunEnd;
	_sun->stopActionByTag(11);
	_sun->runAction(BezierTo::create(time, c));

	_isRuning = true;
}

void Background::reset()
{
	const float timeDelay = 2.0f;
	Size winSize = Director::getInstance()->getWinSize();
	// rollback sun
	ccBezierConfig c;
	c.controlPoint_1 = Point(winSize.width / 2, winSize.height);
	c.controlPoint_2 = Point(winSize.width / 2, winSize.height);
	c.endPosition = Point(_sunStart);
	_sun->stopAllActions();
	_isRuning = false;
	_sun->runAction(BezierTo::create(timeDelay, c));
	//
	_time = -1.0f;
	this->runAction(ChangeColor::create(timeDelay, &_colorNight, Color4F(1, 1, 1, 1)));
}

Sprite* Background::createBackground( ccColor4F color, int width, int height )
{
	RenderTexture* rt = RenderTexture::create(width, height);
	rt->beginWithClear(color.r, color.g, color.b, color.a);
	Sprite* noise = Sprite::create("Noise.png");
	noise->setScaleX(float(width) / noise->getTexture()->getPixelsWide());
	noise->setScaleY(float(height) / noise->getTexture()->getPixelsHigh());
	BlendFunc blenFunc;
	blenFunc.src = GL_DST_COLOR;
	blenFunc.dst = GL_ONE;
	noise->setBlendFunc(blenFunc);
	noise->setAnchorPoint(ccp(0, 0));
	noise->visit();
	rt->end();
	return Sprite::createWithTexture(rt->getSprite()->getTexture());
}

void Background::draw( Renderer *renderer, const Mat4& transform, uint32_t flags )
{
	_customCommand.init(_globalZOrder);
	_customCommand.func = [&]()->void {
		CC_NODE_DRAW_SETUP();
		ccGLEnableVertexAttribs(kCCVertexAttribFlag_Position | kCCVertexAttribFlag_Color);
		glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, &_vertices[0]);
		glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_FLOAT, GL_TRUE, 0, &_colors[0]);
		glBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, _vertices.size());
	};
	renderer->addCommand(&_customCommand);
	_noise->visit(renderer, transform, flags);

	_customCommandNight.init(_globalZOrder);
	_customCommandNight.func = [&]()->void {
		CC_NODE_DRAW_SETUP();
		ccGLEnableVertexAttribs(kCCVertexAttribFlag_Position | kCCVertexAttribFlag_Color);
		glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, &_vertices[0]);
		glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_FLOAT, GL_TRUE, 0, &_colorsNight[0]);
		glBlendFunc(GL_DST_COLOR, GL_ZERO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, _vertices.size());
	};
	renderer->addCommand(&_customCommandNight);
}

void Background::update( float delta )
{
	Node::update(delta);
	_currentTime += delta;
	if (_time != -1)
	{
		float qua = _time * 4 / 5.0f;
		if (_currentTime > qua)
		{
			float c = (_currentTime - qua) / (_time - qua);
			c = 1 - c;

			_colorNight = Color4F(c, c, c, c);
			
		}
	}

	if (_colorBg != _colorBgTarget)
	{
		_colorBg = _colorBgTarget;
		updateColorTex(_colorBg);
	}

	if (_colorNight != _colorsNight[0])
	{
		//update color coord
		_colorsNight.clear();
		for (int i = 0; i < 4; i++)
		{
			_colorsNight.push_back(_colorNight);
		}
	}
}

void Background::changeColor( Color4F color )
{
	_colorBg = _colorBgTarget = color;
	updateColorTex(color);
}

void Background::updateColorTex( Color4F color )
{
	_colors.clear();
	for (int i = 0; i < 4; i++)
		_colors.push_back(color);
}

void Background::changeColorWithAnimation( Color4F color, float time )
{
	this->runAction(ChangeColor::create(time, &_colorBgTarget, color));
}
