#ifndef n2H
#define n2H

#include <math.h>
#include <functional>
#include <algorithm>

struct NVector2 {
	int x;
	int y;
	NVector2() {
		x = y = 0;
	}
	NVector2(int inX, int inY) {
		x = inX;
		y = inY;
	}
	operator == (const NVector2& inVal) {
		return x == inVal.x && y == inVal.y;
	}
};

bool NVector2Compare(NVector2 A, NVector2 B);
NVector2 NVector2Delta(NVector2 A, NVector2 B);
bool NVector2CompareP(NVector2* A, NVector2* B);

struct NVector2Find: std::unary_function<NVector2, bool> {
	NVector2 v;
	NVector2Find(NVector2 inv) {
		v = inv;
	}
	bool operator() (const NVector2& inv) {
		return v.x == inv.x && v.y == inv.y;
	}
};

struct NVector2FindP: std::unary_function<NVector2*, bool> {
	NVector2* v;
	NVector2FindP(NVector2* inv) {
		v = inv;
	}
	bool operator() (NVector2* inv) {
		return v == inv;
	}
};

float NVector2Distance(NVector2 v1, NVector2 v2);
int RotateDirection(NVector2 Start, NVector2 End, NVector2 Test);
bool SegmentsIntersect(NVector2 A, NVector2 B, NVector2 C, NVector2 D);

struct NLine2 {
	NVector2 a;
	NVector2 b;
	NLine2() {
		a = NVector2(0, 0);
		b = NVector2(0, 0);
	}
	NLine2(NVector2 inA, NVector2 inB) {
		a = inA;
		b = inB;
	}
};

bool NLine2CrossPoint(NLine2 LineA, NLine2 LineB, float* Point);
bool PointOnSegment(NLine2 Segment, NVector2 Point);

struct NRectangle2 {
	NVector2 Position;
	NVector2 Size;
	NRectangle2() {
		Position = NVector2(0, 0);
		Size = NVector2(0, 0);
	}
	NRectangle2(int x, int y, int w, int h) {
		Position = NVector2(x, y);
		Size = NVector2(w, h);
	}
	NRectangle2(NVector2 inPosition, NVector2 inSize) {
		Position = inPosition;
		Size = inSize;
	}
	NRectangle(NRectangle2* pR) {
		Position = pR->Position;
		Size = pR->Size;
	}
};

bool NRectangle2Compare(NRectangle2 A, NRectangle2 B);
bool NRectangle2CompareP(NRectangle2* A, NRectangle2* B);

struct NRectangle2Find: std::unary_function<NRectangle2, bool> {
	NRectangle2 r;
	NRectangle2Find(NRectangle2 inr) {
		r = inr;
	}
	bool operator() (const NRectangle2& inr) {
		return r.Position.x == inr.Position.x && r.Position.y == inr.Position.y && r.Size.x == inr.Size.x && r.Size.y == inr.Size.y;
	}
};

struct NRectangle2FindP: std::unary_function<NRectangle2*, bool> {
	NRectangle2* r;
	NRectangle2FindP(NRectangle2* inr) {
		r = inr;
	}
	bool operator() (NRectangle2* inr) {
		return r == inr;
	}
};

#endif
