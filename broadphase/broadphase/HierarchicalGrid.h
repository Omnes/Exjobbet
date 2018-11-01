#pragma once
#include "BroadPhase.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include "BruteForce.h"
#include "Grid.h"




class HierarchicalGrid :
	public BroadPhase {
public:
	HierarchicalGrid();
	~HierarchicalGrid();

	void init(vector<Entity*> &entityVector);
	void update(vector<Entity*> &entityVector);
	PairVector* coraseCollison(vector<Entity*> &entityVector);

	string getId() { return "HGrid"; }

private:
	vector<Grid*> grids;
	vector<Entity*>* getNearbyCells(Grid* grid,Vector3Int pos);
	void collideNearby(Grid* grid, Vector3Int pos, BroadPhase::PairVector* pv);
};

