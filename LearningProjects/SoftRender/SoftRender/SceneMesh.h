#pragma once

#include <vector>
#include <map>
#include <iostream>
using namespace std;

class Vector2f;
class Vector3f;
class Vector4f;
class Triangle;
class Matrix4f;

class SceneMesh
{
public:
    //设置Transform
    virtual void setTransform(shared_ptr<Vector3f> location=nullptr, shared_ptr<Vector3f> rotation=nullptr, shared_ptr<Vector3f> scale=nullptr) = 0;
    //更新Transform
    virtual void updateTransform(shared_ptr<Vector3f> location=nullptr, shared_ptr<Vector3f> rotation=nullptr, shared_ptr<Vector3f> scale=nullptr) = 0;
    
    void updateWorldVerticesData();
    
    //设置材质
    virtual void setMaterial(shared_ptr<class SceneMaterial> material, int index=0) = 0;
    //获取材质
    virtual shared_ptr<class SceneMaterial> getMaterial() = 0;
    //获取三角形某个顶点的信息：位置，法向量，贴图
    virtual shared_ptr<Vector4f> getVertexLocation(const shared_ptr<Triangle>& triangle, int vertex_index) = 0;
    virtual shared_ptr<Vector4f> getVertexNormal(const shared_ptr<Triangle>& triangle, int vertex_index) = 0;
    virtual shared_ptr<Vector4f> getVertexTexture(const shared_ptr<Triangle>& triangle, int vertex_index) = 0;
public:
    //Mesh方位
    shared_ptr<Vector3f> location;
    shared_ptr<Vector3f> rotation;
    shared_ptr<Vector3f> scale;

    //顶点-位置
    vector<shared_ptr<Vector4f>> location_vertices;
    //顶点-法向量
    vector<shared_ptr<Vector4f>> normal_vertices;
    //顶点-贴图映射
    vector<shared_ptr<Vector2f>> texture_vertices;
    //三角形
    vector<shared_ptr<Triangle>> triangles;
    //顶点关联到的三角形
    map<int, vector<shared_ptr<Triangle>>> vertex_relate_triangles;
    
    //材质
    vector<shared_ptr<class SceneMaterial>> materials;

    //mesh变换
    shared_ptr<Matrix4f> mesh_transform;

    //当前顶点位置Transform
	vector<std::shared_ptr<Vector4f>> mvp_location_vertices;

    //世界位置
    vector<shared_ptr<Vector4f>> world_location_vertices;
    //世界法向量
    vector<shared_ptr<Vector4f>> world_normal_vertices;
};



class SceneMeshObj: public SceneMesh
{
public:
    SceneMeshObj(const char* data_path);

    //设置Transform
    virtual void setTransform(shared_ptr<Vector3f> location=nullptr, shared_ptr<Vector3f> rotation=nullptr, shared_ptr<Vector3f> scale=nullptr);
    //更新Transform
    virtual void updateTransform(shared_ptr<Vector3f> location=nullptr, shared_ptr<Vector3f> rotation=nullptr, shared_ptr<Vector3f> scale=nullptr);
    
    //设置材质
    virtual void setMaterial(shared_ptr<class SceneMaterial> material, int index=0);
    
    virtual shared_ptr<class SceneMaterial> getMaterial();

    //获取三角形某个顶点的信息：位置，法向量，贴图
    virtual shared_ptr<Vector4f> getVertexLocation(const shared_ptr<Triangle>& triangle, int vertex_index);
    virtual shared_ptr<Vector4f> getVertexNormal(const shared_ptr<Triangle>& triangle, int vertex_index);
    virtual shared_ptr<Vector4f> getVertexTexture(const shared_ptr<Triangle>& triangle, int vertex_index);
};

