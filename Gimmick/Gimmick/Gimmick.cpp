#include "Gimmick.h"
#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Gimmick.h"
#include "Game.h"

#include "Portal.h"
#include "Bomb.h"
#include "Slide.h"

CGimmick* CGimmick::_instance = NULL;

CGimmick* CGimmick::GetInstance(float x, float y)
{
	if (_instance == NULL)
		_instance = new CGimmick(x, y);

	return _instance;
}

CGimmick::CGimmick(float x, float y) : CGameObject()
{
	untouchable = 0;
	SetState(GIMMICK_STATE_IDLE);

	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
}

void CGimmick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!isSlide)
		vy += GIMMICK_GRAVITY * dt;

	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	//vy += GIMMICK_GRAVITY * dt;	

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);


	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > GIMMiCK_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	if (jump == true && doubleJump_start != 0)
	{
		if (GetTickCount() - doubleJump_start > 100)
		{
			vy = -MARIO_DOUBLE_JUMP_SPEED;
			doubleJump_start = 0;
		}
	}

	if (maxjumping == 1)
	{
		 if (GetTickCount() - time_maxjumping >= 200)
		{
			maxjumping = 0;
			time_maxjumping = 0;
		}
	}
	else
	{
		maxjumping = 0;
		time_maxjumping = 0;
	}


	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

		// block every object first!
		

		//mario touches ground
		if (ny != 0 && nx == 0)
		{
			if (GetJumping() == 1)
			{
				jump = 0;
				doubleJump_start = 0;

			}

			//CanFall = 0;
		}
		//
		// Collision logic with other objects
		//

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CBomb*>(e->obj)) {

				if (e->ny != 0 && e->nx == 0)
					vy = 0;
			}
			else if (dynamic_cast<CSlide*>(e->obj)) {
				/*vx = 0.01f;
				vy = -0.01f;*/
				CSlide* slide = dynamic_cast<CSlide*>(e->obj);

				if ((slide->x1 - x) / (y - slide->y2) == slide->slidePos) {

					vx = 0.01f;
					vy = -0.01f;
				}

				isSlide = true;
			}
			else {
				isSlide = false;
			}
		}

		if (!isSlide) {
			x += min_tx * dx + nx * 0.4f;
			y += min_ty * dy + ny * 0.4f;

			if (nx != 0) vx = 0;
			if (ny != 0) vy = 0;
		}
		else {
			x += dx;
			y += dy;
		}		
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CGimmick::Render()
{
		int ani = -1;

		if (jump == 1)
		{
			if (nx > 0)
				ani = GIMMICK_ANI_JUMPING_RIGHT;
			else
				ani = GIMMICK_ANI_JUMPING_LEFT;
		}
		else if (state == GIMMICK_STATE_WALKING_RIGHT)
		{
			ani = GIMMICK_ANI_WALKING_RIGHT;
		
		}
		else if (state == GIMMICK_STATE_WALKING_LEFT)
		{
			ani = GIMMICK_ANI_WALKING_LEFT;
		}
		else //if (state == GIMMICK_STATE_IDLE)
		{
			if (nx > 0)
			{
				ani = GIMMICK_ANI_IDLE_RIGHT;
			}
			else
				ani = GIMMICK_ANI_IDLE_LEFT;
		}

		int alpha = 255;
		if (untouchable) alpha = 128;

		animation_set->at(ani)->Render(x, y, alpha);

		RenderBoundingBox();
}

void CGimmick::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case GIMMICK_STATE_WALKING_RIGHT:
		vx = GIMMICK_WALKING_SPEED;
		nx = 1;
		break;
	case GIMMICK_STATE_WALKING_LEFT:
		vx = -GIMMICK_WALKING_SPEED;
		nx = -1;
		break;
	case GIMMICK_STATE_JUMP:
		// TODO: need to check if Mario is *current* on a platform before allowing to jump again
		vy = -GIMMICK_JUMP_SPEED_Y;
		break;
	case GIMMICK_STATE_IDLE:
		vx = 0;
		break;

	case MARIO_STATE_JUMP_HIGH_SPEED:
		vy = -MARIO_JUMP_HIGHT_SPEED_Y;
		break;

	}
}

void CGimmick::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y + GIMMICK_BBOX_HORN;

	if (jump == 1){
	
		right = x + GIMMICK_BBOX_WIDTH;
		bottom = y + GIMMICK_JUMP_BBOX_HEIGHT;
	}

	right = x + GIMMICK_BBOX_WIDTH;
	bottom = y + GIMMICK_BBOX_HEIGHT;

}

/*
	Reset Mario status to the beginning state of a scene
*/
void CGimmick::Reset()
{
	
}

