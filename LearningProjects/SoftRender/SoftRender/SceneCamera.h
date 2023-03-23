#pragma once

#include<iostream>
using namespace std;
class Matrix4f;


class SceneCamera
{
public:
	virtual Matrix4f getProjectionMatrix(float viewport_width, float viewport_height)=0;

public:
	shared_ptr<class Vector3f> location;
    shared_ptr<class Vector3f> lookat_target;
    shared_ptr<class Vector3f> up;
    float z_near;
    float z_far;
};

class SceneCameraOrthographic : public SceneCamera
{
public:
    SceneCameraOrthographic(shared_ptr<class Vector3f> location, shared_ptr<class Vector3f> lookat_target, shared_ptr<class Vector3f> up, float z_near, float z_far, float left, float right, float bottom, float top);
    virtual Matrix4f getProjectionMatrix(float viewport_width, float viewport_height);

public:
    float left;
    float right;
    float bottom;
    float top;
};


class SceneCameraPerspective : public SceneCamera
{
public:
    SceneCameraPerspective(shared_ptr<class Vector3f> location, shared_ptr<class Vector3f> lookat_target, shared_ptr<class Vector3f> up, float z_near, float z_far, float fov_y_degree);
	virtual Matrix4f getProjectionMatrix(float viewport_width, float viewport_height);
    
public:
    float fov_y_degree;

};
