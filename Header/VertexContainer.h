#pragma once
#include <vector>
#include <ctype.h>
#include <string>
#include <memory>
#include "Commons.h"
class VertexContainer {

	private:
		void AddPoint(float x, float y, float z);
		Vec3 calculateMidPoint(const Vec3& c1, const Vec3& c2, float c1_coeff, float c2_coeff);
	public:
		Vec3 centerPoint;
		Vec3 geometricCenterPoint;
		std::shared_ptr<Vertex> leftBoundaryPoint, rigthBoundaryPoint,
			upBoundaryPoint, downBoundaryPoint,
			frontBoundaryPoint, backBoundaryPoint; //for creating octree
		std::vector<std::shared_ptr<Vertex>> points;
		void LoadPly(const char* name);
		void MoveCenter(float x, float y, float z, bool snap = false);
		void Scale(float scaleFactor);

};