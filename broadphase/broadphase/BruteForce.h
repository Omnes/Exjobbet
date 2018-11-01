#pragma once
#include "BroadPhase.h"
class BruteForce :
	public BroadPhase {
public:
	BruteForce();
	~BruteForce();

	void update(vector<Entity*> &entityVector);
	PairVector* coraseCollison(vector<Entity*> &entityVector);
	void init(vector<Entity*> &entityVector);
	string getId() { return "Brute"; }

};

