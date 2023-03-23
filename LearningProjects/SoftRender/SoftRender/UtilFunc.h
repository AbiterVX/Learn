#pragma once
#include<string>
#include <vector>
#include <Windows.h>
using namespace std;

class UtilFunc
{
public:
	//string 切分
	static std::vector<std::string> split(const string& input, const string& regex);
	//char* 转 LPCWSTR
	static wchar_t* convertCharArrayToLPCWSTR(const char* charArray);
	//获取HBITMAP的像素数据
	static std::vector<unsigned char> getPixels(HBITMAP bitmap_handle, int& width, int& height);
	//设置Bitmap尺寸
	static void setBitmapSize(HBITMAP bitmap, int& width, int& height);

	//读文件
	static std::vector<string> readFile(const char* data_path);
};

