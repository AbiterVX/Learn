#pragma once
#include <iostream>
using namespace std;

class Vector3f;
class Vector4f;
class Matrix4f;

class SceneLight
{
public:
    SceneLight(shared_ptr<Vector3f> light_color, shared_ptr<Vector3f> location=nullptr, shared_ptr<Vector3f> rotation=nullptr);
    //从当前点到光线的方向
    virtual Vector3f getToLightDir(const Vector3f& current_pos)=0;
public:
    //方位
    shared_ptr<Vector3f> location;
    shared_ptr<Vector3f> rotation;
    //light变换
    shared_ptr<Matrix4f> light_transform;
    //光颜色
    shared_ptr<Vector4f> mvp_location;
    //光颜色
    shared_ptr<Vector3f> light_color;
};



//点光源
class SceneLightPoint: public SceneLight
{
public:
    SceneLightPoint(shared_ptr<Vector3f> light_color, shared_ptr<Vector3f> location = nullptr, shared_ptr<Vector3f> rotation = nullptr) :SceneLight(light_color, location, rotation) {};

    virtual Vector3f getToLightDir(const Vector3f& current_pos);
};

