#include "SceneMesh.h"
#include <iostream>
#include <string>
#include <vector>
#include "UtilFunc.h"
#include "Vector2f.h"
#include "Vector3f.h"
#include "Vector4f.h"
#include "Matrix4f.h"
#include "Matrix3f.h"
#include "Triangle.h"
#include "SceneMaterial.h"

using namespace std;

void SceneMesh::updateWorldVerticesData()
{
    for(int i=0;i<location_vertices.size();i++){
        world_location_vertices[i] = make_shared<Vector4f>(*mesh_transform * *location_vertices[i]);
    }
    for(int i=0;i<normal_vertices.size();i++){
        world_normal_vertices[i] = make_shared<Vector4f>(*mesh_transform * *normal_vertices[i]);
    }
}





SceneMeshObj::SceneMeshObj(const char* data_path)
{
    vector<string> file_data = UtilFunc::readFile(data_path);
    for (auto& current_line : file_data) {
        vector<string> raw_data = UtilFunc::split(current_line, " ");
        if(raw_data[0] == "v"){
            //顶点, w=1
            shared_ptr<Vector4f> new_vertex = make_shared<Vector4f>(stof(raw_data[1]), stof(raw_data[2]), stof(raw_data[3]), 1);
            location_vertices.push_back(new_vertex);
        }
        else if(raw_data[0] == "vt"){
            //贴图坐标
            shared_ptr<Vector2f> new_vertex = make_shared<Vector2f>(stof(raw_data[1]), stof(raw_data[2]));
            texture_vertices.push_back(new_vertex);
        }
        else if(raw_data[0] == "vn"){
            //法向量, w=0
            shared_ptr<Vector4f> new_vertex = make_shared<Vector4f>(stof(raw_data[1]), stof(raw_data[2]), stof(raw_data[3]), 0);
            normal_vertices.push_back(new_vertex);
        }
        else if(raw_data[0] == "f"){
            //面
            shared_ptr<Triangle> new_triangle = make_shared<Triangle>();
            for(int i=0;i<3;i++){
                vector<string> vertex_data = UtilFunc::split(raw_data[i+1], "/");
                if(vertex_data.size() == 1){
                    new_triangle->setVertex(i, stoi(vertex_data[0])-1);
                }
                else if(vertex_data.size() == 2){
                    new_triangle->setVertex(i, stoi(vertex_data[0])-1, stoi(vertex_data[1])-1);
                }
                else if(vertex_data.size() == 3){
                    new_triangle->setVertex(i, stoi(vertex_data[0])-1, stoi(vertex_data[1])-1, stoi(vertex_data[2])-1);
                }
            }
            //面法向量
            Vector3f p1 = location_vertices[new_triangle->location_indexes[0]]->xyz();
            Vector3f p2 = location_vertices[new_triangle->location_indexes[1]]->xyz();
            Vector3f p3 = location_vertices[new_triangle->location_indexes[2]]->xyz();
            Vector3f surface_normal = Vector3f::cross((p2 - p1).normalized(), (p3 - p1).normalized()).normalized();
            new_triangle->surface_normal = make_shared<Vector4f>(surface_normal, 0);


            vector<float> edge_length = { (p2-p1).abs(), (p3-p2).abs(), (p1-p3).abs()  };
            float p = 0;
            for(int i=0;i<3;i++){
                p += edge_length[i];
            }
            p /=2.0f;
            new_triangle->area = sqrt(p*(p-edge_length[0])*(p-edge_length[1])*(p-edge_length[2]));
            
            triangles.push_back(new_triangle);

            //顶点关联三角形
            for(int i=0;i<3;i++){
                int current_index = new_triangle->location_indexes[i];
                if(vertex_relate_triangles.find(current_index) != vertex_relate_triangles.end()){
                    vertex_relate_triangles[current_index].push_back(new_triangle);
                }
                else{
                    vector<shared_ptr<Triangle>> triangle_list;
                    triangle_list.push_back(new_triangle);
                    vertex_relate_triangles[current_index] = triangle_list;
                }
            }
        }
    }

    // 计算点的平均法向量
    if(normal_vertices.size()==0){
        normal_vertices.resize(location_vertices.size(), nullptr);
        for(int i=0;i<location_vertices.size();i++){
            
            float sum_area = 0;
            for(int j=0;j<vertex_relate_triangles[i].size();j++){
                sum_area += vertex_relate_triangles[i][j]->area;
            }

            Vector3f final_normal;
            for(int j=0;j<vertex_relate_triangles[i].size();j++){
                final_normal = final_normal + vertex_relate_triangles[i][j]->area /sum_area * (*vertex_relate_triangles[i][j]->surface_normal).xyz();
            }
            final_normal.normalize();
            normal_vertices[i] = make_shared<Vector4f>(final_normal,0.0f);
        }

        for(auto& current_triangle: triangles){
            for(int i=0;i<3;i++){
                current_triangle->normal_indexes[i] = current_triangle->location_indexes[i];
            }
            
        }
    }
    
    mvp_location_vertices.resize(location_vertices.size(), nullptr);
    world_location_vertices.resize(location_vertices.size(), nullptr);
    world_normal_vertices.resize(normal_vertices.size(), nullptr);
    
    //Transfrom
    setTransform(nullptr, nullptr, nullptr);


    this->materials.push_back(nullptr);
}

void SceneMeshObj::setTransform(shared_ptr<Vector3f> location, shared_ptr<Vector3f> rotation, shared_ptr<Vector3f> scale)
{
    if(location!=nullptr){
        this->location = location;
    }
    else{
        this->location = make_shared<Vector3f>(Vector3f::ZERO);
    }

    if (rotation != nullptr) {
        this->rotation = rotation;
    }
    else {
        this->rotation = make_shared<Vector3f>(Vector3f::ZERO);
    }

    if (scale != nullptr) {
        this->scale = scale;
    }
    else {
        this->scale = make_shared<Vector3f>(Vector3f::ONE);
    }

    //Transform
    //
    Matrix4f transform = Matrix4f::translation(*this->location) * Matrix4f::rotateXYZ(*this->rotation) * Matrix4f::scaleByVector(*this->scale);
    mesh_transform = make_shared<Matrix4f>(transform);

    updateWorldVerticesData();
}

void SceneMeshObj::updateTransform(shared_ptr<Vector3f> location, shared_ptr<Vector3f> rotation, shared_ptr<Vector3f> scale)
{
    if(location!=nullptr){
        this->location = location;
    }

    if (rotation != nullptr) {
        this->rotation = rotation;
    }

    if (scale != nullptr) {
        this->scale = scale;
    }
    
    Matrix4f transform = Matrix4f::translation(*this->location) * Matrix4f::rotateXYZ(*this->rotation) * Matrix4f::scaleByVector(*this->scale);
    mesh_transform = make_shared<Matrix4f>(transform);

    updateWorldVerticesData();
}

void SceneMeshObj::setMaterial(shared_ptr<SceneMaterial> material, int index){
    if(this->materials.size()==0){
        this->materials.push_back(material);
    }
    else{
        this->materials[0] = material;
    }
}

shared_ptr<class SceneMaterial> SceneMeshObj::getMaterial()
{
    return materials[0];
}



shared_ptr<Vector4f> SceneMeshObj::getVertexLocation(const shared_ptr<Triangle>& triangle, int vertex_index)
{
    return location_vertices[triangle->location_indexes[vertex_index]];
}

shared_ptr<Vector4f> SceneMeshObj::getVertexNormal(const shared_ptr<Triangle>& triangle, int vertex_index)
{
    return location_vertices[triangle->normal_indexes[vertex_index]];
}

shared_ptr<Vector4f> SceneMeshObj::getVertexTexture(const shared_ptr<Triangle>& triangle, int vertex_index)
{
    return location_vertices[triangle->texture_indexes[vertex_index]];
}
