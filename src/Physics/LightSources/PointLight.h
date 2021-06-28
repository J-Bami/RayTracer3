#ifndef POINT_LIGHT_SOURCE_H
#define POINT_LIGHT_SOURCE_H

#include "LightSource.h"

template <typename ftype>
class PointLight : public LightSource<ftype>
{
	typedef Maths::Vector<ftype, 3> fvector;
	typedef Optics::SpectrumArray<ftype> sarray;
	typedef Geometry::AxisAlignedBoundingBox<ftype, 3> aabb3;
	typedef Geometry::Space<ftype, 1, 3> linef;
private:
	sarray m_luminosity;
	fvector m_position;

public:

	PointLight() = delete;

	PointLight(const fvector& position, const sarray& luminosity): m_position(position), m_luminosity(luminosity) {}

	PointLight(const fvector& position, const ftype luminosity) : m_position(position), m_luminosity(luminosity){}

	virtual const fvector get_effective_direction(const fvector& position)const override 
	{
		return m_position - position;
	};

	//virtual const fvector get_effective_position(const fvector&)const override{return m_position;}

	virtual const sarray get_intensity(const fvector& point)const override
	{
		return m_luminosity / Maths::mag2(point - m_position);
	}

	//virtual const sarray get_luminosity()const override { return m_luminosity; };

	virtual ftype illumination(const fvector& point) const override
    {   
		const ftype distance2 = Maths::mag2(point - m_position) * Surface<ftype>::rtolerance *Surface<ftype>::rtolerance;
		//make the ray
		const linef ray(point, get_effective_direction(point));

        //make an aabb representing the ray...
        const aabb3 culling_box(point, m_position);

		//get the ptrs to only the surfaces that intersect the aabb
		const Set<Surface<ftype>*> surfaces = Surface<ftype>::surface_cull(culling_box);

		const size_t n = surfaces.get_size();
        for (size_t i = 0; i < n; i++)
        {
			const Surface<ftype>* surface = surfaces[i];
			const ftype dist = surface->first_intersection(ray);
            if ((dist > Surface<ftype>::tolerance) && (dist*dist < distance2))
            {
                return 0;
            }
        }
        return 1;
    }
};

#endif // !
