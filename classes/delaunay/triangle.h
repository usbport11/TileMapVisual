#ifndef triangleH
#define triangleH

#include <functional>
#include "math.h"
#include "edge.h"

class MTriangle {
public:
	glm::vec2 p1;
	glm::vec2 p2;
	glm::vec2 p3;
	MEdge e1;
	MEdge e2;
	MEdge e3;

	MTriangle(const glm::vec2 &_p1, const glm::vec2 &_p2, const glm::vec2 &_p3);
	bool ContainsVertex(const glm::vec2 &v);
	bool CircumCircleContains(const glm::vec2 &v);
};

struct stTriangleContains: std::unary_function<MTriangle, bool> {
	MTriangle Triangle;
	stTriangleContains(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3) : Triangle(p1, p2, p3) {
	}
	bool operator()(const MTriangle &t) {
		return Triangle.ContainsVertex(t.p1) || Triangle.ContainsVertex(t.p2) || Triangle.ContainsVertex(t.p3);
	}
};

inline bool operator == (const MTriangle &t1, const MTriangle &t2) {
	return	(t1.p1 == t2.p1 || t1.p1 == t2.p2 || t1.p1 == t2.p3) &&
		(t1.p2 == t2.p1 || t1.p2 == t2.p2 || t1.p2 == t2.p3) && 
		(t1.p3 == t2.p1 || t1.p3 == t2.p2 || t1.p3 == t2.p3);
}

#endif
