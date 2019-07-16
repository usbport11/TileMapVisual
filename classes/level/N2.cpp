#include "N2.h"

bool NVector2Compare(NVector2 A, NVector2 B) {
	return A.x == B.x && A.y == B.y;
}

NVector2 NVector2Delta(NVector2 A, NVector2 B) {
	NVector2 Diff(abs(A.x - B.x), abs(A.y - B.y));
	return Diff;
}

bool NVector2CompareP(NVector2* A, NVector2* B) {
	if(!A || !B) return false;
	return A->x == B->x && A->y == B->y;
}

float NVector2Distance(NVector2 v1, NVector2 v2) {
	return sqrt((v2.x - v1.x) * (v2.x - v1.x) + (v2.y - v1.y) * (v2.y - v1.y));
}

int RotateDirection(NVector2 Start, NVector2 End, NVector2 Test)
{
	//> 0 - left | < 0 - right
	return (End.x - Start.x) * (Test.y - End.y) - (End.y - Start.y) * (Test.x - End.x);
}

bool SegmentsIntersect(NVector2 A, NVector2 B, NVector2 C, NVector2 D)
{
	return (RotateDirection(A, B, C) * RotateDirection(A, B, D) <= 0) && (RotateDirection(C, D, A) * RotateDirection(C, D, B) < 0);
}

bool NLine2CrossPoint(NLine2 LineA, NLine2 LineB, float* Point) {
	int Determinant;
	NVector2 Delta;
	int A[2], B[2], C[2];
	A[0] = LineA.b.y - LineA.a.y;
	B[0] = LineA.a.x - LineA.b.x;
	C[0] = LineA.a.x * (LineA.a.y - LineA.b.y) + LineA.a.y * (LineA.b.x - LineA.a.x);
	A[1] = LineB.b.y - LineB.a.y;
	B[1] = LineB.a.x - LineB.b.x;
	C[1] = LineB.a.x * (LineB.a.y - LineB.b.y) + LineB.a.y * (LineB.b.x - LineB.a.x);
	Determinant = A[0] * B[1] - B[0] *A[1];
	if(!Determinant) return false;
	Delta.x = -C[0] * B[1] + C[1] * B[0];
	Delta.y = -C[1] * A[0] + C[0] * A[1];
	Point[0] = Delta.x/Determinant;
	Point[1] = Delta.y/Determinant;
	return true;
}

bool PointOnSegment(NLine2 Segment, NVector2 Point)
{
    if(Point.x <= std::max(Segment.a.x, Segment.b.x) && Point.x >= std::min(Segment.a.x, Segment.b.x) &&
            Point.y <= std::max(Segment.a.y, Segment.b.y) && Point.y >= std::min(Segment.a.y, Segment.b.y))
        return true;
    return false;
}

bool NRectangle2Compare(NRectangle2 A, NRectangle2 B) {
	return A.Position.x == B.Position.x && A.Position.y == B.Position.y && A.Size.x == B.Size.x && A.Size.y == B.Size.y;
}

bool NRectangle2CompareP(NRectangle2* A, NRectangle2* B) {
	if(!A || !B) return false;
	return NVector2Compare(A->Position, B->Position) && NVector2Compare(A->Size, B->Size);
}
