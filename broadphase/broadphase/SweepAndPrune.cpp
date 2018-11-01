#include "SweepAndPrune.h"
#include <set>
#include <algorithm>

SweepAndPrune::SweepAndPrune() {
}


SweepAndPrune::~SweepAndPrune() {
}

void SweepAndPrune::update(vector<Entity*> &entityVector) {
	int dimentions = 1;
	//för alla dimentioner uppdatera alla endpoints
	for (int dim = 0; dim < dimentions; dim++) {
		//uppdatera endpunkter
		vector<Endpoint*> *vec = intervalls[dim];
		for (size_t i = 0; i < vec->size(); i++) {
			Endpoint* end = (*vec)[i];
			Entity* owner = end->owner;
			if (end->isMin) {
				end->value = owner->getPos()[dim] - owner->getSize()[dim];
			} else {
				end->value = owner->getPos()[dim] + owner->getSize()[dim];
			}
			owner->resetMoved();
		}
		//sortera listan
		instertionsort(*intervalls[dim]);
		//std::sort(intervalls[dim]->begin(), intervalls[dim]->end(), compareEndpoints());
	}
}

BroadPhase::PairVector* SweepAndPrune::coraseCollison(vector<Entity*> &entityVector) {
	PairVector* result = new PairVector();

	//create sets with all pairs
	set<Entity*> open;

	for (size_t j = 0; j < intervalls[0]->size(); j++) {
		if ((*intervalls[0])[j]->isMin) {
			Entity* owner = (*intervalls[0])[j]->owner;
			for (auto s : open) {
				if (owner->intersects(*s)) {
					result->push_back(make_pair(owner, s));
				}
			}
			open.insert(owner);
		} else {
			open.erase((*intervalls[0])[j]->owner);
		}
	}

	return result;
}

void SweepAndPrune::init(vector<Entity*> &entityVector) {
	int dimentions = 1;
	//for every dimention create a list of the intervalls
	for (int dim = 0; dim < dimentions; dim++) {
		vector<Endpoint*>* intervalList = new vector<Endpoint*>();

		for (size_t i = 0; i < entityVector.size(); i++) {
			//for every entity add the relevant endpoints (pos-size = min, pos + size = max)
			Entity* e = entityVector[i];
			intervalList->push_back(new Endpoint(e, e->getPos()[dim] - e->getSize()[dim], true));
			intervalList->push_back(new Endpoint(e, e->getPos()[dim] + e->getSize()[dim], false));
		}
		//lets sort it before we add it //add a quicksort here to speedup

		std::sort(intervalList->begin(), intervalList->end(), compareEndpoints());

		intervalls.push_back(intervalList);
	}
	return;
}

//insertionsort
void SweepAndPrune::instertionsort(vector<Endpoint*> &vec) {
	for (size_t i = 1; i < vec.size(); i++) {
		Endpoint* temp = vec[i];
		int j = i;
		while (j>0 && vec[j-1]->value > temp->value) {
			vec[j] = vec[j - 1];
			j--;
		}
		vec[j] = temp;
	}
	return;
}
