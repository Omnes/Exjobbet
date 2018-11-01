#pragma once
#include <vector>
#include <utility>
#include "Entity.h"

using namespace std;

class BroadPhase {
public:
	typedef vector < pair<Entity*, Entity*>>  PairVector;
	virtual void update(vector<Entity*> &entityVector) = 0;
	virtual PairVector* coraseCollison(vector<Entity*> &entityVector) = 0;
	virtual void init(vector<Entity*> &entityVector) = 0;
	virtual string getId() = 0;

	
protected:
	BroadPhase() {}
};