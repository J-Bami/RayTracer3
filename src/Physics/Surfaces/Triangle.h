#ifndef SURFACE_TRIANGLE_H
#define SURFACE_TRIANGLE_H


#include "Maths/MathUtil.h"
#include "Surface.h"
#include "Geometry/Triangle.h"

template<typename ftype>
class Triangle: public Geometry::Triangle<ftype, 3>, public Surface<ftype>
{
public:
	typedef Maths::Vector<ftype, 3> fvector;
	typedef Geometry::Space<ftype, 1, 3> linef;
	typedef Geometry::AxisAlignedBoundingBox<ftype, 3> aabbf;
	typedef Geometry::Sphere<ftype> spheref;
	typedef Geometry::Triangle<ftype, 3> trianglef;
	typedef Surface<ftype> Parent;
private:
	fvector m_normal;
public:
	Triangle() = delete;

	Triangle(
		const fvector& p1,
		const fvector& p2,
		const fvector& p3,
		const MaterialComponent<ftype>* const material):

		trianglef(p1, p2, p3),
		Parent(make_aabb(), make_bounding_sphere(), material)
		{
			m_normal = Maths::unit(Maths::cross(p1 - p2, p1-p3));
		}


	Triangle(const trianglef& triangle, const MaterialComponent<ftype>* const material) : 
	trianglef(triangle),
	Parent(make_aabb(), make_bounding_sphere(), material)
	{
		m_normal = Maths::cross(m_triangle.get_vertex(0) - m_triangle.get_vertex(1), m_triangle.get_vertex(0)- m_triangle.get_vertex(2));
	}

	Triangle(const Triangle& other) = delete;

	~Triangle(){}

	virtual const aabbf make_aabb() const override 
	{
		const trianglef tri = static_cast<const trianglef>(*this);
		return tri.make_aabb(); 
	}

	virtual const spheref make_bounding_sphere() const override 
	{ 
		const trianglef tri = static_cast<const trianglef>(*this);
		return tri.make_bounding_sphere();
	}

	virtual const ftype first_intersection(const linef& ray)const override
	{
		return Geometry::intersection(static_cast<const trianglef>(*this), ray);
	};

	virtual const fvector normal(const fvector& point)const override
	{ 
		return m_normal;
	}

	virtual const Maths::Vector<ftype, 2> get_local_coordinates(const fvector& point)const
	{
		const fvector dir1 = Maths::unit(trianglef::get_center() - get_vertex(0));
		const fvector dir2 = Maths::cross(dir1, m_normal);

		return Maths::Vector<ftype, 2>(Maths::dot(dir1, point), Maths::dot(dir2, point));

	}
};




#endif