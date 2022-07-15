#include"Octree.h"
#include"PointCloud.h"
int main() {
	PointCloud* pcl = new PointCloud();
	/*
	Point is a struct defined in PointCloud.h and has following structure

			struct Point {
			float* coords;
			int idx;
			Point(int i, float* c) : idx(i), coords(c) {};
			};
	*/
	Octree<Point>* octree = new Octree<Point>();
	pcl->LoadPly("PointClouds/dragon.ply");
	octree->BuiltOctreeFromPointCloud(pcl, 0.2f, pcl->points);
	//Now our octree is built and we can call other member methods.

}