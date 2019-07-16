#ifndef nodeH
#define nodeH

#include <vector>
#include <list>
#include <algorithm>

template <class T>
class TNode {
private:
	T Value;
public:
	TNode(T inValue) : Value(inValue) {}
	~TNode() {
		Neighbors.clear();
	}
	void AddConnection(TNode<T>* Neighbor) {
		//self add protect
		if(Neighbor == this) return;
		//duplicate protect
		if(std::find(Neighbors.begin(), Neighbors.end(), Neighbor) != Neighbors.end()) return;
		//add
		Neighbors.push_back(Neighbor);
	}
	void RemoveConnection(TNode<T>* Neighbor) {
		Neighbors.erase(std::find(Neighbors.begin(), Neighbors.end(), Neighbor));
	}
	bool NeighborExist(TNode<T>* Neighbor) {
		typename std::list<TNode<T>* >::iterator it;
		it = std::find(Neighbors.begin(), Neighbors.end(), Neighbor);
		if(it != Neighbors.end()) return true;
		return false;
	}
	T GetValue() {
		return Value;
	}
	T* GetValueP() {
		return &Value;
	}
	std::list<TNode<T>* > Neighbors;
};

template <typename T>
void ClearNodes(std::vector<TNode<T>* >* pNodes) {
	for(int i=0; i<pNodes->size(); i++) {
		if(pNodes->at(i)) pNodes->at(i)->Neighbors.clear();
	}
	for(int i=0; i<pNodes->size(); i++) {
		if(pNodes->at(i)) delete pNodes->at(i);
	}
	pNodes->clear();
}

template <typename T>
float Distance2D(T v1, T v2)
{
    return sqrt((v2.x - v1.x) * (v2.x - v1.x) + (v2.y - v1.y) * (v2.y - v1.y));
}

template <typename T>
std::vector<TNode<T>* > CreateMinimalSpanningTree(std::vector<TNode<T>* >* pPointsNodes) {	
	std::vector<TNode<T>* > MST;
	if(!pPointsNodes) return MST;
	if(pPointsNodes->size() <= 1) return MST;
	
	int NodesCount = 0;
	int Number = 0;
	float MinDistance, Distance;
	int NearestNumber;
	
	//copy data
	for(int i=0; i<pPointsNodes->size(); i++) {
		MST.push_back(new TNode<T>(pPointsNodes->at(i)->GetValue()));
	}
	
	//process
	for(int i=0; i<MST.size(); i++) {
		//set min
		for(int j=0; j<MST.size(); j++) {
			if(i == j) continue;
			MinDistance = Distance2D(MST[i]->GetValue(), MST[j]->GetValue());
			NearestNumber = j;
			break;
		}
		if(!MinDistance) return MST;
		
		//find real min
		for(int j=0; j<MST.size(); j++) {
			if(i == j) continue;
			Distance = Distance2D(MST[i]->GetValue(), MST[j]->GetValue());
			if(Distance < MinDistance) {
				MinDistance = Distance;
				NearestNumber = j;
			}
		}
		
		//set cross connections
		MST[i]->AddConnection(MST[NearestNumber]);
		MST[NearestNumber]->AddConnection(MST[i]);
	}
	
	return MST;
}

#endif
