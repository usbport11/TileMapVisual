#include "edge.h"

MEdge::MEdge(const glm::vec2 &p1, const glm::vec2 &p2) : p1(p1), p2(p2) {
}

MEdge::MEdge(const MEdge &e) : p1(e.p1), p2(e.p2) {
}

char MEdge::SecondPointByPoint(const glm::vec2& inp) {
	if(p1.x == inp.x && p1.y == inp.y) return 1;
	if(p2.x == inp.x && p2.y == inp.y) return 0;
	return -1;
}

glm::vec2 MEdge::GetPointByNumber(char Number) {
	if(Number == 0) return p1;
	if(Number == 1) return p2;
	return glm::vec2(0, 0);
}
