#pragma once
#include <cmath>

using int8			= __int8;
using int16			= __int16;
using int32			= __int32;
using int64			= __int64;
using uint8			= unsigned __int8;
using uint16		= unsigned __int16;
using uint32		= unsigned __int32;
using uint64		= unsigned __int64;

struct VectorInt
{
public:
	VectorInt() : x(0), y(0) {}
	VectorInt(int x, int y) : x(x), y(y) {}
	~VectorInt() = default;
	VectorInt(const VectorInt& other) = default;
	
	VectorInt& operator=(const VectorInt& other) = default;

	VectorInt operator+(const VectorInt& other) const
	{
		VectorInt ret(x, y);
		ret.x += other.x;
		ret.y += other.y;
		return ret;
	}
	VectorInt operator-(const VectorInt& other) const
	{
		VectorInt ret(x, y);
		ret.x -= other.x;
		ret.y -= other.y;
		return ret;
	}
	void operator+=(const VectorInt& other)
	{
		x += other.x;
		y += other.y;
	}
	void operator-=(const VectorInt& other)
	{
		x -= other.x;
		y -= other.y;
	}
	VectorInt operator*(const int32 f) const
	{
		VectorInt ret(*this);
		ret.x *= f;
		ret.y *= f;
		return ret;
	}
	void operator*=(const int32 f)
	{
		x *= f;
		y *= f;
	}
	bool operator==(const VectorInt& other) const
	{
		return x == other.x && y == other.y;
	}
	bool operator!=(const VectorInt& other) const
	{
		return x != other.x || y != other.y;
	}
	bool operator<=(const VectorInt& other) const
	{
		return x <= other.x && y <= other.y;
	}
	bool operator>=(const VectorInt& other) const
	{
		return x >= other.x && y >= other.y;
	}
	float LengthSquared()
	{
		return static_cast<float>(x * x + y * y);
	}

	int32 dot(const VectorInt& other) const
	{
		return x * other.x + y * other.y;
	}
	int32 ManhattanDist(const VectorInt& other) const
	{
		return abs(other.x - x) + abs(other.y - y);
	}
	
	

	int32 x = 0;
	int32 y = 0;
};




struct Vector
{
public:
	Vector() : x(0.f), y(0.f) {}
	Vector(float x, float y) : x(x), y(y) {}
	~Vector() = default;
	Vector(const Vector& other) = default;
	Vector& operator=(const Vector& other) = default;

	Vector operator+(const Vector& other) const
	{
		Vector ret(x, y);
		ret.x += other.x;
		ret.y += other.y;
		return ret;
	}
	Vector operator-(const Vector& other) const
	{
		Vector ret(x, y);
		ret.x -= other.x;
		ret.y -= other.y;
		return ret;
	}
	void operator+=(const Vector& other)
	{
		x += other.x;
		y += other.y;
	}
	void operator-=(const Vector& other)
	{
		x -= other.x;
		y -= other.x;
	}
	Vector operator*(const float f) const
	{
		Vector ret(*this);
		ret.x *= f;
		ret.y *= f;
		return ret;
	}
	void operator*=(const float f)
	{
		x *= f;
		y *= f;
	}
	bool operator==(const Vector& other) const
	{
		return x == other.x && y == other.y;
	}
	bool operator!=(const Vector& other) const
	{
		return x != other.x || y != other.y;
	}
	bool operator<(const Vector& other) const
	{
		return x == other.x ? y < other.y : x < other.x;
	}
	VectorInt Cast(Vector& other) const
	{
		VectorInt v;
		v.x = static_cast<int32>(other.x);
		v.y = static_cast<int32>(other.y);
		return v;
	}

	float dot(const Vector& other) const
	{
		return x * other.x + y * other.y;
	}


	float x = 0.f;
	float y = 0.f;

};