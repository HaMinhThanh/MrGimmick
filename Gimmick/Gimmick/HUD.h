#pragma once

#include "Game.h"
#include "GameObject.h"
#include "Gimmick.h"
#include "Event.h"
#include "Star.h"

#define HUD_BBOX_WIDTH	320
#define HUD_BBOX_HEIGHT	32

class CHUD
{
	int number;
	int it1, it2, it3;
	int dola;
	int score;

	Event* event;

	static CHUD* instance;

public:
	CHUD()
	{
		number = 0;
		dola = 0;
		score = 0;
		it1 = it2 = it3 = -1;
		event = new Event();
	}
	~CHUD()
	{
		if (event == NULL)
			delete event;

		event = NULL;
	}

	static CHUD* GetInstance();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	void Render();
};

