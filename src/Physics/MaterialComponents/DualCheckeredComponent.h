#ifndef DUAL_CHECKERED_COMPONENT_H
#define DUAL_CHECKERED_COMPONENT_H

#include "MaterialComponent.h"
#include "Geometry/Space.h"

//we could also make this a child of Space<ftype, 2, 3> to ensure that the axes are perpendicular

template<typename ftype>
class DualCheckeredComponent: public MaterialComponent<ftype>, public Geometry::Space<ftype, 2, 3>
{
	typedef Maths::Vector<ftype, 3> fvector;
	typedef Geometry::Space<ftype, 2, 3> plane;
private:
	//depending on the surface type, size can represent either angle in radians or length in meters
	const ftype inverse_size;
	const Optics::Material<ftype>* const mat1;
	const Optics::Material<ftype>* const mat2;
public:

	DualCheckeredComponent(
		const ftype size,
		const Optics::Material<ftype>* material1, 
		const Optics::Material<ftype>* material2):
		inverse_size(1/size),
		mat1(material1),
		mat2(material2)
	{}

	virtual const Optics::Material<ftype> *get_material(const Maths::Vector<ftype, 2>& local_coords) const override
	{ 
		const Maths::Vector<ftype, 2> f = local_coords * inverse_size;
		const long a = static_cast<long>(floor(f[0]));
		const long b = static_cast<long>(floor(f[1]));

		return (a & 1) ^ (b & 1) ? mat1 : mat2;  
	}
};


#endif // !DUAL_CHECKERED_COMPONENT_H

