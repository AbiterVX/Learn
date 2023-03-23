#pragma once
#include<string>
#include <vector>
#include <Windows.h>
using namespace std;

class UtilFunc
{
public:
	//string �з�
	static std::vector<std::string> split(const string& input, const string& regex);
	//char* ת LPCWSTR
	static wchar_t* convertCharArrayToLPCWSTR(const char* charArray);
	//��ȡHBITMAP����������
	static std::vector<unsigned char> getPixels(HBITMAP bitmap_handle, int& width, int& height);
	//����Bitmap�ߴ�
	static void setBitmapSize(HBITMAP bitmap, int& width, int& height);

	//���ļ�
	static std::vector<string> readFile(const char* data_path);
};

