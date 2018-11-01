#pragma once
#include <math.h>

class Vector3Int {
public:
	Vector3Int() {
		_x = 0;
		_y = 0;
		_z = 0;
	}
	Vector3Int(int x, int y, int z) {
		_x = x;
		_y = y;
		_z = z;
	}
	~Vector3Int() {}

	float getX() { return _x; }
	float getY() { return _y; }
	float getZ() { return _z; }

	int& operator[](int i) {
		if (i == 0) return _x;
		if (i == 1) return _y;
		if (i == 2) return _z;
		return _x;
	}

	Vector3Int operator+=(Vector3Int & b) {
		_x += b.getX();
		_y += b.getY();
		_z += b.getZ();
		return *this;
	}

	Vector3Int operator-=(Vector3Int & b) {
		_x -= b.getX();
		_y -= b.getY();
		_z -= b.getZ();
		return *this;
	}

	Vector3Int operator+(Vector3Int & b) {
		return Vector3Int(_x + b.getX(), _y + b.getY(), _z + b.getZ());
	}

	Vector3Int operator-(Vector3Int & b) {
		return Vector3Int(_x - b.getX(), _y - b.getY(), _z - b.getZ());
	}

	bool compare(Vector3Int other) {
		for (int i = 0; i < 3; i++) {
			if ((*this)[i] != other[i]) {
				return false;
			}
		}
		return true;

	}
private:
	int _x,_y,_z;
};

class Vector3
{
public:
	Vector3(){
		_x = 0;
		_y = 0;
		_z = 0;
	}
	Vector3(float x, float y, float z){
		_x = x;
		_y = y;
		_z = z;
	}
	~Vector3(){}

	float getX(){ return _x; }
	float getY(){ return _y; }
	float getZ(){ return _z; }

	Vector3 operator+=(Vector3 & b){
		_x += b.getX();
		_y += b.getY();
		_z += b.getZ();
		return *this;
	}

	Vector3 operator-=(Vector3 & b){
		_x -= b.getX();
		_y -= b.getY();
		_z -= b.getZ();
		return *this;
	}

	Vector3 operator+(Vector3 & b){
		return Vector3(_x + b.getX(), _y + b.getY(), _z + b.getZ());
	}

	Vector3 operator-(Vector3 & b){
		return Vector3(_x - b.getX(), _y - b.getY(), _z - b.getZ());
	}

	Vector3 operator*(float b){
		return Vector3(_x * b, _y *b, _z *b);
	}

	Vector3 absVector() {
		return Vector3(abs(_x), abs(_y), abs(_z));
	}

	bool compare(Vector3 other) {
		float smallnum = 0.05f;
		for (int i = 0; i < 3; i++) {
			if ((*this)[i] - other[i] < smallnum) {
				return false;
			}
		}
		return true;

	}

	float& operator[](int i){
		if (i == 0) return _x;
		if (i == 1) return _y;
		if (i == 2) return _z;
		return _x;
	}

	void clamp(float min, float max) {
		_x = (_x < 0 ? 0 : (_x > max ? max : _x));
		_y = (_y < 0 ? 0 : (_y > max ? max : _y));
		_z = (_z < 0 ? 0 : (_z > max ? max : _z));
	}

private:
	float _x;
	float _y;
	float _z;
};



