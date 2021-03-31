#pragma once

#include "GameObject.h"

#define SLIDE_SPEED_X
#define SLIDE_GRAVITY

class CSlide: public CGameObject
{
public:

	float x1, x2;
	float y1, y2;

	int state;
	int trendX;
	int trendY;

	float slidePos;

	CSlide(float x1, float x2, float y1, float y2, int state);
	~CSlide();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	
};

