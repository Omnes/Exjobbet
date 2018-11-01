#pragma once
#include "Vector3.h"
#include <math.h>
class AABB{
public:
	AABB(){
		_position = Vector3();
		_size = Vector3();
	}

	AABB(Vector3 &position, Vector3 &size){
		_position = Vector3(position);
		_size = Vector3(size);
	}
	~AABB(){}

	Vector3 getPos(){
		return _position;
	}

	Vector3 getSize() {
		return _size;
	}

	Vector3 getHalfSize(){
		return _size*0.5;
	}

	void setPosition(Vector3 &position) {
		_position = position;
	}

	bool intersects(AABB & b){
		for (int i = 0; i < 3; i++){
			if (abs(getPos()[i] - b.getPos()[i]) > (getHalfSize()[i] + b.getHalfSize()[i])){
				return false;
			}
		}
		return true;
	}

	static bool intersects(AABB & a, AABB & b){
		return a.intersects(b);
	}


private:
	Vector3 _position;
	Vector3 _size;
};

