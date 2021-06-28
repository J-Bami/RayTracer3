#ifndef UNIFORM_MATERIAL_COMPONENT_H
#define UNIFORM_MATERIAL_COMPONENT_H

/*
This file defines the Material Component class that binds to surfaces.
A Surface will point to one of these
*/

#include "MaterialComponent.h"
#include "Optics/Material.h"

template <typename ftype>
class Surface;

template<typename ftype>
class UniformComponent: public MaterialComponent<ftype>
{
	typedef Maths::Vector<ftype, 2> local_fvector;
	typedef Optics::SpectrumArray<ftype> sarray;
	typedef Optics::SpectrumArray<ftype*> rsarray;
private:
	const Optics::Material<ftype>* m_material;
public:
	UniformComponent(const Optics::Material<ftype>* material) : m_material(material), MaterialComponent<ftype>() {}

	virtual const Optics::Material<ftype>* get_material(const local_fvector& position=0) const override { return m_material; }
};


#endif