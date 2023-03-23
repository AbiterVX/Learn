#include "UtilFunc.h"
#include <iostream>
#include <fstream>
#include <iterator>
#include <regex>

std::vector<std::string> UtilFunc::split(const string& input, const string& regex) {
    // passing -1 as the submatch index parameter performs splitting
    std::regex re(regex);
    std::sregex_token_iterator
        first{ input.begin(), input.end(), re, -1 },
        last;
    return { first, last };
}

wchar_t* UtilFunc::convertCharArrayToLPCWSTR(const char* charArray)
{
    wchar_t* wString = new wchar_t[4096];
    MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
    return wString;
}

std::vector<unsigned char> UtilFunc::getPixels(HBITMAP bitmap_handle, int& width, int& height)
{
    BITMAP bmp = { 0 };
    BITMAPINFO info = { 0 };
    std::vector<unsigned char> pixels = std::vector<unsigned char>();

    HDC hdc = CreateCompatibleDC(NULL);
    std::memset(&info, 0, sizeof(BITMAPINFO)); //not necessary really..
    HBITMAP OldBitmap = (HBITMAP)SelectObject(hdc, bitmap_handle);
    GetObject(bitmap_handle, sizeof(bmp), &bmp);

    info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    info.bmiHeader.biWidth = width = bmp.bmWidth;
    info.bmiHeader.biHeight = height = bmp.bmHeight;
    info.bmiHeader.biPlanes = 1;
    info.bmiHeader.biBitCount = bmp.bmBitsPixel;
    info.bmiHeader.biCompression = BI_RGB;
    info.bmiHeader.biSizeImage = ((width * bmp.bmBitsPixel + 31) / 32) * 4 * height;

    pixels.resize(info.bmiHeader.biSizeImage);
    GetDIBits(hdc, bitmap_handle, 0, height, &pixels[0], &info, DIB_RGB_COLORS);
    SelectObject(hdc, OldBitmap);
    height = std::abs(height);
    DeleteDC(hdc);
    return pixels;
}

void UtilFunc::setBitmapSize(HBITMAP bitmap, int& width, int& height)
{
    BITMAP bm;
    GetObject(bitmap, sizeof(bm), &bm);
    width = bm.bmWidth;
    height = bm.bmHeight;
}

vector<string> UtilFunc::readFile(const char* data_path)
{
    vector<string> result;

    ifstream ifs;
    ifs.open(data_path, ios::in);
    string buffer;
    while (getline(ifs, buffer)) {
        result.push_back(buffer);
    }
    return result;
}
