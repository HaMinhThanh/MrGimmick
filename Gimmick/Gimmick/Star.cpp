#include "Star.h"
#include"Gimmick.h"
#include"PlayScene.h"
#include"Game.h"


CStar::CStar()
{
	SetState(STAR_STATE_ACTIVITY);
}

void CStar::Render()
{
	if (Fly == 1)
		animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}

void CStar::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + STAR_BBOX_WIDTH;
	b = y+STAR_BBOX_HEIGHT;
}

void CStar::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGimmick* gimmick = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	CGameObject::Update(dt, coObjects);
	if (GetTickCount() - Firinng_start > 1300 && Fly == 1)
	{
		vx = 0;
		Fly = 0;
	}

	if (gimmick->GetHoldStar() == 1)
	{
		x = gimmick->x;
		y = gimmick->y-10;
	}
	
	else if (gimmick->GetShoot() == 1 && GetFly() == 0 && gimmick->GetHoldStar() != 1)
	{
		SetPosition(gimmick->x, gimmick->y);
		SetFly(1);
		SetY(gimmick->y);
		FiringStart();
		gimmick->SetShoot(0);
		vy = 0.9f;
		vx = 0.15f;

	}

	if (Fly == 1 && gimmick->GetHoldStar() != 1)
	{
		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;
		coEvents.clear();
		CalcPotentialCollisions(coObjects, coEvents);
		if (coEvents.size() == 0)
		{
			if (gimmick->nx > 0)
			{
				x += dx;
				y += dy;
				//dau tien vy >0, dap xuong dat roi thi am
				if (y < this->Y && vy < 0)
				{
					vy = -vy;
				}
			}
			else
			{
				x -= dx;
				y += dy;
				if (y < this->Y && vy < 0)
				{
					vy = -vy;
				}
			}


		}
		else
		{
			float min_tx, min_ty, nx = 0, ny;
			float rdx = 0;
			float rdy = 0;

			// TODO: This is a very ugly designed function!!!!
			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

			if (ny != 0)
			{
				vy = -vy;
				/*if (ny > 0)
				{
					Y = y;
				}*/
			}
			else if (nx != 0)
			{
				vx = -vx;
				
			}

			y += min_ty * dy + ny * 0.4f;
			x += min_tx * dx + nx * 0.4f;
		}
		
	}
}