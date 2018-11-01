#include "BVH.h"


BVH::BVH() {
}


BVH::~BVH() {
}


void BVH::update(vector<Entity*> &entityVector) {
	if (_rootNode != nullptr) {
		delete _rootNode;
	}
	_rootNode = new Node(entityVector);

}

void checkCollision(Node* node, Entity* entity, BVH::PairVector* pairs) {
	entity->visit();
	if (node->isLeaf()) {
		//check collision against this object
		for (auto nodeEntity : node->_values) {
			if (!nodeEntity->hasVisited()) {
				if (entity->intersects(*nodeEntity))
					pairs->push_back(make_pair(entity, nodeEntity));
			}

		}
	}else{
		if (entity->intersects(node->_left->_bounding))
			checkCollision(node->_left, entity, pairs);
		if (entity->intersects(node->_rigth->_bounding))
			checkCollision(node->_rigth, entity, pairs);
	}
	
}

BVH::PairVector* BVH::coraseCollison(vector<Entity*> &entityVector) {
	auto pairs = new PairVector;
	for (auto entity : entityVector) {
		checkCollision(_rootNode, entity, pairs);
	}
	return pairs;
}

void BVH::init(vector<Entity*> &entityVector) {

}
