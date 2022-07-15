#include "Octree.h"

template<typename T>
float Octree<T>::GetSizeFromPointCloud(PointCloud* pcl){
	float tempSize = 0;
	tempSize = pcl->rigthBoundaryPoint[0].coords[0] - pcl->leftBoundaryPoint[0].coords[0];
	if ((pcl->upBoundaryPoint[1].coords[1] - pcl->downBoundaryPoint[1].coords[1]) > tempSize) {
		tempSize = (pcl->upBoundaryPoint[1].coords[1] - pcl->downBoundaryPoint[1].coords[1]);
	}
	if ((pcl->frontBoundaryPoint[2].coords[2] - pcl->backBoundaryPoint[2].coords[2]) > tempSize) {
		tempSize = (pcl->frontBoundaryPoint[2].coords[2] - pcl->backBoundaryPoint[2].coords[2]);
	}

	return tempSize;

}

/// <summary>
/// This method will create an octree from point cloud.
/// </summary>
/// <typeparam name="T"></typeparam>
/// <param name="pcl"></param>
/// <param name="closenessThreshold", Will be used for setting the threshold for stopping the subdivision of octree.></param>
template<typename T>
void Octree<T>::BuiltOctreeFromPointCloud(PointCloud* pcl,float closenessThreshold) {
	size = GetSizeFromPointCloud(pcl);
	root = new OctreeNode<T>(pcl->geometricCenterPoint, size); //root node initialized
	nodes->push_back(new NodeParameters(root->position, size));

	OctreeNode<T> *currentNode;
	currentNode = root;
	//now we will go iteratively over all points in point cloud for creating the octree.

	for (int i = 0; i < pcl->points.size(); i++) {
		if (sqrDistance(currentNode->position, pcl->points[i]->coords)>closenessThreshold) {
			RecursiveSubdivision(currentNode, pcl->points[i]->coords, closenessThreshold);
			currentNode = root; //we need to get back to root for new point.
		}
	}



}

template<typename T>
float Octree<T>::sqrDistance(float* point1, float* point2){
	return (pow(point1[0] - point2[0], 2) + pow(point1[1] - point2[1], 2) + pow(point1[2] - point2[2], 2));
}

template<typename T>
void Octree<T>::RecursiveSubdivision(OctreeNode<T>* _currentNode, float* point, float threshold){
	if (sqrDistance(_currentNode->position, point) > threshold) {
		if(_currentNode->isLeaf())
			_currentNode->SubdivideNode(nodes);

		_currentNode = _currentNode->subNodes[_currentNode->GetIndexByPosition(point)];

		RecursiveSubdivision(_currentNode,point,threshold);
	}
	return;
}

template<typename T>
Octree<T>::Octree()
{
	printf("Octree instance created but empty!\n");
	if (typeid(T) == typeid(PointCloud)) {
		printf("Octree's type is Point Coud\n");
	}
}
