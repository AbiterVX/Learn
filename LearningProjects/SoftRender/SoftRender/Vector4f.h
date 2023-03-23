#pragma once
#include <immintrin.h> 
#include "Vector3f.h"
class Vector2f;

class Vector4f
{
public:
	Vector4f(float new_x =0, float y=0, float z = 0, float w = 0);
	Vector4f( const Vector3f& xyz, float w );
	Vector4f( float x, const Vector3f& yzw );
	Vector4f(__m128 m) : mmvalue(m) {};
	// 赋值
	Vector4f& operator = (const Vector4f& other_vector);
	// 取值
	const float& operator [] ( int i ) const;
	float& operator [] ( int i );

	Vector3f xyz() const{
		return {value_list[0], value_list[1], value_list[2]};
	}
	Vector2f xy() const;
	//获取长度
	float getLength() const;
	//float lengthSquard() const;
	//得到归一化
	Vector4f normalized() const;
	
	void print() const; 

	Vector4f& operator += ( const Vector4f& v );
	Vector4f& operator -= ( const Vector4f& v );
    Vector4f& operator *= ( float f );

	inline void addVector(const Vector4f& v, float weight){
		mmvalue = _mm_add_ps(mmvalue, _mm_mul_ps(v.mmvalue, _mm_set1_ps(weight)));
		
		/*value_list[ 0 ] += v.value_list[ 0 ] * weight;
		value_list[ 1 ] += v.value_list[ 1 ] * weight;
		value_list[ 2 ] += v.value_list[ 2 ] * weight;
		value_list[ 3 ] += v.value_list[ 3 ] * weight;*/
	};
	inline static Vector4f addVectorByBarycentricCoord(const Vector4f& v1, float weight1, const Vector4f& v2, float weight2, const Vector4f& v3, float weight3){
		return _mm_add_ps(_mm_add_ps(_mm_mul_ps(v1.mmvalue, _mm_set1_ps(weight1)), _mm_mul_ps(v2.mmvalue, _mm_set1_ps(weight2))), _mm_mul_ps(v3.mmvalue, _mm_set1_ps(weight3)));
	}

public:
	// 数据
	union
    {
        float value_list[4];
        __m128 mmvalue;
	};
};


//-------------------- 计算 --------------------
Vector4f operator + (const Vector4f& v0, const Vector4f& v1);
Vector4f operator - (const Vector4f& v0, const Vector4f& v1);
Vector4f operator * (const Vector4f& v0, const Vector4f& v1);
Vector4f operator / (const Vector4f& v0, const Vector4f& v1);

Vector4f operator - (const Vector4f& v);

Vector4f operator * (float f, const Vector4f& v);
Vector4f operator * (const Vector4f& v, float f);
Vector4f operator / (const Vector4f& v, float f);

bool operator == (const Vector4f& v0, const Vector4f& v1);
bool operator != (const Vector4f& v0, const Vector4f& v1);
