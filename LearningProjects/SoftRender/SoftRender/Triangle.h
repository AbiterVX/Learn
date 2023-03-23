#pragma once
#include <iostream>
#include <vector>

class Vector4f;
class Matrix4f;



class Triangle
{
public:
	Triangle(int location_index1=-1, int location_index2=-1, int location_index3=-1);
	void setTexutreIndexes(int texture_index1, int texture_index2, int texture_index3);
	void setNormalIndexes(int normal_index1, int normal_index2, int normal_index3);
	void setSurfaceNormal(std::shared_ptr<Vector4f> surface_normal);
	void setVertex(int vertex_index, int location_index, int texture_index=-1, int normal_index=-1);
public:
	//位置
	int location_indexes[3];
	//贴图
	int texture_indexes[3];
	//点法向量
	int normal_indexes[3];
	//面法向量
	std::shared_ptr<Vector4f> surface_normal;

	float area;
	//当前法向量Transfrom
	std::vector<std::shared_ptr<Matrix4f>> normal_matrices;
};

