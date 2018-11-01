#pragma once
#include "BroadPhase.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include "BruteForce.h"
#include "Grid.h"


class HierarchicalGrid3 :
	public BroadPhase {
public:
	HierarchicalGrid3();
	~HierarchicalGrid3();

	void init(vector<Entity*> &entityVector);
	void update(vector<Entity*> &entityVector);
	PairVector* coraseCollison(vector<Entity*> &entityVector);

	string getId() { return "HGrid"; }

private:
	void checkCollison(Entity* entity,int gridNr,PairVector* pv);
	vector<Grid*> grids;
	vector<Entity*>* getNearbyCells(Grid* grid, Vector3Int pos);
};

