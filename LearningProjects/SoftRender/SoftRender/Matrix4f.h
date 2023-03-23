#pragma once
#include <cstdio>

class Vector3f;
class Vector4f;
class Matrix3f;

class Matrix4f
{
public:
	Matrix4f(float fill = 0);
	Matrix4f(float f00, float f01, float f02, float f03,
			 float f10, float f11, float f12, float f13,
			 float f20, float f21, float f22, float f23,
			 float f30, float f31, float f32, float f33);
	Matrix4f(const Vector4f& v0, const Vector4f& v1, const Vector4f& v2, const Vector4f& v3, bool setColumns = true);
	//取值
	const float& operator () ( int i, int j ) const;
	float& operator () ( int i, int j );

	//逆矩阵
	Matrix4f inverse(bool* pbIsSingular = NULL, float epsilon = 0.f) const;
	//3x3子矩阵
	Matrix3f getSubmatrix3x3(int i0, int j0) const;

	void transpose();
	Matrix4f transposed() const;


	void setRow( int i, const Vector4f& v );

	
	void print();
	
public:
	//
	static Matrix4f ones();
	static Matrix4f identity();

	//移动
	static Matrix4f translation( float x, float y, float z );
	static Matrix4f translation( const Vector3f& rTranslation );
	//旋转
	static Matrix4f rotateX( float radians );
	static Matrix4f rotateY( float radians );
	static Matrix4f rotateZ( float radians );
	static Matrix4f rotateXYZ( const Vector3f& rDirection );
	static Matrix4f rotation( const Vector3f& rDirection, float radians );
	//缩放
	static Matrix4f scaling( float sx, float sy, float sz );
	static Matrix4f scaleByVector( const Vector3f& scale );
	static Matrix4f uniformScaling( float s );
	//
	static Matrix4f lookAt( const Vector3f& eye, const Vector3f& center, const Vector3f& up );
	//正交投影
	static Matrix4f orthographicProjection( float left, float right, float bottom, float top, float zNear, float zFar);
	//透视投影
	static Matrix4f perspectiveProjection( float left, float right, float bottom, float top, float zNear, float zFar);
	static Matrix4f perspectiveProjection( float fovYRadians, float aspect, float zNear, float zFar);
	//View相机：相机移到中心点，且方向朝向-z
	static Matrix4f getCameraViewTransform(const Vector3f& pos, const Vector3f& dir, const Vector3f& up);
	//Viewport转换矩阵
	static Matrix4f getViewportTransform(int width, int height);

public:
	float value_list[16];
};

//-------------------- 计算 --------------------
// 矩阵*向量: 4x4 * 4x1 ==> 4x1
Vector4f operator * ( const Matrix4f& m, const Vector4f& v );
// 矩阵*矩阵
Matrix4f operator * ( const Matrix4f& x, const Matrix4f& y );