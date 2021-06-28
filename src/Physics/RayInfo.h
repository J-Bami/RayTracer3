#ifndef RAY_INFO_H
#define RAY_INFO_H

#include "Optics/Spectrum.h"
#include "Geometry/Space.h"

/*
for the future: every time we make a ray, we could find the
spatial regions that it intersects.
we'll let the Camera define the full conversion to rgb therefor there is no need for this rayinfo struct to contain a ptr to the rgb address

we might need to give the ray a block of memory to complete computations...? we'll see

in the eventuality that the ray moves from surface to surface, it might be worth keeping a pointer
member for the last surface that it was in
*/


template<typename ftype>
struct RayInfo
{
    typedef Geometry::Space<ftype, 1, 3> linef;

    static constexpr unsigned char max_generations = 10;
    static size_t rays_created;

    const Optics::SpectrumInt m_bitfield;                //which colours this ray is computing for
    const unsigned char m_generation;                        //where the data needs to end up?
    const ftype m_refractive_index;                      //current medium's refractive index.
    const linef m_ray;                                   //the geometric ray

    RayInfo() = delete;

    RayInfo(
        const linef& ray,                                                       // the geometric line
        const Optics::SpectrumInt bits_set = Optics::all_colours,               // bitset
        const unsigned char gen = 0,                                            // generation
        const ftype index = 1.0) :                                              // refractive index

        m_ray(ray),
        m_bitfield(bits_set),
        m_generation(gen),
        m_refractive_index(index)
    {
        rays_created++;
    }
};

template <typename ftype>
size_t RayInfo<ftype>::rays_created = 0;


#endif
