#ifndef LIGHT_SOURCE_H
#define LIGHT_SOURCE_H


#include "Maths/Vector.h"
#include "Containers/Manager.h"
#include "Optics/SpectrumArray.h"
#include "Surfaces/Surface.h"

/*
* Defines the abstract class that is the light source.
* these must all do the following:
* 
* - have a SpectrumArray get intensity (fvector)  // how much light is hitting position
* - fvector get_effective_position()  // it's effective location
* - Spectrum Array get_luminosity() // absolute luminosity 
* 
* some lights are directional which means that we can cull all surfaces out that aren't in their fov
* 
* 
* I aim to include multiple types of light source in this project including:
*	point source
*   directional
*	torch
*	laser
*	sphere - we can do some geometry tricks to help with this; but we want one ray per source.
*	illuminated surface (this will be hard)
*/


template<typename ftype>
class LightSource
{
    typedef Maths::Vector<ftype, 3> fvector;
    typedef Optics::SpectrumArray<ftype> sarray;
    typedef Geometry::Space<ftype, 1, 3> linef;
protected:
    //we'll keep a member here that can store the results of get_intensity??
    //if this is gonna be multithreded, we'll need a mutex
public:
    static Manager<LightSource> manager;

    LightSource()
    {
        manager.register_object(this);
    }

    ~LightSource()
    {
        manager.remove_object(this);
    }

    static inline const Manager<LightSource>& get_manager() { return manager; }

    //returns a vector that represents the direction from fvector to source
    virtual const fvector get_effective_direction(const fvector&)const = 0;

    //returns a vector that represents the position of the light source
    //virtual const fvector get_effective_position(const fvector&)const = 0;

    //returns the absolute intensity of light from this light source at the input position
    virtual const sarray get_intensity(const fvector&)const = 0;

    // returns the effective luminosity of the light source 
    //virtual const sarray get_luminosity()const = 0;

    //gives us a ratio (0 - 1) telling us how much of the light source illuminates in input point
    //maybe we don;t need to define this now. We could do a cull for surfaces here.
    virtual ftype illumination(const fvector& point) const = 0;

    //gets all the lights in the world
    inline static const LightSource** get_lights()
    {
        return manager.get_objects();
    }

    inline static const LightSource* get_light(const size_t index)
    {
        return manager.get_objects()[index];
    }

    //returns the number of lights in the world
    inline static size_t lights_count()
    {
        return manager.get_size();
    }
};

template<typename ftype>
Manager<LightSource<ftype>> LightSource<ftype>::manager;

#endif