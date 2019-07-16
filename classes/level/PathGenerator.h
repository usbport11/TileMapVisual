//astar path generator
#ifndef pathgeneratorH
#define pathgeneratorH

#include "N2.h"
#include <vector>
#include <set>

typedef unsigned int uint;
typedef uint(*HeuristicFunction)(NVector2, NVector2);
typedef std::vector<NVector2> CoordinateList;

uint manhattan(NVector2 source_, NVector2 target_);
uint euclidean(NVector2 source_, NVector2 target_);
uint octagonal(NVector2 source_, NVector2 target_);

struct Node
{
	uint G, H;
	NVector2 coordinates;
	Node* parent;
	Node(NVector2 coord_, Node* parent_ = 0) {
		parent = parent_;
		coordinates = coord_;
		G = H = 0;
	}
	uint getScore() {
		return G + H;
	}
};

typedef std::set<Node*> NodeSet;

class MPathGenerator {
    bool detectCollision(NVector2 coordinates_);
    Node* findNodeOnList(NodeSet& nodes_, NVector2 coordinates_);
    void releaseNodes(NodeSet& nodes_);
public:
    MPathGenerator();
    void setWorldSize(NVector2 worldSize_);
    void setDiagonalMovement(bool enable_);
    void setHeuristic(HeuristicFunction heuristic_);
    CoordinateList findPath(NVector2 source_, NVector2 target_);
    void addCollision(NVector2 coordinates_);
	void removeCollision(NVector2 coordinates_);
    void clearCollisions();  
private:
    HeuristicFunction heuristic;
    CoordinateList direction, walls;
    NVector2 worldSize;
    uint directions;
};

#endif
