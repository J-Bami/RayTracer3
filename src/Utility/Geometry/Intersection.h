#ifndef GEOMETRIC_INTERSECTION_H
#define GEOMETRIC_INTERSECTION_H

#include "Maths/Vector.h"
#include "AxisAlignedBoundingBox.h"
#include "Capsule.h"
#include "Cylinder.h"
#include "CompoundObject.h"
#include "Space.h"
#include "Sphere.h"
#include "Triangle.h"

/*
Defines all the geometric intersections needed for the Raytracer, including AABB intersections,
SphericalCorrdinate things, line intersections, etc 
*/

namespace Geometry
{
	enum AABB_INTERSECTIONS : unsigned char
	{
		IS_OUTSIDE = 0,
		NO_INTERSECTION = 0,
		TOUCHING = 1,
		ON_BOUNDARY = 1,
		OVERLAPPING = 2,
		IS_INSIDE = 3,
	};

	//AABB inersections - these are very important 
	template<typename ftype, unsigned char dimension>
	const AABB_INTERSECTIONS intersection(
		const AxisAlignedBoundingBox<ftype, dimension>& aabb,
	 	const Maths::Vector<ftype, dimension>& point,
		const ftype tol = 0)
	{
		// perhaps we can start off assuming it's inside 
		AABB_INTERSECTIONS value = AABB_INTERSECTIONS::IS_INSIDE;
		for (size_t i = 0; i < dimension; i++)
		{
			//we need to adjust tolerance
			const ftype upper_tol = Maths::modulus(aabb.get_upper_bounds()()[i] * tol);
			const ftype lower_tol = Maths::modulus(aabb.get_lower_bounds()()[i] * tol);

			//test the requirements for different scenario
			switch (value)
			{
				case AABB_INTERSECTIONS::IS_INSIDE:
				{
					if (point[i] > aabb.get_lower_bounds()[i] && point[i] < aabb.get_upper_bounds()[i])
					{
						break; //we are breaking the switch, not the for loop.
					}
					else
					{
						value = AABB_INTERSECTIONS::ON_BOUNDARY; // we're not inside, and points cannot overlap aabbs
					}
				}
				case AABB_INTERSECTIONS::ON_BOUNDARY:
				{
					if (point[i] >= (aabb.get_lower_bounds()[i] - lower_tol) &&
						point[i] <= (aabb.get_upper_bounds()[i] + upper_tol))
					{
						break;
					}
					else
					{
						return AABB_INTERSECTIONS::IS_OUTSIDE;
					}
				}
				default:
				{
					return AABB_INTERSECTIONS::IS_OUTSIDE;
				}
			}
		}
		return value;
	}

	//This only works ONE WAY. Tests if aabb2 is inside aabb1, then overlapping, then touching. NOT THE OTHER WAY AROUND
	//but this will determine 1 & 2 despite the order, just not 3. This is so we know which is inside which.
	template<typename ftype, unsigned char dimension>
	const AABB_INTERSECTIONS intersection(
		const AxisAlignedBoundingBox<ftype, dimension>& aabb1,
		const AxisAlignedBoundingBox<ftype, dimension>& aabb2,
		const bool full = false,
		const ftype tol = 0)
	{
		//we can start off assuming aabb2 is inside aabb1 
		AABB_INTERSECTIONS value = AABB_INTERSECTIONS::IS_INSIDE;
		for (size_t i = 0; i < dimension; i++)
		{
			//we need to adjust tolerance
			const ftype upper_tol = aabb1.get_upper_bounds()[i] * tol;
			const ftype lower_tol = aabb1.get_lower_bounds()[i] * tol;
			switch (value)
			{
				case AABB_INTERSECTIONS::IS_INSIDE:
				{
					if(full)
					{
						if (((aabb2.get_lower_bounds()[i] >= aabb1.get_lower_bounds()[i]) &&
							(aabb2.get_upper_bounds()[i] <= aabb1.get_upper_bounds()[i])) || 
							((aabb1.get_lower_bounds()[i] >= aabb2.get_lower_bounds()[i]) &&
							(aabb1.get_upper_bounds()[i] <= aabb2.get_upper_bounds()[i])))
						{
							break;  //we are breaking the switch, not the for loop.
						}
						else
						{
							value = AABB_INTERSECTIONS::OVERLAPPING;
						}
					}
					else
					{
						if ((aabb2.get_lower_bounds()[i] >= aabb1.get_lower_bounds()[i]) &&
							(aabb2.get_upper_bounds()[i] <= aabb1.get_upper_bounds()[i]))
						{
							break;  //we are breaking the switch, not the for loop.
						}
						else
						{
							value = AABB_INTERSECTIONS::OVERLAPPING;
						}
					}
					
				}
				case AABB_INTERSECTIONS::OVERLAPPING:
				{
					//for the overlapping case, only one of the limits needs to be between the others
					if ((aabb2.get_lower_bounds()[i] < aabb1.get_upper_bounds()[i] && aabb2.get_lower_bounds()[i] > aabb1.get_lower_bounds()[i]) ||
						(aabb2.get_upper_bounds()[i] > aabb1.get_lower_bounds()[i] && aabb2.get_upper_bounds()[i] < aabb1.get_upper_bounds()[i]))
					{
						break;
					}
					else
					{
						value = AABB_INTERSECTIONS::ON_BOUNDARY;
					}
				}
				case AABB_INTERSECTIONS::ON_BOUNDARY:
				{
					if ((aabb2.get_lower_bounds()[i] <= (aabb1.get_upper_bounds()[i] + upper_tol)
						&& aabb2.get_lower_bounds()[i] >= (aabb1.get_lower_bounds()[i] - lower_tol) ||
						(aabb2.get_upper_bounds()[i] >= (aabb1.get_lower_bounds()[i] - lower_tol) &&
							aabb2.get_upper_bounds()[i] <= (aabb1.get_upper_bounds()[i] + upper_tol))))
					{
						break;
					}
					else
					{
						value = AABB_INTERSECTIONS::IS_OUTSIDE;
					}
				}
				default:
				{
					return AABB_INTERSECTIONS::IS_OUTSIDE;
				}
			}
		}
		return value;
	}


	//sphere - ray intersection; tells us where the first positive intesection is
	template<typename ftype>
	const ftype intersection(const Sphere<ftype>& sphere, const Space<ftype, 1, 3>& ray, const ftype tolerance)
	{
		typedef Maths::Vector<ftype, 3> fvector;
		/*
		there are 0, 1 or 2 intersections between a circle and a line;
		we need the closest one that gives a non-negative distance.

		we should find the linear distance along the line that the thing gives
		*/
		const fvector& position = sphere.get_center();
		const ftype local_projected_point = ray.project(position);
		const fvector ray_point = ray.local_to_global(local_projected_point);

		//this is the minimum squared distance from info.ray
		const ftype seperation2 = Maths::mag2(position - ray_point);

		// if abs(sep2) > radius, or if it's negative, it never intersects
		if (seperation2 > sphere.get_r2())
		{
			return -1.0;
		}

		// we want to find out how much further forward or backward the intersecting points lie.
		const ftype intersection_distance = sqrt(sphere.get_r2() - seperation2); //we need this to be sqrt, trust me.
		return ((local_projected_point - intersection_distance > tolerance) ?
			local_projected_point - intersection_distance :
			local_projected_point + intersection_distance);
	}

	//intersection between a plane and a ray
	template<typename ftype>
	const ftype intersection(const Space<ftype, 2, 3>& plane, const Space<ftype, 1, 3>& ray)
	{
		const Maths::Matrix<ftype, 3, 1> origins({ ray.get_origin() - plane.get_origin() });

		const Maths::Matrix<ftype, 3, 3> axes({ plane.get_axis(0), plane.get_axis(1), -ray.get_axis() });
		try
		{
			const ftype out = Maths::matmul(origins, Maths::inv(axes))[0][2];
			return out ? out : -1.0;
		}
		catch (LINALG_ERRORS) 
		{
			return -1.0;
		}
	}

	//intersection between a 3d triangle and a ray
	template<typename ftype>
	const ftype intersection(const Triangle<ftype, 3> &triangle, const Space<ftype, 1, 3>& ray)
	{
		Maths::Matrix<ftype, 3, 1> displacements({ ray.get_origin() - triangle.get_vertex(0) });
		Maths::Matrix<ftype, 3, 3> dirs({ triangle.get_vertex(1) - triangle.get_vertex(0), triangle.get_vertex(2) - triangle.get_vertex(0), -ray.get_axis() });
		try
		{
			Maths::Vector<ftype, 3> vars = Maths::matmul(displacements, Maths::inv(dirs))[0];
			return ((vars[0] >= 0) && (vars[1] >= 0) && (vars[0] + vars[1] <= 1)) ? vars[2] : ftype(-1.0);
		}
		catch (LINALG_ERRORS)
		{
			return -1.0;
		}
	}
}


#endif // !GEOMETRIC_INTERSECTION_H
