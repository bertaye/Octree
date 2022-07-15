#pragma once
#ifndef POINTCLOUD_H
#define POUNTCLOUD_H
#include <vector>
#include<ctype.h>
#include <string>

using namespace std;

struct Point {
	float* coords;
	int idx;
	Point(int i, float* c) : idx(i), coords(c) {};

};

class PointCloud {
	private:
		void AddPoint(float x, float y, float z);
		float* calculateMidPoint(float* c1, float* c2, float c1_coeff, float c2_coeff);
	public:
		float* centerPoint;
		float* geometricCenterPoint;
		Point *leftBoundaryPoint, * rigthBoundaryPoint,
			* upBoundaryPoint, * downBoundaryPoint,
			*frontBoundaryPoint,*backBoundaryPoint; //for creating octree
		vector<Point*> points;
		void LoadPly(const char* name);
		void MoveCenter(float x, float y, float z, bool snap = false);
		void ScalePly(float scaleFactor);

};

#endif