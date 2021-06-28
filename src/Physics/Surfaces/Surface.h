#ifndef SURFACE_H
#define SURFACE_H

#include "Maths/Linalg.h"
#include "Geometry/Intersection.h"
#include "Containers/Set.h"
#include "MaterialComponents/MaterialComponent.h"

#include <iostream>
/*
* 
* The base class that defines all the virtual functions that Surface types should possess;
* these include:
* 
*  First intersection, for a ray
*  an AABB maker which we can call 
*  a bounding sphere
*  a spherical bound when given a point and direction
*  a surface normal when given a point
*  a material when given a position that lies on the surface;
*/

//#define CULL_SURFACES

template <typename ftype>
class Surface
{
public:
	typedef Maths::Vector<ftype, 3> fvector;
	typedef Optics::SpectrumArray<ftype> sarray;
	typedef Optics::SpectrumArray<ftype&> rsarray;
	typedef Geometry::Space<ftype, 1, 3> linef;
	typedef Geometry::AxisAlignedBoundingBox<ftype, 3> aabbf;
	typedef Geometry::Sphere<ftype> spheref;


	struct SurfaceInfo
	{
		const Surface* m_surface;
		Geometry::AxisAlignedBoundingBox<ftype, 3> m_aabb;
		Geometry::Sphere<ftype> m_sphere;

		SurfaceInfo(): m_surface(nullptr), m_aabb(), m_sphere() {}

		SurfaceInfo(const Surface *const surface) : m_surface(surface) {}

		SurfaceInfo(const Surface *const surface, const Geometry::AxisAlignedBoundingBox<ftype, 3>& aabb, const Geometry::Sphere<ftype>& sphere) :
			m_surface(surface),
			m_aabb(aabb),
			m_sphere(sphere)
		{}

		SurfaceInfo& operator=(const SurfaceInfo& other)
		{
			m_surface = other.m_surface;
			m_aabb = other.m_aabb;
			m_sphere = other.m_sphere;
			return *this;
		}

		~SurfaceInfo() {}

		//test for equivaluence
		friend bool operator==(const SurfaceInfo& surface, const SurfaceInfo& other)
		{
			return other.m_surface == surface.m_surface;
		}
	};

	static Set<SurfaceInfo> manager;
	static Set<Surface*> all_surfaces; //might be extra but *shrug*
	static constexpr ftype tolerance = 1e-4;  //1/10000
	static constexpr ftype rtolerance = (1 - tolerance);
private:
	const MaterialComponent<ftype> *const m_material;
public:
	Surface() = delete;

	Surface(
		const aabbf aabb,
		const spheref sphere,
		const MaterialComponent<ftype>* material):
		m_material(material) 
	{
		const SurfaceInfo info(this, aabb, sphere);
		manager.add(info);
		all_surfaces.add(this);
	}

	Surface(const Surface& other) = delete;

	~Surface()
	{
		const SurfaceInfo info(this);
		manager.remove(info);
		all_surfaces.remove(this);
	}

	const MaterialComponent<ftype>* get_material_component()const //cannot be const because it changes m_material.
	{ 
		return m_material;
	}

	virtual const Maths::Vector<ftype, 2> get_local_coordinates(const fvector&)const = 0;
	
	virtual const aabbf make_aabb() const = 0;

	virtual const spheref make_bounding_sphere() const = 0;  //will be useful for the camera culling

	virtual const ftype first_intersection(const linef& ray)const = 0;

	virtual const fvector normal(const fvector& point)const = 0;

	inline static SurfaceInfo* get_surface_infos()
	{
		return manager.get_objects();
	}

	inline static const size_t &surface_count()
	{
		return manager.get_size();
	}

	//given an aabb, we only add surfaces whose aabbs intersect with the input culling box.
	static const Set<Surface*> surface_cull(const aabbf& culling_box)
	{
#ifdef CULL_SURFACES
		Set<Surface*> out;
		const size_t n = surface_count();
		out.reserve(n); // to save allocation time, perhaps? we can try without this.

		for (size_t i = 0; i < n; i++)
		{
			const SurfaceInfo info = manager[i];
			if (Geometry::intersection(culling_box, info.m_aabb, true))
			{
				const Surface* const ptr = info.m_surface;
				out.add(ptr);
			}
	}
		return out;
#else
		return all_surfaces;
#endif 
	}

	static const Set<Surface*> surface_cull(const fvector& position, const fvector& direction, const float angle)
	{
#ifdef CULL_SURFACES
		Set<Surface*> out;
		//needs modification
		return out;
#else
		return all_surfaces;
#endif 

	}

	inline static const Set<Surface*> get_all_surfaces()
	{
		return all_surfaces;
	}
};

//initialise the manager
template<typename ftype>
Set<typename Surface<ftype>::SurfaceInfo> Surface<ftype>::manager;

template<typename ftype>
Set<Surface<ftype>*> Surface<ftype>::all_surfaces;

template<typename ftype>
std::ostream& operator<<(std::ostream& out, const typename Surface<ftype>::SurfaceInfo& s)
{
	out << "SurfaceInfo Struct: surface address: 0x" << s.m_surface << " aabb: " << s.m_aabb
	<< " bounding sphere: " << s.m_sphere;
	return out;
}

#endif