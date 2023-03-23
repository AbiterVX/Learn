#include "SceneLight.h"
#include "Vector3f.h"
#include "Vector4f.h"
#include "Matrix4f.h"

SceneLight::SceneLight(shared_ptr<Vector3f> light_color, shared_ptr<Vector3f> location, shared_ptr<Vector3f> rotation)
{
	this->light_color = light_color;

	if(location!=nullptr) {
		this->location = location;
	}
	else {
		this->location = make_shared<Vector3f>(Vector3f::ZERO);
	}

	if (rotation != nullptr) {
		this->rotation = rotation;
	}
	else {
		this->rotation = make_shared<Vector3f>(Vector3f::ZERO);
	}
	
	light_transform = make_shared<Matrix4f>(Matrix4f::translation(*this->location) * Matrix4f::rotateXYZ(*this->rotation));
}

Vector3f SceneLightPoint::getToLightDir(const Vector3f& current_pos)
{
	return (*location - current_pos).normalized();
}
