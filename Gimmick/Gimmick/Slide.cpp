#include "Slide.h"

CSlide::CSlide(float _x1, float _y1, float _x2, float _y2, int _state)
{
	x1 = _x1;
	y1 = _y1;

	x2 = _x2;
	y2 = _y2;

	state = _state;
	
	slidePos = (x1 - x2) / (y1 - y2);
}

CSlide::~CSlide()
{
}

void CSlide::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void CSlide::Render()
{
}

void CSlide::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x1;
	right = x2;

	top = y1 < y2 ? y1 : y2;	
	bottom = y1 < y2 ? y2 : y1;
}
