#pragma once
#include "BroadPhase.h"
#include <algorithm>

class Node {
public:
	vector<Entity*> _values;
	Node* _left;
	Node* _rigth;
	AABB _bounding;
	const int LeafCapacity = 10;

	int getLongestAxis(AABB& aabb) {
		int longest = 0;
		int length = aabb.getHalfSize()[0];
		for (int i = 1; i < 3; i++) {
			if (aabb.getHalfSize()[i] > length) {
				length = aabb.getHalfSize()[i];
				longest = i;
			}
		}
		return longest;
	}

	AABB calculateEnclosing(vector<Entity*>& e) {
		Vector3 min;
		Vector3 max;
		max = e[0]->getPos() + e[0]->getHalfSize();
		min = e[0]->getPos() - e[0]->getHalfSize();
		
		for (size_t i = 1; i < e.size(); i++) {
			Vector3 thisMax = e[i]->getPos() + e[i]->getHalfSize();
			Vector3 thisMin = e[i]->getPos() - e[i]->getHalfSize();
			for (size_t j = 0; j < 3; j++) {
				if (thisMax[j] > max[j]) max[j] = thisMax[j];
				if (thisMin[j] < min[j]) min[j] = thisMin[j];
			}
		}
		Vector3 size = max - min;
		size = size.absVector();
		Vector3 pos = min + size*0.5f;
		return AABB(pos, size);
	}

	void sortAlongAxis(vector<Entity*>& vec, int axis) {
		std::sort(vec.begin(), vec.end(),
			[axis](Entity *a, Entity *b)->bool {
			return a->getPos()[axis] < b->getPos()[axis]; 
		});
	}

	bool isLeaf() {
		return _values.size() <= LeafCapacity;
	}

	Node() {}
	Node(vector<Entity*> entities) {
		_values = entities;
		_bounding = calculateEnclosing(_values);
		if (_values.size() > LeafCapacity) {
			//find longest axis
			int longest = getLongestAxis(_bounding);
			//sort objects at longest axis
			sortAlongAxis(_values, longest);
			//split down the middle (left/rigth)
			std::vector<Entity*> left(_values.begin(), _values.begin() + _values.size() / 2);
			std::vector<Entity*> rigth(_values.begin() + _values.size() / 2, _values.end());
			_left = new Node(left);
			_rigth = new Node(rigth);
		}
	}
	~Node() {
		if(_left != nullptr) delete _left;
		if (_rigth != nullptr) delete _rigth;
	}
};


class BVH :
	public BroadPhase {
public:
	BVH();
	~BVH();

	void update(vector<Entity*> &entityVector);
	PairVector* coraseCollison(vector<Entity*> &entityVector);
	void init(vector<Entity*> &entityVector);
	string getId() { return "BVH"; }

private:
	

	Node* _rootNode;
};

