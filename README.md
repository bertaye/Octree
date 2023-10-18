# OctreeForPointClouds
A simple Octree structure that can build up on the Point Clouds
## Build
```
mkdir build
cd build
cmake -DBUILD_EXAMPLE=ON ..
cmake --build .
```
There is only 2 parameters for CMake at the moment:
```
BUILD_EXAMPLE
SILENCE_DEBUG_OUTPUT
```

## How to use
This repo can only load .ply files at the moment (open for all improvements!) and you can use PointCloud class for that.
```c++
  PointCloud pcl;
```
after creating it just call LoadPly function.
```c++
  pcl.LoadPly("PointClouds/dragon.ply");
```
now you loaded your Point Cloud.
To built an octree, first create one.
```c++
  Octree octree;
```
here note that Point is a struct defined in PointCloud.h
```c++
struct Point {
	Vec3 coords;
	int idx;
	Point(int i, Vec3 c) : idx(i), coords(c) {};
};
```

## Notes on Octree Class

You can call following functions:
```c++
  Vec3 GetClosestNodePosFromPoint(float* point);
  Vec3 GetClosestObject(float* point);
```
First one returns the closest Node's pos, whether it is empty or not, independent from size.
Second one returns the position of the closest non empty node. This function can be improved by returning the properties of the objects in that particular node.
## Examples
##### Stanford Dragon Point Cloud
 <img  width="250" src= "https://user-images.githubusercontent.com/39909689/179322471-4fa69af2-231d-49fb-b182-940debe50912.png" alt="Dragon point cloud" right-margin:900px/> 
 
##### Octree Built and only nodes with objects are shown
 
<img src="https://user-images.githubusercontent.com/39909689/179322494-151320bd-80e1-44c4-b188-19fbad2b2596.png" width="250" />

##### Complete Octree Shown
<p float="center">
  <img src= "https://user-images.githubusercontent.com/39909689/179322382-a9eb26de-acdc-424e-9619-4c83da7048e6.png" width="250" />
  <img src="https://user-images.githubusercontent.com/39909689/179324322-cc6959b1-8d4e-4665-8d36-849101d30a3d.png" width="250" /> 
</p>
