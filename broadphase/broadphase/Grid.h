#pragma once
#include <vector>
#include <algorithm>
#include "Entity.h"
#include <math.h>

using namespace std;

class Grid {
public:
	Grid() {}
	Grid(int size, int factor) {
		gridSize = size;
		cellSize = size / (float)factor;
		cellFactor = factor;

		//create a bucket for every cell
		int bucketCount = cellFactor*cellFactor*cellFactor;
		for (int i = 0; i <= cellFactor; i++) {
			vector<vector<vector<Entity*>>> e;
			for (int j = 0; j <= cellFactor; j++) {
				vector<vector<Entity*>> f;
				for (int k = 0; k <= cellFactor; k++) {
					f.push_back(vector<Entity*>());
				}
				e.push_back(f);
			}
			buckets.push_back(e);
		}
	}
	typedef vector<vector<vector<vector<Entity*>>>> EntityGrid;
	EntityGrid buckets;
	vector<Entity*> allEntitiesInGrid;
	int gridSize;
	float cellSize;
	int cellFactor;

	Vector3Int hash(Vector3 vec) {
		return Vector3Int(floor(vec.getX() / cellSize), floor(vec.getY() / cellSize), floor(vec.getZ() / cellSize));
	}

	bool isHashedPosValid(Vector3Int& pos, Vector3Int& offset) {
		for (int i = 0; i < 3; i++) {
			if ((pos[i] + offset[i]) < 0 || (pos[i] + offset[i]) >= cellFactor) {
				return false;
			}
		}
		return true;
	}

	void insert(Entity* entity) {
		Vector3Int hashed = hash(entity->getPos());
		buckets[hashed.getX()][hashed.getY()][hashed.getZ()].push_back(entity);
		allEntitiesInGrid.push_back(entity);
	}

	void removeFromGrid(Entity* entity) {
		removeFromBucket(getBucket(entity->getPos()), entity);
	}

	void removeFromBucket(vector<Entity*>* bucket, Entity* entity) {
		auto newEnd = remove(bucket->begin(), bucket->end(), entity); //remove it from the bucket
		bucket->erase(newEnd, bucket->end());

		auto newAllEnd = remove(allEntitiesInGrid.begin(), allEntitiesInGrid.end(), entity); //remove it from the bucket
		allEntitiesInGrid.erase(newAllEnd, allEntitiesInGrid.end());
	}

	void update(vector<Entity*> &entityVector) {
		for (int q = 0; q < cellFactor; q++) {
			for (int w = 0; w < cellFactor; w++) {
				for (int e = 0; e < cellFactor; e++) {
					vector<Entity*>* bucket = &buckets[q][w][e];

					//for (auto entity : *bucket) {
					for (size_t j = 0; j < bucket->size(); j++) {
						Entity* entity = (*bucket)[j];
						if (entity->hasMoved()) { //if it have moved
							if (!hash(entity->getPos()).compare(Vector3Int(q,w,e))) { //check if it moved out of the cell
								removeFromBucket(bucket, entity);
								insert(entity);

							}
							entity->resetMoved();
						}
					}
				}
			}
		}

	}

	EntityGrid* getBuckets() {
		return &buckets;
	}

	vector<Entity*>* getBucket(Vector3 &pos) {
		Vector3Int hashed = hash(pos);
		return &buckets[hashed.getX()][hashed.getY()][hashed.getZ()];
	}
	vector<Entity*>* getBucketAtHash(Vector3Int &pos) {
		return &buckets[pos.getX()][pos.getY()][pos.getZ()];
	}

};

