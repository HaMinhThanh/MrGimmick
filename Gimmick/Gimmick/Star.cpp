#include "Star.h"
#include"Gimmick.h"
#include"PlayScene.h"
#include"Game.h"

#define STAR_ANIMATION_SET		2


CStar::CStar()
{
	//SetState(STAR_STATE_ACTIVITY);

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(2);

	this->SetAnimationSet(ani_set);

	isFinish = false;
	isActive = false;

	x = y = 0;

	vy = 0;
	vx = 0;
}

void CStar::Render()
{
	if (isActive == false) return;

	animation_set->at(0)->Render(x, y);

	//RenderBoundingBox();
}

void CStar::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case STAR_GOING_RIGHT:

		vx = STAR_SPEED_X;
		vy = -STAR_SPEED_Y;

		if (acting == 0)
			StartActive();

		break;

	case STAR_GOING_LEFT:

		vx = -STAR_SPEED_X;
		vy = -STAR_SPEED_Y;

		if (acting == 0)
			StartActive();

		break;

	case STAR_STATE_HIDDEN:

		vx = vy = 0;
		isActive = false;

		break;
	}
}

void CStar::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (isActive == false) {

		l = t = r = b = 0;
		return;
	}

	l = x;
	t = y;
	r = x + STAR_BBOX_WIDTH;
	b = y + STAR_BBOX_HEIGHT;
}

void CStar::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{	

	CGameObject::Update(dt, coObjects);

	vy += STAR_GRAVITY * dt;

	if (acting == 1) {

		if (GetTickCount() - time_acting > STAR_UNTOUCHABLE_TIME)
		{
			time_acting = 0;
			acting = 0;

			isFinish = true;
			SetState(STAR_STATE_HIDDEN);
		}
	}

	vector<LPGAMEOBJECT> Bricks;
	Bricks.clear();

	for (UINT i = 0; i < coObjects->size(); i++)
		if (dynamic_cast<CBrick*>(coObjects->at(i)))
			Bricks.push_back(coObjects->at(i));

	vector<LPCOLLISIONEVENT>  coEvents;
	vector<LPCOLLISIONEVENT>  coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(&Bricks, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx, rdy;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx + nx * 0.04f;
		y += min_ty * dy + ny * 0.04f;

		if (nx != 0) vx *= -1;
		if (ny != 0) {

			vy *= -1;

			if (ny < 0) {

				vy += 0.0005f;
			}
			else {
				vy -= 0.0005f;
			}
		}
			
			//vy *= -1;

		//if (vy < -FIREBALL_SPEED_MAX_Y) vy = -FIREBALL_SPEED_MAX_Y;

	}
	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];
	//}
}