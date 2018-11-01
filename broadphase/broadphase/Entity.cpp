#include "Entity.h"
#include <math.h>
#include "Constants.h"

Entity::Entity(Vector3 &position,Vector3 &size,int id):bounding(AABB(position,size)),_id(id){

}

Entity::~Entity(){
}

void Entity::setPosition(Vector3 &position){
	bounding.setPosition(position);
	_hasMoved = true;
}
/*
void Entity::move(Vector3 &pos) {
	_position += pos; 
	_position.clamp(0, Area);
	_hasMoved = true;
}*/

Vector3 Entity::getPos() {
	return bounding.getPos();
}

Vector3 Entity::getSize() {
	return bounding.getSize();
}

Vector3 Entity::getHalfSize() {
	return bounding.getHalfSize();
}


bool Entity::intersects(Entity &b) {
	for (int i = 0; i < 3; i++) {
		if (abs(getPos()[i] - b.getPos()[i]) > (getHalfSize()[i] + b.getHalfSize()[i])) {
			return false;
		}
	}
	return true;
}

bool Entity::intersects(AABB &b) {
	for (int i = 0; i < 3; i++) {
		if (abs(getPos()[i] - b.getPos()[i]) > (getHalfSize()[i] + b.getHalfSize()[i])) {
			return false;
		}
	}
	return true;
}

bool Entity::intersects(Entity &a, Entity &b) {
	return a.intersects(b);
}
