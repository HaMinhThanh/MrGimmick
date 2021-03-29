#ifndef __MAP_H__

#define __MAP_H__

#include "Sprites.h"
#include "Textures.h"
#include "Sprites.h"
#include"Utils.h"

#define MAP_CELL_SIZE	32



using namespace std;

class CMap
{

private:
	int _column_max;
	int _row_max;
	int index;
	int align;
	int TileMap[51][51];

	//int cell_row_max;
	//int cell_column_max;
	static CMap* __instance;
public:
	static CMap* GetInstance();

	CMap();
	void SetValueInMap(int row, int column, int index, int align);
	int getTile(int x, int y);
	void LoadMap(wstring filePathTxt);
	void DrawMap(int cam_x, int cam_y);
	int GetColumn() { return _column_max; }
};
#endif 