#pragma once
#include <immintrin.h> 

class Vector3f;


class Vector2f
{
public:
	static const Vector2f ZERO;
	static const Vector2f UP;
	static const Vector2f RIGHT;


	Vector2f(float f = 0.f);
	Vector2f(float x, float y);
	Vector2f(__m128 m) : mmvalue(m) {};
	// copy constructors
	Vector2f(const Vector2f& rv);

	// assignment operators
	Vector2f& operator = (const Vector2f& rv);

	const float& operator [] (int i) const;
	float& operator [] (int i);
	float& x();
	float& y();
	float x() const;
	float y() const;
	Vector2f& operator += (const Vector2f& v);
	Vector2f& operator -= (const Vector2f& v);
	Vector2f& operator *= (float f);

	// 法向量：returns ( -y, x )
	Vector2f normal() const;
    float abs() const;
	float absSquared() const;
	void normalize();
    Vector2f normalized() const;
	
	static float dot(const Vector2f& v0, const Vector2f& v1);
	static Vector3f cross(const Vector2f& v0, const Vector2f& v1);

	inline void addVector(const Vector2f& v, float weight){
		mmvalue = _mm_add_ps(mmvalue, _mm_mul_ps(v.mmvalue, _mm_set1_ps(weight)));
		//value_list[0] += v.value_list[0] * weight;
		//value_list[1] += v.value_list[1] * weight;
	};

	inline static Vector2f addVectorByBarycentricCoord(const Vector2f& v1, float weight1, const Vector2f& v2, float weight2, const Vector2f& v3, float weight3){
		return _mm_add_ps(_mm_add_ps(_mm_mul_ps(v1.mmvalue, _mm_set1_ps(weight1)), _mm_mul_ps(v2.mmvalue, _mm_set1_ps(weight2))), _mm_mul_ps(v3.mmvalue, _mm_set1_ps(weight3)));
	}
public:
	// 数据
	union
    {
        float value_list[2];
        __m128 mmvalue;
	};
};

//-------------------- 计算 --------------------
Vector2f operator + (const Vector2f& v0, const Vector2f& v1);
Vector2f operator - (const Vector2f& v0, const Vector2f& v1);
Vector2f operator * (const Vector2f& v0, const Vector2f& v1);
Vector2f operator / (const Vector2f& v0, const Vector2f& v1);

Vector2f operator - (const Vector2f& v);

Vector2f operator * (float f, const Vector2f& v);
Vector2f operator * (const Vector2f& v, float f);
Vector2f operator / (const Vector2f& v, float f);

bool operator == (const Vector2f& v0, const Vector2f& v1);
bool operator != (const Vector2f& v0, const Vector2f& v1);

