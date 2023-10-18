#pragma once

#include "PointCloud.h"
#include "OctreeNode.h"
#include "Commons.h"
#include <typeinfo>

class Octree {
public:
	Octree();
	OctreeNode root;
	std::vector<std::shared_ptr<NodeParameters>> nodes;
	void BuiltOctreeFromPointCloud(VertexContainer& pcl,float closenessThreshold);
	Vec3 GetClosestNodePosFromPoint(Vec3 point);
	const OctreeNode* GetNodeFromPoint(Vec3 point);
	Vec3 GetClosestObject(Vec3 point);
	int totalSubdiv = 0;
private:
	float size; //size of biggest box
	float threshold;
	float GetSizeFromPointCloud( VertexContainer& pcl);
	float sqrDistance(Vec3 point1, Vec3 point2);
	void RecursiveSubdivision(OctreeNode* _currentNode, Vec3& point);
	const OctreeNode* RecursiveNodeFinder(const OctreeNode* _currentNode, Vec3 point);
};