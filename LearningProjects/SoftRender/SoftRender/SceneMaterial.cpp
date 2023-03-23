#include "SceneMaterial.h"
#include "UtilFunc.h"
#include "BitmapDrawManager.h"
#include <cmath>
#include "Vector3f.h"
#include "Vector4f.h"
#include "Vector2f.h"
#include "SceneLight.h"
#include <algorithm> 

//----------------- SceneMaterial

SceneMaterial::SceneMaterial(const char* texture_path)
{
    LPCWSTR str = UtilFunc::convertCharArrayToLPCWSTR(texture_path);
    texture = (HBITMAP)LoadImage(NULL, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    //设置尺寸
    UtilFunc::setBitmapSize(texture, width, height);
    //获取像素
    texture_pixels = UtilFunc::getPixels(texture, width, height);

    max_x_index = width - 1;
	max_y_index = height - 1;
    //std::cout << width << "," << height << std::endl;
}

BitmapPixelColor SceneMaterial::getPixelColor(int x, int y){
    int pixel_index = (y*width + x)*4;
    return { texture_pixels[pixel_index + 2], texture_pixels[pixel_index + 1], texture_pixels[pixel_index] };
}

BitmapPixelColor SceneMaterial::getPixelColorbyPercent(const Vector2f& texture_coord, const Vector3f& current_normal, const Vector3f& current_pos, const map<string, shared_ptr<SceneLight>>& scene_lights){
    int x = max(0, min(int(texture_coord[0] * width), max_x_index));
    int y = max(0, min(int(texture_coord[1] * height), max_y_index));
    return getPixelColor(x, y);
}






//----------------- SceneMaterialTexture

BitmapPixelColor SceneMaterialTextureBilinear::getPixelColorbyPercent(const Vector2f& texture_coord, const Vector3f& current_normal, const Vector3f& current_pos, const map<string, shared_ptr<SceneLight>>& scene_lights)
{
    //坐标
    float coord_x = texture_coord.value_list[0] * width;
    float coord_y = texture_coord.value_list[1] * height;
    //方格范围
    float x_min = floor(coord_x);
    float x_max = ceil(coord_x);
    float y_min = floor(coord_y);
    float y_max = ceil(coord_y);
    //占比权重
    float x_weight = coord_x-x_min;
    float y_weight = coord_y-y_min;
    //四个顶点颜色
    int x_min_i = min(int(x_min), max_x_index);
    int x_max_i = min(int(x_max), max_x_index);
    int y_min_i = min(int(y_min), max_y_index);
    int y_max_i = min(int(y_max), max_y_index);

    // 插值公式： (1-y_weight)* (color1 + color2) + y_weight* (color3 + color4);
    BitmapPixelColor color1 = getPixelColor(x_min_i, y_min_i);
    BitmapPixelColor color2 = getPixelColor(x_max_i, y_min_i);
    BitmapPixelColor color3 = getPixelColor(x_min_i, y_max_i);
    BitmapPixelColor color4 = getPixelColor(x_max_i, y_max_i);
    //水平插值
    color1 *= 1-x_weight;
    color2 *= x_weight;
    color3 *= 1-x_weight;
    color4 *= x_weight;
    //纵向插值
    color1 *= 1-y_weight;
    color2 *= 1-y_weight;
    color3 *= y_weight;
    color4 *= y_weight;
    //得到最终颜色
    BitmapPixelColor color;
    color += color1;
    color += color2;
    color += color3;
    color += color4; 
    return color;
}

vector<int> SceneMaterial::getValidCoord(int x, int y)
{ 
    y = max(0, min(y, height - 1));
    x = max(0, min(x, width - 1));
    //vector<int> valid_coord = ;
    return { x,y };
}


//------------------------ Phong-材质

SceneMaterialPhong::SceneMaterialPhong(const char* texture_path, shared_ptr<Vector3f> diffuse_color, shared_ptr<Vector3f> specular_color, int shininess, shared_ptr<Vector3f> ambient_light):SceneMaterial(texture_path)
{
    this->diffuse_color = diffuse_color;
    this->specular_color = specular_color;
    this->shininess = shininess;
    this->ambient_light = ambient_light;
}

BitmapPixelColor SceneMaterialPhong::getPixelColorbyPercent(const Vector2f& texture_coord,  const Vector3f& current_normal, const Vector3f& current_pos, const map<string, shared_ptr<SceneLight>>& scene_lights)
{   
    int x = max(0, min(int(texture_coord[0] * width), width - 1));
    int y = max(0, min(int(texture_coord[1] * height), height - 1));

    BitmapPixelColor raw_texture_color = getPixelColor(x, y);
    Vector3f texture_color(int(raw_texture_color.color_data[2]), int(raw_texture_color.color_data[1]), int(raw_texture_color.color_data[0])); 
    Vector3f final_color;
    for (auto& element : scene_lights) {
        final_color += getPhongColor(current_normal, current_pos, element.second);
    }
    final_color *= texture_color;

    int r = max(0, min(final_color.value_list[0], 255));
    int g = max(0, min(final_color.value_list[1], 255));
    int b = max(0, min(final_color.value_list[2], 255));
    return BitmapPixelColor(r, g, b);
    
}
Vector3f SceneMaterialPhong::getPhongColor(const Vector3f& current_normal, const Vector3f& current_pos, const shared_ptr<SceneLight>& scene_light)
{
    //当前点法向量
    Vector3f n = current_normal;
    n.normalize();
    // 
    //当前点指向相机的方向
    Vector3f v = - current_pos;
    v.normalize();
    //当前点指向光的方向
    Vector3f light = scene_light->getToLightDir(current_pos);
    //点到光的距离
    float r = (*scene_light->location - current_pos).abs();
    //高光-半程向量
    Vector3f h = (v + light);
    h.normalize();


    float n_dot_l = Vector3f::dot(n, light);
    float n_dot_h = Vector3f::dot(n, h);
    float r_square = r * r;
    if (n_dot_l<0) {
        n_dot_l = 0;
    }
    if (n_dot_h<0) {
        n_dot_h = 0;
    }

    Vector3f diffuse = *diffuse_color;
    diffuse *= n_dot_l;
    diffuse /= r_square;

    Vector3f specular = *specular_color;
    specular *= pow(n_dot_h, shininess);
    specular /= r_square;

    // Phong公式
    //环境光
    Vector3f final_color = *ambient_light;
    //漫反射
    final_color += diffuse;
    //高光
    final_color += specular;
    //乘上光颜色
    final_color *= *scene_light->light_color;
    return final_color;
    
}
