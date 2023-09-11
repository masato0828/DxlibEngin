#pragma once
#include <Dxlib.h>
#include "Vector2.h"

// 3D座標,ベクトルの構造体
struct Vector3
{
	float x_, y_, z_;
	Vector3() :x_(0), y_(0), z_(0) {};
	Vector3(float x, float y, float z) :x_(x), y_(y), z_(z) {};

	bool operator==(const Vector3& v);
	bool operator!=(const Vector3& v);
	bool operator>=(const Vector3& v);
	bool operator>(const Vector3& v);
	bool operator<=(const Vector3& v);
	bool operator<(const Vector3& v);
	void operator+=(const Vector3& v);
	void operator-=(const Vector3& v);
	void operator*=(const Vector3& v);
	void operator/=(const Vector3& v);
	void operator*=(float scale);
	void operator/=(float scale);
	bool operator>=(float scale);
	bool operator>(float scale);
	bool operator<=(float scale);
	bool operator<(float scale);
	

	Vector3 operator+(const Vector3& v);
	Vector3 operator-(const Vector3& v);
	Vector3 operator*(const Vector3& v);
	Vector3 operator/(const Vector3& v);
	Vector3 operator+(float scale);

	Vector3 operator+(const VECTOR& v);
	Vector3 operator=(const VECTOR& v);

	VECTOR toVECTOR();
	void setVECTOR(float x, float y, float z);
	void setVECTOR(VECTOR vec);
};