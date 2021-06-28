#ifndef COMPOUND_OBJECT_H
#define COMPOUND_OBJECT_H

/*
This file defines a class that holds a number of the same types of object with set relative displacements
this will require a 2D and 3D version for obvious reasons
*/

#include "Maths/Linalg.h"

namespace Geometry
{
    template<typename object_type, typename ftype>
    class Compound2d
    {
        typedef Maths::Vector<ftype, 2> fvector;
    private:
        size_t n_objects;
        ftype rotation;
        fvector position;
        fvector *relative_positions;
        object_type *objects;

    private:
        void allocate()
        {
            relative_positions = new fvector[n_objects];
            objects = new object_type[n_objects];
        }
    public:

    Compound2d() = delete;

    Compound2d(const object_type *objects_, const fvector *rel_pos, const size_t n): n_objects(n), angle(0), position({})
    {
        allocate();
        for (size_t i = 0; i < n; i++)
        {
            relative_positions[i] = rel_pos[i];
            objects[i] = objects_[i];
        }
    }

    Compound2d(const Compound2d& other): n_objects(other.n_objects), angle(other.angle), position(other.position)
    {
        allocate();
        for (size_t i = 0; i < n_objects; i++)
        {
            relative_positions[i] = other.relative_positions[i];
            objects[i] = other.objects[i];
        }
    }

    ~Compound2d()
    {
        delete [] relative_positions;
        delete [] objects;
    }
    

    //is there a way we can call a function on all the objects in this wrapper easily?
    };
}


#endif