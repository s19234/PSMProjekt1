#ifndef __STRUKTURY_H
#define __STRUKTURY_H

#include <cmath>

class Wektor
{
public:
	Wektor(double _x = 0, double _y = 0, double _z = 0)
		: x(_x), y(_y), z(_z)
	{}

	
	Wektor operator+(Wektor const&);
	Wektor operator-(Wektor const&);
	double operator*(Wektor const&);
	Wektor operator*(double);
	
	double length();
	void normalize();
	void setX(double);
	void setY(double);
	void setZ(double);

	double getX() const
	{
		return x;
	}

	double getY() const
	{
		return y;
	}

	double getZ() const
	{
		return z;
	}

private:
	double x, y, z;
};

Wektor operator*(double, Wektor const&);

Wektor Wektor::operator-(Wektor const& w)
{
	return (x - w.x, y - w.y, z - w.z);
}

double Wektor::operator*(Wektor const& w)
{
	return (x * w.x + y * w.y + z * w.z);
}

double Wektor::length()
{
	return std::sqrt((*this) * (*this));
}

Wektor Wektor::operator+(Wektor const& w)
{
	return (x + w.x, y + w.y, z + w.z);
}

Wektor Wektor::operator*(double l)
{
	return Wektor(x * l, y * l, z * l);
}

Wektor operator*(double s, Wektor const& w)
{
	return Wektor(s * w.getX(), s * w.getY(), s * w.getZ());
}

void Wektor::normalize()
{
	double d = length();
	if (d)
		(*this) = (*this) * (1 / d);
}

void Wektor::setX(double x)
{
	this->x = x;
}

void Wektor::setY(double y)
{
	this->y = y;
}

void Wektor::setZ(double z)
{
	this->z = z;
}

struct Punkt
{
	int flag : 1;
	float m;
	
	Wektor f;
	Wektor r;
	Wektor v;

	Wektor dr;
	Wektor dv;

	Punkt* right;
};

#endif