#pragma once
#include <vector>
#include "Entity.h"
#include "BroadPhase.h"
#include <chrono>

using namespace std;

class ObjectData {
public:
	int _id;
	Vector3 _vec;
	ObjectData(int id, Vector3 vec) {
		_id = id;
		_vec = vec;
	}
};

class EntityManager {
public:
	EntityManager();
	~EntityManager();
	long long update();
	pair<long long, int> doCollision();
	void setBroadPhase(BroadPhase* broad);
	void reset();
	void add(Entity* entity);
	void setPositions(vector<ObjectData*>& positions);
	void initBroadPhase();
	void resetVisits();

private:
	vector<Entity*> entityVector;
	BroadPhase *currentMetod;
	typedef std::chrono::high_resolution_clock Clock;

	//vector position data
};




