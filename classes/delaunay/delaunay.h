#ifndef delaunayH
#define delaunayH

#include <vector>
#include <algorithm>
#include "triangle.h"

struct stGPoint {
	stGPoint() {
		p = glm::vec2(0, 0);
	}
	stGPoint(glm::vec2 inp) {
		p = inp;
	}
	std::vector<stGPoint*> NearPoints;
	glm::vec2 p;
};

struct stGPointEqual {
	stGPoint GPoint;
	stGPointEqual(glm::vec2 inp) {
		GPoint = stGPoint(inp);
	}
	bool operator() (const stGPoint& gp) const {
		return GPoint.p.x == gp.p.x && GPoint.p.y == gp.p.y;
	}
	bool operator() (const stGPoint* gp) const {
		return GPoint.p.x == gp->p.x && GPoint.p.y == gp->p.y;
	}
};

class MDelaunay {
private:
	std::vector<MTriangle> _triangles;
	std::vector<MEdge> _edges;
	std::vector<glm::vec2> _vertices;
public:
	std::vector<MTriangle>& Triangulate(std::vector<glm::vec2> &vertices);
	std::vector<MTriangle>& GetTriangles();
	std::vector<MEdge>& GetEdges();
	std::vector<glm::vec2>& GetVertices();
	std::vector<MEdge> CreateMSTEdges();
	void Clear();
};

#endif
