#include "PathGenerator.h"

uint manhattan(NVector2 source_, NVector2 target_) {
	NVector2 delta = NVector2Delta(source_, target_);
    return static_cast<uint>(10 * (delta.x + delta.y));
}

uint euclidean(NVector2 source_, NVector2 target_) {
	NVector2 delta = NVector2Delta(source_, target_);
    return static_cast<uint>(10 * sqrt(pow(delta.x, 2) + pow(delta.y, 2)));
}

uint octagonal(NVector2 source_, NVector2 target_) {
	NVector2 delta = NVector2Delta(source_, target_);
    return 10 * (delta.x + delta.y) + (-6) * std::min(delta.x, delta.y);
}

bool MPathGenerator::detectCollision(NVector2 coordinates_) {
	if (coordinates_.x < 0 || coordinates_.x >= worldSize.x ||
        coordinates_.y < 0 || coordinates_.y >= worldSize.y ||
        std::find(walls.begin(), walls.end(), coordinates_) != walls.end()) {
        return true;
    }
    return false;
}

Node* MPathGenerator::findNodeOnList(NodeSet& nodes_, NVector2 coordinates_) {
	for(NodeSet::iterator node = nodes_.begin(); node != nodes_.end(); node ++) {
        if ((*node)->coordinates == coordinates_) {
            return *node;
        }
    }
    return NULL;
}

void MPathGenerator::releaseNodes(NodeSet& nodes_) {
	for(NodeSet::iterator it = nodes_.begin(); it != nodes_.end(); it++) {
        delete *it;
   }
   nodes_.clear();
}

MPathGenerator::MPathGenerator() {
	setDiagonalMovement(false);
    setHeuristic(&manhattan);
    direction.push_back(NVector2(0, 1));
    direction.push_back(NVector2(1, 0 ));
    direction.push_back(NVector2(0, -1));
    direction.push_back(NVector2(-1, 0));
    direction.push_back(NVector2(-1, -1));
    direction.push_back(NVector2(1, 1));
    direction.push_back(NVector2(-1, 1));
    direction.push_back(NVector2(1, -1));
}

void MPathGenerator::setWorldSize(NVector2 worldSize_) {
	worldSize = worldSize_;
}

void MPathGenerator::setDiagonalMovement(bool enable_) {
	directions = (enable_ ? 8 : 4);
}

void MPathGenerator::setHeuristic(HeuristicFunction heuristic_) {
	heuristic = heuristic_;
}

CoordinateList MPathGenerator::findPath(NVector2 source_, NVector2 target_) {
	Node *current = NULL;
    NodeSet openSet, closedSet;
    openSet.insert(new Node(source_));

    while (!openSet.empty()) {
        current = *openSet.begin();
        for (NodeSet::iterator node = openSet.begin(); node != openSet.end(); node++) {
            if ((*node)->getScore() <= current->getScore()) {
                current = (*node);
            }
        }

        if (current->coordinates == target_) {
            break;
        }

        closedSet.insert(current);
        openSet.erase(std::find(openSet.begin(), openSet.end(), current));

        for (uint i = 0; i < directions; ++i) {
            NVector2 newCoordinates;
			newCoordinates.x = current->coordinates.x + direction[i].x;
			newCoordinates.y = current->coordinates.y + direction[i].y;
            if (detectCollision(newCoordinates) ||
                findNodeOnList(closedSet, newCoordinates)) {
                continue;
            }

            uint totalCost = current->G + ((i < 4) ? 10 : 14);

            Node *successor = findNodeOnList(openSet, newCoordinates);
            if (successor == NULL) {
                successor = new Node(newCoordinates, current);
                successor->G = totalCost;
                successor->H = heuristic(successor->coordinates, target_);
                openSet.insert(successor);
            }
            else if (totalCost < successor->G) {
                successor->parent = current;
                successor->G = totalCost;
            }
        }
    }

    CoordinateList path;
    while (current != NULL) {
        path.push_back(current->coordinates);
        current = current->parent;
    }

    releaseNodes(openSet);
    releaseNodes(closedSet);
    
    std::reverse(path.begin(), path.end());

    return path;
}

void MPathGenerator::addCollision(NVector2 coordinates_) {
	walls.push_back(coordinates_);
}

void MPathGenerator::removeCollision(NVector2 coordinates_) {
	CoordinateList::iterator it = std::find(walls.begin(), walls.end(), coordinates_);
    if(it != walls.end()) {
        walls.erase(it);
    }
}

void MPathGenerator::clearCollisions() {
	walls.clear();
}
