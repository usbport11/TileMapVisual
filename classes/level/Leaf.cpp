#include "Leaf.h"
#include "../system/Random.h"

bool SplitTreeLeaf(std::list<TNode<stLeaf>* >* pTree, TNode<stLeaf>* pNodeLeaf, int MinLeafSize) {
	if(!pTree) {
		return false;
	}
	if(!pNodeLeaf) {
		return false;
	}
	if(pNodeLeaf->Neighbors.size()) {
		return false;
	}
	
	//create new leafs by split main leaf (some logic)
	NVector2 Position = pNodeLeaf->GetValue().Body.Position;
	NVector2 Size = pNodeLeaf->GetValue().Body.Size;
	if(!Size.x || !Size.y) {
		return false;
	}
	
	TNode<stLeaf>* Leaf1;
	TNode<stLeaf>* Leaf2;
	bool SplitByHeight = rand() % 2;
	if(Size.x > Size.y && Size.x / Size.y >= 1.25) SplitByHeight = false;
	else if(Size.y > Size.x && Size.y / Size.x >= 1.25) SplitByHeight = true;
	int MaxLeafSize = (SplitByHeight ? Size.y : Size.x) - MinLeafSize;
	if(MaxLeafSize <= MinLeafSize) {
		return false;
	}
	int SplitSize = rand() % (MaxLeafSize - MinLeafSize) + MinLeafSize;
	if(SplitByHeight) {
		Leaf1 = new TNode<stLeaf>(stLeaf(NVector2(Position.x, Position.y), NVector2(Size.x, SplitSize), NULL));
		Leaf2 = new TNode<stLeaf>(stLeaf(NVector2(Position.x, Position.y + SplitSize), NVector2(Size.x, Size.y - SplitSize), NULL));
	}
	else {
		Leaf1 = new TNode<stLeaf>(stLeaf(NVector2(Position.x, Position.y), NVector2(SplitSize, Size.y), NULL));
		Leaf2 = new TNode<stLeaf>(stLeaf(NVector2(Position.x + SplitSize, Position.y), NVector2(Size.x - SplitSize, Size.y), NULL));
	}
	
	//add in leafs graph
	pTree->push_back(Leaf1);
	pTree->push_back(Leaf2);
	
	//add sub leafs to main leaf
	std::list<TNode<stLeaf>* >::iterator it = find(pTree->begin(), pTree->end(), pNodeLeaf);
	(*it)->AddConnection(Leaf1);
	(*it)->AddConnection(Leaf2);
	
	return true;
}

bool SplitTree(std::list<TNode<stLeaf>* >* pTree, int RootLeafSizeX, int RootLeafSizeY, int MinLeafSize, int MaxLeafSize) {
	if(!pTree) return false;
	
	//add main leaf
	pTree->push_back(new TNode<stLeaf>(stLeaf(NVector2(0,0), NVector2(RootLeafSizeX, RootLeafSizeY), NULL)));

	//split
	bool SplitSuccess = true;
	std::list<TNode<stLeaf>* >::iterator it;
	while(SplitSuccess) {
		SplitSuccess = false;
		it = pTree->begin();
		while(it != pTree->end()) {
			if((*it)->GetValue().Body.Size.x > MaxLeafSize || (*it)->GetValue().Body.Size.y > MaxLeafSize || rand() % 100 > 25) {
				SplitSuccess = SplitTreeLeaf(pTree, (*it), MinLeafSize);
			}
			it ++;
		}
	}
	
	return true;
}

TNode<NRectangle2>* CreateRoomInLeaf(TNode<stLeaf>* pNodeLeaf, int MinRoomSize) {
	if(!pNodeLeaf) return NULL;
	if(pNodeLeaf->Neighbors.size()) {
		return NULL;
	}
	stLeaf* pLeaf = pNodeLeaf->GetValueP();
	if(pLeaf->Room) {
		return NULL;
	}
	
	NVector2 Position;
	NVector2 Size;
	Size.x = rand() % (pNodeLeaf->GetValueP()->Body.Size.x - (MinRoomSize + 2)) + MinRoomSize;
	Size.y = rand() % (pNodeLeaf->GetValueP()->Body.Size.y - (MinRoomSize + 2)) + MinRoomSize;
	Position.x = pNodeLeaf->GetValueP()->Body.Position.x + rand() % (pNodeLeaf->GetValueP()->Body.Size.x - Size.x - 2) + 1;
	Position.y = pNodeLeaf->GetValueP()->Body.Position.y + rand() % (pNodeLeaf->GetValueP()->Body.Size.y - Size.y - 2) + 1;
	pLeaf->Room = new TNode<NRectangle2>(NRectangle2(Position, Size));
	
	return pLeaf->Room;
}

void ClearTree(std::list<TNode<stLeaf>* >* pTree) {
	if(!pTree) return;
	
	TNode<NRectangle2>* Room;
	std::list<TNode<stLeaf>* >::iterator it;
	for(it = pTree->begin(); it != pTree->end(); it++) {
		//delete room
		Room = (*it)->GetValueP()->Room;
		if(Room) {
			Room->Neighbors.clear();
			delete Room;
		}
		//delete leaf
		if(*it) delete *it;
	}
	pTree->clear();
}

std::vector<NRectangle2> CreateHalls1(NRectangle2* inRoomLeft, NRectangle2* inRoomRight) {
	std::vector<NRectangle2> Halls;
	Halls.clear();
	
	if(!inRoomLeft || !inRoomRight) {
		return Halls;
	}
	if(inRoomLeft->Size.x <= 0 || inRoomLeft->Size.y <= 0) {
		return Halls;
	}
	if(inRoomRight->Size.x <= 0 || inRoomRight->Size.y <= 0) {
		return Halls;
	}

	NVector2 PointLeft;
	NVector2 PointRight;
	NVector2 PointDiff;
	PointLeft.x = RandomValue(inRoomLeft->Position.x + 1, inRoomLeft->Position.x + inRoomLeft->Size.x - 2);
	PointLeft.y = RandomValue(inRoomLeft->Position.y + 1, inRoomLeft->Position.y + inRoomLeft->Size.y - 2);
	PointRight.x = RandomValue(inRoomRight->Position.x + 1, inRoomRight->Position.x + inRoomRight->Size.x - 2);
	PointRight.y = RandomValue(inRoomRight->Position.y + 1, inRoomRight->Position.y + inRoomRight->Size.y - 2);
	PointDiff = NVector2(PointRight.x - PointLeft.x, PointRight.y - PointLeft.y);
	if(PointDiff.x < 0) {
		if(PointDiff.y < 0) {
			if(rand() % 2) {
				Halls.push_back(NRectangle2(NVector2(PointRight.x, PointLeft.y), NVector2(abs(PointDiff.x), 1)));
				Halls.push_back(NRectangle2(NVector2(PointRight.x, PointRight.y), NVector2(1, abs(PointDiff.y))));
			}
			else {
				Halls.push_back(NRectangle2(NVector2(PointRight.x, PointRight.y), NVector2(abs(PointDiff.x), 1)));
				Halls.push_back(NRectangle2(NVector2(PointLeft.x, PointRight.y), NVector2(1, abs(PointDiff.y))));
			}
		}
		else if(PointDiff.y > 0) {
			if(rand() % 2) {
				Halls.push_back(NRectangle2(NVector2(PointRight.x, PointLeft.y), NVector2(abs(PointDiff.x), 1)));
				Halls.push_back(NRectangle2(NVector2(PointRight.x, PointLeft.y), NVector2(1, abs(PointDiff.y))));
			}
			else {
				Halls.push_back(NRectangle2(NVector2(PointRight.x, PointRight.y), NVector2(abs(PointDiff.x) + 1, 1)));
				Halls.push_back(NRectangle2(NVector2(PointLeft.x, PointLeft.y), NVector2(1, abs(PointDiff.y))));
			}
		}
		else Halls.push_back(NRectangle2(NVector2(PointRight.x, PointRight.y), NVector2(abs(PointDiff.x), 1)));
	}
	else if(PointDiff.x > 0) {
		if(PointDiff.y < 0) {
			if(rand() % 2) {
				Halls.push_back(NRectangle2(NVector2(PointLeft.x, PointRight.y), NVector2(abs(PointDiff.x), 1)));
				Halls.push_back(NRectangle2(NVector2(PointLeft.x, PointRight.y), NVector2(1, abs(PointDiff.y))));
			}
			else {
				Halls.push_back(NRectangle2(NVector2(PointLeft.x, PointLeft.y), NVector2(abs(PointDiff.x) + 1, 1)));
				Halls.push_back(NRectangle2(NVector2(PointRight.x, PointRight.y), NVector2(1, abs(PointDiff.y))));
			}
		}
		else if(PointDiff.y > 0) {
			if(rand() % 2) {
				Halls.push_back(NRectangle2(NVector2(PointLeft.x, PointLeft.y), NVector2(abs(PointDiff.x), 1)));
				Halls.push_back(NRectangle2(NVector2(PointRight.x, PointLeft.y), NVector2(1, abs(PointDiff.y))));
			}
			else {
				Halls.push_back(NRectangle2(NVector2(PointLeft.x, PointRight.y), NVector2(abs(PointDiff.x), 1)));
				Halls.push_back(NRectangle2(NVector2(PointLeft.x, PointLeft.y), NVector2(1, abs(PointDiff.y))));
			}
		}
		else {
			Halls.push_back(NRectangle2(NVector2(PointLeft.x, PointLeft.y), NVector2(abs(PointDiff.x), 1)));
		}
	}
	else {
		if(PointDiff.y < 0) {
			Halls.push_back(NRectangle2(NVector2(PointRight.x, PointRight.y), NVector2(1, abs(PointDiff.y))));
		}
		else if(PointDiff.y > 0) {
			Halls.push_back(NRectangle2(NVector2(PointLeft.x, PointLeft.y), NVector2(1, abs(PointDiff.y))));
		}
	}
	
	return Halls;
}

std::vector<NRectangle2> CreateHalls2(NRectangle2* pRoom1, NRectangle2* pRoom2) {
	std::vector<NRectangle2> Halls;
	if(!pRoom1 || !pRoom2) return Halls;
	if(pRoom1 == pRoom2) return Halls;

	std::vector<int> DataX;
	std::vector<int> DataY;
	std::vector<int> CopyDataX;
	std::vector<int> CopyDataY;
	int MaxHeight, MaxWidth;
	bool Vertical = false, Horizontal = false;
	NRectangle2 Hall0, Hall1, Hall2;
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
	
	if(MaxWidth <= 0) MaxWidth = 1;
	if(MaxHeight <= 0) MaxHeight = 1;
	
	if(Vertical && !Horizontal) {
		Hall0.Position.y = DataY[1];
		Hall0.Size.y = DataY[2] - DataY[1];
		if(MaxWidth > 3 && rand() % 2) {
			Hall0.Size.x = 1;
			Hall0.Position.x = DataX[1];
			Halls.push_back(Hall0);
			Hall0.Size.x = MaxWidth - (DataX[1] + 2);//1;
			if(Hall0.Size.x > 0) {
				Hall0.Position.x = DataX[1] + 2;
				Halls.push_back(Hall0);
			}
		}
		else {
			Hall0.Size.x = (MaxWidth <= 2) ? 1 : rand() % (MaxWidth - 1) + 1;//1
			Hall0.Position.x = (MaxWidth == 1) ? DataX[1] : DataX[1] + 1;//DataX[1]
			Halls.push_back(Hall0);
		}
	}
	if(!Vertical && Horizontal) {
		Hall0.Position.x = DataX[1];
		Hall0.Size.x = DataX[2] - DataX[1];
		if(MaxHeight > 3 && rand() % 2) {
			Hall0.Size.y = 1;
			Hall0.Position.y = DataY[1];
			Halls.push_back(Hall0);
			Hall0.Size.y = MaxHeight - (DataY[1] + 2);//1;
			if(Hall0.Size.y > 0) {
				Hall0.Position.y = DataY[1] + 2;
				Halls.push_back(Hall0);
			}
		}
		else {
			Hall0.Size.y = (MaxHeight <= 2) ? 1 : rand() % (MaxHeight - 1) + 1;//1
			Hall0.Position.y = (MaxHeight == 1) ? DataY[1] : DataY[1] + 1;//DataY[1]
			Halls.push_back(Hall0);
		}
		
	}
	if(Vertical && Horizontal) {
		NVector2 Center1(pRoom1->Position.x + pRoom1->Size.x * 0.5, pRoom1->Position.y + pRoom1->Size.y * 0.5);
		NVector2 Center2(pRoom2->Position.x + pRoom2->Size.x * 0.5, pRoom2->Position.y + pRoom2->Size.y * 0.5);
		int rnd = rand() % 2;
		switch(rnd) {
			case 0://Center1.x, Center2.y
				//room1 hall vertical
				if(Center2.y < Center1.y) {
					//room1 over room2
					Hall1.Size.y = pRoom1->Position.y - Center2.y;
					Hall1.Size.x = pRoom2->Size.x * 0.5;//1
					Hall1.Position.x = Center1.x;
					Hall1.Position.y = Center2.y;
				}
				else {
					//room2 over room1
					Hall1.Size.y = Center2.y - (pRoom1->Position.y + pRoom1->Size.y) + 1;
					Hall1.Size.x = pRoom2->Size.x * 0.5;//1
					Hall1.Position.x = Center1.x;
					Hall1.Position.y = pRoom1->Position.y + pRoom1->Size.y;
				}
				//room2 hall horizontal
				if(Center2.x < Center1.x) {
					//room1 right of room2
					Hall2.Size.x = Center1.x - (pRoom2->Position.x + pRoom2->Size.x);
					Hall2.Size.y = pRoom2->Size.y * 0.5;//1
					Hall2.Position.x = pRoom2->Position.x + pRoom2->Size.x;
					Hall2.Position.y = Center2.y;
				}
				else {
					//room2 right of room1
					Hall2.Size.x = pRoom2->Position.x - Center1.x - 1;
					Hall2.Size.y = pRoom2->Size.y * 0.5;//1
					Hall2.Position.x = Center1.x + 1;
					Hall2.Position.y = Center2.y;
				}
				break;
			case 1://Center2.x, Center1.y
				//room2 hall vertical
				if(Center2.y < Center1.y) {
					//room1 over room2
					Hall2.Size.y = Center1.y - (pRoom2->Position.y + pRoom2->Size.y) + 1;
					Hall2.Size.x = pRoom2->Size.x * 0.5;//1
					Hall2.Position.x = Center2.x;
					Hall2.Position.y = pRoom2->Position.y + pRoom2->Size.y;
				}
				else {
					//room2 over room1
					Hall2.Size.y = pRoom2->Position.y - Center1.y;
					Hall2.Size.x = pRoom2->Size.y * 0.5;//1
					Hall2.Position.x = Center2.x;
					Hall2.Position.y = Center1.y;
				}
				//room1 hall horizontal
				if(Center2.x < Center1.x) {
					//room1 right of room2
					Hall1.Size.x = pRoom1->Position.x - Center2.x - 1;
					Hall1.Size.y = pRoom2->Size.y * 0.5;//1
					Hall1.Position.x = Center2.x + 1;
					Hall1.Position.y = Center1.y;
				}
				else {
					//room2 right of room1
					Hall1.Size.x = Center2.x - (pRoom1->Position.x + pRoom1->Size.x);
					Hall1.Size.y = pRoom2->Size.y * 0.5;//1
					Hall1.Position.x = pRoom1->Position.x + pRoom1->Size.x;
					Hall1.Position.y = Center1.y;
				}
				break;
		}
		Halls.push_back(Hall1);
		Halls.push_back(Hall2);
	}
	
	return Halls;
}
