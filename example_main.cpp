#include"Octree.h"
#include"VertexContainer.h"
int main() {
	VertexContainer pcl;
	/*
	Point is a struct defined in PointCloud.h and has following structure

			struct Point {
			float* coords;
			int idx;
			Point(int i, float* c) : idx(i), coords(c) {};
			};
	*/
	Octree octree;
	pcl.LoadPly("C:/CG-Work/MyWork/OctreeForPointClouds/PointClouds/dragon.ply");
	octree.BuiltOctreeFromVertexContainer(pcl, 0.2f);
	//Now our octree is built and we can call other member methods.

}