
#include "Vector3f.h"
#include <cmath>
#include "Vector2f.h"
#include <cstdio>
#include <cstdlib>


const Vector3f Vector3f::ZERO = Vector3f( 0, 0, 0 );
const Vector3f Vector3f::ONE = Vector3f( 1, 1, 1 );
const Vector3f Vector3f::UP = Vector3f( 0, 1, 0 );
const Vector3f Vector3f::RIGHT = Vector3f( 1, 0, 0 );
const Vector3f Vector3f::FORWARD = Vector3f( 0, 0, -1 );
static const __m128 SIGNMASK = _mm_castsi128_ps(_mm_set1_epi32(0x80000000));

Vector3f::Vector3f( float f )
{
    /*value_list[0] = f;
    value_list[1] = f;
    value_list[2] = f;*/
    mmvalue = _mm_set_ps(0, f, f, f);
}

Vector3f::Vector3f( float x, float y, float z )
{
    mmvalue = _mm_set_ps(0, z, y, x);
    /*value_list[0] = x;
    value_list[1] = y;
    value_list[2] = z;*/
}

float& Vector3f::x()
{
    return value_list[0];
}

float& Vector3f::y()
{
    return value_list[1];
}

float& Vector3f::z()
{
    return value_list[2];
}

float Vector3f::x() const
{
    return value_list[0];
}

float Vector3f::y() const
{
    return value_list[1];
}

float Vector3f::z() const
{
    return value_list[2];
}

Vector2f Vector3f::xy() const
{
	return Vector2f( value_list[0], value_list[1] );
}




const float& Vector3f::operator [] ( int i ) const
{
    return value_list[i];
}

float& Vector3f::operator [] ( int i )
{
    return value_list[i];
}

float Vector3f::abs() const
{
    return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(mmvalue, mmvalue, 0x71))); 

	//return sqrt( value_list[0] * value_list[0] + value_list[1] * value_list[1] + value_list[2] * value_list[2] );
}

void Vector3f::normalize()
{
    mmvalue = _mm_div_ps(mmvalue, _mm_set1_ps(abs()));
}

Vector3f Vector3f::normalized() const
{
    //return _mm_div_ps(mmvalue, _mm_sqrt_ps(_mm_dp_ps(mmvalue, mmvalue, 0xFF)));
    return _mm_div_ps(mmvalue, _mm_set1_ps(abs()));
}

Vector3f& Vector3f::operator += ( const Vector3f& v )
{
    mmvalue = _mm_add_ps(mmvalue, v.mmvalue); 
    return *this;

	/*value_list[ 0 ] += v.value_list[ 0 ];
	value_list[ 1 ] += v.value_list[ 1 ];
	value_list[ 2 ] += v.value_list[ 2 ];
	return *this;*/
}

Vector3f& Vector3f::operator -= ( const Vector3f& v )
{
    mmvalue = _mm_sub_ps(mmvalue, v.mmvalue); 
    return *this;

	/*value_list[ 0 ] -= v.value_list[ 0 ];
	value_list[ 1 ] -= v.value_list[ 1 ];
	value_list[ 2 ] -= v.value_list[ 2 ];
	return *this;*/
}

void Vector3f::print() const
{
	printf( "< %.4f, %.4f, %.4f >\n",
		value_list[0], value_list[1], value_list[2] );
}




//-------------------- 计算 --------------------
Vector3f operator + ( const Vector3f& v0, const Vector3f& v1 )
{
    return _mm_add_ps(v0.mmvalue, v1.mmvalue);
    //return Vector3f( v0[0] + v1[0], v0[1] + v1[1], v0[2] + v1[2] );
}

Vector3f operator - ( const Vector3f& v0, const Vector3f& v1 )
{
    return _mm_sub_ps(v0.mmvalue, v1.mmvalue);
    //return Vector3f( v0[0] - v1[0], v0[1] - v1[1], v0[2] - v1[2] );
}

Vector3f operator * ( const Vector3f& v0, const Vector3f& v1 )
{
    return _mm_mul_ps(v0.mmvalue, v1.mmvalue);
    //return Vector3f( v0[0] * v1[0], v0[1] * v1[1], v0[2] * v1[2] );
}

Vector3f operator / ( const Vector3f& v0, const Vector3f& v1 )
{
    return _mm_div_ps(v0.mmvalue, v1.mmvalue);
    //return Vector3f( v0[0] / v1[0], v0[1] / v1[1], v0[2] / v1[2] );
}

Vector3f operator - ( const Vector3f& v )
{
    return _mm_xor_ps(v.mmvalue, SIGNMASK);
    //return Vector3f( -v[0], -v[1], -v[2] );
}

Vector3f operator * ( float f, const Vector3f& v )
{
    return Vector3f( v[0] * f, v[1] * f, v[2] * f );
}

Vector3f operator * ( const Vector3f& v, float f )
{
    return Vector3f( v[0] * f, v[1] * f, v[2] * f );
}

Vector3f operator / ( const Vector3f& v, float f )
{
    return Vector3f( v[0] / f, v[1] / f, v[2] / f );
}

bool operator == ( const Vector3f& v0, const Vector3f& v1 )
{
    return( v0[0] == v1[0] && v0[1] == v1[1] && v0[2] == v1[2] );
}

bool operator != ( const Vector3f& v0, const Vector3f& v1 )
{
    return !( v0 == v1 );
}
