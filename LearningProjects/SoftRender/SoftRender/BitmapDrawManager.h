#pragma once
#include <windows.h>
#include <immintrin.h> 
class Vector3f;

//BitMap 单个像素的颜色
class BitmapPixelColor
{
public:

    BitmapPixelColor(UCHAR red=0, UCHAR green=0, UCHAR blue=0);
    UCHAR& b();
    UCHAR& g();
    UCHAR& r();
    UCHAR b() const;
    UCHAR g() const;
    UCHAR r() const;
    //获取颜色数据指针（用于读写）
    UCHAR* getDataPtr();

    Vector3f toVector3f();

    BitmapPixelColor& operator += ( const BitmapPixelColor& v );
	BitmapPixelColor& operator -= ( const BitmapPixelColor& v );
    BitmapPixelColor& operator *= ( float f );
    
public:
    //颜色数据（顺序:蓝绿红）
    UCHAR color_data[4];
        
};

BitmapPixelColor operator * ( float f, const BitmapPixelColor& v );
BitmapPixelColor operator * ( const BitmapPixelColor& v, float f );
BitmapPixelColor operator + ( const BitmapPixelColor& v0, const BitmapPixelColor& v1 );





//BitMap绘制管理器
class BitmapDrawManager
{
public:
    BitmapDrawManager(void* bitmap_buffer, int width, int height);
    //绘制某一个像素： 位置（x,y）,颜色: pixel_color
    void drawPixel(int x, int y, BitmapPixelColor& pixel_color);
    //清除图片
    void clearBitmap();
public:
    //写入数据的地址
    UCHAR* temp_bitmap;
    //Bitmap尺寸
    int width;
    int height;

    void* bitmap_buffer;
};

