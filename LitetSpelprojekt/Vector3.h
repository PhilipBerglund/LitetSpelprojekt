#pragma once
#include <cmath>

class Vector3
{
private:
	double x, y, z;
public:
	Vector3() 
		:x(0.0), y(0.0), z(0.0)
	{}

	Vector3(const double& x, const double& y, const double& z)
		: x(x), y(y), z(z)
	{}

	Vector3(const Vector3& vector)
	{
		this->x = vector.GetX();
		this->y = vector.GetY();
		this->z = vector.GetZ();
	}

	Vector3 operator+(const Vector3& vector)
	{
		return Vector3(this->x + vector.GetX(), this->y + vector.GetY(), this->z + vector.GetZ());
	}

	Vector3 operator-(const Vector3& vector)
	{
		return Vector3(this->x - vector.GetX(), this->y - vector.GetY(), this->z - vector.GetZ());
	}

	Vector3 operator*(const double& value)
	{
		return Vector3(this->x * value, this->y * value, this->z * value);
	}

	Vector3 operator/(const double& value)
	{
		Vector3 return_vector;

		if (value != 0.0)
			return_vector = Vector3(this->x / value, this->y / value, this->z / value);

		return return_vector;
	}

	Vector3 operator=(const Vector3& vector)
	{
		this->x = vector.GetX();
		this->y = vector.GetY();
		this->z = vector.GetZ();
		return *this;
	}

	double DotProduct(const Vector3& vector)
	{
		return (this->x * vector.GetX() + this->y * vector.GetY() + this->z * vector.GetZ());
	}

	Vector3 CrossProduct(const Vector3& vector)
	{
		double new_x = this->y * vector.GetZ() - this->z * vector.GetY();
		double new_y = this->z * vector.GetX() - this->x * vector.GetZ();
		double new_z = this->x * vector.GetY() - this->y * vector.GetX();

		return Vector3(new_x, new_y, new_z);
	}

	double Magnitude()
	{
		return sqrt(Square());
	}

	double Square()
	{
		return this->x * this->x + this->y * this->y + this->z * this->z;
	}

	Vector3 Normalize()
	{
		Vector3 return_vector;

		if (Magnitude() != 0)
			return_vector = *this * (1 / Magnitude());

		return return_vector;
	}

	double GetX() const
	{
		return this->x;
	}

	double GetY() const
	{
		return this->y;
	}

	double GetZ() const
	{
		return this->z;
	}
};