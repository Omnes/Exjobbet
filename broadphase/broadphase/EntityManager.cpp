#include "EntityManager.h"
#include "BruteForce.h"
#include <iostream>
#include <Windows.h>
using namespace std::chrono;

EntityManager::EntityManager() {
	currentMetod = new BruteForce();
}


EntityManager::~EntityManager() {
	reset();
}
void EntityManager::setPositions(vector<ObjectData*>& positions) {
	for (size_t i = 0; i < positions.size(); i++) {
		entityVector[positions[i]->_id]->setPosition(positions[i]->_vec);
	}
}

long long EntityManager::update() {

	//start time
	LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds;
	LARGE_INTEGER Frequency;

	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&StartingTime);

	currentMetod->update(entityVector);

	QueryPerformanceCounter(&EndingTime);
	ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
	ElapsedMicroseconds.QuadPart *= 1000000;
	ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;

	return ElapsedMicroseconds.QuadPart;
}

pair<long long,int> EntityManager::doCollision() {

	LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds;
	LARGE_INTEGER Frequency;

	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&StartingTime);

	//auto t1 = Clock::now();
	BroadPhase::PairVector* pairs = currentMetod->coraseCollison(entityVector);
	//auto t2 = Clock::now();

	QueryPerformanceCounter(&EndingTime);
	ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;

	ElapsedMicroseconds.QuadPart *= 1000000;
	ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;

	auto result = make_pair(ElapsedMicroseconds.QuadPart, pairs->size());
	delete pairs;
	resetVisits();
	return result;
}

void EntityManager::setBroadPhase(BroadPhase* broad) {
	currentMetod = broad;
}

void EntityManager::resetVisits() {
	for (auto e : entityVector) {
		e->resetVisited();
	}
}

void EntityManager::reset() {
	while (!entityVector.empty()) {
		delete entityVector.back();
		entityVector.pop_back();
	}
}

void EntityManager::initBroadPhase() {
	if (currentMetod != nullptr) {
		currentMetod->init(entityVector);
	}
}

void EntityManager::add(Entity* entity) {
	entityVector.push_back(entity);
}
