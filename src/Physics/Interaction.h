#ifndef INTERACTION_H
#define INTERACTION_H

#include "Optics/Spectrum.h"
#include "LightSources/LightSource.h"
#include "Surfaces/Surface.h"
#include "Physics/Intersection.h"
#include "Physics/RayInfo.h"
//#include "Camera/Camera.h"

/*
file that defines all the interactions and
intensity calculations that can be computed on
a single thread.
*/

/**
 * TO DO:
 * Change the camera class to:
 *  - order the spatial regions with distance to the camera
 *  - automate the raytracing prcess in a single method after ordering the regions
 * Modify both space and first_intersection
 * - ensure that regions don't intersect
 */

/*
A ray is produced and cast into the scene. we want to find the intensity
of light this ray delivers to the camera
*/
template<typename ftype>
Optics::SpectrumArray<ftype> find_ray_intensity(
    RayInfo<ftype>& info);

/*
* given an array of surface pointers, we find the surface that the ray intersects first.
* it would be good to order the surfaces by distance... perhaps in a variation of this
*/
template<typename ftype>
Intersection<ftype> first_intersection(
    const Geometry::Space<ftype, 1, 3>& ray,
    const Surface<ftype>** surfaces,
    const size_t n_surfaces)
{
    Intersection<ftype> info;
    for(size_t i=0; i < n_surfaces; i++)
    {
        const Surface<ftype>* surface = surfaces[i];
        info.update(surface, surface->first_intersection(ray));
    }
    return info;
}


/*
finds the ray that is reflected specularly of a surface with input normal at input position*/
template<typename ftype>
Optics::SpectrumArray<ftype> compute_specular_reflection(
    RayInfo<ftype>& info,
    const Maths::Vector<ftype, 3>& position,
    const Maths::Vector<ftype, 3>& normal)
{
    //find the direction of the reflected ray, this should be a unit vector
    const Maths::Vector<ftype, 3> new_direction = info.m_ray.get_axis() - (ftype(2) * Maths::dot(normal, info.m_ray.get_axis()))*normal;

    //make a new ray:
    RayInfo<ftype> new_info(
        Geometry::Space<ftype, 1, 3>(position, new_direction, true), 
        info.m_bitfield, 
        info.m_generation + 1);

    return find_ray_intensity(new_info);
}

/*
finds the effective intensity value for each light source in the scene
after a ray has been incident on a diffusive surface 
*/
template<typename ftype>
Optics::SpectrumArray<ftype> compute_diffusive_reflection(
    RayInfo<ftype>& info,
    const Maths::Vector<ftype, 3>& position,
    const Maths::Vector<ftype, 3>& normal)
{
    //define the diffusive constant
    constexpr static ftype diffusive_constant = 1 / Maths::two_pi<ftype>;

    Optics::SpectrumArray<ftype> out;

    //loop through the light sources...
    const size_t n_lights = LightSource<ftype>::lights_count();
    for (size_t i = 0; i < n_lights; i++)
    {
        const LightSource<ftype>* light = LightSource<ftype>::get_light(i);
        const ftype illumination_ratio = light->illumination(position);

        //see if this point is illuminated by our source
        if (illumination_ratio)
        {
            //find the cosine of direction to normal and multiply with the other constants
            const ftype constant = Maths::modulus(Maths::dot(normal, light->get_effective_direction(position)) * diffusive_constant * illumination_ratio);
            Optics::SpectrumArray<ftype> this_intensity = light->get_intensity(position) * constant;

            if (info.m_bitfield != Optics::all_colours)
            {
                BEGIN_SPECTRUM_LOOP(j)
                    if (!(info.m_bitfield & (1 << j)))
                    {
                        this_intensity[j] = ftype(0.0);
                    }
                END_SPECTRUM_LOOP
            }
            out += this_intensity;
        }
    }
    return out;
}


/*
finds the direction the refracted ray when an incident direction, refraction position and
surface normal are input.
*/
template<typename ftype>
const Geometry::Space<ftype, 1, 3> refracted_ray(
    const Maths::Vector<ftype, 3>& incident_direction,
    const Maths::Vector<ftype, 3>& position,
    const Maths::Vector<ftype, 3>& normal,
    const ftype refractive_index1,
    const ftype refractive_index2)
{
    const ftype cos1 = Maths::dot(normal, incident_direction);
    const ftype theta1 = acos(cos1);
    //compute the tangent 

    Maths::Vector<ftype, 3> tangent = incident_direction - normal * cos1;
    tangent.normalise(); //uses a sqrt so we probably want to avoid this actually.

    const ftype theta2 = asin(sin(theta1) * refractive_index1 / refractive_index2);
    const Maths::Vector<ftype, 3> direction = Maths::unit(ftype(tan(theta2)) * normal + tangent);
    return Geometry::Space<ftype, 1, 3>(position, direction);
}

/*
returns the rayinfo struct when a 
*/
template<typename ftype>
Optics::SpectrumArray<ftype> compute_refraction(
    RayInfo<ftype>& info,
    const Maths::Vector<ftype, 3>& position,
    const Maths::Vector<ftype, 3>& normal,
    const Optics::Material<ftype>* material)
{

    Optics::SpectrumArray<ftype> out;
    const Optics::SpectrumArray<const ftype*> transmissivities = material->get_transmissivity();

    for (size_t i = 0; i < material->unique_refractions(); i++)
    {
        const Optics::SpectrumInt colours = info.m_bitfield & material->get_refractive_split(i);
        if (colours)
        {
            const ftype new_n = material->get_refractive_index(i);
            //make a ray using only these colours;
            const Geometry::Space<ftype, 1, 3> new_ray =
                refracted_ray(info.m_ray.get_axis(), position, normal, info.m_refractive_index, new_n);

            //make a new rayInfo
            RayInfo<ftype> new_info(
                new_ray,
                colours,
                info.m_generation + 1,
                new_n);
            
            out += (find_ray_intensity(new_info)* transmissivities);
        }
    }
    return out;
}


/*

*/
template<typename ftype>
Optics::SpectrumArray<ftype> find_ray_intensity(RayInfo<ftype>& info) // we could provide a background colour here.
{
    typedef Geometry::AxisAlignedBoundingBox<ftype, 3> aabbf;


    //define the threshold value that the properties must be above to calculate something
    static constexpr ftype threshold_value = 1e-3;

    Optics::SpectrumArray<ftype> out;

    //check that we're not past max generations or this could go on forever.
    if (info.m_generation >= RayInfo<ftype>::max_generations) { return out; }

    aabbf culling_box(info.m_ray.get_origin(), info.m_ray.get_axis() * ftype(INFINITY) + info.m_ray.get_origin());
    Set<Surface<ftype>*> remaining_surfaces = Surface<ftype>::surface_cull(culling_box);

    //we shoot this ray into space to find the surface of the first intersection...
    const Intersection<ftype> intersection_info = 
        first_intersection<ftype>(
            info.m_ray,
            static_cast<const Surface<ftype>**>(remaining_surfaces),
            remaining_surfaces.get_size());

    //if we don't collide with a surface, we don't modify the array whatsoever, as it is 0.0
    if (!intersection_info.closest) { return out; }

    //get the intersection position:
    const Maths::Vector<ftype, 3> intersection_position = info.m_ray.get_origin() + info.m_ray.get_axis() * intersection_info.distance;

    //get the surface normal
    const Maths::Vector<ftype, 3> surface_normal = intersection_info.closest->normal(intersection_position);

    //get the material properties:
    const Surface<ftype>* closest_surface = intersection_info.closest;

    const Optics::Material<ftype>* material = 
        closest_surface->get_material_component()->get_material(closest_surface->get_local_coordinates(intersection_position));

    const Optics::SpectrumArray<const ftype*> diffusivity(material->get_diffusivity());
    const Optics::SpectrumArray<const ftype*> specularity(material->get_specularity());
    const Optics::SpectrumArray<const ftype*> transmissivity(material->get_transmissivity());

    //think about the best way to do this. It's likely there will be a lot of memory allocation
    Optics::SpectrumArray<ftype> diff;
    Optics::SpectrumArray<ftype> spec;
    Optics::SpectrumArray<ftype> trans;

    if (diffusivity.threshold(threshold_value))
    {
        //we can pass coefficients through these?
        diff = compute_diffusive_reflection<ftype>(info, intersection_position, surface_normal)*diffusivity;
    }

    if (specularity.threshold(threshold_value))
    {
        spec = compute_specular_reflection<ftype>(info, intersection_position, surface_normal)*specularity;
    }

    if (transmissivity.threshold(threshold_value))
    {
        trans = compute_refraction<ftype>(info, intersection_position, surface_normal, material) * transmissivity;
    }
//#define CAMERA_DEBUG
#ifdef CAMERA_DEBUG
    std::cout << "\nCompleted find_ray_intensity for ray of generation: " << int(info.m_generation)
        << "\ndiff: " << diff << "\nspec: " << spec << "\ntrans: " << trans;
    std::cout << "\nintersection position: " << intersection_position;
#endif

    return diff+spec+trans;
}
#endif