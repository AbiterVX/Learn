#pragma once
#include <immintrin.h> 

class Vector2f;


class Vector3f
{
public:
    static const Vector3f ZERO;
    static const Vector3f ONE;
	static const Vector3f UP;
	static const Vector3f RIGHT;
	static const Vector3f FORWARD;


    Vector3f( float f = 0.f );
    Vector3f( float x, float y, float z );
    Vector3f(__m128 m) : mmvalue(m) {};
    float& x();
	float& y();
	float& z();
    
    float x() const;
	float y() const;
	float z() const;
    Vector2f xy() const;


    const float& operator [] ( int i ) const;
    float& operator [] ( int i );

    float abs() const;

    void normalize();
	Vector3f normalized() const;


    Vector3f& operator += ( const Vector3f& v );
	Vector3f& operator -= ( const Vector3f& v );
    inline Vector3f& operator *= ( float f ) {
        mmvalue = _mm_mul_ps(mmvalue, _mm_set1_ps(f));
        return *this;
        /*value_list[ 0 ] *= f;
        value_list[ 1 ] *= f;
        value_list[ 2 ] *= f;*/
    }
    inline Vector3f& operator *= ( const Vector3f& v ) {
        mmvalue = _mm_mul_ps(mmvalue, v.mmvalue);
        return *this;
        /*value_list[ 0 ] *= v.value_list[ 0 ];
        value_list[ 1 ] *= v.value_list[ 1 ];
        value_list[ 2 ] *= v.value_list[ 2 ];
        return *this;*/
    }
    
    Vector3f& operator /= ( float f  )
    {
        mmvalue = _mm_div_ps(mmvalue, _mm_set1_ps(f)); 
        return *this;
    }
    Vector3f& operator /= ( const Vector3f& v  )
    {
        mmvalue = _mm_div_ps(mmvalue, v.mmvalue); 
        return *this;
    }

    static inline float dot( const Vector3f& v0, const Vector3f& v1 ){
        return _mm_cvtss_f32(_mm_dp_ps(v0.mmvalue, v1.mmvalue, 0x71));
        //return v0[0] * v1[0] + v0[1] * v1[1] + v0[2] * v1[2];
    }
	static inline Vector3f cross( const Vector3f& v0, const Vector3f& v1 ){
        /*__m128 m128_v1 = _mm_load_ps(v0.value_list);
        __m128 m128_v2 = _mm_load_ps(v1.value_list);*/
        return _mm_sub_ps(
            _mm_mul_ps(_mm_shuffle_ps(v0.mmvalue, v0.mmvalue, _MM_SHUFFLE(3, 0, 2, 1)), _mm_shuffle_ps(v1.mmvalue, v1.mmvalue, _MM_SHUFFLE(3, 1, 0, 2))),
            _mm_mul_ps(_mm_shuffle_ps(v0.mmvalue, v0.mmvalue, _MM_SHUFFLE(3, 1, 0, 2)), _mm_shuffle_ps(v1.mmvalue, v1.mmvalue, _MM_SHUFFLE(3, 0, 2, 1)))
        );
        /*return Vector3f
            (
                v0[1] * v1[2] - v0[2] * v1[1],
                v0[2] * v1[0] - v0[0] * v1[2],
                v0[0] * v1[1] - v0[1] * v1[0]
            );*/
    }


    inline static Vector3f addVectorByBarycentricCoord(const Vector3f& v1, float weight1, const Vector3f& v2, float weight2, const Vector3f& v3, float weight3){
		return _mm_add_ps(_mm_add_ps(_mm_mul_ps(v1.mmvalue, _mm_set1_ps(weight1)), _mm_mul_ps(v2.mmvalue, _mm_set1_ps(weight2))), _mm_mul_ps(v3.mmvalue, _mm_set1_ps(weight3)));
	}
    void print() const;	
public:
    union
    {
        float value_list[3];
        __m128 mmvalue;
    };
};


//-------------------- 计算 --------------------
Vector3f operator + ( const Vector3f& v0, const Vector3f& v1 );
Vector3f operator - ( const Vector3f& v0, const Vector3f& v1 );
Vector3f operator * ( const Vector3f& v0, const Vector3f& v1 );
Vector3f operator / ( const Vector3f& v0, const Vector3f& v1 );

Vector3f operator - ( const Vector3f& v );

Vector3f operator * ( float f, const Vector3f& v );
Vector3f operator * ( const Vector3f& v, float f );
Vector3f operator / ( const Vector3f& v, float f );

bool operator == ( const Vector3f& v0, const Vector3f& v1 );
bool operator != ( const Vector3f& v0, const Vector3f& v1 );