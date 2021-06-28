#ifndef TRIANGULAR_MESH_H
#define TRIANGULAR_MESH_H

#include "Triangle.h"

/*
A geometry interface which collects a whole bunch of points and then decomposes them into triangles.
*/

namespace Geometry
{
	template<typename type, unsigned char dimension>
	class TriangularMesh
	{
		typedef Maths::Vector<ftype, dimension> fvector;
	private:
		size_t n_vertices;
		size_t n_triangles;
		fvector* vertices;
		Triangle<ftype*, dimension>* triangles;
	public:
	};
}

#endif