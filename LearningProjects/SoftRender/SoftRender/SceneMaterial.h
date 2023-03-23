#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <map>
using namespace std;

class Vector2f;
class Vector3f;
class BitmapPixelColor;
class SceneLight;


//"material": "mat_box",
//"material": "mat_steve",

class SceneMaterial
{
public:
	SceneMaterial(const char* texture_path);
	//得到当前像素点的颜色
	virtual BitmapPixelColor getPixelColor(int x, int y);
	//得到当前像素点的颜色-根据uv坐标
	virtual BitmapPixelColor getPixelColorbyPercent(const Vector2f& texture_coord, const Vector3f& current_normal, 
												    const Vector3f& current_pos, const map<string, shared_ptr<class SceneLight>>& scene_lights);
	//给出合法的像素点
	vector<int> getValidCoord(int x, int y);
public:
	//贴图
	HBITMAP texture;
	//贴图数据:bitmap数据，每个像素点的数据长为4，按bgr和空的顺序排列。
	std::vector<unsigned char> texture_pixels;
	//贴图尺寸
	int width;
	int height;
	//传入的buffer
	UCHAR* texture_buffer;


	int max_x_index;
	int max_y_index;
};

//贴图
class SceneMaterialTextureBilinear: public SceneMaterial
{
public:
	SceneMaterialTextureBilinear(const char* texture_path):SceneMaterial(texture_path){};
	virtual BitmapPixelColor getPixelColorbyPercent(const Vector2f& texture_coord, const Vector3f& current_normal, const Vector3f& current_pos, const map<string, shared_ptr<class SceneLight>>& scene_lights);
	
};


//Phong-材质
class SceneMaterialPhong : public SceneMaterial
{
public:
	SceneMaterialPhong(const char* texture_path, 
		shared_ptr<Vector3f> diffuse_color, shared_ptr<Vector3f> specular_color, int shininess, shared_ptr<Vector3f> ambient_light);

	virtual BitmapPixelColor getPixelColorbyPercent(const Vector2f& texture_coord, const Vector3f& current_normal, const Vector3f& current_pos, const map<string, shared_ptr<class SceneLight>>& scene_lights);

	Vector3f getPhongColor(const Vector3f& current_normal, const Vector3f& current_pos, const shared_ptr<SceneLight>& scene_light);
public:
	//漫反射颜色
	shared_ptr<Vector3f> diffuse_color;
	//高光反射颜色
	shared_ptr<Vector3f> specular_color;
	//高光反射系数
	int shininess;
	//环境光
	shared_ptr<Vector3f> ambient_light;
};