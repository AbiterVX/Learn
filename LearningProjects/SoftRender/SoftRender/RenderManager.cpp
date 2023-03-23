#include "RenderManager.h"
#include <iostream>
#include <cstddef>
#include <cstdlib>
#include <algorithm>
#include <iterator>
#include <string>
#include <iostream>
#include <windows.h>
#include "BitmapDrawManager.h"
#include "SceneMesh.h"
#include "SceneMaterial.h"
#include "Matrix4f.h"
#include "Vector4f.h"
#include "Vector3f.h"

#include "Triangle.h"

#include "SceneCamera.h"
#include "SceneLight.h"
#include"time.h"
#include <limits>

#define WIN32_LEAN_AND_MEAN
bool RenderManager::is_running = true;
bool RenderManager::is_need_update = true;


RenderManager::RenderManager(const char* render_config_path)
{
    // 解析配置数据
	ifstream f(render_config_path);
	config_data = json::parse(f);
	cout << config_data;

    this->viewport_width = config_data["viewport"]["width"];
    this->viewport_height = config_data["viewport"]["height"];
    z_buffer.resize(this->viewport_width * this->viewport_height, FLT_MAX);
}

RenderManager::~RenderManager()
{
    if(render_thread!=nullptr){
        render_thread->join();
    }
}

LRESULT RenderManager::win_proc(HWND hWnd, ::UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg){
        case WM_KEYDOWN:
            switch (wParam){
                case VK_ESCAPE:
                    is_running = false;
                    break;
                case VK_UP:
                    break;
                case VK_DOWN:
                    break;
                case VK_LEFT:
                    break;
                case VK_RIGHT:
                    break;
            }
            break;
        case WM_CLOSE:
        case WM_DESTROY:
            is_running = false;
            PostQuitMessage(0);
            break;
	default:
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void RenderManager::startRender()
{   
    cout<<"[Start Render]"<<endl;

    //创建窗口,初始化
    {
        const char* viewport_window_name = "SoftRender";
    
        WNDCLASSA wndclass;
        wndclass.cbClsExtra = 0;
        wndclass.cbWndExtra = 0;
        wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_PEN);
        wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
        wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        wndclass.hInstance = GetModuleHandle(NULL);
        wndclass.lpfnWndProc = (WNDPROC)win_proc;
        wndclass.lpszClassName = viewport_window_name;
        wndclass.lpszMenuName = NULL;
        wndclass.style = CS_HREDRAW | CS_VREDRAW;
        RegisterClassA(&wndclass);
        //创建窗口
        viewport_window = CreateWindowA(viewport_window_name, viewport_window_name, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,0, 0, 0, 0, NULL, NULL, wndclass.hInstance, NULL);
        if (viewport_window == NULL){
            cout<<"NULL";
        }

        // 获取的HDC直接与相关设备沟通
        viewport_hdc = GetDC(viewport_window);
        //创建一个与指定设备兼容的内存设备上下文环境（DC）: https://baike.baidu.com/item/CreateCompatibleDC
        viewport_compatible_hdc = CreateCompatibleDC(viewport_hdc);


        // BITMAPINFO:https://learn.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapinfoheader
        //            https://blog.csdn.net/wangwenjing90/article/details/8537355
        BITMAPINFO bmp_info;
        bmp_info.bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
        bmp_info.bmiHeader.biWidth			= viewport_width;
        bmp_info.bmiHeader.biHeight			= viewport_height;
        bmp_info.bmiHeader.biPlanes			= 1;
        bmp_info.bmiHeader.biBitCount		= 32;
        bmp_info.bmiHeader.biCompression	= BI_RGB;
        bmp_info.bmiHeader.biSizeImage		= viewport_width*viewport_height * 4;
        bmp_info.bmiHeader.biXPelsPerMeter	= 0;
        bmp_info.bmiHeader.biYPelsPerMeter	= 0;
        bmp_info.bmiHeader.biClrUsed		= 0;
        bmp_info.bmiHeader.biClrImportant	= 0;

        // 创建应用程序可以直接写入的、与设备无关的位图: https://baike.baidu.com/item/CreateDIBSection/6377219
        viewport_display_bitmap = CreateDIBSection(viewport_compatible_hdc, &bmp_info, DIB_RGB_COLORS, &viewport_display_bitmap_buff, 0, 0);
        //Copy bitmap into hdc_mem
        SelectObject(viewport_compatible_hdc, viewport_display_bitmap);

        //设置窗口位置，显示窗口
        RECT rect = { 0, 0, viewport_width, viewport_height };
        int wx, wy, sx, sy;
        AdjustWindowRect(&rect, GetWindowLong(viewport_window, GWL_STYLE), 0);
        wx = rect.right - rect.left;
        wy = rect.bottom - rect.top;
        sx = (GetSystemMetrics(SM_CXSCREEN) - wx) / 2;
        sy = (GetSystemMetrics(SM_CYSCREEN) - wy) / 2;
        if (sy < 0) sy = 0;
        SetWindowPos(viewport_window, NULL, sx, sy, wx, wy, (SWP_NOCOPYBITS | SWP_NOZORDER | SWP_SHOWWINDOW));
        SetForegroundWindow(viewport_window);
        ShowWindow(viewport_window, SW_SHOWNORMAL);
        // 更新窗口  
        UpdateWindow(viewport_window);

        //绘制管理
        bitmap_draw_mamager = make_shared<BitmapDrawManager>(viewport_display_bitmap_buff, viewport_width, viewport_height);

        
    }
    
    //解析配置数据
    {
        loadSceneFromConfig();
    }

    //开始渲染
    {
        render_thread = make_shared<thread>(&RenderManager::loopRendering, this);
    }
}

void RenderManager::loadSceneFromConfig()
{
    //[相机]
    {
        json camera_config = config_data["scene"]["camera"];
        shared_ptr<Vector3f> location = paraseJsonArray(camera_config["location"]);
        shared_ptr<Vector3f> lookat_target = paraseJsonArray(camera_config["lookat_target"]);

        Vector3f y_up = Vector3f(0.0f, 0.0f, 1.0f);
        Vector3f dir = (*lookat_target - *location).normalized();
        Vector3f right = Vector3f::cross(dir, y_up).normalized();
        Vector3f real_up = Vector3f::cross(right, dir).normalized();
        shared_ptr<Vector3f> real_up_ptr = make_shared<Vector3f>(real_up);

        if (camera_config["type"] == "perspective") {
            sceneCamera = make_shared<SceneCameraPerspective>(location, lookat_target, real_up_ptr, camera_config["z_near"], camera_config["z_far"], camera_config["fov_y_degree"]);
        }
        else if (camera_config["type"] == "orthographic") {
            sceneCamera = make_shared<SceneCameraOrthographic>(location, lookat_target, real_up_ptr, camera_config["z_near"], camera_config["z_far"],
                camera_config["left"], camera_config["right"], camera_config["bottom"], camera_config["top"]);
        }
    }
    
    //[灯光]
    {
        for (auto& element : config_data["scene"]["light"].items()) {
            json current_light = element.value();
            if (current_light["type"] == "point") {
                shared_ptr<Vector3f> location = paraseJsonArray(current_light["location"]);
                shared_ptr<Vector3f> color = paraseJsonArray(current_light["color"]);
                sceneLights[element.key()] = make_shared<SceneLightPoint>(color, location);
            }
        }
    }
    
    //[材质&贴图]
    {
        json matrial_config = config_data["scene"]["material"];
        for (auto& element : matrial_config.items()) {
            json current_material = element.value();
            string texture_path = current_material["texture_path"];
            if (current_material["type"] == "texture") {
                shared_ptr<SceneMaterial> new_material = make_shared<SceneMaterial>(texture_path.c_str());
                materials[element.key()] = new_material;
            }
            else if (current_material["type"] == "texture_bilinear") {
                materials[element.key()] = make_shared<SceneMaterialTextureBilinear>(texture_path.c_str());
            }
            else if (current_material["type"] == "phong") {
                shared_ptr<Vector3f> diffuse_color = paraseJsonArray(current_material["diffuse_color"]);
                shared_ptr<Vector3f> specular_color = paraseJsonArray(current_material["specular_color"]);
                shared_ptr<Vector3f> ambient_light = paraseJsonArray(current_material["ambient_light"]);

                int shininess = current_material["shininess"];
                materials[element.key()] = make_shared<SceneMaterialPhong>(texture_path.c_str(), diffuse_color, specular_color, shininess, ambient_light);
            }
        }
    }

    //[模型]
    {
        for (auto& element : config_data["scene"]["mesh"].items()) {
            json current_mesh = element.value();
            if (current_mesh["type"] == "obj") {
                string path = current_mesh["path"];
                shared_ptr<SceneMeshObj> new_mesh = make_shared<SceneMeshObj>(path.c_str());
                if (current_mesh.contains("material") && materials.find(current_mesh["material"]) != materials.end()) {
                    new_mesh->setMaterial(materials[current_mesh["material"]]);
                }
                shared_ptr<Vector3f> location = paraseJsonArray(current_mesh["location"]);
                shared_ptr<Vector3f> rotation = paraseJsonArray(current_mesh["rotation"]);
                shared_ptr<Vector3f> scale = paraseJsonArray(current_mesh["scale"]);

                new_mesh->setTransform(location, rotation, scale);
                sceneMeshes[element.key()] = new_mesh;
            }
        }
    }
}

void RenderManager::loopRendering()
{
    clock_t start, finish;
    float cost_time=0;
    float remain_time = 0;
    while (is_running){
        start = clock();
        if (remain_time<0) {
            //- remain_time
            this->tick(DELTA_TIME_MS );
        }
        else {
            this->tick(DELTA_TIME_MS);
        }
        
        finish = clock();
        cost_time = finish - start;
        remain_time = DELTA_TIME_MS - cost_time;
        if (remain_time>0) {
            Sleep(remain_time);
        }
        
        printf("time:%f , fps:%f \n", cost_time / CLOCKS_PER_SEC, 1/ (cost_time / CLOCKS_PER_SEC));
    }
}

void RenderManager::tick(float delta_time)
{
    clearZBuffer();
    bitmap_draw_mamager->clearBitmap();

    //[修改 Actor]:调整location,rotation,scale等
    {
        //物体
        if (sceneMeshes.find("box") != sceneMeshes.end()) {
            shared_ptr<SceneMeshObj> current_mesh = sceneMeshes["box"];
            current_mesh->rotation->z() = fmod(current_mesh->rotation->z() + 25.0f * delta_time / 1000.0f, 360.0f);
            current_mesh->updateTransform(nullptr, current_mesh->rotation, nullptr);
        }

        if (sceneMeshes.find("steve") != sceneMeshes.end()) {
            shared_ptr<SceneMeshObj> current_mesh = sceneMeshes["steve"];
            current_mesh->rotation->y() = fmod(current_mesh->rotation->y() + 50.0f * delta_time / 1000.0f, 360.0f);
            current_mesh->updateTransform(nullptr, current_mesh->rotation, nullptr);
        }
        //相机
        //shared_ptr<SceneCameraPerspective> perspective_camera = std::dynamic_pointer_cast<SceneCameraPerspective>(sceneCamera);
        /*perspective_camera->location->x() += 0.05f * delta_time / 1000.0;
        perspective_camera->dir->x() += 0.05f * delta_time / 1000.0;
        perspective_camera->fov_y_degree += 10.0f * delta_time / 1000.0;
        */
    }

    //[更新顶点]: mvp矩阵转换到屏幕坐标
    {
        //mvp 矩阵
        Matrix4f camera_projection = sceneCamera->getProjectionMatrix(this->viewport_width, this->viewport_height);
        Matrix4f viewport_transform = Matrix4f::getViewportTransform(this->viewport_width, this->viewport_height);
        //[场景物体]
        for (auto& element : sceneMeshes) {
            shared_ptr<class SceneMeshObj> current_mesh = element.second;
            Matrix4f current_transform = viewport_transform * camera_projection * (*current_mesh->mesh_transform);
            
            //根据MVP更新点位置
            for (int i=0; i<current_mesh->location_vertices.size(); i++) {
                Vector4f temp_location = current_transform * (*current_mesh->location_vertices[i]);
                temp_location[0] /= temp_location[3];
                temp_location[1] /= temp_location[3];
                temp_location[2] /= temp_location[3];
                //temp_location = temp_location / temp_location[3];
                current_mesh->mvp_location_vertices[i] = make_shared<Vector4f>(temp_location);
            }
        }

        //[光]
        for (auto& element : sceneLights) {
            shared_ptr<SceneLight> current_light = element.second;
            Vector4f temp_location = viewport_transform * camera_projection * Vector4f(*current_light->location, 1);
            temp_location[0] /= temp_location[3];
            temp_location[1] /= temp_location[3];
            temp_location[2] /= temp_location[3];
            current_light->mvp_location = make_shared<Vector4f>(temp_location);
        }
    }

    //[渲染三角形]: 光栅化
    {
        for (auto& element : sceneMeshes) {
            shared_ptr<SceneMeshObj> current_mesh = element.second;
            for (auto& current_triangle : current_mesh->triangles) {             
                renderTriangle(current_mesh, current_triangle);
            }
        }
    }

    //[刷新图像]
    {
        BitBlt(viewport_hdc, 0, 0, viewport_width, viewport_height, viewport_compatible_hdc, 0, 0, SRCCOPY);
    }
}

void RenderManager::renderTriangle(shared_ptr<SceneMeshObj>& scene_mesh, shared_ptr<Triangle>& triangle)
{
    //2d坐标
    vector<Vector2f> points;
    //w = -z
    vector<float> w_values;
    {
        for(int i=0; i<3; i++){
            shared_ptr<Vector4f> mvp_location = scene_mesh->mvp_location_vertices[triangle->location_indexes[i]];
            points.push_back(mvp_location->xy());
            w_values.push_back(mvp_location->value_list[3]);
        }
    }
    Vector3f z_values(-w_values[0], -w_values[1], -w_values[2]);
    // 背面剔除
    {
        float z = Vector2f::cross((points[1] - points[0]).normal(), (points[2] - points[1]).normal()).value_list[2];
        if (z < 0) {
            return;
        }
    }

    //包围盒(AABB): 获取2d平面中的范围
    vector<int> min_point = {viewport_width, viewport_height};
    vector<int> max_point = {0, 0};
    {  
        //AABB包围盒    
        for(int i=0; i<3; i++){      
            if(points[i].value_list[0] < min_point[0]){
                min_point[0] = floor(points[i].value_list[0]);
            }
            if(points[i].value_list[1] < min_point[1]){
                min_point[1] = floor(points[i].value_list[1]);
            }

            if(points[i].value_list[0] > max_point[0]){
                max_point[0] = ceil(points[i].value_list[0]);
            }
            if(points[i].value_list[1] > max_point[1]){
                max_point[1] = ceil(points[i].value_list[1]);
            }
        }
        if (min_point[0] < 0) {
            min_point[0] = 0;
        }
        if (min_point[1] < 0) {
            min_point[1] = 0;
        }
        if (max_point[0] > viewport_width-1) {
            max_point[0] = viewport_width-1;
        }
        if (max_point[1] > viewport_height - 1) {
            max_point[1] = viewport_height - 1;
        }
    }
    

    //------------------渲染---------------------   
    //当前三角形顶点数据: 贴图坐标,法向量,位置
    Vector2f texture_vertex1 = *scene_mesh->texture_vertices[triangle->texture_indexes[0]];
    Vector2f texture_vertex2 = *scene_mesh->texture_vertices[triangle->texture_indexes[1]];
    Vector2f texture_vertex3 = *scene_mesh->texture_vertices[triangle->texture_indexes[2]];                     
    Vector3f world_normal_vertex1 = scene_mesh->world_normal_vertices[triangle->normal_indexes[0]]->xyz();
    Vector3f world_normal_vertex2 = scene_mesh->world_normal_vertices[triangle->normal_indexes[1]]->xyz();
    Vector3f world_normal_vertex3 = scene_mesh->world_normal_vertices[triangle->normal_indexes[2]]->xyz();
    Vector3f world_location_vertex1 = scene_mesh->world_location_vertices[triangle->location_indexes[0]]->xyz();
    Vector3f world_location_vertex2 = scene_mesh->world_location_vertices[triangle->location_indexes[1]]->xyz();
    Vector3f world_location_vertex3 = scene_mesh->world_location_vertices[triangle->location_indexes[2]]->xyz();

    //材质
    shared_ptr<SceneMaterial> current_material = scene_mesh->getMaterial();
    //遍历像素
    bool is_in_triangle;
    for (int i = min_point[0]; i <= max_point[0]; i++) {
        for (int j = min_point[1]; j <= max_point[1]; j++) {              
            
            //得到重心坐标, 像素中点(i + 0.5f, j + 0.5f)
            Vector3f barycentric_coord = getBarycentricCoord(points[0], points[1], points[2], i + 0.5f, j + 0.5f);
            
            //判断：是否在三角形内，即重心坐标在0-1之间
            is_in_triangle = true;
            for (auto& current_value : barycentric_coord.value_list) {
                if (current_value < 0 || 1 < current_value) {
                    is_in_triangle = false;
                    break;
                }
            }
            
            //绘制像素
            if (is_in_triangle) {
                //矫正后权重
                barycentric_coord /= z_values;
                float z_value = 1/ (barycentric_coord.value_list[0] + barycentric_coord.value_list[1] + barycentric_coord.value_list[2]);
                barycentric_coord *= z_value;
                float weight1 = barycentric_coord.value_list[0];
                float weight2 = barycentric_coord.value_list[1];
                float weight3 = barycentric_coord.value_list[2];
                
                //Z-Buffer
                float z_buffer_value = 1/z_value;
                int index = j * viewport_width + i;
                if ( z_buffer_value< z_buffer[index]) {
                    //使用 1/z 作为z-buffer,(z_value <0)
                    z_buffer[index] = z_buffer_value;

                    //获取当前像素的颜色
                    BitmapPixelColor pixel_color;
                    {
                        //获取贴图的颜色
                        if (current_material!=nullptr) {
                            //当前点贴图坐标
                            
                            //无透视矫正
                            //texture_coord = (*texture_coord1) * barycentric_coord.x() + (*texture_coord2) * barycentric_coord.y() + (*texture_coord3) * barycentric_coord.z();   
                            //透视矫正
                            Vector2f texture_coord = Vector2f::addVectorByBarycentricCoord(
                                texture_vertex1, weight1,
                                texture_vertex2, weight2,
                                texture_vertex3, weight3
                            );

                            //法向量
                            Vector3f normal = Vector3f::addVectorByBarycentricCoord(
                                world_normal_vertex1, weight1,
                                world_normal_vertex2, weight2,
                                world_normal_vertex3, weight3
                            );

                            //位置
                            Vector3f current_pos = Vector3f::addVectorByBarycentricCoord(
                                world_location_vertex1, weight1,
                                world_location_vertex2, weight2,
                                world_location_vertex3, weight3
                            );
                            
                            //着色:根据重心坐标求出颜色
                            pixel_color = current_material->getPixelColorbyPercent(texture_coord, normal, current_pos, sceneLights);
                            //pixel_color = BitmapPixelColor(int(255 * barycentric_coord.x()), int(255 * barycentric_coord.y()), int(255 * barycentric_coord.z()));
                        }
                        else {
                            //测试颜色
                            pixel_color = BitmapPixelColor(int(255 * barycentric_coord.x()), int(255 * barycentric_coord.y()), int(255 * barycentric_coord.z()));
                        }                   
                    }
                    
                    //绘制像素
                    bitmap_draw_mamager->drawPixel(i, j, pixel_color);
                }
            }
            
        }
        
    } 
}

void RenderManager::clearZBuffer()
{
    //std::fill(z_buffer.begin(), z_buffer.end(), FLT_MAX);
    for (auto& currentValue: z_buffer) {
        currentValue = FLT_MAX;
    }
}

shared_ptr<Vector3f> RenderManager::paraseJsonArray(const json& current_array)
{
    return make_shared<Vector3f>(current_array[0], current_array[1], current_array[2]);
}


/*
//判断像素是否在三角形内：三个边分别做叉乘，值全为正或全为负时在三角形内部。
    //边向量
    vector<Vector2f> edge_vectors;
    {
        
        //边向量
        for (int i = 0; i<3; i++) {
            edge_vectors.push_back( (points[(i+1)%3] - points[i%3]).normal() );
        }
    }
            bool is_in_triangle = true;
            {
                float cross_product = 0;
                bool is_positive = false;
                for(int k=0;k<3;k++){
                    cross_product = Vector2f::cross( edge_vectors[k], (current_point - points[k]).normal() ).z();
                    if(cross_product>0){
                        if(k==0){
                            is_positive = true;
                        }
                        else{
                            if(!is_positive){
                                is_in_triangle = false;
                                break;
                            }
                        }
                    }
                    else{
                        if(k==0){
                            is_positive = false;
                        }
                        else{
                            if(is_positive){
                                is_in_triangle = false;
                                break;
                            }
                        }
                    }
                }
            }
*/