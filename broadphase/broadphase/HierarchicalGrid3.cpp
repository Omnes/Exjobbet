#include "HierarchicalGrid3.h"
#include "BruteForce.h"
#include "Constants.h"
#include <math.h>


HierarchicalGrid3::HierarchicalGrid3() {
	grids.push_back(new Grid(Area, 32));
	grids.push_back(new Grid(Area, 8));
	grids.push_back(new Grid(Area, 1));


}

HierarchicalGrid3::~HierarchicalGrid3() {

}

void HierarchicalGrid3::init(vector<Entity*> &entityVector) {
	for (auto entity : entityVector) {
		bool placed = false;
		for (size_t i = 0; i< grids.size() && !placed; i++) {
			Grid* grid = grids[i];
			Vector3 size = entity->getSize();
			float largestSide = max(size.getX(), max(size.getY(), size.getZ()));
			if (grid->cellSize > largestSide) {
				grid->insert(entity);
				placed = true;
			}
		}
	}
}

void HierarchicalGrid3::update(vector<Entity*> &entityVector) {
	for (auto grid : grids) {
		grid->update(entityVector);
	}
}

vector<Entity*>* HierarchicalGrid3::getNearbyCells(Grid* grid, Vector3Int pos) {
	vector<Entity*>* nearby = new vector<Entity*>;

	for (int q = -1; q <= 1; q++) {
		for (int w = -1; w <= 1; w++) {
			for (int e = -1; e <= 1; e++) {
				if (grid->isHashedPosValid(pos, Vector3Int(q, w, e))) {
					vector<Entity*>* bucket = grid->getBucketAtHash(Vector3Int(pos.getX() + q, pos.getY() + w, pos.getZ() + e));
					if (!bucket->empty()) {
						nearby->insert(nearby->end(), bucket->begin(), bucket->end());
					}
				}
			}
		}
	}
	return nearby;
}

void HierarchicalGrid3::checkCollison(Entity* entity,int gridNr, BroadPhase::PairVector* pv) {
	for (int i = gridNr; i < grids.size(); i++) {
		Grid* currentGrid = grids[i];
		Vector3Int hashedPos = currentGrid->hash(entity->getPos());
		auto nearbyEntites = getNearbyCells(currentGrid, hashedPos);
		for (auto otherEntity : (*nearbyEntites)) {
			if (!otherEntity->hasVisited()) {
				if (entity->intersects(*otherEntity)) {
					pv->push_back(make_pair(entity, otherEntity));
				}
			}
		}
		delete nearbyEntites;
	}

}


BroadPhase::PairVector* HierarchicalGrid3::coraseCollison(vector<Entity*> &entityVector) {
	PairVector* pv = new PairVector();

	for (int i = 0; i < grids.size(); i++) {
		Grid* currentGrid = grids[i];
		for (auto entity : currentGrid->allEntitiesInGrid) {
			entity->visit();
			checkCollison(entity,i, pv);
		}
	}
	for (auto entity : entityVector) {
		entity->resetVisited();
	}
	return pv;
}
