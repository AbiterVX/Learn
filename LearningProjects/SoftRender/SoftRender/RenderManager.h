#pragma once
#include <Windows.h>
#include <thread>

//JSON解析: https://github.com/nlohmann/json
#include <fstream>
#include "json.hpp"
#include <vector>
#include "Vector2f.h"
#include "Vector3f.h"
using json = nlohmann::json;
using namespace std;

const float DELTA_TIME_MS = 10;



class RenderManager
{
public:
    RenderManager(const char* render_config_path);
    ~RenderManager();
public:
    static LRESULT win_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
    //开始渲染
    void startRender();
    __declspec(align(16)) float barycentric_buffer[4];
    __declspec(align(16)) float barycentric_buffer2[4];
private:
    // 从配置文件中导入场景
    void loadSceneFromConfig(); 

    //循环渲染
    void loopRendering();
    //渲染一次
    void tick(float delta_time);
    //渲染三角形
    void renderTriangle(shared_ptr<class SceneMeshObj>& scene_mesh, shared_ptr<class Triangle>& triangle);

    //获取三角形2d重心坐标
    inline Vector3f getBarycentricCoord(const Vector2f& P1, const Vector2f& P2, const Vector2f& P3, float p_x, float p_y){
        Vector3f s1(P2.value_list[0] - P1.value_list[0], P3.value_list[0] - P1.value_list[0], P1.value_list[0] - p_x);
        Vector3f s2(P2.value_list[1] - P1.value_list[1], P3.value_list[1] - P1.value_list[1], P1.value_list[1] - p_y);
        Vector3f u = Vector3f::cross(s1,s2);   
        return {1-(u.value_list[0]+u.value_list[1])/u.value_list[2], u.value_list[0]/u.value_list[2], u.value_list[1]/u.value_list[2] };
        /*float p1_x =P1.value_list[0];
        float p1_y =P1.value_list[1];
        float p2_x =P2.value_list[0];
        float p2_y =P2.value_list[1];
        float p3_x =P3.value_list[0];
        float p3_y =P3.value_list[1];  
        //p2_x * p3_y - p3_x * p2_y     , p1_x * p3_y - p3_x * p1_y
        _mm_store_ps(barycentric_buffer, _mm_mul_ps(_mm_set_ps(p3_x,p1_x,p3_x,p2_x), _mm_set_ps(p1_y,p3_y,p2_y,p3_y)));
        float p2_p3_determinant = barycentric_buffer[0] - barycentric_buffer[1];
        float p1_p3_determinant = barycentric_buffer[2] - barycentric_buffer[3];
        //p2_y - p3_y, p3_x - p2_x, p1_y - p3_y,p3_x - p1_x;
        _mm_store_ps(barycentric_buffer, _mm_sub_ps(_mm_set_ps(p3_x,p1_y,p3_x,p2_y), _mm_set_ps(p1_x,p3_y,p2_x,p3_y)));
        // u = ((P2.y - P3.y) * P.x + (P3.x - P2.x) * P.y + (P2.x * P3.y - P3.x * P2.y)) / ((P2.y - P3.y) * P1.x + (P3.x - P2.x) * P1.y + (P2.x * P3.y - P3.x * P2.y));
        // v = ((P1.y - P3.y) * P.x + (P3.x - P1.x) * P.y + (P1.x * P3.y - P3.x * P1.y)) / ((P1.y - P3.y) * P2.x + (P3.x - P1.x) * P2.y + (P1.x * P3.y - P3.x * P1.y));
        float u = (barycentric_buffer[0] * p_x + barycentric_buffer[1] * p_y + p2_p3_determinant) / (barycentric_buffer[0] * p1_x + barycentric_buffer[1] * p1_y + p2_p3_determinant);
        float v = (barycentric_buffer[2] * p_x + barycentric_buffer[3] * p_y + p1_p3_determinant) / (barycentric_buffer[2] * p2_x + barycentric_buffer[3] * p2_y + p1_p3_determinant);
        return {u, v, 1 - u - v};*/
    }
    //清除z-buffer
    void clearZBuffer();

    //解析json
    shared_ptr<Vector3f> paraseJsonArray(const json& current_array);
public:
    //是否在运行
    static bool is_running;
    //是否需要刷新
    static bool is_need_update;
public:
    //配置数据
    json config_data;

    //视口大小
    int viewport_width;
    int viewport_height;

    //视口的窗口
    HWND viewport_window;
    //视口窗口的hdc
    HDC viewport_hdc;
    //视口窗口的compatible_hdc
    HDC	viewport_compatible_hdc;
    //绘制的位图
    HBITMAP	viewport_display_bitmap;
    //绘制的位图的数据
    void* viewport_display_bitmap_buff;
    //渲染线程
    shared_ptr<thread> render_thread;
    //bitmap绘制工具
    shared_ptr<class BitmapDrawManager> bitmap_draw_mamager;
    //Z-Buffer,记录bitmap像素深度信息
    vector<float> z_buffer;
public:
    //相机
    shared_ptr<class SceneCamera> sceneCamera;
    //灯光
    map<string, shared_ptr<class SceneLight>> sceneLights;
    //模型
    map<string, shared_ptr<class SceneMeshObj>> sceneMeshes;
    //材质
    map<string, shared_ptr<class SceneMaterial>> materials;
};