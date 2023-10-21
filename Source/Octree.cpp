#include "Octree.h"
Octree::Octree() : size(-1), threshold(INFINITY)
{

}
float Octree::GetSizeFromVertexContainer(VertexContainer& pcl) {
	float tempSize = 0;

	tempSize = pcl.rigthBoundaryPoint->coords[0] - pcl.leftBoundaryPoint->coords[0];

	if ((pcl.upBoundaryPoint->coords[1] - pcl.downBoundaryPoint->coords[1]) > tempSize) {
		tempSize = (pcl.upBoundaryPoint->coords[1] - pcl.downBoundaryPoint->coords[1]);
	}
	if ((pcl.frontBoundaryPoint->coords[2] - pcl.backBoundaryPoint->coords[2]) > tempSize) {
		tempSize = (pcl.frontBoundaryPoint->coords[2] - pcl.backBoundaryPoint->coords[2]);
	}

	return tempSize;
}

void Octree::BuiltOctreeFromVertexContainer(VertexContainer& pcl, float minNodeSize) {

	size = GetSizeFromVertexContainer(pcl);
	threshold = minNodeSize;
	root.position[0] = pcl.geometricCenterPoint[0];
	root.position[1] = pcl.geometricCenterPoint[1];
	root.position[2] = pcl.geometricCenterPoint[2];
	root.size = size;
	root.nodeIdx = nodes.size();


	nodes.push_back(std::make_shared<NodeParameters>(root.position, size));
	OctreeNode* currentNode = &root;

	for (int i = 0; i < pcl.points.size(); i++) {
		if (sqrDistance(currentNode->position, pcl.points[i]->coords) > minNodeSize) {
			RecursiveSubdivision(currentNode, pcl.points[i]->coords);
			currentNode = &root; //we need to get back to root for new point.
		}
	}
}

void Octree::RecursiveSubdivision(OctreeNode* _currentNode, Vec3& point) {
	if (_currentNode->size > threshold) {
		if (_currentNode->isLeaf()) {
			_currentNode->SubdivideNode(nodes, threshold);
			totalSubdiv++;

		}
		_currentNode = _currentNode->subNodes[_currentNode->GetIndexByPosition(point)].get();
		RecursiveSubdivision(_currentNode, point);
	}
	else {
		_currentNode->RecursivelyMarkParent();
		nodes[_currentNode->nodeIdx]->hasObject = true;
		_currentNode->objects.push_back(point);

	}
	return;
}

float Octree::sqrDistance(Vec3 point1, Vec3 point2) {
	float temp = (pow(point1[0] - point2[0], 2) + pow(point1[1] - point2[1], 2) + pow(point1[2] - point2[2], 2));
	return temp;
}

const OctreeNode* Octree::GetNodeFromPoint(Vec3 point) {
	const OctreeNode* tempNode;
	tempNode = &root;
	tempNode = RecursiveNodeFinder(tempNode, point);
	return tempNode;
}

const OctreeNode* Octree::RecursiveNodeFinder(const OctreeNode* _currentNode, Vec3 point)
{
	if (!_currentNode->isLeaf() && (sqrDistance(_currentNode->position, point) > threshold)) {
		_currentNode = _currentNode->subNodes[_currentNode->GetIndexByPosition(point)].get();
		_currentNode = RecursiveNodeFinder(_currentNode, point);
		return _currentNode;
	}

	else {
		return _currentNode;
	}
}
Vec3 Octree::GetClosestNodePosFromPoint(Vec3 point) {
	const OctreeNode* temp;
	if(temp = GetNodeFromPoint(point))
		return temp->position;
	else
		return Vec3();
}

/// <summary>
/// This function will return the position of closest non-empty(also the smallest) octree node's
/// position
/// 
/// </summary>
/// <param name="point"></param>
/// <returns></returns>
Vec3 Octree::GetClosestObject(Vec3 point) {
	const OctreeNode* temp;
	temp = root.GetClosestNonEmptyNodeByPosition(point, &root);
	return temp->position;
}
