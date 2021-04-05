#include "Gimmick.h"
#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Gimmick.h"
#include "Game.h"

#include "Portal.h"
#include "Bomb.h"
#include "Slide.h"
#include "Brick.h"

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

	star = new CStar();

	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
}

void CGimmick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!isSlide)
		vy += GIMMICK_GRAVITY * dt;

	


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

	if (loading == 1) {

		if (GetTickCount() - time_load >= GIMMICK_TIME_LOADING_STAR) {

			loading = 2;
			time_load = 0;

			isCanShot = true;			
		}
	}

	if (isCanShot) {

		star->SetPosition(x, y - STAR_BBOX_HEIGHT + 4);
	}

	if (star != NULL)
		star->Update(dt, coObjects);

	if (loading == 1 )
		load_star->Update(dt, coObjects);


	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	//vy += GIMMICK_GRAVITY * dt;	

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);

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

		if (nx != 0 && ny == 0)
			SetState(GIMMICK_STATE_IDLE);

		//
		// Collision logic with other objects
		//		

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CBomb*>(e->obj) && isSlide == false) {

				if (e->ny != 0 && e->nx == 0)
					vy = 0;
				
			}
			if (dynamic_cast<CSlide*>(e->obj)) {
				/*vx = 0.01f;
				vy = -0.01f;*/
				CSlide* slide = dynamic_cast<CSlide*>(e->obj);

				float l1, t1, r1, b1;
				float l2, t2, r2, b2;

				this->GetBoundingBox(l1, t1, r1, b1);
				slide->GetBoundingBox(l2, t2, r2, b2);

				//if (this->isCollision(l1, t1, r1, b1, l2, t2, r2, b2)) {
				
				if (slide->state == 0) {
					if (tan((slide->x1 - x) / (slide->y2 - y)) <= tan(slide->slidePos)) {

						vx = -0.03f;
						vy = 0.015f;
					}

					isSlide = true;
					isColisionWithBrick = false;
				}
				/*}
				else {
					
					isSlide = false;
				}*/
				
			}
			else {
				isSlide = false;
			}
		}

		if (!isSlide ) {

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

		if (star != NULL)
			star->Render();
		
		if (loading == 1)
			load_star->Render();

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
		isSlide = false;

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

void CGimmick::isCanSlide(vector<LPGAMEOBJECT>& listObj)
{
	float l, t, r, b;
	float l1, t1, r1, b1;

	GetBoundingBox(l, t, r, b);

	for (int i = 0; i < listObj.size(); i++) {

		if (listObj.at(i)->GetState() == SLIDE_TYPE_UP
			|| listObj.at(i)->GetState()== SLIDE_TYPE_DOWN) {

			listObj.at(i)->GetBoundingBox(l1, t1, r1, b1);

			if (CGame::GetInstance()->isCollision(l, t, r, b, l1, t1, r1, b1)) {

				CSlide* slide = dynamic_cast<CSlide*>(listObj.at(i));

				if (slide->GetState() == SLIDE_TYPE_UP) {


				}
				else if (slide->GetState() == SLIDE_TYPE_DOWN) {


				}
			}
		}
	}
}

void CGimmick::ShotStar()
{
	if (isCanShot) {
		
		isCanShot = false;
		star->isActive = true;
		loading = 0;		

		if (nx > 0) {
			
			star->SetState(STAR_GOING_RIGHT);
		}
		else {

			star->SetState(STAR_GOING_LEFT);
		}
	}
}

void CGimmick::isPrepareShot()
{
	if (loading == 0) {

		StarLoading();
	}
}

void CGimmick::SetLoadingStar()
{
	if (load_star == NULL)
		load_star = new LoadingStar(x + 8, y - 8);

	load_star->TurnToBegin(x + 8, y - 8);
}

/*
	Reset Mario status to the beginning state of a scene
*/
void CGimmick::Reset()
{
	
}

