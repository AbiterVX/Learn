#include "SceneCamera.h"
#include "Vector3f.h"
#include "Matrix4f.h";
#include <iostream>

using namespace std;

SceneCameraOrthographic::SceneCameraOrthographic(shared_ptr<class Vector3f> location, shared_ptr<class Vector3f> lookat_target, shared_ptr<class Vector3f> up, float z_near, float z_far, float left, float right, float bottom, float top)
{
    this->location = location;
    this->lookat_target = lookat_target;
    this->up = up;
    this->z_near = z_near;
    this->z_far = z_far;
    this->left = left;
    this->right = right;
    this->bottom = bottom;
    this->top = top;
}

Matrix4f SceneCameraOrthographic::getProjectionMatrix(float viewport_width, float viewport_height)
{
    Matrix4f projection_transform = Matrix4f::orthographicProjection(left, right, bottom, top, z_near, z_far);

    Vector3f dir = (*lookat_target - *location).normalized();
    Matrix4f camera_view_transform = Matrix4f::getCameraViewTransform(*location, dir, *up);
    return (projection_transform * camera_view_transform);
}



SceneCameraPerspective::SceneCameraPerspective(shared_ptr<class Vector3f> location, shared_ptr<class Vector3f> lookat_target, shared_ptr<class Vector3f> up, float z_near, float z_far, float fov_y_degree)
{
    this->location = location;
    this->lookat_target = lookat_target;
    this->up = up;
    this->z_near = z_near;
    this->z_far = z_far;
    this->fov_y_degree = fov_y_degree;
}

Matrix4f SceneCameraPerspective::getProjectionMatrix(float viewport_width, float viewport_height)
{
    float aspect_ratio = viewport_width/viewport_height;
    Matrix4f projection_transform = Matrix4f::perspectiveProjection(fov_y_degree, aspect_ratio, z_near, z_far);
    Vector3f dir = (*lookat_target - *location).normalized();
    Matrix4f camera_view_transform = Matrix4f::lookAt(*location, *lookat_target, *up);
    return (projection_transform * camera_view_transform);
}

