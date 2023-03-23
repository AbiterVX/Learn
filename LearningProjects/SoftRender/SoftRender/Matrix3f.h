#pragma once
class Matrix3f
{
public:
	Matrix3f(float fill = 0.f);
	Matrix3f(float f00, float f01, float f02,
		     float f10, float f11, float f12,
		     float f20, float f21, float f22);


	const float& operator () (int i, int j) const;
	float& operator () ( int i, int j );
	
	// 行列式
	static float determinant3x3(float m00, float m01, float m02,
								float m10, float m11, float m12,
								float m20, float m21, float m22 );

public:
	float value_list[9];
};

