#pragma once
#include "BroadPhase.h"
class SweepAndPrune :
	public BroadPhase {
public:
	SweepAndPrune();
	~SweepAndPrune();

	void update(vector<Entity*> &entityVector);
	PairVector* coraseCollison(vector<Entity*> &entityVector);
	void init(vector<Entity*> &entityVector);

	struct Endpoint {
		Entity* owner;
		float value;
		bool isMin;

		Endpoint(Entity* own, float val, bool min) {
			owner = own;
			value = val;
			isMin = min;
		}
		
	};
	
	struct CollisionPair {
		Entity* first;
		Entity* second;
		CollisionPair(Entity* a,Entity* b) {
			first = a;
			second = b;
		}

		bool operator==(const CollisionPair &rigth) const {
			return first + (int)second == rigth.first + (int)rigth.second;
		}

		bool operator<(const CollisionPair &rigth)const {
			return first + (int)second < rigth.first + (int)rigth.second;
		}

		pair<Entity*, Entity*> toPair() {
			return make_pair(first, second);
		}
	};

	struct compareEndpoints {
		bool operator()(Endpoint* a, Endpoint* b) {
			return a->value < b->value;
		}
	};

	string getId() { return "SaP"; }

private:
	vector<vector<Endpoint*>*> intervalls;

	void instertionsort(vector<Endpoint*> &vec);


};

