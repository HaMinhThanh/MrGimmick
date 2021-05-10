#pragma once

#include "GameObject.h"

#define LOADING_SPEED_X1	0.01f
#define LOADING_SPEED_X2	0
#define LOADING_SPEED_X3	-0.01
#define LOADING_SPEED_X4	-0.01
#define LOADING_SPEED_X5	0.01

#define LOADING_SPEED_Y1	0.01
#define LOADING_SPEED_Y2	0.005
#define LOADING_SPEED_Y3	0.005
#define LOADING_SPEED_Y4	-0.01
#define LOADING_SPEED_Y5	-0.01

#define LOADING_STAR_ANIMATION	21


class CLoadingStar : public CGameObject
{
public:

	CLoadingStar(float x, float y);
	~CLoadingStar();

	float x1, x2, x3, x4, x5;
	float y1, y2, y3, y4, y5;

	float backupX, backupY;

	void TurnToBegin(float x, float y);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

};

