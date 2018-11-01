#pragma once
#include "BroadPhase.h"



class HierarcicalGrid2 :
	public BroadPhase {
public:
	HierarcicalGrid2();
	~HierarcicalGrid2();


	class Grid {
	public:

		typedef vector<vector<vector<vector<Entity*>>>> EntityGrid;
		EntityGrid buckets;
		vector<Entity*> allEntitiesInGrid;
		int gridSize;
		float cellSize;
		int cellFactor;


		typedef vector<Entity*> Cell;
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
	};
};

