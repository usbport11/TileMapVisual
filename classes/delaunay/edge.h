#ifndef edgeH
#define edgeH

#include <glm/glm.hpp>

class MEdge {
public:
	glm::vec2 p1;
	glm::vec2 p2;
	MEdge(const glm::vec2& p1, const glm::vec2& p2);
	MEdge(const MEdge& e);
	char SecondPointByPoint(const glm::vec2& inp);
	glm::vec2 GetPointByNumber(char Number);
};

struct stIsEdgeBackward{
	MEdge Edge;
	stIsEdgeBackward(const MEdge& inEdge) : Edge(inEdge) {
	}
	bool operator() (const MEdge& e) {
		return (Edge.p1.x == e.p2.x && Edge.p1.y == e.p2.y && Edge.p2.x == e.p1.x && Edge.p2.y == e.p1.y);
	}
};

inline bool operator == (const MEdge& e1, const MEdge& e2) {
	return 	(e1.p1 == e2.p1 && e1.p2 == e2.p2) || 
		(e1.p1 == e2.p2 && e1.p2 == e2.p1);
}

#endif 

