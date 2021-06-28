#ifndef COLOUR_H
#define COLOUR_H

#include "Maths/MathUtil.h"

#include <stdint.h>
#include <iostream>

/*
this file defines the colour struct.
We would also need to define how to transform from floating point values to these colours somewhere in 
the Optics folder 
*/


struct Colour
{
	union
	{
		struct
		{
			uint8_t m_blue;
			uint8_t m_green;
			uint8_t m_red;
			uint8_t m_alpha;
		};
		uint8_t m_array [4];
		uint32_t m_data;
	};

	Colour(): m_red(0), m_green(0), m_blue(0), m_alpha(255) {} // black;

	Colour(const unsigned char red, const unsigned char green, const unsigned char blue, const unsigned char alpha = 255U) :
		m_red(red), m_green(green), m_blue(blue), m_alpha(alpha) {}

	Colour(const uint32_t value) : m_data(value) {}

	Colour(const Colour& other) : m_data(other.m_data) {}

	explicit operator uint32_t() { return m_data; }

	operator bool()
	{
		return m_blue || m_green || m_red;
	}

	Colour& operator+=(const Colour c)
	{
		//we don't want it to be saturated, so it might be worth making it safe, even if it's slower
		m_red =	static_cast<uint8_t>(
				Maths::min(static_cast<uint16_t>(m_red) + static_cast<uint16_t>(c.m_red), 255));
		m_green = static_cast<uint8_t>(
				Maths::min(static_cast<uint16_t>(m_green) + static_cast<uint16_t>(c.m_green), 255));
		m_blue = static_cast<uint8_t>(
				Maths::min(static_cast<uint16_t>(m_blue) + static_cast<uint16_t>(c.m_blue), 255));
		m_alpha = static_cast<uint8_t>(
				Maths::min(static_cast<uint16_t>(m_alpha) + static_cast<uint16_t>(c.m_alpha), 255));
		return *this;
	}

	friend Colour operator+(const Colour c1, const Colour c2)
	{
		//we don't want it to be saturated, so it might be worth making it safe, even if it's slower
		return Colour(
			static_cast<uint8_t>(
				Maths::min(static_cast<uint16_t>(c1.m_red) + static_cast<uint16_t>(c2.m_red), 255)),
			static_cast<uint8_t>(
				Maths::min(static_cast<uint16_t>(c1.m_green) + static_cast<uint16_t>(c2.m_green), 255)),
			static_cast<uint8_t>(
				Maths::min(static_cast<uint16_t>(c1.m_blue) + static_cast<uint16_t>(c2.m_blue), 255)),
			static_cast<uint8_t>(
				Maths::min(static_cast<uint16_t>(c1.m_alpha) + static_cast<uint16_t>(c2.m_alpha), 255))
		);
	}

	//should be a 0-1 scalar
	template<typename ftype>
	friend Colour operator*(const Colour c, const ftype scalar)
	{
		return Colour(
			Maths::iround<uint8_t, ftype>(ftype(c.m_red)*scalar),
			Maths::iround<uint8_t, ftype>(ftype(c.m_green)*scalar),
			Maths::iround<uint8_t, ftype>(ftype(c.m_blue)*scalar),
			c.m_alpha);
	}

	//should be a 0-1 scalar
	template<typename ftype>
	friend Colour operator*(const ftype scalar, const Colour c)
	{
		return Colour(
			Maths::iround<uint8_t, ftype>(ftype(c.m_red) * scalar),
			Maths::iround<uint8_t, ftype>(ftype(c.m_green) * scalar),
			Maths::iround<uint8_t, ftype>(ftype(c.m_blue) * scalar),
			c.m_alpha);
	}

};

std::ostream& operator<<(std::ostream& out, const Colour c)
{
	out << "Colour: (" 
		<< int(c.m_red) << ", " 
		<< int(c.m_green) << ", " 
		<< int(c.m_blue) << "), alpha = " 
		<< int(c.m_alpha);
	return out;
}

const Colour Red(255, 0, 0);
const Colour Yellow(255, 255, 0);
const Colour Green(0, 255, 0);
const Colour Cyan(0, 255, 255);
const Colour Blue(0, 0, 255);
const Colour Magenta(0, 255, 255);

const Colour White(255, 255, 255);
const Colour Black(0, 0, 0);

const Colour Orange(128, 255, 0);
const Colour Indigo(76, 0, 130);
const Colour Violet(127, 0, 255);

#endif // !COLOUR_H