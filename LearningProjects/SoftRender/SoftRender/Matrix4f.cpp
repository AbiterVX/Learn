#include "Matrix4f.h"
#include "Matrix3f.h"
#include "Vector3f.h"
#include "Vector4f.h"
#include <cmath>
#include <numbers>
#include <iostream>
#define M_PI 3.1415926f
using namespace std;

Matrix4f::Matrix4f(float fill)
{
	for (int i = 0; i < 16; ++i)
	{
		value_list[i] = fill;
	}
}

Matrix4f::Matrix4f(float f00, float f01, float f02, float f03,
	float f10, float f11, float f12, float f13,
	float f20, float f21, float f22, float f23,
	float f30, float f31, float f32, float f33) 
{
	value_list[0] = f00;
	value_list[1] = f10;
	value_list[2] = f20;
	value_list[3] = f30;

	value_list[4] = f01;
	value_list[5] = f11;
	value_list[6] = f21;
	value_list[7] = f31;

	value_list[8] = f02;
	value_list[9] = f12;
	value_list[10] = f22;
	value_list[11] = f32;

	value_list[12] = f03;
	value_list[13] = f13;
	value_list[14] = f23;
	value_list[15] = f33;
}


const float& Matrix4f::operator () ( int i, int j ) const
{
	return value_list[ j * 4 + i ];
}

float& Matrix4f::operator () ( int i, int j )
{
	return value_list[ j * 4 + i ];
}



Matrix4f Matrix4f::inverse( bool* pbIsSingular, float epsilon ) const
{
	float m00 = value_list[ 0 ];
	float m10 = value_list[ 1 ];
	float m20 = value_list[ 2 ];
	float m30 = value_list[ 3 ];

	float m01 = value_list[ 4 ];
	float m11 = value_list[ 5 ];
	float m21 = value_list[ 6 ];
	float m31 = value_list[ 7 ];

	float m02 = value_list[ 8 ];
	float m12 = value_list[ 9 ];
	float m22 = value_list[ 10 ];
	float m32 = value_list[ 11 ];

	float m03 = value_list[ 12 ];
	float m13 = value_list[ 13 ];
	float m23 = value_list[ 14 ];
	float m33 = value_list[ 15 ];

    float cofactor00 =  Matrix3f::determinant3x3( m11, m12, m13, m21, m22, m23, m31, m32, m33 );
    float cofactor01 = -Matrix3f::determinant3x3( m12, m13, m10, m22, m23, m20, m32, m33, m30 );
    float cofactor02 =  Matrix3f::determinant3x3( m13, m10, m11, m23, m20, m21, m33, m30, m31 );
    float cofactor03 = -Matrix3f::determinant3x3( m10, m11, m12, m20, m21, m22, m30, m31, m32 );
    
    float cofactor10 = -Matrix3f::determinant3x3( m21, m22, m23, m31, m32, m33, m01, m02, m03 );
    float cofactor11 =  Matrix3f::determinant3x3( m22, m23, m20, m32, m33, m30, m02, m03, m00 );
    float cofactor12 = -Matrix3f::determinant3x3( m23, m20, m21, m33, m30, m31, m03, m00, m01 );
    float cofactor13 =  Matrix3f::determinant3x3( m20, m21, m22, m30, m31, m32, m00, m01, m02 );
    
    float cofactor20 =  Matrix3f::determinant3x3( m31, m32, m33, m01, m02, m03, m11, m12, m13 );
    float cofactor21 = -Matrix3f::determinant3x3( m32, m33, m30, m02, m03, m00, m12, m13, m10 );
    float cofactor22 =  Matrix3f::determinant3x3( m33, m30, m31, m03, m00, m01, m13, m10, m11 );
    float cofactor23 = -Matrix3f::determinant3x3( m30, m31, m32, m00, m01, m02, m10, m11, m12 );
    
    float cofactor30 = -Matrix3f::determinant3x3( m01, m02, m03, m11, m12, m13, m21, m22, m23 );
    float cofactor31 =  Matrix3f::determinant3x3( m02, m03, m00, m12, m13, m10, m22, m23, m20 );
    float cofactor32 = -Matrix3f::determinant3x3( m03, m00, m01, m13, m10, m11, m23, m20, m21 );
    float cofactor33 =  Matrix3f::determinant3x3( m00, m01, m02, m10, m11, m12, m20, m21, m22 );

	float determinant = m00 * cofactor00 + m01 * cofactor01 + m02 * cofactor02 + m03 * cofactor03;

	bool isSingular = ( fabs( determinant ) < epsilon );
	if( isSingular )
	{
		if( pbIsSingular != NULL )
		{
			*pbIsSingular = true;
		}
		return Matrix4f();
	}
	else
	{
		if( pbIsSingular != NULL )
		{
			*pbIsSingular = false;
		}

		float reciprocalDeterminant = 1.0f / determinant;

		return Matrix4f
			(
				cofactor00 * reciprocalDeterminant, cofactor10 * reciprocalDeterminant, cofactor20 * reciprocalDeterminant, cofactor30 * reciprocalDeterminant,
				cofactor01 * reciprocalDeterminant, cofactor11 * reciprocalDeterminant, cofactor21 * reciprocalDeterminant, cofactor31 * reciprocalDeterminant,
				cofactor02 * reciprocalDeterminant, cofactor12 * reciprocalDeterminant, cofactor22 * reciprocalDeterminant, cofactor32 * reciprocalDeterminant,
				cofactor03 * reciprocalDeterminant, cofactor13 * reciprocalDeterminant, cofactor23 * reciprocalDeterminant, cofactor33 * reciprocalDeterminant
			);
	}
}


Matrix3f Matrix4f::getSubmatrix3x3( int i0, int j0 ) const
{
	Matrix3f out;

	for( int i = 0; i < 3; ++i ){
		for( int j = 0; j < 3; ++j ){
			out( i, j ) = ( *this )( i + i0, j + j0 );
		}
	}

	return out;
}

void Matrix4f::transpose()
{
	float temp;

	for( int i = 0; i < 3; ++i ){
		for( int j = i + 1; j < 4; ++j ){
			temp = ( *this )( i, j );
			( * this )( i, j ) = ( *this )( j, i );
			( *this )( j, i ) = temp;
		}
	}
}

Matrix4f Matrix4f::transposed() const
{
	Matrix4f out;
	for( int i = 0; i < 4; ++i ){
		for( int j = 0; j < 4; ++j ){
			out( j, i ) = ( *this )( i, j );
		}
	}

	return out;
}

void Matrix4f::setRow( int i, const Vector4f& v )
{
	value_list[ i ] = v[0];
	value_list[ i + 4 ] = v[1];
	value_list[ i + 8 ] = v[2];
	value_list[ i + 12 ] =  v[3];
}

void Matrix4f::print()
{
	printf( "[ %.4f %.4f %.4f %.4f ]\n[ %.4f %.4f %.4f %.4f ]\n[ %.4f %.4f %.4f %.4f ]\n[ %.4f %.4f %.4f %.4f ]\n",
		value_list[ 0 ], value_list[ 4 ], value_list[ 8 ], value_list[ 12 ],
		value_list[ 1 ], value_list[ 5 ], value_list[ 9 ], value_list[ 13 ],
		value_list[ 2 ], value_list[ 6 ], value_list[ 10], value_list[ 14 ],
		value_list[ 3 ], value_list[ 7 ], value_list[ 11], value_list[ 15 ] );
}

// -------------------------------------- static
Matrix4f Matrix4f::ones()
{
	Matrix4f m;
	for( int i = 0; i < 16; ++i )
	{
		m.value_list[i] = 1;
	}
	return m;
}

Matrix4f Matrix4f::identity()
{
	Matrix4f m;
	m( 0, 0 ) = 1;
	m( 1, 1 ) = 1;
	m( 2, 2 ) = 1;
	m( 3, 3 ) = 1;
	return m;
}


Matrix4f Matrix4f::translation( float x, float y, float z )
{
	return Matrix4f
	(
		1, 0, 0, x,
		0, 1, 0, y,
		0, 0, 1, z,
		0, 0, 0, 1
	);
}

Matrix4f Matrix4f::translation(const Vector3f& rTranslation)
{
	return Matrix4f
	(
		1, 0, 0, rTranslation[0],
		0, 1, 0, rTranslation[1],
		0, 0, 1, rTranslation[2],
		0, 0, 0, 1
	);
}



Matrix4f Matrix4f::rotateX( float radians )
{
	float c = cos( radians );
	float s = sin( radians );
	return Matrix4f
	(
		1, 0, 0, 0,
		0, c, -s, 0,
		0, s, c, 0,
		0, 0, 0, 1
	);
}

Matrix4f Matrix4f::rotateY( float radians )
{
	float c = cos( radians );
	float s = sin( radians );
	return Matrix4f
	(
		c, 0, s, 0,
		0, 1, 0, 0,
		-s, 0, c, 0,
		0, 0, 0, 1
	);
}

Matrix4f Matrix4f::rotateZ( float radians )
{
	float c = cos( radians );
	float s = sin( radians );
	return Matrix4f
	(
		c, -s, 0, 0,
		s, c, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
}

Matrix4f Matrix4f::rotateXYZ( const Vector3f& rDirection )
{
	return Matrix4f::rotateX(rDirection[0] * M_PI / 180)*Matrix4f::rotateY(rDirection[1] * M_PI / 180)*Matrix4f::rotateZ(rDirection[2] * M_PI / 180);
}


Matrix4f Matrix4f::rotation( const Vector3f& rDirection, float radians )
{
	Vector3f normalized_dir = rDirection.normalized();
	
	float cosTheta = cos(radians);
	float sinTheta = sin(radians);

	float x = normalized_dir[0];
	float y = normalized_dir[1];
	float z = normalized_dir[2];

	return Matrix4f
	(
		x * x * ( 1.0f - cosTheta ) + cosTheta,			y * x * ( 1.0f - cosTheta ) - z * sinTheta,		z * x * ( 1.0f - cosTheta ) + y * sinTheta,		0.0f,
		x * y * ( 1.0f - cosTheta ) + z * sinTheta,		y * y * ( 1.0f - cosTheta ) + cosTheta,			z * y * ( 1.0f - cosTheta ) - x * sinTheta,		0.0f,
		x * z * ( 1.0f - cosTheta ) - y * sinTheta,		y * z * ( 1.0f - cosTheta ) + x * sinTheta,		z * z * ( 1.0f - cosTheta ) + cosTheta,			0.0f,
		0.0f,											0.0f,											0.0f,											1.0f
	);
}

Matrix4f Matrix4f::scaling( float sx, float sy, float sz )
{
	return Matrix4f
	(
		sx, 0, 0, 0,
		0, sy, 0, 0,
		0, 0, sz, 0,
		0, 0, 0, 1
	);
}

Matrix4f Matrix4f::scaleByVector( const Vector3f& scale )
{
	return Matrix4f::scaling(scale[0], scale[1], scale[2]);
}


Matrix4f Matrix4f::uniformScaling( float s )
{
	return Matrix4f
	(
		s, 0, 0, 0,
		0, s, 0, 0,
		0, 0, s, 0,
		0, 0, 0, 1
	);
}

Matrix4f Matrix4f::lookAt( const Vector3f& eye, const Vector3f& center, const Vector3f& up )
{
	// z is negative forward
	Vector3f z = ( eye - center ).normalized();
	Vector3f y = up;
	Vector3f x = Vector3f::cross( y, z );

	// the x, y, and z vectors define the orthonormal coordinate system
	// the affine part defines the overall translation
	Matrix4f view;
	

	view.setRow( 0, Vector4f( x, -Vector3f::dot( x, eye ) ) );
	view.setRow( 1, Vector4f( y, -Vector3f::dot( y, eye ) ) );
	view.setRow( 2, Vector4f( z, -Vector3f::dot( z, eye ) ) );
	view.setRow( 3, Vector4f( 0, 0, 0, 1 ) );

	return view;
}

Matrix4f Matrix4f::orthographicProjection( float left, float right, float bottom, float top, float zNear, float zFar)
{
	return Matrix4f(
		2.0f/(right-left), 0,                  0,                 -( left + right ) / ( right-left ),
		0,                 2.0f/(top-bottom),  0,                 -( top + bottom ) /(top-bottom),
		0,                 0,                  2.0f/(zNear-zFar), -( zNear + zFar )/(zNear-zFar),
		0,                 0,                  0,                 1
	);
}


Matrix4f Matrix4f::perspectiveProjection(float left, float right, float bottom, float top, float zNear, float zFar)
{
	Matrix4f per_to_ortho(
		zNear,  0.0f,  0.0f,       0.0f,
		0.0f,   zNear, 0.0f,       0.0f,
		0.0f,   0.0f,  zNear+zFar, -zFar*zNear,
		0.0f,   0.0f,  1.0f,       0.0f
	);


	return Matrix4f::orthographicProjection(left, right, bottom, top, zNear, zFar)*per_to_ortho;





	/*return Matrix4f(
		2.0f*zNear/(right-left), 0,                       -( right+left ) / ( right-left ), 0,
		0,                       2.0f*zNear/(top-bottom), -( top + bottom ) /(top-bottom),    0,
		0,                       0,                       ( zNear + zFar )/(zFar-zNear),      -2.0f*zFar*zNear/(zFar-zNear),
		0,                       0,                       1,                                  0
	);*/
}

Matrix4f Matrix4f::perspectiveProjection( float fov_y_degree, float aspect_ratio, float zNear, float zFar)
{
	//fov_y_degree：垂直视野，aspect_ratio：宽高比width/height
	float xScale = 1.f / tanf((fov_y_degree * M_PI) / 180.0f / 2.0f);
	float yScale = xScale;
	return Matrix4f(
		xScale/ aspect_ratio, 0,       0,                          0,
		0,      yScale,  0,                          0,
		0,      0,       -(zFar+zNear)/(zFar-zNear), 2.0f*zFar*zNear/(zNear-zFar),
		0,      0,       -1,                         0
	);

	/*Matrix4f m; // zero matrix

	float yScale = 1.f / tanf(0.5f * (fov_y_degree * M_PI) / 180.0f);
	float xScale = yScale / aspect_ratio;

	m(0, 0) = xScale;
	m(1, 1) = yScale;
	m(3, 2) = -1;

	//m( 2, 2 ) = zFar / ( zNear - zFar );
	//m( 2, 3 ) = zNear * zFar / ( zNear - zFar );
	m(2, 2) = (zFar + zNear) / (zNear - zFar);
	m(2, 3) = 2.f * zFar * zNear / (zNear - zFar);
	return m;*/
}



Matrix4f Matrix4f::getCameraViewTransform(const Vector3f& pos, const Vector3f& dir, const Vector3f& up)
{
	// pos: e, dir: g, up: t
	Vector3f w = (-dir).normalized();
	Vector3f u = Vector3f::cross(up.normalized(), w.normalized()).normalized();
	Vector3f v = Vector3f::cross(w.normalized(), u.normalized()).normalized();
	Matrix4f rotation = Matrix4f(
		u[0],  u[1],  u[2],  0,
		v[0],  v[1],  v[2],  0,
		w[0],  w[1],  w[2],  0,
		0,     0,     0,     1
	);

	/*Vector3f g = dir.normalized();
	Vector3f t = up.normalized();
	Vector3f g_cross_t = Vector3f::cross(   t.normalized(), g.normalized()).normalized();
	Matrix4f rotation = Matrix4f(
		g_cross_t[0],  g_cross_t[1],  g_cross_t[2],  0,
		t[0],  t[1],  t[2],  0,
		-g[0], -g[1], -g[2], 0,
		0,     0,     0,     1
	);
	cout<<"rotation"<<endl;
	rotation.print();*/

	Matrix4f location = Matrix4f(
		1, 0, 0, -pos[0],
		0, 1, 0, -pos[1],
		0, 0, 1, -pos[2],
		0, 0, 0, 1
	);
	
	return (rotation * location);
}



Matrix4f Matrix4f::getViewportTransform(int width, int height)
{
	return Matrix4f(
		width/2.0f, 0,           0,   width/2.0f,
		0,          height/2.0f, 0,   height/2.0f,
		0,          0,           1,   0,
		0,          0,           0,   1
	);
}










Vector4f operator * ( const Matrix4f& m, const Vector4f& v )
{
	Vector4f output( 0, 0, 0, 0 );
	for( int i = 0; i < 4; ++i ){
		for( int j = 0; j < 4; ++j ){
			output[ i ] += m( i, j ) * v[ j ];
		}
	}
	return output;
}

Matrix4f operator * ( const Matrix4f& x, const Matrix4f& y )
{
	Matrix4f product;
	for( int i = 0; i < 4; ++i ){
		for( int j = 0; j < 4; ++j ){
			for( int k = 0; k < 4; ++k ){
				product( i, k ) += x( i, j ) * y( j, k );
			}
		}
	}
	return product;
}
