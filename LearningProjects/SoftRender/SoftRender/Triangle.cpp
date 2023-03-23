#include "Triangle.h"

Triangle::Triangle(int location_index1, int location_index2, int location_index3)
{
	location_indexes[0] = location_index1;
	location_indexes[1] = location_index2;
	location_indexes[2] = location_index3;
	setTexutreIndexes(-1 ,-1, -1);
	setNormalIndexes(-1, -1, -1);
	area = 0;
}

void Triangle::setTexutreIndexes(int texture_index1, int texture_index2, int texture_index3)
{
	texture_indexes[0] = texture_index1;
	texture_indexes[1] = texture_index2;
	texture_indexes[2] = texture_index3;
}

void Triangle::setNormalIndexes(int normal_index1, int normal_index2, int normal_index3)
{
	normal_indexes[0] = normal_index1;
	normal_indexes[1] = normal_index2;
	normal_indexes[2] = normal_index3;
}

void Triangle::setSurfaceNormal(std::shared_ptr<Vector4f> surface_normal)
{
	this->surface_normal = surface_normal;
}

void Triangle::setVertex(int vertex_index, int location_index, int texture_index, int normal_index)
{
	location_indexes[vertex_index] = location_index;
	texture_indexes[vertex_index] = texture_index;
	normal_indexes[vertex_index] = normal_index;


}


