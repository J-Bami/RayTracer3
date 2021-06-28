#ifndef SPECTRUM_H
#define SPECTRUM_H


#include "Colour.h"
#include <cassert>

/*
This file defines the processes that allow custom spectra to be used within the Raytracer.

A Spectrum is the collection of the elementary colours in a world. 
.. if that makes any sense.
For example, most humans have 3 cone cells which detect only red, green and blue light; 
hence the conversion to RGB later in the rendering process.

This ray tracer allows for up to sizeof(SpectrumBitfield) different elementary colours to be a part of the spectrum; 
and they are all housed in this spectrum. If the user decides they would like to use a spectrum of just red and blue, this should be possible.

Note that the number of colours in the spectrum is only set at runtime but is used almost everywhere in this project

only one instance of the spectrum class is allowed during runtime.
inputs must be:
	-all the colours for this spectrum
	-the rgb values for each colour in this spectrum


	add in code to disallow black as a colour
*/

namespace Optics
{
	typedef unsigned char SpectrumInt;
	constexpr unsigned char max_colours = 8*sizeof(SpectrumInt);
	constexpr SpectrumInt all_colours = ~SpectrumInt(0);

	class Spectrum
	{
	public:
		static Spectrum* instance;
		const static inline SpectrumInt n() { return instance->ncolours; }

		static Spectrum* initialise(const Colour* colours, const SpectrumInt n_colours)
		{
			if (nullptr == instance)
			{
				instance = new Spectrum(colours, n_colours);
			}
			return instance;
		}

	private:
		const SpectrumInt ncolours;
		Colour m_colours[max_colours];

		Spectrum() = delete;

		Spectrum(const Colour* colours, const SpectrumInt n_colours): ncolours(n_colours)
		{
			assert(n_colours <= max_colours && "Too many colours for the spectrum!!");
			memcpy(m_colours, colours, sizeof(Colour)*n_colours);
		}

		~Spectrum(){}

		const Colour* _get_colours()const
		{
			return m_colours;
		}

		const Colour& _get_colour(const size_t index)const
		{
			assert(index < ncolours);
			return m_colours[index];
		}
	public:
		inline static const Colour* get_colours()
		{
			return instance->_get_colours();
		}

		inline static Colour get_colour(const size_t index)
		{
			return instance->_get_colour(index);
		}
	};


	Spectrum* Spectrum::instance = nullptr;

	const Colour rgb[3] = { Red, Green, Blue };
	const Colour greyscale[1] = { White };

	//define the colour sets for various common spectra

}

#define BEGIN_SPECTRUM_LOOP(loop_index)\
const Optics::SpectrumInt n_spectrum_colours = Optics::Spectrum::n();\
for (Optics::SpectrumInt loop_index = 0; loop_index < n_spectrum_colours; loop_index++){

#define END_SPECTRUM_LOOP }


#endif