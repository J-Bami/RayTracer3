#ifndef CANVAS_H
#define CANVAS_H

/*
defines a class that holds memory for the floating-point array of colours
*/

#include "Optics/SpectrumArray.h"
#include <stdint.h>
#include <cstring>

template<typename ftype>
class Canvas
{
private:
	uint64_t n_fragments;
	uint64_t counter;
	ftype max_intensity;
	ftype* data;

	inline void allocate()
	{
		data = new ftype[n_fragments * Optics::Spectrum::n()]{}; 
		//initialise it all to ftype(0);
	}

public:
	Canvas() = delete;

	Canvas(
		const unsigned short horizontal_res,
		const unsigned short vertical_res) :
		n_fragments(static_cast<uint64_t>(horizontal_res) * static_cast<uint64_t>(vertical_res)),
		counter(0),
		max_intensity(0)
	{
		allocate();
	}

	~Canvas()
	{
		delete[] data;
	}

	ftype* const get_data() const
	{
		return data;
	}

	const size_t next_fragment()
	{
		const size_t out = counter;
		if (counter == n_fragments)
		{
			return ~size_t(0);
		}
		else
		{
			counter++;
		}
		return out;
	}

	void reset()
	{
		counter = 0;
		memset(data, 0, n_fragments * sizeof(ftype) * Optics::Spectrum::n());
	}

	inline void write_to_canvas(const ftype* input_data, const size_t relative_address)
	{
		ftype* ptr = data + (Optics::Spectrum::n()*relative_address);
		BEGIN_SPECTRUM_LOOP(i)
			max_intensity = Maths::max(max_intensity, input_data[i]);
			*ptr = input_data[i];
			ptr++;
		END_SPECTRUM_LOOP
	}

	ftype get_max_intensity()const 
	{
		return max_intensity;
	}

	const uint64_t& get_size()const
	{
		return n_fragments;
	}

};


#endif