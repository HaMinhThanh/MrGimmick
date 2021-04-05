#pragma once
#include "GameObject.h"

#define STAR_STATE_ACTIVITY 100
#define STAR_STATE_INACTIVITY 200
#define STAR_STATE_HOLDING 300
#define STAR_FLY_SPEED 0.05f;

#define STAR_ANI_FLY 0

#define STAR_BBOX_WIDTH 16
#define STAR_BBOX_HEIGHT 16

//
#define STAR_BBOX_WIDTH		4
#define STAR_BBOX_HEIGHT	5	

#define STAR_GOING_RIGHT	1
#define STAR_GOING_LEFT		2	
#define STAR_STATE_HIDDEN   3	

#define STAR_GRAVITY	0.0005f
#define STAR_SPEED_X	0.1f
#define STAR_SPEED_Y	0.0001f	

#define STAR_SPEED_MAX_Y	0.0001f

#define STAR_UNTOUCHABLE_TIME	3000

class CStar : public CGameObject
{
public:

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);

public:
	/*int GetFly() { return Fly; };
	void SetFly(int fly) { Fly = fly; };
	DWORD Firinng_start;
	void FiringStart() { Fly = 1; Firinng_start = GetTickCount(); };
	void SetY(float y) { Y = y+30.0f; };
	CStar();*/

	CStar();
	~CStar();

	bool isFinish;
	bool isActive;

	int acting = 0;
	DWORD time_acting;
	void StartActive() { time_acting = GetTickCount(); acting = 1; }

};

