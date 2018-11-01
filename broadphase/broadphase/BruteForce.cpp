#include "BruteForce.h"


BruteForce::BruteForce() {
}


BruteForce::~BruteForce() {
}

void BruteForce::update(vector<Entity*> &entityVector) {
	return;
}

BroadPhase::PairVector* BruteForce::coraseCollison(vector<Entity*> &entityVector) {
	PairVector* result = new PairVector();
	for (size_t i = 0; i < entityVector.size(); i++) {
		for (size_t j = i + 1; j < entityVector.size(); j++) {
			if (entityVector[i]->intersects(*entityVector[j])) {
				result->push_back(make_pair(entityVector[i], entityVector[j]));
			}
		}
	}
	return result;
}

void BruteForce::init(vector<Entity*> &entityVector) {
	return;
}
