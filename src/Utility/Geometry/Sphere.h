#ifndef GEOMETRY_SPHERE_H
#define GEOMETRY_SPHERE_H

#include "Maths/Vector.h"
#include "AxisAlignedBoundingBox.h"

#include <iostream>

namespace Geometry
{
	template<typename ftype>
	class Sphere
	{
		typedef Maths::Vector<ftype, 3> fvector;
	private:
		ftype m_radius;
		ftype m_r2;
		fvector m_center;
	public:
		Sphere() {}

		Sphere(const ftype radius, const fvector& position) : m_radius(radius), m_center(position), m_r2(radius*radius) {}

		Sphere(const Sphere& other): m_radius(other.m_radius), m_r2(other.m_r2), m_center(other.m_center){}

		Sphere& operator=(const Sphere& other)
		{
			m_radius = other.m_radius;
			m_r2 = other.m_r2;
			m_center = other.m_center;
			return *this;
		}

		~Sphere(){}

		const ftype& get_radius()const { return m_radius; }
		const ftype& get_r2()const { return m_r2; }
		const fvector& get_center()const { return m_center; }

		const AxisAlignedBoundingBox<ftype, 3> make_aabb()
		{
			return AxisAlignedBoundingBox(m_center - m_radius, m_center + m_radius);
		}

	};
}

template<typename ftype>
std::ostream& operator<<(std::ostream& out, const Geometry::Sphere<ftype>& s)
{
	out << "Sphere: radius " << s.get_radius() << ", center: " << s.get_center();
	return out;
}

#endif 