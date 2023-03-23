#include "BitmapDrawManager.h"
#include <cmath>
#include "Vector3f.h"

//----------------------------------------------------- BitmapPixelColor

BitmapPixelColor::BitmapPixelColor(UCHAR red, UCHAR green, UCHAR blue)
{
    color_data[0] = blue;
    color_data[1] = green;
    color_data[2] = red;
}


UCHAR& BitmapPixelColor::b()
{
    return color_data[0];
}

UCHAR& BitmapPixelColor::g()
{
    return color_data[1];
}

UCHAR& BitmapPixelColor::r()
{
    return color_data[2];
}

UCHAR BitmapPixelColor::b() const
{
    return color_data[0];
}

UCHAR BitmapPixelColor::g() const
{
    return color_data[1];
}

UCHAR BitmapPixelColor::r() const
{
    return color_data[2];
}

BitmapPixelColor& BitmapPixelColor::operator += ( const BitmapPixelColor& v )
{
	color_data[ 0 ] += v.color_data[ 0 ];
	color_data[ 1 ] += v.color_data[ 1 ];
	color_data[ 2 ] += v.color_data[ 2 ];
	return *this;
}

BitmapPixelColor& BitmapPixelColor::operator -= ( const BitmapPixelColor& v )
{
	color_data[ 0 ] -= v.color_data[ 0 ];
	color_data[ 1 ] -= v.color_data[ 1 ];
	color_data[ 2 ] -= v.color_data[ 2 ];
	return *this;
}

BitmapPixelColor& BitmapPixelColor::operator *= ( float f )
{
	color_data[ 0 ] *= f;
	color_data[ 1 ] *= f;
	color_data[ 2 ] *= f;
	return *this;
}


UCHAR* BitmapPixelColor::getDataPtr()
{
    return color_data;
}

Vector3f BitmapPixelColor::toVector3f()
{
    return Vector3f(float(r()), float(g()), float(b()));
}




BitmapPixelColor operator * ( float f, const BitmapPixelColor& v )
{
    return BitmapPixelColor( round(v.r() * f), round(v.g() * f), round(v.b() * f) );
}

BitmapPixelColor operator * ( const BitmapPixelColor& v, float f )
{
    return BitmapPixelColor( round(v.r() * f), round(v.g() * f), round(v.b() * f) );
}

BitmapPixelColor operator + ( const BitmapPixelColor& v0, const BitmapPixelColor& v1 )
{
    return BitmapPixelColor( v0.r() + v1.r(), v0.g() + v1.g(), v0.b() + v1.b() );
}



//----------------------------------------------------- BitmapDrawManager

BitmapDrawManager::BitmapDrawManager(void* bitmap_buffer, int width, int height)
{
    this->bitmap_buffer = bitmap_buffer;
    this->width = width;
    this->height = height;
    temp_bitmap = (UCHAR*)this->bitmap_buffer;
    //初始bitmap
    clearBitmap();
}

void BitmapDrawManager::drawPixel(int x, int y, BitmapPixelColor& pixel_color)
{
    int index =  (y * width + x) * 4;
    memcpy(temp_bitmap+index, pixel_color.getDataPtr(), 3);
    //memset(temp_bitmap+ (i*viewport_width+j)*4+1, 255, 1);
}

void BitmapDrawManager::clearBitmap()
{
    memset(this->bitmap_buffer, 0, this->width * this->height * 4);
}