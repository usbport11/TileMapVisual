#ifndef tilemapH
#define tilemapH

#include <string.h>
#include "N2.h"

#define WT_NONE 0
#define WT_ALLSIDE -1
#define WT_LEFT -2
#define WT_RIGHT -3
#define WT_TOP -4
#define WT_BOTTOM -5
#define WT_LEFT_TOP -6
#define WT_LEFT_BOTTOM -7
#define WT_RIGHT_TOP -8
#define WT_RIGHT_BOTTOM -9

#define FT_ALLSIDE 1
#define FT_LEFT 2
#define FT_RIGHT 3
#define FT_TOP 4
#define FT_BOTTOM 5
#define FT_LEFT_TOP 6
#define FT_LEFT_BOTTOM 7
#define FT_RIGHT_BOTTOM 8
#define FT_RIGHT_TOP 9
#define FT_HALL_VERTICAL 10
#define FT_HALL_HORIZONTAL 11

class MTileMap {
private:
	char** Map;
	NVector2 Size;
public:
	MTileMap();
	MTileMap(int SizeX, int SizeY);
	MTileMap(NVector2 inSize);
	void SetVector(int x, int y, char Value);
	void SetVector(NVector2 Position, char Value);
	void SetRectangle(int x, int y, int w, int h, char Value);
	void SetRectangle(NRectangle2 Rectangle, char Value);
	char GetValue(int x, int y);
	char GetValue(NVector2 Position);
	bool CreateWalls();
	bool CreateFloor();
	void Clear();
	void Close();
};

#endif
