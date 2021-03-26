#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <unordered_map>

#include "Sprites.h"

class CAnimationFrame
{
	LPSPRITE sprite;
	DWORD time;
public:
	CAnimationFrame(LPSPRITE sprite, int time) { this->sprite = sprite, this->time = time; }
	DWORD GetTime() { return time; }
	LPSPRITE GetSprite() { return sprite; }
};

typedef CAnimationFrame* LPANIMATION_FRAME;

class CAnimation
{
	DWORD lastFrameTime;
	int defaultTime;
	int currentFrame;
	vector<LPANIMATION_FRAME> frames;
public:

	CAnimation(int defaultTime = 100) { this->defaultTime = defaultTime, lastFrameTime = -1, currentFrame = -1; }
	void AddAnimation(int spriteId, DWORD time = 0);

	void Render(float x, float y, int alpha = 255);
	void RenderDirect(float x, float y, int direct, int alpha = 255);
};

typedef CAnimation* LPANIMATION;

class CAnimations
{
	static CAnimations* instance;

	unordered_map<int, LPANIMATION> animations;

public:
	void AddAnimations(int id, LPANIMATION ani);
	LPANIMATION Get(int id);
	void Clear();

	static CAnimations* GetInstance();
};

typedef vector<LPANIMATION> AnimationSet;

typedef AnimationSet* LPANIMATION_SET;

class CAnimationSets
{
	static CAnimationSets* instance;

	unordered_map<int, LPANIMATION_SET> animation_sets;

public:
	CAnimationSets();
	void AddAnimationSet(int id, LPANIMATION_SET ani);
	LPANIMATION_SET Get(unsigned int id);

	static CAnimationSets* GetInstance();
};

