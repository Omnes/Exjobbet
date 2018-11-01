#include "HierarchicalGrid.h"
#include "BruteForce.h"
#include "Constants.h"
#include <math.h>

HierarchicalGrid::HierarchicalGrid() {
	grids.push_back(new Grid(Area, 32));
	grids.push_back(new Grid(Area, 8));
	grids.push_back(new Grid(Area, 1));
	
	
}

HierarchicalGrid::~HierarchicalGrid() {

}

void HierarchicalGrid::init(vector<Entity*> &entityVector) {
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

void HierarchicalGrid::update(vector<Entity*> &entityVector) {
	for (auto grid : grids) {
		grid->update(entityVector);
	}
}
//between one entity and a group
BroadPhase::PairVector* brute(Entity* entity, vector<Entity*>& others) {
	BroadPhase::PairVector* pv = new BroadPhase::PairVector();
	for each (auto other in others) {
		if (entity->intersects(*other)) {
			pv->push_back(make_pair(entity, other));
		}
	}
	return pv;
}

//between groups of entities
BroadPhase::PairVector* brute(vector<Entity*>& entities, vector<Entity*>& others) {
	BroadPhase::PairVector* pv = new BroadPhase::PairVector();
	for (auto entity : entities) {
		for (auto other : others) {
			if (entity->intersects(*other)) {
				pv->push_back(make_pair(entity, other));
			}
		}
	}
	return pv;
}
//inside a group
BroadPhase::PairVector* brute(vector<Entity*>& eVector) {
	BroadPhase::PairVector* pv = new BroadPhase::PairVector();
	for (size_t i = 0; i < eVector.size(); i++) {
		for (size_t j = i + 1; j < eVector.size(); j++) {
			//migth want to do a intersection here too?
			if (eVector[i]->intersects(*eVector[j]))
				pv->push_back(make_pair(eVector[i], eVector[j]));
		}
	}
	
	return pv;
}

vector<Entity*>* HierarchicalGrid::getNearbyCells(Grid* grid, Vector3Int pos) {
	vector<Entity*>* nearby = new vector<Entity*>;

	for (int q = -1; q <= 1; q++) {
		for (int w = -1; w <= 1; w++) {
			for (int e = -1; e <= 1; e++) {
				if (q != 0 && w != 0 && e != 0) {
					if (grid->isHashedPosValid(pos, Vector3Int(q, w, e))) {
						vector<Entity*>* bucket = grid->getBucketAtHash(pos + Vector3Int(q, w, e));
						if (!bucket->empty()) {
							nearby->insert(nearby->end(), bucket->begin(), bucket->end());
						}
					}
				}
			}
		}
	}
	return nearby;
}

void HierarchicalGrid::collideNearby(Grid* grid, Vector3Int pos, BroadPhase::PairVector* pv) {
	vector<Entity*>* nearby = new vector<Entity*>;
	vector<Entity*>* inbucket = grid->getBucketAtHash(pos);

	for (int q = 0; q <= 1; q++) {
		for (int w = 0; w <= 1; w++) {
			for (int e = 0; e <= 1; e++) {
				if (q != 0 && w != 0 && e != 0) {
					if (grid->isHashedPosValid(pos, Vector3Int(q, w, e))) {
						vector<Entity*>* bucket = grid->getBucketAtHash(pos + Vector3Int(q, w, e));
						if (!bucket->empty()) {
							for (auto entity : *inbucket) {
								for (auto other : *bucket) {
									if (entity->intersects(*other)) {
										pv->push_back(make_pair(entity,other));
									}
								}
							}
						}
					}
				}
			}
		}
	}
}


BroadPhase::PairVector* HierarchicalGrid::coraseCollison(vector<Entity*> &entityVector) {
	PairVector* pv = new PairVector();

	for (size_t i = 0; i < grids.size(); i++) {
		Grid* currentGrid = grids[i];
		//migth be more effective to store wich cells are occupied
		for (size_t q = 0; q < currentGrid->cellFactor; q++) {
			for (size_t w = 0; w < currentGrid->cellFactor; w++) {
				for (size_t e = 0; e < currentGrid->cellFactor; e++) {
					vector<Entity*>* bucket = currentGrid->getBucketAtHash(Vector3Int(q,w,e));
					if (!bucket->empty()) {
						auto insideSameBucket = brute(*bucket);
						pv->insert(pv->end(), insideSameBucket->begin(), insideSameBucket->end());
						delete insideSameBucket;

						//for half of the nearby cells(currently all)
						// do brute between this cell and the nearby one 
						
						collideNearby(currentGrid, Vector3Int(q, w, e),pv);

					}
				}

			}
		}

		//for everything in this grid, do kollision checks 
		//with everything in corresponding cells in higher grids
		for (auto entity : currentGrid->allEntitiesInGrid) {
			for (size_t k = i + 1; k < grids.size(); k++) {
				//in the corresponding cell
				Vector3Int hashedPos = grids[k]->hash(entity->getPos());
				vector<Entity*>* bucket = grids[k]->getBucketAtHash(hashedPos);
				if (!bucket->empty()) {
					auto pairs = brute(entity, *bucket);
					if (!pairs->empty()) {
						pv->insert(pv->end(), pairs->begin(), pairs->end());
					}
					delete pairs;
				}
				//in all the nearby cells in the higher grid

				auto inNearby = getNearbyCells(grids[k], hashedPos);
				if (!inNearby->empty()) {
					auto  nearbyPairs = brute(entity, *inNearby);
					if (!nearbyPairs->empty()) {
						pv->insert(pv->end(), nearbyPairs->begin(), nearbyPairs->end());
					}
					delete nearbyPairs;
				}
				delete inNearby;
				

			}
		}
	}

	return pv;
}
