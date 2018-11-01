#pragma once
#include "EntityManager.h"
class Session {
public:
	Session();
	~Session();
	void load(string filepath);
	void run();
private:
	void writeResultsToXml();

	EntityManager man;
	vector<vector<ObjectData*>*> frames;
	vector<BroadPhase*> algorithms;
	typedef vector<pair<int, int>> ResultVector;
	vector<ResultVector> results;
	string loadedJsonPath;
};

