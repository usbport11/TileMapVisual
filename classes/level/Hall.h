#ifndef hallH
#define hallH

#include <vector>
#include <algorithm>
#include "N2.h"

struct stHall {
	std::vector<NRectangle2> Parts;
	std::vector<NVector2> Connects;
};

stHall ConnectRooms2(NRectangle2* pRoom1, NRectangle2* pRoom2) {
	stHall Hall;
	if(!pRoom1 || !pRoom2) return Hall;
	if(pRoom1 == pRoom2) return Hall;

	std::vector<int> DataX;
	std::vector<int> DataY;
	std::vector<int> CopyDataX;
	std::vector<int> CopyDataY;
	int MaxHeight, MaxWidth;
	bool Vertical = false, Horizontal = false;
	NRectangle2 Hall0;
	NVector2 Connect0;
	
	DataX.push_back(pRoom1->Position.x);
	DataX.push_back(pRoom1->Position.x + pRoom1->Size.x);
	DataX.push_back(pRoom2->Position.x);
	DataX.push_back(pRoom2->Position.x + pRoom2->Size.x);
	
	DataY.push_back(pRoom1->Position.y);
	DataY.push_back(pRoom1->Position.y + pRoom1->Size.y);
	DataY.push_back(pRoom2->Position.y);
	DataY.push_back(pRoom2->Position.y + pRoom2->Size.y);
	
	CopyDataX.insert(CopyDataX.begin(), DataX.begin(), DataX.end());
	CopyDataY.insert(CopyDataY.begin(), DataY.begin(), DataY.end());
	
	std::sort(DataX.begin(), DataX.end());
	std::sort(DataY.begin(), DataY.end());
	
	if((CopyDataY[0] == DataY[2] && CopyDataY[1] == DataY[3]) || (CopyDataY[0] == DataY[0] && CopyDataY[1] == DataY[1])) {
		Vertical = true;
		MaxWidth = DataX[2] - DataX[1];
	}
	if((CopyDataX[0] == DataX[2] && CopyDataX[1] == DataX[3]) || (CopyDataX[0] == DataX[0] && CopyDataX[1] == DataX[1])) {
		Horizontal = true;
		MaxHeight = DataY[2] - DataY[1];
	}
	
	if(Vertical && !Horizontal) {
		Hall0.Position.y = DataY[1];
		Hall0.Size.y = DataY[2] - DataY[1];
		Hall0.Size.x = (MaxWidth == 1) ? 1 : rand() % MaxWidth + 1;
		Hall0.Position.x = DataX[1];
		Hall.Parts.push_back(Hall0);
		Connect0 = NVector2(Hall0.Position.x, Hall0.Position.y - 1);
		Hall.Connects.push_back(Connect0);
		Connect0 = NVector2(Hall0.Position.x, Hall0.Position.y + Hall0.Size.y + 1);
		Hall.Connects.push_back(Connect0);
	}
	if(!Vertical && Horizontal) {
		Hall0.Position.x = DataX[1];
		Hall0.Size.x = DataX[2] - DataX[1];
		Hall0.Size.y = (MaxHeight == 1) ? 1 : rand() % MaxHeight + 1;
		Hall0.Position.y = DataY[1];
		Hall.Parts.push_back(Hall0);
		Connect0 = NVector2(Hall0.Position.x - 1, Hall0.Position.y);
		Hall.Connects.push_back(Connect0);
		Connect0 = NVector2(Hall0.Position.x + Hall0.Size.x + 1, Hall0.Position.y);
		Hall.Connects.push_back(Connect0);
	}
	if(Vertical && Horizontal) {
		NVector2 Center1(pRoom1->Position.x + pRoom1->Size.x * 0.5, pRoom1->Position.y + pRoom1->Size.y * 0.5);
		NVector2 Center2(pRoom2->Position.x + pRoom2->Size.x * 0.5, pRoom2->Position.y + pRoom2->Size.y * 0.5);
		NRectangle2 Hall1, Hall2;
		NVector2 Connect1, Connect2, Connect3;
		int rnd = rand() % 2;
		switch(rnd) {
			case 0://Center1.x, Center2.y
				Connect3 = NVector2(Center1.x, Center2.y);
				//room1 hall vertical
				if(Center2.y < Center1.y) {
					//room1 over room2
					Hall1.Size.y = pRoom1->Position.y - Center2.y;
					Hall1.Size.x = 1;
					Hall1.Position.x = Center1.x;
					Hall1.Position.y = Center2.y;
					Connect1 = NVector2(Hall1.Position.x, pRoom1->Position.y);
				}
				else {
					//room2 over room1
					Hall1.Size.y = Center2.y - (pRoom1->Position.y + pRoom1->Size.y) + 1;
					Hall1.Size.x = 1;
					Hall1.Position.x = Center1.x;
					Hall1.Position.y = pRoom1->Position.y + pRoom1->Size.y;
					Connect1 = NVector2(Hall1.Position.x, pRoom1->Position.y + pRoom1->Size.y - 1);
				}
				//room2 hall horizontal
				if(Center2.x < Center1.x) {
					//room1 right of room2
					Hall2.Size.x = Center1.x - (pRoom2->Position.x + pRoom2->Size.x);
					Hall2.Size.y = 1;
					Hall2.Position.x = pRoom2->Position.x + pRoom2->Size.x;
					Hall2.Position.y = Center2.y;
					Connect2 = NVector2(pRoom2->Position.x + pRoom2->Size.x - 1, Hall2.Position.y);
				}
				else {
					//room2 right of room1
					Hall2.Size.x = pRoom2->Position.x - Center1.x - 1;
					Hall2.Size.y = 1;
					Hall2.Position.x = Center1.x + 1;
					Hall2.Position.y = Center2.y;
					Connect2 = NVector2(pRoom1->Position.x + pRoom1->Size.x + 1, Hall2.Position.y);
				}
				break;
			case 1://Center2.x, Center1.y
				Connect3 = NVector2(Center2.x, Center1.y);
				//room2 hall vertical
				if(Center2.y < Center1.y) {
					//room1 over room2
					Hall2.Size.y = Center1.y - (pRoom2->Position.y + pRoom2->Size.y) + 1;
					Hall2.Size.x = 1;
					Hall2.Position.x = Center2.x;
					Hall2.Position.y = pRoom2->Position.y + pRoom2->Size.y;
					Connect2 = NVector2(Hall2.Position.x, pRoom2->Position.y + pRoom2->Size.y - 1);
				}
				else {
					//room2 over room1
					Hall2.Size.y = pRoom2->Position.y - Center1.y;
					Hall2.Size.x = 1;
					Hall2.Position.x = Center2.x;
					Hall2.Position.y = Center1.y;
					Connect2 = NVector2(Hall2.Position.x, pRoom2->Position.y);
				}
				//room1 hall horizontal
				if(Center2.x < Center1.x) {
					//room1 right of room2
					Hall1.Size.x = pRoom1->Position.x - Center2.x - 1;
					Hall1.Size.y = 1;
					Hall1.Position.x = Center2.x + 1;
					Hall1.Position.y = Center1.y;
					Connect1 = NVector2(pRoom1->Position.x, Hall1.Position.y);
				}
				else {
					//room2 right of room1
					Hall1.Size.x = Center2.x - (pRoom1->Position.x + pRoom1->Size.x);
					Hall1.Size.y = 1;
					Hall1.Position.x = pRoom1->Position.x + pRoom1->Size.x;
					Hall1.Position.y = Center1.y;
					Connect1 = NVector2(pRoom1->Position.x + pRoom1->Size.x - 1, Hall1.Position.y);
				}
				break;
		}
		Hall.Parts.push_back(Hall1);
		Hall.Parts.push_back(Hall2);
		Hall.Connects.push_back(Connect1);
		Hall.Connects.push_back(Connect3);
		Hall.Connects.push_back(Connect2);
	}
	
	return Hall;
}

#endif
