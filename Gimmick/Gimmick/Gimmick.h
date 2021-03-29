#pragma once
#include "GameObject.h"

#define GIMMICK_WALKING_SPEED		0.09f 
//0.1f
#define GIMMICK_JUMP_SPEED_Y		0.24f
#define MARIO_JUMP_SPEED_Y_2		-0.1f
#define MARIO_DOUBLE_JUMP_SPEED	0.245f
#define MARIO_JUMP_HIGHT_SPEED_Y		0.6f

#define GIMMICK_JUMP_DEFLECT_SPEED 0.2f
#define GIMMICK_GRAVITY			0.0008f
#define GIMMICK_DIE_DEFLECT_SPEED	 0.5f
#define GIMMiCK_UNTOUCHABLE_TIME 5000

#define GIMMICK_STATE_IDLE			0
#define GIMMICK_STATE_WALKING_RIGHT	100
#define GIMMICK_STATE_WALKING_LEFT	200
#define GIMMICK_STATE_JUMP			300
#define GIMMICK_STATE_COLISION				400
#define MARIO_STATE_JUMP_HIGH_SPEED	500

#define GIMMICK_ANI_IDLE_RIGHT		0
#define GIMMICK_ANI_IDLE_LEFT			1
#define GIMMICK_ANI_WALKING_RIGHT		2
#define GIMMICK_ANI_WALKING_LEFT		3
#define GIMMICK_ANI_JUMPING_RIGHT		4
#define GIMMICK_ANI_JUMPING_LEFT		5
#define GIMMICK_ANI_COLISION_RIGHT		6
#define GIMMICK_ANI_COLISION_LEFT		7

#define GIMMICK_BBOX_WIDTH  16
#define GIMMICK_BBOX_HEIGHT 19
#define GIMMICK_JUMP_BBOX_HEIGHT 24


class CGimmick : public CGameObject
{
	int untouchable;
	DWORD untouchable_start;

	float start_x;			// initial position of Mario at scene
	float start_y;
	int jump = 0;
	int maxjumping = 0;
	bool isMaxJumping = false;
public:
	CGimmick(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	void Reset();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	int Jumping() { return jump; };
	void SetJumping(int jump) { this->jump = jump; };
	int GetJumping() { return jump; };
	DWORD time_maxjumping = 0;
	// Check for double jump
	DWORD doubleJump_start = 0;
	void StartJumpingMax() { maxjumping = 1, time_maxjumping = GetTickCount(); }
	void ResetDoubleJumpStart() { doubleJump_start = 0; }
	void SetDoubleJumpStart() { doubleJump_start = GetTickCount(); }
	DWORD GetDoubleJumpStart() { return doubleJump_start; }
};

