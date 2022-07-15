#pragma once
#ifndef OCTREENODE_H
#define OCTREENODE_H

#define NO_CHILDREN 8
#include<vector>
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
	float* coords;
	float size;
	bool isLeaf;
	bool hasObject;
	NodeParameters(float* _coords, float _size) : coords(_coords), size(_size), isLeaf(true), hasObject(false) {
		//printf("Node parameter created!");
	};
};

template<typename T>
class OctreeNode {
public:
	OctreeNode() {
		
	}
	

private:
	bool subNodesHasObject=false;
	float* position = new float[3]; //poisiton of each node in xyz coords
	float size;
	int nodeIdx;
	OctreeNode<T> *subNodes[NO_CHILDREN]; //subnodes
	OctreeNode<T> *parent;
	std::vector <T*> objects; //objects of this node. this can be points, vertices, etc.
	void SubdivideNode(std::vector<NodeParameters*> &nodes,float threshold);
	void RecursivelyMarkParent();
	bool isLeaf();
	int GetIndexByPosition(float* lookUpPosition);
	OctreeNode<T>* GetClosestNonEmptyNodeByPosition(float* lookUpPosition, OctreeNode<T>* node);
	OctreeNode<T>* RecursiveNonEmptyFinder(float* lookUpPosition, OctreeNode<T>* node, float *lastDist);
	float sqrDistance(float* point1, float* point2);
	template<typename U>
	friend class Octree;
	friend class OctreeNode;

};
template<typename T>
void OctreeNode<T>::SubdivideNode(std::vector<NodeParameters*> &nodes,float threshold) {
	//printf("Subdivide node called!\n");
	nodes[nodeIdx]->isLeaf = false;
	float newPos[3] = { 0,0,0 };
	newPos[0] = position[0];
	newPos[1] = position[1];
	newPos[2] = position[2];
	//printf("Position before subdivision = %f, %f, %f \n", newPos[0], newPos[1], newPos[2]);
	//printf("Size before subdivision = %f \n", size);
	for (int i = 0; i < 8; i++) {
		newPos[0] = position[0];
		newPos[1] = position[1];
		newPos[2] = position[2];
		/*LeftDownBack = 0,
			LeftUpBack = 2,
			RightDownBack = 4,
			RightUpBack = 6,

			LeftDownFront = 1,
			LeftUpFront = 3,
			RightDownFront = 5,
			RightUpFront = 7,*/

		//printf("%i th subdivision!\n", i);
		/*
			We have _ _ _ = X Y Z, so 100 = 4 ->RIGHT
									  010 = 2 -> UP
									  001 = 1 -> FRONT
		*/
		if (i==4 || i==6 || i==5 || i==7) { //right
			newPos[0] += size * 0.25f;
		}
		else {
			newPos[0] -= size * 0.25f;
		}

		if (i == 2 || i==6 || i==3 || i==7) { //up
			newPos[1] += size * 0.25f;
		}
		else {
			newPos[1] -= size * 0.25f;
		}

		if (i==1 || i==3 || i==5 || i==7) { //front
			newPos[2] += size * 0.25f;
		}
		else {
			newPos[2] -= size * 0.25f;
		}

		subNodes[i] = new OctreeNode<T>();
		subNodes[i]->size = size * 0.5f;
		subNodes[i]->position[0] = newPos[0];
		subNodes[i]->position[1] = newPos[1];
		subNodes[i]->position[2] = newPos[2];
		subNodes[i]->nodeIdx = nodes.size();
		subNodes[i]->parent = this;
		nodes.push_back(new NodeParameters(subNodes[i]->position, size * 0.5f));

	}
}

template<typename T>
bool OctreeNode<T>::isLeaf() {
	return subNodes[0] == nullptr;
}

template<typename T>
int OctreeNode<T>::GetIndexByPosition(float* lookUpPosition) {
	if (isLeaf()) {
		//printf("Node leaf cant give index by position!\n");
		return -1;
	}
	int index = 0;
	//printf("Look Up Position = %f,%f,%f\n", lookUpPosition[0], lookUpPosition[1], lookUpPosition[2]);
	//printf("Node Position = %f,%f,%f\n", position[0], position[1], position[2]);

	if (lookUpPosition[0] > position[0]) {
		index += 4;
		//printf("Lookup position is on RIGHT of this node\n");
	}
	else {
		//printf("Lookup position is on LEFT of this node\n");

	}
	if (lookUpPosition[1] > position[1]) {
		index += 2;
		//printf("Lookup position is on UP of this node\n");

	}
	else {
		//printf("Lookup position is on DOWN of this node\n");

	}
	if (lookUpPosition[2] > position[2]) {
		index += 1;
		//printf("Lookup position is on FRONT of this node\n");

	}
	else{
		//printf("Lookup position is on BACK of this node\n");

	}
	//printf("\n\nReturned Index = %d\n\n", index);
	return index;
}
template<typename T>
void OctreeNode<T>::RecursivelyMarkParent()
{
	if (parent != nullptr) {
		this->subNodesHasObject = true;
		parent->RecursivelyMarkParent();
	}
}

template<typename T>
OctreeNode<T>* OctreeNode<T>::GetClosestNonEmptyNodeByPosition(float* lookUpPosition, OctreeNode<T>* node)
{
	/*
		If we have a concave shape, then we cant just simply look at position by indexing. Because this may mislead
		Instead we need to find the shortest distanced and with object subnode of each node.
		We start with root, then we will compare all results coming from 8 child of root.
		Thus we will obtain the smallest node who is closest to the lookUpPosition.
	*/
	float* tempDist = new float[8];
	OctreeNode<T>** tempNode = new OctreeNode<T>*[8];

	for (int i = 0; i < 8; i++) {
		tempDist[i] = 1e9;
		tempNode[i] = RecursiveNonEmptyFinder(lookUpPosition, node->subNodes[i], &tempDist[i]);
	}
	int shortestIdx = -1;
	float temp=1e9;
	for (int i = 0; i < 8; i++) {
		if (temp > tempDist[i]) {
			temp = tempDist[i];
			shortestIdx = i;
		}
	}

	return tempNode[shortestIdx];
}

template<typename T>
OctreeNode<T>* OctreeNode<T>::RecursiveNonEmptyFinder(float* lookUpPosition, OctreeNode<T>* node, float *lastDist) {
	
	if (node->isLeaf())
		return node;
	float currentDist = 0.0f, shortestDist = 1e9;
	OctreeNode<T>* tempNode = new OctreeNode<T>();
	tempNode = node;
	for (int i = 0; i < 8; i++) {
		if (node->subNodes[i]->subNodesHasObject) {
			currentDist = sqrDistance(node->subNodes[i]->position, lookUpPosition);
			if (shortestDist > currentDist) {
				shortestDist = currentDist;
				*lastDist = shortestDist;
				tempNode = node->subNodes[i];
			}
		}
	}
	if (tempNode != node)
		tempNode = RecursiveNonEmptyFinder(lookUpPosition, tempNode, lastDist);

	return tempNode;
	


}


template<typename T>
float OctreeNode<T>::sqrDistance(float* point1, float* point2) {
	float temp = (pow(point1[0] - point2[0], 2) + pow(point1[1] - point2[1], 2) + pow(point1[2] - point2[2], 2));
	return temp;
}
#endif // !
