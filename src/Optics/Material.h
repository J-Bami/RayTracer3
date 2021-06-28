#ifndef OPTIC_MATERIAL_H
#define OPTIC_MATERIAL_H

/*
defines the complete set of optical properties that a Material is allowed to have.

if a Material has differing refractive indices for each colour, there should be a function
that assists; we should know how to split the light.

Each MaterialComponent will house pointers to each material that is used in this Raytracer,
Therefore it will be possible to just house a collection of materials next to each other when/
if it comes to optimisation

*/

#include "Maths/Vector.h"
#include "Colour.h"
#include "Spectrum.h"
#include "SpectrumArray.h"

#include <iostream>

namespace Optics
{

	template<typename ftype>
	struct MaterialStruct
	{
		SpectrumArray<ftype> absorptivity;
		SpectrumArray<ftype> diffusivity;
		SpectrumArray<ftype> specularity;
		SpectrumArray<ftype> transmissivity;
		SpectrumArray<ftype> refractive_indices;
	};

	template<typename ftype>
	class Material
	{
	private:
		union
		{
			struct
			{
				SpectrumArray<ftype> absorptivity;
				SpectrumArray<ftype> diffusivity;
				SpectrumArray<ftype> specularity;
				SpectrumArray<ftype> transmissivity;
				SpectrumArray<ftype> refractive_indices;
			};
			SpectrumArray<ftype> data[5];
		}; 
		
		SpectrumInt n;							 //info about how many refractive indices we need to read for the split
		SpectrumInt split[Optics::max_colours];  //an array of bitflags that will give us information about how light is refracted by this material 

		void normalise_properties(
			const ftype* absorptivites,
			const ftype* diffusivities,
			const ftype* specularities,
			const ftype* transmissivities)
		{
			BEGIN_SPECTRUM_LOOP(i)
				Maths::Vector<ftype, 4> props(absorptivites[i], diffusivities[i], specularities[i], transmissivities[i]);
				assert(props.any() && "All Optic properties for a colour in a material are set to zero.");

				props /= props.sum();
				absorptivity[i] = props[0];
				diffusivity[i] = props[1];
				specularity[i] = props[2];
				transmissivity[i] = props[3];
			END_SPECTRUM_LOOP
		}

		void compute_diffusive_split(const ftype* refractive_indices_)
		{
			n = 0;
			refractive_indices[0] = refractive_indices_[0] < 1 ? 1 : refractive_indices_[0];
			split[0] = 1;
			BEGIN_SPECTRUM_LOOP(i)
				const ftype refractive_index = refractive_indices_[i];
				for (size_t j = 0; j < i; j++)
				{
					if (refractive_index != refractive_indices[j])
					{
						if (j == i - 1) //if we have gone through the previous refractive indexes,
						{
							n++;
							split[n] |= (1 << i);
							refractive_indices[n] = refractive_index < 1 ? 1 : refractive_index;
						}
						else
						{
							continue;
						}
					}
					else
					{
						split[n] |= (1 << i);
					}
				}
			END_SPECTRUM_LOOP
			n++; //tells us the full number of different splits
		}

	public:

		Material() = delete;

		Material(
			const ftype* absorptivites,
			const ftype* diffusivities,
			const ftype* specularities,
			const ftype* transmissivities,
			const ftype* refractive_indices) : split{}
		{
			normalise_properties(absorptivites, diffusivities, specularities, transmissivities);
			compute_diffusive_split(refractive_indices);
		}

		Material(
			const ftype absorptivity_,
			const ftype diffusivity_,
			const ftype specularity_,
			const ftype transmissivity_,
			const ftype refractive_index_): n(1),  split{}
		{
			n = refractive_index_ >= 1 ? refractive_index_ : 1;
			
			Maths::Vector<ftype, 4> props{ absorptivity_, diffusivity_, specularity_, transmissivity_ };
			props /= props.sum();
			BEGIN_SPECTRUM_LOOP(i)
				absorptivity[i] = props[0];
				diffusivity[i] = props[1];
				specularity[i] = props[2];
				transmissivity[i] = props[3];
			END_SPECTRUM_LOOP
			split[0] = all_colours;
			refractive_indices[0] = refractive_index_;
		}


		Material(const MaterialStruct<ftype> m)
		{
			normalise_properties(
				m.absorptivity.get_data(),
				m.diffusivity.get_data(),
				m.specularity.get_data(),
				m.transmissivity.get_data());
			compute_diffusive_split(m.refractive_indices.get_data());
		}

		//Material(const Material& other) : data(other.data), n(n), split(other.split) {};

		~Material() {}

		//getters 
		const ftype* const get_absorptivity()const { return absorptivity.get_data(); }
		const ftype* const get_diffusivity()const { return diffusivity.get_data(); }
		const ftype* const get_specularity()const { return specularity.get_data(); }
		const ftype* const get_transmissivity()const { return transmissivity.get_data(); }

		const ftype* const get_refractive_indices()const { return refractive_indices.get_data(); }
		const ftype& get_refractive_index(const size_t index)const
		{
			assert(index < max_colours);
			return refractive_indices.get_data()[index];
		}

		//get the number of unique refractive indices.
		const SpectrumInt unique_refractions()const { return n;}

		const SpectrumInt get_refractive_split(const SpectrumInt which)const
		{
			assert(which < n);
			return split[which];
		}

		const SpectrumInt* get_refractive_splits()const { return split; }
	};

	//define some basic optic materials

	template<typename ftype>
	Material<ftype> Absorber()
	{
		return Material<ftype>( 1, 0, 0, 0, 1 );
	}

	template<typename ftype>
	Material<ftype> DiffuseReflector()
	{
		return Material<ftype>(0, 1, 0, 0, 1);
	}

	template<typename ftype>
	Material<ftype> SpecularReflector()
	{
		return Material<ftype>(0, 0, 1, 0, 1);
	}

	//absorbs all other colours but this one
	template<typename ftype>
	Material<ftype> Coloured(
		const ftype absorptance,
		const ftype diffuse,
		const ftype specular,
		const ftype transmittance,
		const ftype refractive_index,
		const SpectrumInt colour)
	{
		MaterialStruct<ftype> m;
		BEGIN_SPECTRUM_LOOP(i)
			if (i == colour)
			{
				m.absorptivity[colour] = absorptance;
				m.diffusivity[colour] = diffuse;
				m.specularity[colour] = specular;
				m.transmissivity[colour] = transmittance;
				m.refractive_indices[colour] = refractive_index;
			}
			else
			{
				m.absorptivity[i] = ftype(1);
			}
		END_SPECTRUM_LOOP
		return Material<ftype>(m);
	}


	template<typename ftype>
	Material<ftype> MultiColoured(
		const ftype absorptance,
		const ftype diffuse,
		const ftype specular,
		const ftype transmittance,
		const ftype refractive_index,
		const SpectrumInt bitflag)
	{
		MaterialStruct<ftype> m;
		BEGIN_SPECTRUM_LOOP(i)
			if(bitflag & (1 << i))
			{
				m.absorptivity[i] = absorptance;
				m.diffusivity[i] = diffuse;
				m.specularity[i] = specular;
				m.transmissivity[i] = transmittance;
				m.refractive_indices[i] = refractive_index;
			}
			else
			{
				m.absorptivity[i] = ftype(1);
			}
		END_SPECTRUM_LOOP
		return Material<ftype>(m);
	}

	template <typename ftype>
	Material<ftype> Refractor(const ftype refractive_index = 1.5)
	{
		return Material<ftype>(0, 0, 0, 1, refractive_index);
	}



}

template <typename ftype>
std::ostream& operator<<(std::ostream& out, const Optics::Material<ftype> m)
{
	out << "\nOptical Material:";
	out << "\nAbsorptivity:       " << m.get_absorptivity();
	out << "\nDiffusivity:        " << m.get_diffusivity();
	out << "\nSpecularity:        " << m.get_specularity();
	out << "\nTransmissivity:     " << m.get_transmissivity();
	out << "\nunique refractions = " << int(m.unique_refractions());
	out << "\nRefractive indices: ";

	for (size_t j = 0; j < m.unique_refractions(); j++)
	{
		out << "\n" << m.get_refractive_index(j) << " for: ";
		BEGIN_SPECTRUM_LOOP(i)
			if ((1 << i) & m.get_refractive_split(j))
			{
				out << "\n\t" << Optics::Spectrum::get_colour(i);
			}
		END_SPECTRUM_LOOP
	}
	return out;
}


#endif
