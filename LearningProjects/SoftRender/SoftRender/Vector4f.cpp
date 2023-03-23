#include "Vector4f.h"

#include "Vector2f.h"
#include <cmath>
#include <cstdio>
#include <cstdlib>

Vector4f::Vector4f(float x, float y, float z, float w)
{
	value_list[0] = x;
	value_list[1] = y;
	value_list[2] = z;
	value_list[3] = w;
}

Vector4f::Vector4f( float x, const Vector3f& yzw )
{
	value_list[0] = x;
	value_list[1] = yzw.x();
	value_list[2] = yzw.y();
	value_list[3] = yzw.z();
}

Vector4f::Vector4f( const Vector3f& xyz, float w )
{
	value_list[0] = xyz.x();
	value_list[1] = xyz.y();
	value_list[2] = xyz.z();
	value_list[3] = w;
}


Vector4f& Vector4f::operator = (const Vector4f& other_vector)
{
	if (this != &other_vector)
	{
		value_list[0] = other_vector.value_list[0];
		value_list[1] = other_vector.value_list[1];
		value_list[2] = other_vector.value_list[2];
		value_list[3] = other_vector.value_list[3];
	}
	return *this;
}

const float& Vector4f::operator [] (int i) const
{
	return value_list[i];
}
float& Vector4f::operator [] ( int i )
{
	return value_list[ i ];
}

Vector2f Vector4f::xy() const
{
	return Vector2f(value_list[0], value_list[1]);
}

float Vector4f::getLength() const
{
	return sqrt(value_list[0] * value_list[0] + 
				value_list[1] * value_list[1] +
				value_list[2] * value_list[2] +
				value_list[3] * value_list[3]);
}

Vector4f Vector4f::normalized() const
{
	float length = getLength();
	return Vector4f
	(
		value_list[0] / length,
		value_list[1] / length,
		value_list[2] / length,
		value_list[3] / length
	);
}

void Vector4f::print() const
{
	printf( "< %.4f, %.4f, %.4f, %.4f >\n",
		value_list[0], value_list[1], value_list[2], value_list[3] );
}

Vector4f& Vector4f::operator += ( const Vector4f& v )
{
	mmvalue = _mm_add_ps(mmvalue, v.mmvalue);

	/*value_list[ 0 ] += v.value_list[ 0 ];
	value_list[ 1 ] += v.value_list[ 1 ];
	value_list[ 2 ] += v.value_list[ 2 ];
	value_list[ 3 ] += v.value_list[ 3 ];*/
	return *this;
}

Vector4f& Vector4f::operator -= ( const Vector4f& v )
{
	mmvalue = _mm_sub_ps(mmvalue, v.mmvalue);

	/*value_list[ 0 ] -= v.value_list[ 0 ];
	value_list[ 1 ] -= v.value_list[ 1 ];
	value_list[ 2 ] -= v.value_list[ 2 ];
	value_list[ 3 ] -= v.value_list[ 3 ];*/
	return *this;
}

Vector4f& Vector4f::operator *= ( float f )
{
	value_list[ 0 ] *= f;
	value_list[ 1 ] *= f;
	value_list[ 2 ] *= f;
	value_list[ 3 ] *= f;
	return *this;
}


//-------------------- º∆À„ --------------------

Vector4f operator + (const Vector4f& v0, const Vector4f& v1)
{
	return Vector4f(v0.value_list[0] + v1.value_list[0],v0.value_list[1] + v1.value_list[1],v0.value_list[2] + v1.value_list[2],v0.value_list[3] + v1.value_list[3]);
}

Vector4f operator - (const Vector4f& v0, const Vector4f& v1)
{
	return Vector4f(v0.value_list[0] - v1.value_list[0],v0.value_list[1] - v1.value_list[1],v0.value_list[2] - v1.value_list[2],v0.value_list[3] - v1.value_list[3]);
}

Vector4f operator * (const Vector4f& v0, const Vector4f& v1)
{
	return Vector4f(v0.value_list[0] * v1.value_list[0],v0.value_list[1] * v1.value_list[1],v0.value_list[2] * v1.value_list[2],v0.value_list[3] * v1.value_list[3]);
}

Vector4f operator / (const Vector4f& v0, const Vector4f& v1)
{
	return Vector4f(v0.value_list[0] / v1.value_list[0],v0.value_list[1] / v1.value_list[1],v0.value_list[2] / v1.value_list[2],v0.value_list[3] / v1.value_list[3]);
}

Vector4f operator - (const Vector4f& v)
{
	return Vector4f(-v.value_list[0], -v.value_list[1], -v.value_list[2], -v.value_list[3]);
}

Vector4f operator * (float f, const Vector4f& v)
{
	return Vector4f(f * v.value_list[0], f * v.value_list[1], f * v.value_list[2], f * v.value_list[3]);
}

Vector4f operator * (const Vector4f& v, float f)
{
	return Vector4f(f * v.value_list[0], f * v.value_list[1], f * v.value_list[2], f * v.value_list[3]);
}

Vector4f operator / (const Vector4f& v, float f)
{
	return Vector4f(v.value_list[0] / f, v.value_list[1] / f, v.value_list[2] / f, v.value_list[3] / f);
}

bool operator == (const Vector4f& v0, const Vector4f& v1)
{
	return(v0.value_list[0] == v1.value_list[0] && v0.value_list[1] == v1.value_list[1] && v0.value_list[2] == v1.value_list[2] &&v0.value_list[3] == v1.value_list[3]);
}

bool operator != (const Vector4f& v0, const Vector4f& v1)
{
	return !(v0 == v1);
}
