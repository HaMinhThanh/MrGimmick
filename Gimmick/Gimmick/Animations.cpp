#include "Animations.h"
#include "Utils.h"

CAnimationSets* CAnimationSets::instance = NULL;
void CAnimation::AddAnimation(int spriteId, DWORD time)
{
    int t = time;
    if (time == 0) t = this->defaultTime;

    LPSPRITE sprite = CSprites::GetInstance()->Get(spriteId);
    LPANIMATION_FRAME frame = new CAnimationFrame(sprite, t);
    frames.push_back(frame);
}

void CAnimation::Render(float x, float y, int alpha)
{
	DWORD now = GetTickCount();
	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		if (now - lastFrameTime > t)
		{
			currentFrame++;
			lastFrameTime = now;
			if (currentFrame == frames.size()) currentFrame = 0;
		}
	}
	frames[currentFrame]->GetSprite()->Draw(x, y, alpha);
}

void CAnimation::RenderDirect(float x, float y, int direct, int alpha)
{
	DWORD now = GetTickCount();
	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		if (now - lastFrameTime > t)
		{
			currentFrame++;
			lastFrameTime = now;
			if (currentFrame == frames.size()) currentFrame = 0;
			//DebugOut(L"now: %d, lastFrameTime: %d, t: %d\n", now, lastFrameTime, t);
		}
	}
	if (direct < 0)
		frames[currentFrame]->GetSprite()->Draw(x, y, alpha);
	else
		frames[currentFrame]->GetSprite()->DrawFlipX(x, y, alpha);
}

CAnimations* CAnimations::instance = NULL;

void CAnimations::AddAnimations(int id, LPANIMATION ani)
{
	animations[id] = ani;
}

LPANIMATION CAnimations::Get(int id)
{
	LPANIMATION ani = animations[id];
	if (ani == NULL)
		DebugOut(L"[ERROR] Failed to find animation id: %d\n", id);
	return ani;
}

void CAnimations::Clear()
{
	for (auto x : animations)
	{
		LPANIMATION ani = x.second;
		delete ani;
	}

	animations.clear();
}

CAnimations* CAnimations::GetInstance()
{
	if (instance == NULL) instance = new CAnimations();
	return instance;
}

CAnimationSets::CAnimationSets()
{
}

void CAnimationSets::AddAnimationSet(int id, LPANIMATION_SET ani)
{
	animation_sets[id] = ani;
}

LPANIMATION_SET CAnimationSets::Get(unsigned int id)
{
	LPANIMATION_SET ani_set = animation_sets[id];
	if (ani_set == NULL)
		DebugOut(L"[ERROR] Failed to find animation set id: %d\n", id);

	return ani_set;
}

CAnimationSets* CAnimationSets::GetInstance()
{
	if (instance == NULL) instance = new CAnimationSets();
	return instance;
}
