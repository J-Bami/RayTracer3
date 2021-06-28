#ifndef MATERIAL_COMPONENT_H
#define MATERIAL_COMPONENT_H

/*
This file defines the Material Component class that binds to surfaces.
A Surface will point to one of these. 

current
*/

#include "Maths/Vector.h"
#include "Optics/Material.h"

template<typename ftype>
class MaterialComponent
{
	typedef Maths::Vector<ftype, 2> local_fvector;
public:
	MaterialComponent() {}

	virtual const Optics::Material<ftype>* get_material(const local_fvector& position=0)const = 0;
};


#endif