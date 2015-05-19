#include "Terrain.h"
#include "base\ccMacros.h"
#include "Config.h"
#include "GameLevelData.h"


Terrain::Terrain(void)
	:_stripes(NULL),
	_world(NULL), _body(NULL),
	_offsetX(0), _formIndexPoint(-1), _toIndexPoint(-1)
{
}

Terrain::~Terrain(void)
{
	CC_SAFE_RELEASE(_stripes);
}

void Terrain::generatePoint()
{
	
}

void Terrain::draw( Renderer *renderer, const Mat4& transform, uint32_t flags )
{
	_customCommand.init(_globalZOrder);
	_customCommand.func = [&]() -> void {
		// draw line top
		ccColor4F color = Color4F(Color4B(138, 142, 83, 255));
		setGLProgram(GLProgramCache::sharedShaderCache()->getGLProgram(GLProgram::SHADER_NAME_POSITION_U_COLOR));
		int color_location = _glProgramState->getGLProgram()->getUniformLocation("u_color");
		CC_NODE_DRAW_SETUP();
		_glProgramState->getGLProgram()->setUniformLocationWith4fv(color_location, &color.r, 1);
		//glEnable(GL_LINE_SMOOTH);
		glLineWidth(10);
		GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION);
		glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, &_borderVertices[0]);
		glDrawArrays(GL_LINE_STRIP, 0, _borderVertices.size());

		// draw terrain
		setGLProgram(GLProgramCache::sharedShaderCache()->getGLProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE));
		CC_NODE_DRAW_SETUP();
		ccGLBindTexture2D(_stripes->getTexture()->getName());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		ccGLEnableVertexAttribs(kCCVertexAttribFlag_Position | kCCVertexAttribFlag_TexCoords);
		glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, &_hillVertices[0]);
		glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, &_hillTexCoord[0]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, _hillVertices.size());
	};
	renderer->addCommand(&_customCommand);
}

bool Terrain::initWithWorld(b2World* world)
{
	if (!Node::init())
		return false;
	
	CCASSERT(world != nullptr, "b2 World null");
	_world = world;

	_stripes = createSpriteWithStrip(Color4F(Color4B(255, 0, 0, 255)), Color4F(Color4B(0, 255, 0, 255)), 512, 512, 6);
	_stripes->retain();

	return true;
}

void Terrain::setOffset( int offset )
{
	offset -= 200;
	_offsetX = offset;
	this->setPositionX(-_offsetX);
	resetPointVertices();
}

void Terrain::resetPointVertices()
{
	Size winSize = Director::sharedDirector()->getWinSize();

	for (int i = preFromIndex; ; i++)
	{
		if (i >= _listPoint.size())
			break;
		if (_listPoint[i].x < _offsetX)
			_formIndexPoint = MAX(i - 1, 0);
		else
			break;
	}
	for (int i = _formIndexPoint; ; i++)
	{
		if (i >= _listPoint.size())
			break;
		if (_listPoint[i].x > _offsetX + winSize.width)
		{
			_toIndexPoint = i + 1;
			break;
		}
	}

	_toIndexPoint = _toIndexPoint - _formIndexPoint < 2 ? _toIndexPoint + 2 : _toIndexPoint;
	// refresh vertices when necessary
	if (preFromIndex != _formIndexPoint || preToIndex != _toIndexPoint)
	{
		_borderVertices.clear();
		_hillVertices.clear();
		_hillTexCoord.clear();
		int const segmentWidth = 5;
		for (int i = _formIndexPoint; i < _toIndexPoint - 1; i++)
		{
			Point p0 = _listPoint[i];
			Point p1 = _listPoint[i + 1];
			Point pt0, pt1;
			int nSegment = floorf((p1.x - p0.x) / segmentWidth);
			float da = M_PI / nSegment;
			float ymid = (p0.y + p1.y) / 2;
			float h = p0.y - ymid;
			pt0 = p0;
			_borderVertices.push_back(pt0);
			bool first = true;
			int texWidth = _stripes->getTexture()->getPixelsWide();
			for (int j = 0; j < nSegment; j++)
			{
				pt1.x = p0.x + j * segmentWidth;
				pt1.y = ymid + h * cosf(j * da);
				_borderVertices.push_back(pt1);

				if (first)
				{
					_hillVertices.push_back(Point(pt0.x, pt0.y));
					_hillTexCoord.push_back(Point(pt0.x / texWidth, 0));

					_hillVertices.push_back(Point(pt0.x, MIN_TERRAIN));
					_hillTexCoord.push_back(Point(pt0.x / texWidth, 1));
					first = false;
				}
				_hillVertices.push_back(Point(pt1.x, pt1.y));
				_hillTexCoord.push_back(Point(pt1.x / texWidth, 0));

				_hillVertices.push_back(Point(pt1.x, MIN_TERRAIN));
				_hillTexCoord.push_back(Point(pt1.x / texWidth, 1));

				pt0 = pt1;
			}
		}
		preFromIndex = _formIndexPoint;
		preToIndex = _toIndexPoint;
		resetBox2dBody();
	}
}

cocos2d::Sprite* Terrain::createSpriteWithStrip( ccColor4F color1, ccColor4F color2, int width, int height, int nStrips )
{
	RenderTexture* rt = RenderTexture::create(width, height);
	rt->beginWithClear(color1.r, color1.g, color1.b, color1.a);

	/*Sprite* noise = Sprite::create("Noise.png");
	noise->setAnchorPoint(ccp(0, 0));
	BlendFunc blenFunc;
	blenFunc.src = GL_DST_COLOR;
	blenFunc.dst = GL_ZERO;
	noise->setBlendFunc(blenFunc);*/
	//noise->visit();
	
	_customCommand2.init(_globalZOrder);
	_customCommand2.func = [=]()->void {
		vector<Point> vertices;
		vector<ccColor4F> colors;

		float dx = width / nStrips;
		int n = ceil(nStrips * (height / (float)width));
		float px1, px2;
		for (int i = 0; i < n ; i++)
		{
			px1 = i * 2 * dx;
			px2 = (i * 2 + 1) * dx;
			vertices.push_back(Point(px1, height));
			vertices.push_back(Point(px1 - width, 0));
			vertices.push_back(Point(px2, height));
			vertices.push_back(Point(px1 - width, 0));
			vertices.push_back(Point(px2, height));
			vertices.push_back(Point(px2 - width, 0));
			for (int j = 0; j < 6; j++)
				colors.push_back(color2);
		}
		setGLProgram(GLProgramCache::sharedShaderCache()->getGLProgram(GLProgram::SHADER_NAME_POSITION_COLOR));
		CC_NODE_DRAW_SETUP();
		ccGLEnableVertexAttribs(kCCVertexAttribFlag_Position | kCCVertexAttribFlag_Color);
		glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, &vertices[0]);
		glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_FLOAT, GL_TRUE, 0, &colors[0]);
		glBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		// gratient
		float boderA = 0.0f;
		vertices.clear();
		colors.clear();
		vertices.push_back(ccp(0, 0));
		colors.push_back(ccColor4F(ccColor4B(200, 200, 200, 255)));
		vertices.push_back(ccp(width, 0));
		colors.push_back(ccColor4F(ccColor4B(200, 200, 200, 255)));
		vertices.push_back(ccp(0, height));
		colors.push_back(ccColor4F(ccColor4B(62, 62, 62, 255)));
		vertices.push_back(ccp(width, height));
		colors.push_back(ccColor4F(ccColor4B(62, 62, 62, 255)));

		glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, &vertices[0]);
		glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_FLOAT, GL_TRUE, 0, &colors[0]);
		glBlendFunc(GL_DST_COLOR, GL_SRC_COLOR);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size());

	};
	Director::getInstance()->getRenderer()->addCommand(&_customCommand2);

	rt->end();
	return rt->getSprite();
}

Terrain* Terrain::createWithWorld( b2World* world )
{
	Terrain* result = new Terrain();
	if (result && result->initWithWorld(world))
	{
		result->autorelease();
		return result;
	} else {
		delete result;
		result = NULL;
		return NULL;
	}
}

void Terrain::resetBox2dBody()
{
	if (_body)
		_world->DestroyBody(_body);
	b2BodyDef bDefine;
	bDefine.position.Set(0, 0);

	_body = _world->CreateBody(&bDefine);

	b2EdgeShape shape;
	b2Vec2 p1, p2;
	Point cp1, cp2;
	for (int i = 0; i < _borderVertices.size() - 1; i++)
	{
		cp1 = _borderVertices[i];
		cp2 = _borderVertices[i+1];
		p1.Set(cp1.x / PTM_RATIO, cp2.y / PTM_RATIO);
		p2.Set(cp2.x / PTM_RATIO, cp2.y / PTM_RATIO);
		shape.Set(p1, p2);
		_body->CreateFixture(&shape, 0);
	}

}

Point Terrain::getStartPoint()
{
	return ccp(_listPoint[2].x, _listPoint[3].y + 200);
}

int Terrain::getOffset()
{
	return _offsetX;
}

void Terrain::parseTerrain( GameLevelData* gameData )
{
	_listPoint = gameData->terrain;
	setPositionY(gameData->terrainY);
	// reset;
	preFromIndex = 0;
	preToIndex = 0;
	_formIndexPoint = 0;
	_toIndexPoint = 0;
	_offsetX = 0;
	_stripes->release();
	_stripes = NULL;
	//_stripes = createSpriteWithStrip(gameData.colorStrip1, gameData.colorStrip2, 512, 512, gameData.nStrips);
	_stripes = gameData->stripsSprite;
	_stripes->retain();
	resetPointVertices();
}

Point Terrain::getEndPoint()
{
	return _listPoint[_listPoint.size() - 1];
}

int Terrain::preToIndex = 0;

int Terrain::preFromIndex = 0;
