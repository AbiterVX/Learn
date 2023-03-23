#include "Matrix3f.h"


Matrix3f::Matrix3f(float fill)
{
	for (int i = 0; i < 9; ++i)
	{
		value_list[i] = fill;
	}
}

Matrix3f::Matrix3f(float f00, float f01, float f02,
	float f10, float f11, float f12,
	float f20, float f21, float f22)
{
	value_list[0] = f00;
	value_list[1] = f10;
	value_list[2] = f20;

	value_list[3] = f01;
	value_list[4] = f11;
	value_list[5] = f21;

	value_list[6] = f02;
	value_list[7] = f12;
	value_list[8] = f22;
}

const float& Matrix3f::operator () (int i, int j) const
{
	return value_list[j * 3 + i];
}
float& Matrix3f::operator () ( int i, int j )
{
	return value_list[ j * 3 + i ];
}

float Matrix3f::determinant3x3(float m00, float m01, float m02,
							   float m10, float m11, float m12,
							   float m20, float m21, float m22 )
{
	return
		(
			  m00 * ( m11 * m22 - m12 * m21 )
			- m01 * ( m10 * m22 - m12 * m20 )
			+ m02 * ( m10 * m21 - m11 * m20 )
		);
}

