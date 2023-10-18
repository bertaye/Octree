#pragma once
#include <stdexcept>
#if defined(SILENCE_DEBUG)
#define COMMANTABLE_OUTPUT(...) \
			do { } while (0)
#else
#define COMMANTABLE_OUTPUT(...) \
			printf(__VA_ARGS__) 
#endif

const int CHILD_COUNT = 8;

struct Vec3
{
	float _x, _y, _z;
	float _err;
	Vec3() = default;
	Vec3(float x, float y, float z) :_x(x), _y(y), _z(z), _err(INFINITY) {};

	float& Vec3::operator[](int idx) {
		switch (idx) {
		case 0:
			return _x;
		case 1:
			return _y;
		case 2:
			return _z;
		default:
			COMMANTABLE_OUTPUT("Invalid idx called for Vec3: %d", idx);
			throw std::invalid_argument("Incorrect indexing for Vec3");
			return _err; //Some invalid value...
		}
	}

	const float Vec3::operator[](int idx) const{
		switch (idx) {
		case 0:
			return _x;
		case 1:
			return _y;
		case 2:
			return _z;
		default:
			COMMANTABLE_OUTPUT("Invalid idx called for Vec3: %d", idx);
			throw std::invalid_argument("Incorrect indexing for Vec3");
			return _err; //Some invalid value...
		}
	}
};

struct Vertex {
	Vec3 coords;
	int idx;
	Vertex(int i, Vec3 c) : idx(i), coords(c) {};
};


enum class OctreeNodeIndex {
	/*
	*	______________
	*	|	   |	  |
		| LUB  | RUB  |
		|______|______|		LUB: Left Up Back
		|	   |	  |
		| LDB  | RDB  |
		|______|______|

		000 -> LEFT, DOWN, BACK
		111 -> RIGHT, UP, FRONT
	*/

	LeftDownBack = 0,
	LeftUpBack = 2,
	RightDownBack = 4,
	RightUpBack = 6,

	LeftDownFront = 1,
	LeftUpFront = 3,
	RightDownFront = 5,
	RightUpFront = 7,
};
struct NodeParameters {
	Vec3 coords;
	float size;
	bool isLeaf;
	bool hasObject;
	NodeParameters(Vec3 _coords, float _size) : coords(_coords), size(_size), isLeaf(true), hasObject(false) {
		//printf("Node parameter created!");
	};
};