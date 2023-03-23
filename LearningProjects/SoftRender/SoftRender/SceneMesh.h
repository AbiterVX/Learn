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
    //����Transform
    virtual void setTransform(shared_ptr<Vector3f> location=nullptr, shared_ptr<Vector3f> rotation=nullptr, shared_ptr<Vector3f> scale=nullptr) = 0;
    //����Transform
    virtual void updateTransform(shared_ptr<Vector3f> location=nullptr, shared_ptr<Vector3f> rotation=nullptr, shared_ptr<Vector3f> scale=nullptr) = 0;
    
    void updateWorldVerticesData();
    
    //���ò���
    virtual void setMaterial(shared_ptr<class SceneMaterial> material, int index=0) = 0;
    //��ȡ����
    virtual shared_ptr<class SceneMaterial> getMaterial() = 0;
    //��ȡ������ĳ���������Ϣ��λ�ã�����������ͼ
    virtual shared_ptr<Vector4f> getVertexLocation(const shared_ptr<Triangle>& triangle, int vertex_index) = 0;
    virtual shared_ptr<Vector4f> getVertexNormal(const shared_ptr<Triangle>& triangle, int vertex_index) = 0;
    virtual shared_ptr<Vector4f> getVertexTexture(const shared_ptr<Triangle>& triangle, int vertex_index) = 0;
public:
    //Mesh��λ
    shared_ptr<Vector3f> location;
    shared_ptr<Vector3f> rotation;
    shared_ptr<Vector3f> scale;

    //����-λ��
    vector<shared_ptr<Vector4f>> location_vertices;
    //����-������
    vector<shared_ptr<Vector4f>> normal_vertices;
    //����-��ͼӳ��
    vector<shared_ptr<Vector2f>> texture_vertices;
    //������
    vector<shared_ptr<Triangle>> triangles;
    //�����������������
    map<int, vector<shared_ptr<Triangle>>> vertex_relate_triangles;
    
    //����
    vector<shared_ptr<class SceneMaterial>> materials;

    //mesh�任
    shared_ptr<Matrix4f> mesh_transform;

    //��ǰ����λ��Transform
	vector<std::shared_ptr<Vector4f>> mvp_location_vertices;

    //����λ��
    vector<shared_ptr<Vector4f>> world_location_vertices;
    //���編����
    vector<shared_ptr<Vector4f>> world_normal_vertices;
};



class SceneMeshObj: public SceneMesh
{
public:
    SceneMeshObj(const char* data_path);

    //����Transform
    virtual void setTransform(shared_ptr<Vector3f> location=nullptr, shared_ptr<Vector3f> rotation=nullptr, shared_ptr<Vector3f> scale=nullptr);
    //����Transform
    virtual void updateTransform(shared_ptr<Vector3f> location=nullptr, shared_ptr<Vector3f> rotation=nullptr, shared_ptr<Vector3f> scale=nullptr);
    
    //���ò���
    virtual void setMaterial(shared_ptr<class SceneMaterial> material, int index=0);
    
    virtual shared_ptr<class SceneMaterial> getMaterial();

    //��ȡ������ĳ���������Ϣ��λ�ã�����������ͼ
    virtual shared_ptr<Vector4f> getVertexLocation(const shared_ptr<Triangle>& triangle, int vertex_index);
    virtual shared_ptr<Vector4f> getVertexNormal(const shared_ptr<Triangle>& triangle, int vertex_index);
    virtual shared_ptr<Vector4f> getVertexTexture(const shared_ptr<Triangle>& triangle, int vertex_index);
};

