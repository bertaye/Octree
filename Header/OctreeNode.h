#pragma once
#include<iostream>
#include "Commons.h"
#include<vector>

class OctreeNode {
public:
	OctreeNode();
	
private:
	bool subNodesHasObject=false;
	Vec3 position; //poisiton of each node in xyz coords
	float size;
	int nodeIdx;
	std::unique_ptr<OctreeNode> subNodes[CHILD_COUNT]; //subnodes
	OctreeNode* parent;
	std::vector<Vec3> objects; //objects of this node. this can be points, vertices, etc.
	void SubdivideNode(std::vector<std::shared_ptr<NodeParameters>>& nodes,float threshold);
	void RecursivelyMarkParent();
	bool isLeaf() const;
	int GetIndexByPosition(Vec3 lookUpPosition) const;
	const OctreeNode* GetClosestNonEmptyNodeByPosition(Vec3 lookUpPosition,const OctreeNode* node) const;
	const OctreeNode* RecursiveNonEmptyFinder(const Vec3& lookUpPosition,const OctreeNode* node, float& lastDist) const;
	float sqrDistance(Vec3 point1, Vec3 point2) const;
	friend class Octree;
	friend class OctreeNode;

};