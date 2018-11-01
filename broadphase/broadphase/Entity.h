#pragma once
#include "Vector3.h"
#include "AABB.h"
class Entity{
public:
	Entity(Vector3 &position,Vector3 &size,int id);
	~Entity();
	void setPosition(Vector3 &position);
	//void move(Vector3 &pos);
	Vector3 getPos();
	Vector3 getSize();
	Vector3 getHalfSize();
	bool intersects(Entity &b);
	bool intersects(AABB &b);
	static bool intersects(Entity &a, Entity &b);
	bool hasMoved() { return _hasMoved; }
	void resetMoved() {
		_hasMoved = false;
	}
	void visit() {
		_visited = true;
	}
	bool hasVisited() {
		return _visited;
	}
	void resetVisited() {
		_visited = false;
	}
	int getId() {
		return _id;
	}

private:
	AABB bounding;
	bool _hasMoved;
	bool _visited;
	int _id;

};

