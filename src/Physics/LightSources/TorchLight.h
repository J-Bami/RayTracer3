#ifndef TORCH_LIGHT_H
#define TORCH_LIGHT_H

#include "LightSource.h"

template<typename ftype>
class TorchLight : public LightSource<ftype>
{
	typedef Maths::Vector<ftype, 3> fvector;
	typedef Optics::SpectrumArray<ftype> sarray;
	typedef Geometry::AxisAlignedBoundingBox<ftype, 3> aabb3;
	typedef Geometry::Space<ftype, 1, 3> linef;

};
