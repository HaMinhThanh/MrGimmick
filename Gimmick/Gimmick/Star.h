#pragma once
#include "GameObject.h"

#define STAR_STATE_ACTIVITY 100
#define STAR_STATE_INACTIVITY 200
#define STAR_STATE_HOLDING 300
#define STAR_FLY_SPEED 0.05f;

#define STAR_ANI_FLY 0

#define STAR_BBOX_WIDTH 16
#define STAR_BBOX_HEIGHT 16

class CStar : public CGameObject
{
	int Fly = 0;
	//int count = 0;
	float Y;// cai thanh lam moc de bong nay xuong
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
public:
	int GetFly() { return Fly; };
	void SetFly(int fly) { Fly = fly; };
	DWORD Firinng_start;
	void FiringStart() { Fly = 1; Firinng_start = GetTickCount(); };
	void SetY(float y) { Y = y+30.0f; };
	CStar();
};

