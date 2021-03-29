#include "Bomb.h"

CBomb::CBomb(float _x, float _y)
{
	this->x = _x;
	this->y = _y;
}

CBomb::~CBomb()
{
}

void CBomb::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
}

void CBomb::Render()
{
	animation_set->at(0)->Render(x, y);
}

void CBomb::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BOMB_BBOX_WIDTH;
	bottom = y + BOMB_BBOX_HEIGHT;
}
