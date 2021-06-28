#ifndef AXIS_ALIGNED_BOUNDING_BOX_H
#define AXIS_ALIGNED_BOUNDING_BOX_H

#include "Maths/Linalg.h"
#include <iostream>

namespace Geometry
{
    template<typename ftype, unsigned char dimension>
    class AxisAlignedBoundingBox
    {
        typedef Maths::Vector<ftype, dimension> fvector;
    private:
        fvector lower_bounds;
        fvector upper_bounds;

        void sort_bounds()
        {
            for (size_t i = 0; i < dimension; i++)
            {
                const ftype value1 = lower_bounds[i];
                const ftype value2 = upper_bounds[i];
                lower_bounds[i] = value1 > value2 ? value2 : value1; 
                upper_bounds[i] = value1 > value2 ? value1 : value2;
            }
        }

    public:
        AxisAlignedBoundingBox() {}

        AxisAlignedBoundingBox(const fvector& lower, const fvector& upper) :
            lower_bounds(lower), upper_bounds(upper)
        {
            sort_bounds();
        }

        AxisAlignedBoundingBox(const AxisAlignedBoundingBox& other) :
            upper_bounds(other.upper_bounds), lower_bounds(other.lower_bounds) {}

        AxisAlignedBoundingBox& operator=(const AxisAlignedBoundingBox& other)
        {
            upper_bounds = other.upper_bounds;
            lower_bounds = other.lower_bounds;
            return *this;
        }
        
        ~AxisAlignedBoundingBox(){}

        const fvector get_center()const { return ftype(0.5) * (lower_bounds + upper_bounds); }
        const fvector& get_lower_bounds()const { return lower_bounds; }
        const fvector& get_upper_bounds()const { return upper_bounds; }

        void set_upper_bounds(const fvector& new_upper)
        {
            upper_bounds = new_upper;
            sort_bounds();
        }

        void set_lower_bounds(const fvector& new_lower)
        {
            lower_bounds = new_lower;
            sort_bounds();
        }

        //if any bounds are non-zero;
        operator bool()const
        {
            for (size_t i = 0; i < dimension; i++)
            {
                if (lower_bounds[i] || upper_bounds[i])
                {
                    return true;
                }
            }
            return false;
        }

        const bool all()
        {
            return upper_bounds.all() && lower_bounds.all();
        }

        const bool any()
        {
            return upper_bounds.any() || lower_bounds.any();
        }

        const ftype volume()const
        {
            return Maths::product(upper_bounds - lower_bounds, dimension);
        }

        const fvector get_delta()const
        {
            return upper_bounds - lower_bounds;
        }
    };
}

typedef Geometry::AxisAlignedBoundingBox<float, 2> aabb2f;
typedef Geometry::AxisAlignedBoundingBox<float, 3> aabb3f;
typedef Geometry::AxisAlignedBoundingBox<double, 2> aabb2d;
typedef Geometry::AxisAlignedBoundingBox<double, 3> aabb3d;

template<typename ftype, unsigned char dimension>
std::ostream& operator<<(std::ostream& out, const Geometry::AxisAlignedBoundingBox<ftype, dimension>& box)
{
    out << "Axis-Aligned Bounding Box, limits: " << box.get_lower_bounds() << ", " << box.get_upper_bounds();
    return out;
}


#endif