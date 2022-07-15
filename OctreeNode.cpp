#include "OctreeNode.h"
template<typename T>
void OctreeNode<T>::SubdivideNode(std::vector<NodeParameters>* nodes) {
	subNodes = new OctreeNode<T>[8];
	float newPos[3] = { 0,0,0 };
	newPos[0] = position[0];
	newPos[1] = position[1];
	newPos[2] = position[2];
	for (int i = 0; i < 8; i++) {

		/*
			We have _ _ _ = X Y Z, so 100 = 4 ->RIGHT
									  010 = 2 -> UP
									  001 = 1 -> FRONT
		*/
		if ((i & 4) == 4) { //right
			newPos[0] += size * 0.25f;
		}
		else {
			newPos[0] -= size * 0.25f;
		}

		if ((i & 2) == 2) { //up
			newPos[1] += size * 0.25f;
		}
		else {
			newPos[1] -= size * 0.25f;
		}

		if ((i & 1) == 1) { //front
			newPos[2] += size * 0.25f;
		}
		else {
			newPos[2] -= size * 0.25f;
		}

		subNodes[i] = new OctreeNode<T>(newPos, size * 0.5f);
		nodes->push_back(new NodeParameters(newPos, size * 0.5f));
		
	}
}

template<typename T>
bool OctreeNode<T>::isLeaf(){
	return subNodes==nullptr;
}

template<typename T>
int OctreeNode<T>::GetIndexByPosition(float* lookUpPosition){
	if (isLeaf()) {
		printf("Node leaf cant give index by position!\n");
		return -1;
	}
	int index = 0;

	index |= lookUpPosition[0] > position[0] ? 4 : 0;
	index |= lookUpPosition[1] > position[1] ? 2 : 0;
	index |= lookUpPosition[2] > position[2] ? 1 : 0;

	return 0;
}
