#ifndef QUARTERNION_H
#define QUARTERNION_H

#include "Linalg.h"

namespace Maths
{
	template <typename ftype>
	struct Quarternion
	{
		union
		{
			struct
			{
				ftype a;
				ftype b;
				ftype c;
				ftype d;
			};

			ftype values[4];

			struct
			{
				ftype scalar;
				Vector<ftype, 3> vector;
			};

			Vector<ftype, 4> array;
		};

		Quarternion() {};

		Quarternion(const ftype& scalar_): a(scalar_), b(0), c(0), d(0){}  //acts as a typecast 

		Quarternion(const ftype a_, const ftype b_, const ftype c_, const ftype d_): a(a_), b(b_), c(c_), d(d_){}

		Quarternion(const ftype* const vals)
		{
			a = vals[0];
			b = vals[1];
			c = vals[2];
			d = vals[3];
		}

		Quarternion(const ftype scalar_, const Vector<ftype, 3>& vector_)
		{
			scalar = scalar_;
			vector = vector_;
		}

		Quarternion(const std::initializer_list<ftype>& list)
		{
			size_t i = 0;
			for (ftype item : list)
			{
				values[i] = item;
				i++;
				if (i == 4) { break; }
			}
		}

		/*
		Quarternion(const Quarternion& other)
		{
			scalar = other.scalar;
			vector = other.vector;
		}*/

		//destructor
		~Quarternion() {}

		bool operator==(const Quarternion& other)const
		{
			return (a == other.a) && (b == other.b) && (c == other.c) && (d == other.d);
		}
	
		Quarternion inverse()const
		{
			const ftype size = ftype(1) / (a*a + b*b + c*c + d*d);
			return Quarternion(a*size, -b*size, -c*size, -d*size);
		}

		Quarternion conjugate()const
		{
			return Quarternion(a, -b, -c, -d);
		}

		ftype norm()
		{
			return sqrt(a*a + b*b + c*c + d*d);
		}

		Quarternion& normalise()
		{
			const ftype multiplier = ftype(1)/norm();
			scalar *= multiplier;
			vector *= multiplier;
			return *this;
		}

		Quarternion reciprocal()const
		{
			return inverse();
		}

		Quarternion& operator+=(const Quarternion& other)
		{
			array += other.array;
			return *this;
		}

		Quarternion& operator-=(const Quarternion& other)
		{
			array -= other.array;
			return *this;
		}

		Quarternion& operator*=(const Quarternion& other)
		{
			const ftype scalar_ = other.scalar*scalar - dot(vector, other.vector);
			vector = cross(vector, other.vector) + scalar*other.vector + other.scalar*vector;
			scalar = scalar_;
			return *this;
		}

		Quarternion& operator/=(const Quarternion& other)
		{
			const Quarternion rec = other.inverse();
			return (*this *= rec);
		}

		Quarternion& operator+=(const ftype& other)
		{
			a += other;
			return *this;
		}

		Quarternion& operator-=(const ftype& other)
		{
			a -= other;
			return *this;
		}

		Quarternion& operator*=(const ftype& other)
		{
			array *= other;
			return *this;
		}

		Quarternion& operator/=(const ftype& other)
		{

			array /= other;
			return *this;
		}
	};
	
	template <typename ftype>
	Quarternion<ftype> operator+(const Quarternion<ftype>& q1, const Quarternion<ftype>& q2)
	{
		return Quarternion<ftype>(q1.scalar + q2.scalar, q1.vector + q2.vector);
	}

	template <typename ftype>
	Quarternion<ftype> operator-(const Quarternion<ftype>& q1, const Quarternion<ftype>& q2)
	{
		return Quarternion<ftype>(q1.scalar - q2.scalar, q1.vector - q2.vector);
	}

	template <typename ftype>
	Quarternion<ftype> operator*(const Quarternion<ftype>& q1, const Quarternion<ftype>& q2)
	{
		const ftype scalar = q1.scalar * q2.scalar + dot(q1.vector, q2.vector);
		const Vector<ftype, 3> vector = cross(q1.vector, q2.vector) + q1.scalar*q2.vector + q2.scalar*q1.vector;
		return Quarternion<ftype>(scalar, vector);
	}

	template <typename ftype>
	Quarternion<ftype> operator/(const Quarternion<ftype>& q1, const Quarternion<ftype>& q2)
	{
		const Quarternion<ftype> rec = q2.inverse();
		return q1 * rec;
	}

	template<typename ftype>
	Quarternion<ftype> operator+(const Quarternion<ftype>& q1, const ftype& q2)
	{
		Quarternion<ftype> out(q1);
		out.a += q2;
		return out;
	}

	template<typename ftype>
	Quarternion<ftype> operator+(const ftype& q1, const Quarternion<ftype>& q2)
	{
		Quarternion<ftype> out(q2);
		out.a += q1;
		return out;
	}


	template<typename ftype>
	Quarternion<ftype> operator-(const Quarternion<ftype>& q1, const ftype& q2)
	{
		Quarternion<ftype> out(q1);
		out.a -= q2;
		return out;
	}

	template<typename ftype>
	Quarternion<ftype> operator-(const ftype& q1, const Quarternion<ftype>& q2)
	{
		Quarternion<ftype> out(q2);
		out.a -= q1;
		return out;
	}

	template<typename ftype>
	Quarternion<ftype> operator*(const Quarternion<ftype>& q1, const ftype& q2)
	{
		return Quarternion<ftype>(q1.a * q2, q1.b * q2, q1.c * q2, q1.d * q2);
	}

	template<typename ftype>
	Quarternion<ftype> operator*(const ftype& q1, const Quarternion<ftype>& q2)
	{
		return Quarternion<ftype>(q2.a * q1, q2.b * q1, q2.c * q1, q2.d * q1);
	}


	template<typename ftype>
	Quarternion<ftype> operator/(const Quarternion<ftype>& q1, const ftype& q2)
	{
		const ftype rec = 1 / q2;
		return Quarternion<ftype>(q1.a * rec, q1.b * rec, q1.c * rec, q1.d * rec);
	}

	template<typename ftype>
	Quarternion<ftype> operator/(const ftype& q1, const Quarternion<ftype>& q2)
	{
		const ftype rec = 1 / q1;
		return Quarternion<ftype>(q2.a * rec, q2.b * rec, q2.c * rec, q2.d * rec);
	}

	template <typename ftype>
	Quarternion<ftype> rotation(const Vector<ftype, 3>& unit_axis, const ftype angle)
	{
		const ftype half_angle = 0.5 * angle;
		const ftype scalar = cos(half_angle);
		const Vector<ftype, 3> vector = unit_axis * -sin(half_angle);
		return Quarternion<ftype>(scalar, vector);
	}
}


//printing for io purposes
template <typename dtype>
std::ostream& operator<<(std::ostream& out, const Maths::Quarternion<dtype>& q)
{
	out << q.a << (q.b >= 0 ? " +" : ' ') << q.b << 'i'
		<< (q.c >= 0 ? " +" : ' ') << q.c << 'j'
		<< (q.d >= 0 ? " +" : ' ') << q.d << 'k';
	return out;
}

#endif