#ifndef SURFACE_SPHERE_H
#define SURFACE_SPHERE_H


#include "Surface.h"

template<typename ftype>
class Sphere : public Geometry::Sphere<ftype>, public Surface<ftype>
{
public:
	typedef Maths::Vector<ftype, 3> fvector;
	typedef Geometry::Space<ftype, 1, 3> linef;
	typedef Geometry::AxisAlignedBoundingBox<ftype, 3> aabbf;
	typedef Geometry::Sphere<ftype> spheref;

	typedef Surface<ftype> Parent;
public:
	Sphere() = delete;

	Sphere(
		const ftype radius, 
		const fvector& position, 
		const MaterialComponent<ftype>* const material):

		spheref(radius, position),
		Parent(make_aabb(), make_bounding_sphere(), material)
		{
		}

	Sphere(
		const spheref& sphere,
		const MaterialComponent<ftype>* const material):

		spheref(sphere),
		Parent(make_aabb(), make_bounding_sphere(), material){}

	virtual const aabbf make_aabb() const override 
	{ 
		return aabbf(spheref::get_radius() + spheref::get_center(),
			spheref::get_center() - spheref::get_radius());
	}

	virtual const spheref make_bounding_sphere() const override 
	{ 
		return spheref(spheref::get_radius(), spheref::get_center()); 
	}

	virtual const ftype first_intersection(const linef& ray)const override
	{
		return Geometry::intersection(static_cast<spheref>(*this), ray, Surface<ftype>::tolerance);
	}

	virtual const fvector normal(const fvector& point)const override
	{
		return Maths::unit(point - spheref::get_center());
	}


	virtual const Maths::Vector<ftype, 2> get_local_coordinates(const fvector& position)const override
	{
		//long and lat coordinates -  find the angle the displacement makes with the x&z axes
		const fvector v = position - spheref::get_center();
		//as these are axis aligned, we can use the x, y,z values.
		const ftype z_angle = acos(v.z / (v.x * v.x + v.y * v.y));
		const ftype x_angle = Maths::true_angle(v.x, v.y);
		return Maths::Vector<ftype, 2>(x_angle, z_angle);
	}
};


#endif // !SURFACE_SPHERE_H
