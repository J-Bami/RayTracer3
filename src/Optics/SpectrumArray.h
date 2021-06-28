#ifndef SPECTRUM_ARRAY_H
#define SPECTRUM_ARRAY_H

/*
defines an array that may or may not own its data, 
and is the size of Spectrum::n()

the class template SpectrumArray<type*> does NOT own its own data and writes to a pre-allocated portion of memory;
it is essentially a wrapper for ftype* to reduce typing lol

note if certain functions don#t work, we can just define them outside of the classes. I should know this lol
*/

#include "Colour.h"
#include "Spectrum.h"
#include <cstring>

namespace Optics
{
	/*
	An array used for basic arithmetic on Spectra intensity arrays
	this templated version owns its memory as a static array
	*/
	template<typename ftype>
	class SpectrumArray
	{
		friend class SpectrumArray<ftype*>;
		friend class SpectrumArray<const ftype*>;
	private:
		ftype data[Optics::max_colours];

		inline void copy_data(const ftype* const values)
		{
			memcpy(data, values, sizeof(ftype) * Spectrum::n());
		}

	public:
		SpectrumArray(): data{} {} //initialise all values to zero

		SpectrumArray(const ftype value)
		{
			BEGIN_SPECTRUM_LOOP(i)
				data[i] = value;
			END_SPECTRUM_LOOP
		}

		SpectrumArray(const std::initializer_list<ftype>& list)
		{
			size_t i = 0;
			for (ftype item : list)
			{
				data[i] = item;
				i++;
				if (i == Spectrum::n()) { break; }
			}
		}

		SpectrumArray(const ftype* const address)
		{
			copy_data(address);
		}

		SpectrumArray(const SpectrumArray<ftype*>& other);
		SpectrumArray(const SpectrumArray<const ftype*>& other);

		SpectrumArray &operator=(const SpectrumArray& other)
		{
			copy_data(other.data);
			return *this;
		}

		~SpectrumArray(){}

		//define some mathematical functions
		ftype &operator[](const size_t index)
		{
			assert(index < Spectrum::n());
			return data[index];
		}

		SpectrumArray &operator+=(const SpectrumArray& other)
		{
			BEGIN_SPECTRUM_LOOP(i)
				data[i] += other.data[i];
			END_SPECTRUM_LOOP
			return *this;
		}

		SpectrumArray &operator-=(const SpectrumArray& other)
		{
			BEGIN_SPECTRUM_LOOP(i)
				data[i] -= other.data[i];
			END_SPECTRUM_LOOP
			return *this;
		}

		SpectrumArray& operator*=(const SpectrumArray& other)
		{
			BEGIN_SPECTRUM_LOOP(i)
				data[i] *= other.data[i];
			END_SPECTRUM_LOOP
			return *this;
		}


		SpectrumArray& operator/=(const SpectrumArray& other)
		{
			BEGIN_SPECTRUM_LOOP(i)
				data[i] /= other.data[i];
			END_SPECTRUM_LOOP
			return *this;
		}

		SpectrumArray& operator+=(const SpectrumArray<ftype*>& other);

		SpectrumArray& operator-=(const SpectrumArray<ftype*>& other);

		SpectrumArray& operator*=(const SpectrumArray<ftype*>& other);

		SpectrumArray& operator/=(const SpectrumArray<ftype*>& other);

		SpectrumArray& operator+=(const SpectrumArray<const ftype*>& other);

		SpectrumArray& operator-=(const SpectrumArray<const ftype*>& other);

		SpectrumArray& operator*=(const SpectrumArray<const ftype*>& other);

		SpectrumArray& operator/=(const SpectrumArray<const ftype*>& other);

		SpectrumArray& operator/=(const ftype scalar)
		{
			const ftype inverse = 1 / scalar;
			BEGIN_SPECTRUM_LOOP(i)
				data[i] *= inverse;
			END_SPECTRUM_LOOP
			return *this;
		}

		const bool threshold(const ftype value)const
		{
			BEGIN_SPECTRUM_LOOP(i)
				if (data[i] > value)
				{
					return true;
				}
			END_SPECTRUM_LOOP
			return false;
		}

		inline operator ftype* () { return data; }

		inline const ftype* const get_data()const { return data; }

		inline void clear() { memset(data, 0, Spectrum::n()); }
	};

	//We point this one at some some data and can change the values, but not the address.
	template<typename ftype>
	class SpectrumArray<ftype*>
	{
		friend class SpectrumArray<ftype>;
		friend class SpectrumArray<const ftype*>;
	private:
		ftype *const data;

		inline void copy_data(const ftype* const address)
		{
			memcpy(address, data, sizeof(ftype) * Spectrum::n());
		}

	public:
		SpectrumArray() = delete;

		SpectrumArray(ftype* const address): data(address) {}

		SpectrumArray(SpectrumArray<ftype>& other) : data(other.data) {};

		SpectrumArray(SpectrumArray& other) : data(other.data) {};

		//not sure how i feel about this.
		SpectrumArray& operator=(const SpectrumArray& other) = delete;
		/*{
			copy_data(other.data);
		}*/

		~SpectrumArray() {}

		//define some mathematical functions
		ftype& operator[](const size_t index)
		{
			assert(index < Spectrum::n());
			return data[index];
		}

		SpectrumArray& operator+=(const SpectrumArray& other)
		{
			BEGIN_SPECTRUM_LOOP(i)
				data[i] += other.data[i];
			END_SPECTRUM_LOOP
			return *this;
		}

		SpectrumArray& operator-=(const SpectrumArray& other)
		{
			BEGIN_SPECTRUM_LOOP(i)
				data[i] -= other.data[i];
			END_SPECTRUM_LOOP
			return *this;
		}

		SpectrumArray& operator*=(const SpectrumArray& other)
		{
			BEGIN_SPECTRUM_LOOP(i)
				data[i] *= other.data[i];
			END_SPECTRUM_LOOP
			return *this;
		}

		SpectrumArray& operator/=(const SpectrumArray& other)
		{
			BEGIN_SPECTRUM_LOOP(i)
				data[i] /= other.data[i];
			END_SPECTRUM_LOOP
			return *this;
		}

		SpectrumArray& operator+=(const SpectrumArray<ftype>& other);

		SpectrumArray& operator-=(const SpectrumArray<ftype>& other);

		SpectrumArray& operator*=(const SpectrumArray<ftype>& other);

		SpectrumArray& operator/=(const SpectrumArray<ftype>& other);

		SpectrumArray& operator+=(const SpectrumArray<const ftype*>& other);

		SpectrumArray& operator-=(const SpectrumArray<const ftype*>& other);

		SpectrumArray& operator*=(const SpectrumArray<const ftype*>& other);

		SpectrumArray& operator/=(const SpectrumArray<const ftype*>& other);

		SpectrumArray& operator*=(const ftype scalar)
		{
			BEGIN_SPECTRUM_LOOP(i)
				data[i] *= scalar;
			END_SPECTRUM_LOOP
			return *this;
		}

		SpectrumArray &operator/=(const ftype scalar)
		{
			const ftype inv = ftype(1) / scalar;
			BEGIN_SPECTRUM_LOOP(i)
				data[i] *= inv;
			END_SPECTRUM_LOOP
			return *this;
		}

		const bool threshold(const ftype value)const
		{
			BEGIN_SPECTRUM_LOOP(i)
				if (data[i] > value)
				{
					return true;
				}
			END_SPECTRUM_LOOP
			return false;
		}

		inline operator ftype* () { return data; }

		inline ftype* get_data()const { return data; }

		inline void clear() { memset(data, 0, Spectrum::n()); }
	};
	
	//one which we can never modify its data
	template<typename ftype>
	class SpectrumArray<const ftype*>
	{
	friend class SpectrumArray<ftype>;
	friend class SpectrumArray<ftype*>;

	private:
		const ftype *const data;
	public:
		SpectrumArray() = delete;

		SpectrumArray(const ftype* address):data(address) {}

		SpectrumArray(const SpectrumArray<ftype>& other) : data(other.data) {};

		SpectrumArray(const SpectrumArray& other) : data(other.data) {};

		SpectrumArray& operator=(const SpectrumArray& other) = delete;

		~SpectrumArray() {}

		//define some mathematical functions
		const ftype& operator[](const size_t index)const
		{
			assert(index < Spectrum::n());
			return data[index];
		}

		const bool threshold(const ftype value)const
		{
			BEGIN_SPECTRUM_LOOP(i)
				if (data[i] > value)
				{
					return true;
				}
			END_SPECTRUM_LOOP
			return false;
		}

		inline operator const ftype* () { return data; }

		inline const ftype *const get_data()const { return data; }
	};

	template<typename ftype>
	Optics::SpectrumArray<ftype>::SpectrumArray<ftype>(const SpectrumArray<ftype*>& other)
	{
		copy_data(other.get_data());
	}

	template<typename ftype>
	Optics::SpectrumArray<ftype>::SpectrumArray<ftype>(const SpectrumArray<const ftype*>& other)
	{
		copy_data(other.get_data());
	}


	template<typename ftype>
	SpectrumArray<ftype> operator+(const SpectrumArray<ftype>& v1, const SpectrumArray<ftype> &v2)
	{
		SpectrumArray<ftype> out(v1);
		out += v2;
		return out;
	}

	template<typename ftype>
	SpectrumArray<ftype> operator+(const SpectrumArray<ftype>& v1, const SpectrumArray<ftype*>& v2)
	{
		SpectrumArray<ftype> out(v1);
		out += v2;
		return out;
	}

	template<typename ftype>
	SpectrumArray<ftype> operator+(const SpectrumArray<ftype>& v1, const SpectrumArray<const ftype*>& v2)
	{
		SpectrumArray<ftype> out(v1);
		out += v2;
		return out;
	}

	template<typename ftype>
	SpectrumArray<ftype> operator+(const SpectrumArray<ftype*>& v1, const SpectrumArray<ftype*>& v2)
	{
		SpectrumArray<ftype> out(v2);
		out += v1;
		return out;
	}

	template<typename ftype>
	SpectrumArray<ftype> operator+(const SpectrumArray<ftype*>& v1, const SpectrumArray<const ftype*>& v2)
	{
		SpectrumArray<ftype> out(v2);
		out += v1;
		return out;
	}

	template<typename ftype>
	SpectrumArray<ftype> operator+(const SpectrumArray<ftype*>& v1, const SpectrumArray<ftype>& v2)
	{
		SpectrumArray<ftype> out(v2);
		out += v1;
		return out;
	}

	template<typename ftype>
	SpectrumArray<ftype> operator+(const SpectrumArray<ftype*>& v1, const SpectrumArray<const ftype>& v2)
	{
		SpectrumArray<ftype> out(v2);
		out += v1;
		return out;
	}

	
	template<typename ftype>
	SpectrumArray<ftype> operator-(const SpectrumArray<ftype>& v1, const SpectrumArray<ftype> &v2)
	{
		SpectrumArray<ftype> out(v1);
		out -= v2;
		return out;
	}

	template<typename ftype>
	SpectrumArray<ftype> operator-(const SpectrumArray<ftype>& v1, const SpectrumArray<ftype*>& v2)
	{
		SpectrumArray<ftype> out(v1);
		out -= v2;
		return out;
	}

	template<typename ftype>
	SpectrumArray<ftype> operator-(const SpectrumArray<ftype>& v1, const SpectrumArray<const ftype*>& v2)
	{
		SpectrumArray<ftype> out(v1);
		out -= v2;
		return out;
	}

	template<typename ftype>
	SpectrumArray<ftype> operator-(const SpectrumArray<ftype*>& v1, const SpectrumArray<ftype*>& v2)
	{
		SpectrumArray<ftype> out(v2);
		out -= v1;
		return out;
	}

	template<typename ftype>
	SpectrumArray<ftype> operator-(const SpectrumArray<ftype*>& v1, const SpectrumArray<const ftype*>& v2)
	{
		SpectrumArray<ftype> out(v2);
		out -= v1;
		return out;
	}

	template<typename ftype>
	SpectrumArray<ftype> operator-(const SpectrumArray<ftype*>& v1, const SpectrumArray<ftype>& v2)
	{
		SpectrumArray<ftype> out(v2);
		out -= v1;
		return out;
	}

	template<typename ftype>
	SpectrumArray<ftype> operator-(const SpectrumArray<ftype*>& v1, const SpectrumArray<const ftype>& v2)
	{
		SpectrumArray<ftype> out(v2);
		out -= v1;
		return out;
	}
	
	template<typename ftype>
	SpectrumArray<ftype> operator*(const SpectrumArray<ftype>& v1, const SpectrumArray<ftype> &v2)
	{
		SpectrumArray<ftype> out(v1);
		out *= v2;
		return out;
	}

	template<typename ftype>
	SpectrumArray<ftype> operator*(const SpectrumArray<ftype>& v1, const SpectrumArray<ftype*>& v2)
	{
		SpectrumArray<ftype> out(v1);
		out *= v2;
		return out;
	}

	template<typename ftype>
	SpectrumArray<ftype> operator*(const SpectrumArray<ftype>& v1, const SpectrumArray<const ftype*>& v2)
	{
		SpectrumArray<ftype> out(v1);
		out *= v2;
		return out;
	}

	template<typename ftype>
	SpectrumArray<ftype> operator*(const SpectrumArray<ftype*>& v1, const SpectrumArray<ftype*>& v2)
	{
		SpectrumArray<ftype> out(v2);
		out *= v1;
		return out;
	}

	template<typename ftype>
	SpectrumArray<ftype> operator*(const SpectrumArray<ftype*>& v1, const SpectrumArray<const ftype*>& v2)
	{
		SpectrumArray<ftype> out(v2);
		out *= v1;
		return out;
	}

	template<typename ftype>
	SpectrumArray<ftype> operator*(const SpectrumArray<ftype*>& v1, const SpectrumArray<ftype>& v2)
	{
		SpectrumArray<ftype> out(v2);
		out *= v1;
		return out;
	}

	template<typename ftype>
	SpectrumArray<ftype> operator*(const SpectrumArray<ftype*>& v1, const SpectrumArray<const ftype>& v2)
	{
		SpectrumArray<ftype> out(v2);
		out *= v1;
		return out;
	}
	
	template<typename ftype>
	SpectrumArray<ftype> operator*(const SpectrumArray<ftype*>& v, const ftype s)
	{
		SpectrumArray<ftype> out(v);
		out *= s;
		return out;
	}
	
	template<typename ftype>
	SpectrumArray<ftype> operator*(const SpectrumArray<const ftype*>& v, const ftype s)
	{
		SpectrumArray<ftype> out(v);
		out *= s;
		return out;
	}

	template<typename ftype>
	SpectrumArray<ftype> operator*(const SpectrumArray<ftype> &v, const ftype s)
	{
		SpectrumArray<ftype> out(v);
		out *= s;
		return out;
	}

	template<typename ftype>
	SpectrumArray<ftype> operator/(const SpectrumArray<ftype>& v, const ftype s)
	{
		SpectrumArray<ftype> out(v);
		out /= s;
		return out;
	}

	
	template<typename ftype>
	SpectrumArray<ftype> operator/(const SpectrumArray<ftype*>& v, const ftype s)
	{
		SpectrumArray<ftype> out(v);
		out /= s;
		return out;
	}

	
	template<typename ftype>
	SpectrumArray<ftype> operator/(const SpectrumArray<const ftype*>& v, const ftype s)
	{
		SpectrumArray<ftype> out(v);
		out /= s;
		return out;
	}

	template<typename ftype>
	SpectrumArray<ftype>& SpectrumArray<ftype>::operator+=(const SpectrumArray<ftype*>& other)
	{
		BEGIN_SPECTRUM_LOOP(i)
			data[i] += other.data[i];
		END_SPECTRUM_LOOP
		return *this;
	}

	template<typename ftype>
	SpectrumArray<ftype>& SpectrumArray<ftype>::operator-=(const SpectrumArray<ftype*>& other)
	{
		BEGIN_SPECTRUM_LOOP(i)
			data[i] -= other.data[i];
		END_SPECTRUM_LOOP
		return *this;
	}

	template<typename ftype>
	SpectrumArray<ftype>& SpectrumArray<ftype>::operator*=(const SpectrumArray<ftype*>& other)
	{
		BEGIN_SPECTRUM_LOOP(i)
			data[i] *= other.data[i];
		END_SPECTRUM_LOOP
		return *this;
	}

	template<typename ftype>
	SpectrumArray<ftype>& SpectrumArray<ftype>::operator/=(const SpectrumArray<ftype*>& other)
	{
		BEGIN_SPECTRUM_LOOP(i)
			data[i] /= other.data[i];
		END_SPECTRUM_LOOP
		return *this;
	}

	
	template<typename ftype>
	SpectrumArray<ftype>& SpectrumArray<ftype>::operator+=(const SpectrumArray<const ftype*>& other)
	{
		BEGIN_SPECTRUM_LOOP(i)
			data[i] += other.data[i];
		END_SPECTRUM_LOOP
		return *this;
	}

	template<typename ftype>
	SpectrumArray<ftype>& SpectrumArray<ftype>::operator-=(const SpectrumArray<const ftype*>& other)
	{
		BEGIN_SPECTRUM_LOOP(i)
			data[i] -= other.data[i];
		END_SPECTRUM_LOOP
		return *this;
	}

	template<typename ftype>
	SpectrumArray<ftype>& SpectrumArray<ftype>::operator*=(const SpectrumArray<const ftype*>& other)
	{
		BEGIN_SPECTRUM_LOOP(i)
			data[i] *= other.data[i];
		END_SPECTRUM_LOOP
		return *this;
	}

	template<typename ftype>
	SpectrumArray<ftype>& SpectrumArray<ftype>::operator/=(const SpectrumArray<const ftype*>& other)
	{
		BEGIN_SPECTRUM_LOOP(i)
			data[i] /= other.data[i];
		END_SPECTRUM_LOOP
		return *this;
	}

	template<typename ftype>
	SpectrumArray<ftype*>& SpectrumArray<ftype*>::operator+=(const SpectrumArray<ftype>& other)
	{
		BEGIN_SPECTRUM_LOOP(i)
			data[i] += other.data[i];
		END_SPECTRUM_LOOP
		return *this;
	}

	template<typename ftype>
	SpectrumArray<ftype*>& SpectrumArray<ftype*>::operator-=(const SpectrumArray<ftype>& other)
	{
		BEGIN_SPECTRUM_LOOP(i)
			data[i] -= other.data[i];
		END_SPECTRUM_LOOP
		return *this;
	}

	template<typename ftype>
	SpectrumArray<ftype*>& SpectrumArray<ftype*>::operator*=(const SpectrumArray<ftype>& other)
	{
		BEGIN_SPECTRUM_LOOP(i)
			data[i] *= other.data[i];
		END_SPECTRUM_LOOP(i)
		return *this;
	}

	template<typename ftype>
	SpectrumArray<ftype*>& SpectrumArray<ftype*>::operator/=(const SpectrumArray<ftype>& other)
	{
		BEGIN_SPECTRUM_LOOP(i)
			data[i] /= other.data[i];
		END_SPECTRUM_LOOP
		return *this;
	}
	
	template<typename ftype>
	SpectrumArray<ftype*>& SpectrumArray<ftype*>::operator+=(const SpectrumArray<const ftype *>& other)
	{
		BEGIN_SPECTRUM_LOOP(i)
			data[i] += other.data[i];
		END_SPECTRUM_LOOP
		return *this;
	}

	template<typename ftype>
	SpectrumArray<ftype*>& SpectrumArray<ftype*>::operator-=(const SpectrumArray<const ftype *>& other)
	{
		BEGIN_SPECTRUM_LOOP(i)
			data[i] -= other.data[i];
		END_SPECTRUM_LOOP
		return *this;
	}

	template<typename ftype>
	SpectrumArray<ftype*>& SpectrumArray<ftype*>::operator*=(const SpectrumArray<const ftype *>& other)
	{
		BEGIN_SPECTRUM_LOOP(i)
			data[i] *= other.data[i];
		END_SPECTRUM_LOOP(i)
		return *this;
	}

	template<typename ftype>
	SpectrumArray<ftype*>& SpectrumArray<ftype*>::operator/=(const SpectrumArray<const ftype *>& other)
	{
		BEGIN_SPECTRUM_LOOP(i)
			data[i] /= other.data[i];
		END_SPECTRUM_LOOP
		return *this;
	}
}

template <typename ftype>
std::ostream& operator<<(std::ostream& out, const Optics::SpectrumArray<ftype> s)
{
	out << "Spectrum Array: <";
	BEGIN_SPECTRUM_LOOP(i)
		out << s.get_data()[i] << (i == (n_spectrum_colours - 1) ? ">" : ", ");
	END_SPECTRUM_LOOP
	return out;
}


template <typename ftype>
std::ostream& operator<<(std::ostream& out, const Optics::SpectrumArray<ftype*> s)
{
	out << "Spectrum Array*: <";
	BEGIN_SPECTRUM_LOOP(i)
		out << s.get_data()[i] << (i == (n_spectrum_colours - 1) ? ">" : ", ");
	END_SPECTRUM_LOOP
		return out;
}


template <typename ftype>
std::ostream& operator<<(std::ostream& out, const Optics::SpectrumArray<const ftype*> s)
{
	out << "Spectrum Array*: <";
	BEGIN_SPECTRUM_LOOP(i)
		out << s.get_data()[i] << (i == (n_spectrum_colours - 1) ? ">" : ", ");
	END_SPECTRUM_LOOP
		return out;
}


#endif // !SPECTRUM_ARRAY_H
