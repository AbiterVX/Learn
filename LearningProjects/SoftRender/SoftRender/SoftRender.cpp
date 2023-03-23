// SoftRender.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>
#include <immintrin.h>  
#include "RenderManager.h"

//配置数据路径
const char* DATA_PATH_RENDER_CONFIG = "data/render_config.json";

int main()
{
	RenderManager render_manager(DATA_PATH_RENDER_CONFIG);
	render_manager.startRender();
	return 0;
}