#include "Vector2f.h"
#include "Vector3f.h"
#include <cmath>

const Vector2f Vector2f::ZERO = Vector2f( 0, 0 );
const Vector2f Vector2f::UP = Vector2f( 0, 1 );
const Vector2f Vector2f::RIGHT = Vector2f( 1, 0 );


Vector2f::Vector2f(float f)
{
    value_list[0] = f;
    value_list[1] = f;
}

Vector2f::Vector2f( float x, float y )
{
    value_list[0] = x;
    value_list[1] = y;
}

Vector2f::Vector2f( const Vector2f& rv )
{
    value_list[0] = rv[0];
    value_list[1] = rv[1];
}

Vector2f& Vector2f::operator = ( const Vector2f& rv )
{
 	if( this != &rv ){
        value_list[0] = rv[0];
        value_list[1] = rv[1];
    }
    return *this;
}

const float& Vector2f::operator [] ( int i ) const
{
    return value_list[i];
}

float& Vector2f::operator [] ( int i )
{
    return value_list[i];
}


float& Vector2f::x()
{
    return value_list[0];
}

float& Vector2f::y()
{
    return value_list[1];
}

float Vector2f::x() const
{
    return value_list[0];
}	

float Vector2f::y() const
{
    return value_list[1];
}

Vector2f& Vector2f::operator += ( const Vector2f& v )
{
	value_list[ 0 ] += v.value_list[ 0 ];
	value_list[ 1 ] += v.value_list[ 1 ];
	return *this;
}

Vector2f& Vector2f::operator -= ( const Vector2f& v )
{
	value_list[ 0 ] -= v.value_list[ 0 ];
	value_list[ 1 ] -= v.value_list[ 1 ];
	return *this;
}

Vector2f& Vector2f::operator *= ( float f )
{
	value_list[ 0 ] *= f;
	value_list[ 1 ] *= f;
	return *this;
}



Vector2f Vector2f::normal() const
{
    return Vector2f( -value_list[1], value_list[0] );
}

float Vector2f::abs() const
{
    return sqrt(absSquared());
}

float Vector2f::absSquared() const
{
    return value_list[0] * value_list[0] + value_list[1] * value_list[1];
}

void Vector2f::normalize()
{
    float norm = abs();
    value_list[0] /= norm;
    value_list[1] /= norm;
}

Vector2f Vector2f::normalized() const
{
    float norm = abs();
    return Vector2f( value_list[0] / norm, value_list[1] / norm );
}

float Vector2f::dot( const Vector2f& v0, const Vector2f& v1 )
{
    return v0[0] * v1[0] + v0[1] * v1[1];
}

Vector3f Vector2f::cross( const Vector2f& v0, const Vector2f& v1 )
{
	return Vector3f(0, 0,  v0.x()*v1.y()-v0.y()*v1.x() );
}







Vector2f operator + ( const Vector2f& v0, const Vector2f& v1 )
{
    return Vector2f( v0.x() + v1.x(), v0.y() + v1.y() );
}

Vector2f operator - ( const Vector2f& v0, const Vector2f& v1 )
{
    return Vector2f( v0.x() - v1.x(), v0.y() - v1.y() );
}

Vector2f operator * ( const Vector2f& v0, const Vector2f& v1 )
{
    return Vector2f( v0.x() * v1.x(), v0.y() * v1.y() );
}

Vector2f operator / ( const Vector2f& v0, const Vector2f& v1 )
{
    return Vector2f( v0.x() * v1.x(), v0.y() * v1.y() );
}

Vector2f operator - ( const Vector2f& v )
{
    return Vector2f( -v.x(), -v.y() );
}

Vector2f operator * ( float f, const Vector2f& v )
{
    return Vector2f( f * v.x(), f * v.y() );
}

Vector2f operator * ( const Vector2f& v, float f )
{
    return Vector2f( f * v.x(), f * v.y() );
}

Vector2f operator / ( const Vector2f& v, float f )
{
    return Vector2f( v.x() / f, v.y() / f );
}

bool operator == ( const Vector2f& v0, const Vector2f& v1 )
{
    return( v0.x() == v1.x() && v0.y() == v1.y() );
}

bool operator != ( const Vector2f& v0, const Vector2f& v1 )
{
    return !( v0 == v1 );
}
