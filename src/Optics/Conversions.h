#ifndef OPTIC_CONVERSIONS_H
#define OPTIC_CONVERSIONS_H


/*
this series of functions makes 2 major assumptions:
- all Colours that form the spectrum have at least one of r, g, b saturated (value = 255)
- intensity arrays are unbounded and need to be converted to the range 0 <= x <= 1
*/

#include "Maths/Vector.h"
#include "Maths/MathUtil.h"
#include "Colour.h"
#include "SpectrumArray.h"
#include <tgmath.h>

namespace Optics
{
	//to convert a spectrum array to a raw colour
	//template<typename ftype>
	
	//helper functions
	template<typename ftype>
	Maths::Vector<ftype, 2> find_min_max(const ftype* const intensities, const size_t length)
	{
		Maths::Vector<ftype, 2> out({INFINITY, 0});
		const size_t n = length*Spectrum::n();
		for (size_t i = 0; i < n; i++)
		{
			if(intensities[i] < out[0])
			{
				out[0] = intensities[i];
			}
			if(intensities[i] > out[1])
			{
				out[1] = intensities[i];
			}
		}
		return out;
	}

	/*takes in a ptr to a single SpectrumArray, with intensities clamped from 0-1 and finds the
	resulting pixel colour */
	template<typename ftype>
	inline const Colour intensity_to_pixel(const ftype* intensities)
	{
		Colour out; //currently black
		BEGIN_SPECTRUM_LOOP(j)
			out += Spectrum::get_colour(j) * intensities[j];
		END_SPECTRUM_LOOP
		return out;
	}


	//some different conversion functions
	template<typename ftype>
	void linear_conversion(
		const ftype* const intensities, 
		uint32_t* bitmap,
		const size_t length,
		const ftype max_intensity)
	{
		const ftype multiplier = 1/max_intensity;
		const ftype* ptr = intensities;
		for (size_t i = 0; i < length; i++)
		{
			//we have the intensity array		
			const SpectrumArray<const ftype*> intensity_array(ptr);
			const SpectrumArray<ftype> clamped_intensities = intensity_array*multiplier; //will lie between 0 and 1.
			const Colour pixel_colour = intensity_to_pixel(clamped_intensities.get_data());

			bitmap[i] = pixel_colour.m_data;
			ptr += Spectrum::n();
		}
	}

	//we can have a logarithmic conversion too but what's the point?

	template<typename ftype>
	void clamped_conversion(
		const ftype* const intensities, 
		uint32_t* bitmap,
		const size_t length,
		const ftype max_intensity,
		const ftype min_intensity
	);

}

#endif // !OPTIC_CONVERSIONS_H
