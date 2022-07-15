#pragma once
#ifndef OCTREE_H
#define OCTREE_H

#ifdef DISABLE_PRINTF
#define printf(fmt, ...) (0)
#endif
#include "PointCloud.h"
#include "OctreeNode.h"
#include <typeinfo>

template<typename T>
class Octree {
private:
	float size; //size of biggest box
	float threshold;
	float GetSizeFromPointCloud( PointCloud* pcl);
	float sqrDistance(float* point1, float* point2);
	void RecursiveSubdivision(OctreeNode<T>* _currentNode, float* point, float threshold, T* objects);
	OctreeNode<T>* RecursiveNodeFinder(OctreeNode<T>* _currentNode, float* point);
public:
	int totalSubdiv = 0;
	Octree();
	OctreeNode<T> *root = new OctreeNode<T>();
	vector<NodeParameters*> nodes;
	void BuiltOctreeFromPointCloud(PointCloud *pcl,float closenessThreshold, vector<T*> vect);
	float* GetClosestNodePosFromPoint(float* point);
	OctreeNode<T>* GetNodeFromPoint(float* point);
	float* GetClosestObject(float* point);
};

template<typename T>
float Octree<T>::GetSizeFromPointCloud(PointCloud* pcl) {
	float tempSize = 0;

	tempSize = pcl->rigthBoundaryPoint->coords[0] - pcl->leftBoundaryPoint->coords[0];

	if ((pcl->upBoundaryPoint->coords[1] - pcl->downBoundaryPoint->coords[1]) > tempSize) {
		tempSize = (pcl->upBoundaryPoint->coords[1] - pcl->downBoundaryPoint->coords[1]);
	}
	if ((pcl->frontBoundaryPoint->coords[2] - pcl->backBoundaryPoint->coords[2]) > tempSize) {
		tempSize = (pcl->frontBoundaryPoint->coords[2] - pcl->backBoundaryPoint->coords[2]);
	}

	return tempSize;

}

/// <summary>
/// This method will create octree from the loaded pointcloud.
/// Min Node Size will be the minimum size of the node which will be an integer power of 2 
/// </summary>
/// <typeparam name="T"></typeparam>
/// <param name="pcl"></param>
/// <param name="minNodeSize"></param>
/// <param name="vect"></param>
template<typename T>
void Octree<T>::BuiltOctreeFromPointCloud(PointCloud* pcl, float minNodeSize, vector<T*> vect) {
	if (vect.size() != pcl->points.size()) {
		return;
	}
	size = GetSizeFromPointCloud(pcl);
	threshold = minNodeSize;
	root->position[0] = pcl->geometricCenterPoint[0];
	root->position[1] = pcl->geometricCenterPoint[1];
	root->position[2] = pcl->geometricCenterPoint[2];
	root->size = size;
	root->nodeIdx = nodes.size();


	nodes.push_back(new NodeParameters(root->position, size));
	OctreeNode<T>* currentNode = root;
	currentNode = root;

	for (int i = 0; i < pcl->points.size(); i++) {
		if (sqrDistance(currentNode->position, pcl->points[i]->coords) > minNodeSize) {
			RecursiveSubdivision(currentNode, pcl->points[i]->coords, minNodeSize,vect[i]);
			currentNode = root; //we need to get back to root for new point.
		}
	}


}

template<typename T>
void Octree<T>::RecursiveSubdivision(OctreeNode<T>* _currentNode, float* point, float threshold, T* objects) {
	if (_currentNode->size > threshold) {
		if (_currentNode->isLeaf()) {
			_currentNode->SubdivideNode(nodes, threshold);
			totalSubdiv++;

		}
		_currentNode = _currentNode->subNodes[_currentNode->GetIndexByPosition(point)];
		RecursiveSubdivision(_currentNode, point, threshold,objects);
	}
	else {
		_currentNode->RecursivelyMarkParent();
		nodes[_currentNode->nodeIdx]->hasObject = true;
		_currentNode->objects.push_back(objects);
	
	}
	return;
}

template<typename T>
float Octree<T>::sqrDistance(float* point1, float* point2) {
	float temp = (pow(point1[0] - point2[0], 2) + pow(point1[1] - point2[1], 2) + pow(point1[2] - point2[2], 2));
	return temp;
}

template<typename T>
Octree<T>::Octree()
{
	
}
template<typename T>
OctreeNode<T>* Octree<T>::GetNodeFromPoint(float* point) {
	OctreeNode<T> *tempNode = new OctreeNode<T>();
	tempNode = root;
	tempNode = RecursiveNodeFinder(tempNode, point);
	float* temp = new float[4];
	temp[0] = tempNode->position[0];
	temp[1] = tempNode->position[1];
	temp[2] = tempNode->position[2];
	temp[3] = tempNode->size;
	return tempNode;
}

template<typename T>
OctreeNode<T>* Octree<T>::RecursiveNodeFinder(OctreeNode<T>* _currentNode, float* point)
{
	if (!_currentNode->isLeaf() && (sqrDistance(_currentNode->position,point)>threshold)) {
		_currentNode = _currentNode->subNodes[_currentNode->GetIndexByPosition(point)];
		_currentNode = RecursiveNodeFinder(_currentNode, point);
		return _currentNode;
	}

	else {
		return _currentNode;

	}

}
template <typename T>
float* Octree<T>::GetClosestNodePosFromPoint(float* point) {
	OctreeNode<T>* temp;
	temp = GetNodeFromPoint(point);
	float* tempF = new float[4];
	tempF[0] = temp->position[0];
	tempF[1] = temp->position[1];
	tempF[2] = temp->position[2];
	tempF[3] = temp->size;
	return tempF;
}

/// <summary>
/// This function will return the position of closest non-empty(also the smallest) octree node's
/// position
/// 
/// </summary>
/// <typeparam name="T"></typeparam>
/// <param name="point"></param>
/// <returns></returns>
template<typename T>
float* Octree<T>::GetClosestObject(float* point) {
	OctreeNode<T>* temp;
	temp = root->GetClosestNonEmptyNodeByPosition(point, root);
	float* tempf = new float[4];
	tempf[0] = temp->position[0];
	tempf[1] = temp->position[1];
	tempf[2] = temp->position[2];
	tempf[3] = temp->size;
	return tempf;
}

#endif // !OCTREE_H

