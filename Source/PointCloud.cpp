#include "PointCloud.h"

void VertexContainer::AddPoint(float x, float y, float z) {
	//COMMANTABLE_OUTPUT("New added point coordinats : %.5g, %.5g, %.5g \n", x, y, z);


	int idx = points.size();
	Vec3 c(x,y,z);

	points.push_back(std::make_shared<Vertex>(idx, c));
	if (points.size() > 1) {
		if (points.size() == 2)
			centerPoint = points[idx - 1]->coords;

		centerPoint = calculateMidPoint(centerPoint, points[idx]->coords, (float)points.size() - 1, 1.0f);
	}

	if (points.size() == 1) {
		geometricCenterPoint = points[0]->coords;
		upBoundaryPoint = points[0];
		downBoundaryPoint = points[0];
		leftBoundaryPoint = points[0];
		rigthBoundaryPoint = points[0];
		frontBoundaryPoint = points[0];
		backBoundaryPoint = points[0];
	}
	else {
		if (points[points.size() - 1]->coords[0] < leftBoundaryPoint->coords[0]) {
			leftBoundaryPoint = points[points.size() - 1];
		}
		if (points[points.size() - 1]->coords[0] > rigthBoundaryPoint->coords[0]) {
			rigthBoundaryPoint = points[points.size() - 1];
		}
		if (points[points.size() - 1]->coords[1] < downBoundaryPoint->coords[1]) {
			downBoundaryPoint = points[points.size() - 1];
		}
		if (points[points.size() - 1]->coords[1] > upBoundaryPoint->coords[1]) {
			upBoundaryPoint = points[points.size() - 1];
		}
		if (points[points.size() - 1]->coords[2] < backBoundaryPoint->coords[2]) {
			backBoundaryPoint = points[points.size() - 1];
		}
		if (points[points.size() - 1]->coords[2] > frontBoundaryPoint->coords[2]) {
			frontBoundaryPoint = points[points.size() - 1];
		}

		geometricCenterPoint[0] = (leftBoundaryPoint->coords[0] + rigthBoundaryPoint->coords[0]) / 2.0f;
		geometricCenterPoint[1] = (upBoundaryPoint->coords[1] + downBoundaryPoint->coords[1]) / 2.0f;
		geometricCenterPoint[2] = (backBoundaryPoint->coords[2] + frontBoundaryPoint->coords[2]) / 2.0f;

	}
}

Vec3 VertexContainer::calculateMidPoint(const Vec3& c1,const Vec3& c2, float c1_coeff, float c2_coeff)
{
	Vec3 midP;
	midP[0] = (c1_coeff * c1[0] + c2_coeff * c2[0]) / (c1_coeff + c2_coeff);
	midP[1] = (c1_coeff * c1[1] + c2_coeff * c2[1]) / (c1_coeff + c2_coeff);
	midP[2] = (c1_coeff * c1[2] + c2_coeff * c2[2]) / (c1_coeff + c2_coeff);
	return midP;
}

void VertexContainer::LoadPly(const char* name) {
	FILE* fPtr;
	fopen_s(&fPtr, name, "r");
	if (fPtr == 0)
		return;
	char str[334];
	int counter = 0;
	float x, y, z;

	char firstChar;
	fscanf_s(fPtr, "%s", str, sizeof(str));

	int nVerts, nTris, n, i = 0;
	while (counter < 16) {

		fscanf_s(fPtr, "%s", str, sizeof(str));
		//printf("str read: %s\n", str);
		counter++;
	}

	while (fscanf_s(fPtr, "%f %f %f", &x, &y, &z) != EOF) {
		//printf("x: %f, y: %f, z: %f\n", x, y, z);
		AddPoint(x, y, z);
	}

	fclose(fPtr);
}
/// <summary>
/// Moves center of point cloud. snap=true will snap to point cloud given x,y,z coordinats
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
/// <param name="snap"></param>
void VertexContainer::MoveCenter(float x, float y, float z, bool snap)
{
	if (!snap) {
		for (int i = 0; i < points.size(); i++) {
			points[i]->coords[0] += x;
			points[i]->coords[1] += y;
			points[i]->coords[2] += z;

		}
		centerPoint[0] += x;
		centerPoint[1] += y;
		centerPoint[2] += z;
	}
	else {
		for (int i = 0; i < points.size(); i++) {
			points[i]->coords[0] += (x - centerPoint[0]);
			points[i]->coords[1] += (y - centerPoint[1]);
			points[i]->coords[2] += (z - centerPoint[2]);
		}
		centerPoint[0] = x;
		centerPoint[1] = y;
		centerPoint[2] = z;
	}
}

void VertexContainer::ScalePly(float scaleFactor)
{
	Vec3 temp;
	for (int i = 0; i < points.size(); i++) {
		temp[0] = (points[i]->coords[0] - centerPoint[0]) * scaleFactor;
		temp[1] = (points[i]->coords[1] - centerPoint[1]) * scaleFactor;
		temp[2] = (points[i]->coords[2] - centerPoint[2]) * scaleFactor;

		points[i]->coords[0] = centerPoint[0] + temp[0];
		points[i]->coords[1] = centerPoint[1] + temp[1];
		points[i]->coords[2] = centerPoint[2] + temp[2];
	}
}

