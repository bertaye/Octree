#include "OctreeNode.h"
OctreeNode::OctreeNode() : parent(nullptr), nodeIdx(-1), size(-1), position(0,0,0)
{
}


void OctreeNode::SubdivideNode(std::vector<std::shared_ptr<NodeParameters>>& nodes, float threshold) {
	COMMANTABLE_OUTPUT("Subdivide node called");
	nodes[nodeIdx]->isLeaf = false;
	float newPos[3] = { 0,0,0 };
	newPos[0] = position[0];
	newPos[1] = position[1];
	newPos[2] = position[2];
	COMMANTABLE_OUTPUT("Position before subdivision = %f, %f, %f \n", newPos[0], newPos[1], newPos[2]);
	COMMANTABLE_OUTPUT("Size before subdivision = %f \n", size);
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

		COMMANTABLE_OUTPUT("%i th subdivision!\n", i);
		/*
			We have _ _ _ = X Y Z, so 100 = 4 ->RIGHT
									  010 = 2 -> UP
									  001 = 1 -> FRONT
		*/
		if (i == 4 || i == 6 || i == 5 || i == 7) { //right
			newPos[0] += size * 0.25f;
		}
		else {
			newPos[0] -= size * 0.25f;
		}

		if (i == 2 || i == 6 || i == 3 || i == 7) { //up
			newPos[1] += size * 0.25f;
		}
		else {
			newPos[1] -= size * 0.25f;
		}

		if (i == 1 || i == 3 || i == 5 || i == 7) { //front
			newPos[2] += size * 0.25f;
		}
		else {
			newPos[2] -= size * 0.25f;
		}

		subNodes[i] = std::make_unique<OctreeNode>();
		subNodes[i]->size = size * 0.5f;
		subNodes[i]->position[0] = newPos[0];
		subNodes[i]->position[1] = newPos[1];
		subNodes[i]->position[2] = newPos[2];
		subNodes[i]->nodeIdx = nodes.size();
		subNodes[i]->parent = this;
		nodes.push_back(std::make_shared<NodeParameters>(subNodes[i]->position, size * 0.5f));

	}
}

bool OctreeNode::isLeaf() const{
	return subNodes[0] == nullptr;
}

int OctreeNode::GetIndexByPosition(Vec3 lookUpPosition) const{
	if (isLeaf()) {
		COMMANTABLE_OUTPUT("Node leaf cant give index by position!\n");
		return -1;
	}
	int index = 0;
	COMMANTABLE_OUTPUT("Look Up Position = %f,%f,%f\n", lookUpPosition[0], lookUpPosition[1], lookUpPosition[2]);
	COMMANTABLE_OUTPUT("Node Position = %f,%f,%f\n", position[0], position[1], position[2]);

	if (lookUpPosition[0] > position[0]) {
		index += 4;
		COMMANTABLE_OUTPUT("Lookup position is on RIGHT of this node\n");
	}
	else {
		COMMANTABLE_OUTPUT("Lookup position is on LEFT of this node\n");

	}
	if (lookUpPosition[1] > position[1]) {
		index += 2;
		COMMANTABLE_OUTPUT("Lookup position is on UP of this node\n");

	}
	else {
		COMMANTABLE_OUTPUT("Lookup position is on DOWN of this node\n");

	}
	if (lookUpPosition[2] > position[2]) {
		index += 1;
		COMMANTABLE_OUTPUT("Lookup position is on FRONT of this node\n");

	}
	else {
		COMMANTABLE_OUTPUT("Lookup position is on BACK of this node\n");

	}
	COMMANTABLE_OUTPUT("\n\nReturned Index = %d\n\n", index);
	return index;
}
void OctreeNode::RecursivelyMarkParent()
{
	if (parent != nullptr) {
		this->subNodesHasObject = true;
		parent->RecursivelyMarkParent();
	}
}

const OctreeNode* OctreeNode::GetClosestNonEmptyNodeByPosition(Vec3 lookUpPosition, const OctreeNode* node) const
{
	/*
		If we have a concave shape, then we cant just simply look at position by indexing. Because this may mislead
		Instead we need to find the shortest distanced and with object subnode of each node.
		We start with root, then we will compare all results coming from 8 child of root.
		Thus we will obtain the smallest node who is closest to the lookUpPosition.
	*/
	float tempDist[CHILD_COUNT];
	const OctreeNode* tempNode[CHILD_COUNT];

	for (int i = 0; i < CHILD_COUNT; i++) {
		tempDist[i] = INFINITY;
		tempNode[i] = RecursiveNonEmptyFinder(lookUpPosition, node->subNodes[i].get(), tempDist[i]);
	}
	int shortestIdx = -1;
	float temp = INFINITY;
	for (int i = 0; i < 8; i++) {
		if (temp > tempDist[i]) {
			temp = tempDist[i];
			shortestIdx = i;
		}
	}

	return tempNode[shortestIdx];
}

const OctreeNode* OctreeNode::RecursiveNonEmptyFinder(const Vec3& lookUpPosition,const OctreeNode* node, float& lastDist) const{

	if (node->isLeaf())
		return node;
	float currentDist = 0.0f, shortestDist = 1e9;
	const OctreeNode* tempNode;
	tempNode = node;
	for (int i = 0; i < 8; i++) {
		if (node->subNodes[i]->subNodesHasObject) {
			currentDist = sqrDistance(node->subNodes[i]->position, lookUpPosition);
			if (shortestDist > currentDist) {
				shortestDist = currentDist;
				lastDist = shortestDist;
				tempNode = node->subNodes[i].get();
			}
		}
	}
	if (tempNode != node)
		tempNode = RecursiveNonEmptyFinder(lookUpPosition, tempNode, lastDist);

	return tempNode;



}


float OctreeNode::sqrDistance(Vec3 point1, Vec3 point2) const {
	float temp = (pow(point1[0] - point2[0], 2) + pow(point1[1] - point2[1], 2) + pow(point1[2] - point2[2], 2));
	return temp;
}
