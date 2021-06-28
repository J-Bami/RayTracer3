#ifndef SURFACE_PLANE_H
#define SURFACE_PLANE_H


#include "Surface.h"

template<typename ftype>
class Plane : public Geometry::Space<ftype, 2, 3>, public Surface<ftype>
{
	typedef Maths::Vector<ftype, 3> fvector;
	typedef Geometry::Space<ftype, 1, 3> linef;
	typedef Geometry::AxisAlignedBoundingBox<ftype, 3> aabbf;
	typedef Geometry::Space<ftype, 2, 3> planef;

	typedef Surface<ftype> Parent;
	typedef Geometry::Space<ftype, 2, 3> GPlane;
private:
	fvector m_normal;
public:
	Plane(
		const fvector& origin,
		const fvector& axis1,
		const fvector& axis2,
		const MaterialComponent<ftype>* material) :
		GPlane(origin, { axis1, axis2 }),
		Parent(make_aabb(), make_bounding_sphere(), material)
	{
		m_normal = Maths::cross(GPlane::get_axis(0), GPlane::get_axis(1));
	}

	Plane(
		const planef& plane,
		const MaterialComponent<ftype>* const material) : 
		GPlane(plane),
		Parent(make_aabb(), make_bounding_sphere(), material)
	{
		m_normal = Maths::cross(plane.get_axis(0), plane.get_axis(1));
	}

	Plane(const Plane& other) = delete;

	virtual const Maths::Vector<ftype, 2> get_local_coordinates(const fvector& position)const override
	{
		return planef::project(position);
	}

	virtual const aabbf make_aabb() const override
	{
		return aabbf({-INFINITY, -INFINITY, -INFINITY}, {INFINITY, INFINITY, INFINITY});
	}

	virtual const spheref make_bounding_sphere() const override
	{
		return spheref(INFINITY, GPlane::get_origin());
	}

	virtual const ftype first_intersection(const linef& ray)const override
	{
		return Geometry::intersection(static_cast<GPlane>(*this), ray);
	};

	virtual const fvector normal(const fvector& point)const { return m_normal; };
};



#endif // !SURFACE_PLANE_H
