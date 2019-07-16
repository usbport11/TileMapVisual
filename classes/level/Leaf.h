#ifndef leafH
#define leafH

#include "N2.h"
#include "Node.h"

struct stLeaf {
	NRectangle2 Body;
	TNode<NRectangle2>* Room;
	stLeaf() {
		Body = NRectangle2(NVector2(0, 0), NVector2(0, 0));
		Room = NULL;
	}
	stLeaf(NRectangle2 inBody, TNode<NRectangle2>* inRoom) {
		Body = inBody;
		Room = inRoom;
	}
	stLeaf(NVector2 Position, NVector2 Size, TNode<NRectangle2>* inRoom) {
		Body = NRectangle2(NVector2(Position.x, Position.y), NVector2(Size.x, Size.y));
		Room = inRoom;
	}
};

bool SplitTreeLeaf(std::list<TNode<stLeaf>* >* pTree, TNode<stLeaf>* pNodeLeaf, int MinLeafSize);
bool SplitTree(std::list<TNode<stLeaf>* >* pTree, int RootLeafSizeX, int RootLeafSizeY, int MinLeafSize, int MaxLeafSize);
TNode<NRectangle2>* CreateRoomInLeaf(TNode<stLeaf>* pNodeLeaf, int MinRoomSize);
void ClearTree(std::list<TNode<stLeaf>* >* pTree);
std::vector<NRectangle2> CreateHalls1(NRectangle2* inRoomLeft, NRectangle2* inRoomRight);
std::vector<NRectangle2> CreateHalls2(NRectangle2* pRoom1, NRectangle2* pRoom2);

#endif
