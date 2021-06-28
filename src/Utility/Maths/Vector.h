#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include <cstring>
#include <tgmath.h>
#include <stddef.h>

/*
Template vector class. provides a simple, fixed-size mathematical container for floating point types
*/
namespace Maths
{
	template <typename ftype, size_t dim>
	struct Vector
	{
		ftype values[dim];

		//default constructor - initialize all values to 0.0
		Vector(): values({}) {}

		Vector(const ftype value)
		{
			for (size_t i = 0; i < dim; i++)
			{
				values[i] = value;
			}
		}

		//from an array
		Vector(const ftype data[])
		{
			for (size_t i = 0; i < dim; i++)
			{
				values[i] = data[i];
			}
		}

		Vector(const std::initializer_list<ftype>& list)
		{
			size_t i = 0;
			for (ftype item : list)
			{
				values[i] = item;
				i++;
				if (i == dim) { break; }
			}
		}

		//from another vector; the default does this so we comment this out
		/*
		Vector(const Vector& other)
		{
			for (size_t i = 0; i < dim; i++)
			{
				values[i] = other.values[i];
			}
		}
		*/

		//destructor
		~Vector() {}

		bool operator==(const Vector& other)const
		{
			for (size_t i = 0; i < dim; i++)
			{
				if(values[i] != other.values[i])
				{
					return false;
				}
			}
			return true;
		}

		//copy constructor; apprently the default does this so we comment this out
		/*
		Vector& operator=(const Vector& other)
		{
			for (size_t i = 0; i < dim; i++)
			{
				values[i] = other.values[i];
			}
			return *this;
		}*/

		//+= assignment 
		Vector& operator+= (const Vector& other)
		{
			for (size_t i = 0; i < dim; i++)
			{
				values[i] += other.values[i];
			}
			return *this;
		}

		// += assignment 
		Vector& operator+= (const ftype& other)
		{
			for (size_t i = 0; i < dim; i++)
			{
				values[i] += other;
			}
			return *this;
		}

		//-= assignment 
		Vector& operator-= (const Vector& other)
		{
			for (size_t i = 0; i < dim; i++)
			{
				values[i] -= other.values[i];
			}
			return *this;
		}

		// -= assignment 
		Vector& operator-= (const ftype& other)
		{
			for (size_t i = 0; i < dim; i++)
			{
				values[i] *= other;
			}
			return *this;
		}

		//*= assignment 
		Vector& operator*= (const Vector& other)
		{
			for (size_t i = 0; i < dim; i++)
			{
				values[i] -= other.values[i];
			}
			return *this;
		}

		// *= assignment 
		Vector& operator*= (const ftype& other)
		{
			for (size_t i = 0; i < dim; i++)
			{
				values[i] *= other;
			}
			return *this;
		}

		///= assignment 
		Vector& operator/= (const Vector& other)
		{
			for (size_t i = 0; i < dim; i++)
			{
				values[i] /= other.values[i];
			}
			return *this;
		}

		// *= assignment 
		Vector& operator/= (const ftype& other)
		{
			for (size_t i = 0; i < dim; i++)
			{
				values[i] /= other;
			}
			return *this;
		}

		//any
		bool any()const
		{
			for (size_t i = 0; i < dim; i++)
			{
				if (values[i])
				{
					return true;
				}
			}
			return false;
		}

		//all
		bool all()const
		{
			for (size_t i = 0; i < dim; i++)
			{
				if (!values[i])
				{
					return false;
				}
			}
			return true;
		}

		ftype sum()const
		{
			ftype out = 0.0;
			for (size_t i = 0; i < dim; i++)
			{
				out += values[i];
			}
			return out;
		}

		ftype product()const
		{
			ftype out = 1.0;
			for (size_t i = 0; i < dim; i++)
			{
				out *= values[i];
			}
			return out;
		}

		void normalise();

		inline ftype& operator[](const size_t index)
		{
			return values[index];
		}

		inline const ftype& operator[](const size_t index)const
		{
			return values[index];
		}
	};

	//arithmetic for scalars and other vectors
	template <typename ftype, size_t dim>
	Vector<ftype, dim> operator-(const Vector<ftype, dim>& vec)
	{
		return vec * ftype(-1);
	}


	//vector-vector addition
	template <typename ftype, size_t dim>
	Vector<ftype, dim> operator+(const Vector<ftype, dim>& v1, const Vector<ftype, dim>& v2)
	{
		Vector<ftype, dim> out;
		for (size_t i = 0; i < dim; i++)
		{
			out.values[i] = v1.values[i] + v2.values[i];
		}
		return out;
	}


	//vector-scalar addition
	template <typename ftype, size_t dim>
	Vector<ftype, dim> operator+(const Vector<ftype, dim>& v1, const ftype& scalar)
	{
		Vector<ftype, dim> out;
		for (size_t i = 0; i < dim; i++)
		{
			out.values[i] = v1.values[i] + scalar;
		}
		return out;
	}

	//scalar - vector addition
	template <typename ftype, size_t dim>
	Vector<ftype, dim> operator+(const ftype& scalar, const Vector<ftype, dim>& v2)
	{
		Vector<ftype, dim> out;
		for (size_t i = 0; i < dim; i++)
		{
			out.values[i] = scalar + v2.values[i];
		}
		return out;
	}


	//vector-vector subtraction
	template <typename ftype, size_t dim>
	Vector<ftype, dim> operator-(const Vector<ftype, dim>& v1, const Vector<ftype, dim>& v2)
	{
		Vector<ftype, dim> out;
		for (size_t i = 0; i < dim; i++)
		{
			out.values[i] = v1.values[i] - v2.values[i];
		}
		return out;
	}


	//vector-scalar subtraction
	template <typename ftype, size_t dim>
	Vector<ftype, dim> operator-(const Vector<ftype, dim>& v1, const ftype& scalar)
	{
		Vector<ftype, dim> out;
		for (size_t i = 0; i < dim; i++)
		{
			out.values[i] = v1.values[i] - scalar;
		}
		return out;
	}

	//scalar - vector subtraction
	template <typename ftype, size_t dim>
	Vector<ftype, dim> operator-(const ftype& scalar, const Vector<ftype, dim>& v2)
	{
		Vector<ftype, dim> out;
		for (size_t i = 0; i < dim; i++)
		{
			out.values[i] = scalar - v2.values[i];
		}
		return out;
	}


	//vector-vector multiplication
	template <typename ftype, size_t dim>
	Vector<ftype, dim> operator*(const Vector<ftype, dim>& v1, const Vector<ftype, dim>& v2)
	{
		Vector<ftype, dim> out;
		for (size_t i = 0; i < dim; i++)
		{
			out.values[i] = v1.values[i] * v2.values[i];
		}
		return out;
	}


	//vector-scalar multiplication
	template <typename ftype, size_t dim>
	Vector<ftype, dim> operator*(const Vector<ftype, dim>& v1, const ftype& scalar)
	{
		Vector<ftype, dim> out;
		for (size_t i = 0; i < dim; i++)
		{
			out.values[i] = v1.values[i] * scalar;
		}
		return out;
	}

	//scalar - vector multiplication
	template <typename ftype, size_t dim>
	Vector<ftype, dim> operator*(const ftype& scalar, const Vector<ftype, dim>& v2)
	{
		Vector<ftype, dim> out;
		for (size_t i = 0; i < dim; i++)
		{
			out.values[i] = scalar * v2.values[i];
		}
		return out;
	}


	//vector-vector division
	template <typename ftype, size_t dim>
	Vector<ftype, dim> operator/(const Vector<ftype, dim>& v1, const Vector<ftype, dim>& v2)
	{
		Vector<ftype, dim> out;
		for (size_t i = 0; i < dim; i++)
		{
			out.values[i] = v1.values[i] / v2.values[i];
		}
		return out;
	}


	//vector-scalar division
	template <typename ftype, size_t dim>
	Vector<ftype, dim> operator/(const Vector<ftype, dim>& v1, const ftype& scalar)
	{
		const ftype inverse = 1 / scalar;
		Vector<ftype, dim> out;
		for (size_t i = 0; i < dim; i++)
		{
			out.values[i] = v1.values[i] * inverse;
		}
		return out;
	}

	//scalar - vector division
	template <typename ftype, size_t dim>
	Vector<ftype, dim> operator/(const ftype& scalar, const Vector<ftype, dim>& v2)
	{
		Vector<ftype, dim> out;
		for (size_t i = 0; i < dim; i++)
		{
			out.values[i] = scalar / v2.values[i];
		}
		return out;
	}


	//Vector - specific functions

	//very generic version of dot
	template <typename ftype, size_t dim>
	ftype dot(const ftype* v1, const ftype* v2)
	{
		ftype sum = 0;
		for (size_t i = 0; i < dim; i++)
		{
			sum += v1[i] * v2[i];
		}
		return sum;
	}

	//dot product
	template<typename ftype, size_t dim>
	ftype dot(const Vector<ftype, dim>& v1, const Vector<ftype, dim>& v2)
	{
		ftype sum = 0;
		for (size_t i = 0; i < dim; i++)
		{
			sum += v1.values[i] * v2.values[i];
		}
		return sum;
	}


	//magnitude^2
	template<typename ftype, size_t dim>
	ftype mag2(const Vector<ftype, dim>& vec)
	{
		ftype sum = 0;
		for (size_t i = 0; i < dim; i++)
		{
			sum += vec.values[i] * vec.values[i];
		}
		return sum;
	}

	template<class ftype, size_t dim>
	ftype mag(const Vector<ftype, dim>& vec)
	{
		return sqrt(mag2(vec));
	}

	template<class ftype, size_t dim>
	Vector<ftype, dim> unit(const Vector<ftype, dim>& vector)
	{
		ftype multiplier = 1.0f / mag<ftype>(vector);
		return multiplier * vector;
	}


	//equality
	template <typename ftype, size_t dim>
	bool operator==(const Vector<ftype, dim>& v1, const Vector<ftype, dim>& v2)
	{
		for (size_t i = 0; i < dim; i++)
		{
			if (v1[i] != v2[i]) { return false; }
		}
		return true;
	}

	template<typename ftype, size_t dim>
	void Vector<ftype, dim>::normalise()
	{
		*this = unit(*this);
	}



	//2D Specialisation
	template <typename ftype>
	struct Vector<ftype, 2>
	{
		union
		{
			struct
			{
				ftype x;
				ftype y;
			};

			ftype values[2];
		};

		//default constructor - initialize all values to 0.0
		Vector(const ftype value = 0): x(value), y(value){}

		Vector(const ftype xval, const ftype yval) :
			x(xval), y(yval){}

		//from an array
		Vector(const ftype data[])
		{
			memcpy(values, data, sizeof(values));
		}

		Vector(const std::initializer_list<ftype>& list)
		{
			size_t i = 0;
			for (ftype item : list)
			{
				values[i] = item;
				i++;
				if (i == 2) { break; }
			}
		}

		//from another vector
		/*
		Vector(const Vector& other)
		{
			memcpy(values, other.values, sizeof(values));
		}*/


		bool operator==(const Vector& other)const
		{
			return (x == other.x) && (y == other.y);
		}


		//copy assignment
		/*
		Vector& operator=(const Vector& other)
		{
			x = other.x;
			y = other.y;
			return *this;
		}*/

		//+= assignment 
		Vector& operator+= (const Vector& other)
		{
			x += other.x;
			y += other.y;
			return *this;
		}

		// += assignment 
		Vector& operator+= (const ftype& other)
		{
			x += other;
			y += other;
			return *this;
		}

		//-= assignment 
		Vector& operator-= (const Vector& other)
		{
			x -= other.x;
			y -= other.y;
			return *this;
		}

		// -= assignment 
		Vector& operator-= (const ftype& other)
		{
			x -= other;
			y -= other;
			return *this;
		}

		//*= assignment 
		Vector& operator*= (const Vector& other)
		{
			x *= other.x;
			y *= other.y;
			return *this;
		}

		// *= assignment 
		Vector& operator*= (const ftype& other)
		{
			x *= other;
			y *= other;
			return *this;
		}

		///= assignment 
		Vector& operator/= (const Vector& other)
		{
			x /= other.x;
			y /= other.y;
			return *this;
		}

		// /= assignment 
		Vector& operator/= (const ftype& other)
		{
			const ftype inverse = ftype(1) / other;
			x *= inverse;
			y *= inverse;
			return *this;
		}

		//any
		bool any()const
		{
			return (x || y);
		}

		//all
		bool all()const
		{
			return x && y;
		}

		ftype sum()const
		{
			return x + y;
		}

		ftype product()const
		{
			return x * y;
		}

		void normalise()
		{
			const ftype size = mag(*this);
			(*this) /= size;
		};

		ftype& operator[](const size_t index)
		{
			return values[index];
		}

		const ftype& operator[](const size_t index)const
		{
			return values[index];
		}
	};


	//arithmetic for scalars and other vectors
	template <typename ftype>
	Vector<ftype, 2> operator-(const Vector<ftype, 2>& vec)
	{
		return vec * -1.0f;
	}


	//vector-vector addition
	template <typename ftype>
	Vector<ftype, 2> operator+(const Vector<ftype, 2>& v1, const Vector<ftype, 2>& v2)
	{
		return Vector<ftype, 2>(v1.x + v2.x, v1.y + v2.y);
	}


	//vector-scalar addition
	template <typename ftype>
	Vector<ftype, 2> operator+(const Vector<ftype, 2>& v1, const ftype scalar)
	{
		return Vector<ftype, 2>(v1.x + scalar, v1.y + scalar);
	}

	//scalar - vector addition
	template <typename ftype>
	Vector<ftype, 2> operator+(const ftype& scalar, const Vector<ftype, 2>& v2)
	{
		return Vector<ftype, 2>(v2.x + scalar, v2.y + scalar);
	}


	//vector-vector subtraction
	template <typename ftype>
	Vector<ftype, 2> operator-(const Vector<ftype, 2>& v1, const Vector<ftype, 2>& v2)
	{
		return Vector<ftype, 2>(v1.x - v2.x, v1.y - v2.y);
	}


	//vector-scalar subtraction
	template <typename ftype>
	Vector<ftype, 2> operator-(const Vector<ftype, 2>& v1, const ftype& scalar)
	{
		return Vector<ftype, 2>(v1.x - scalar, v1.y - scalar);
	}

	//scalar - vector subtraction
	template <typename ftype>
	Vector<ftype, 2> operator-(const ftype& scalar, const Vector<ftype, 2>& v2)
	{
		return Vector<ftype, 2>(scalar - v2.x, scalar - v2.y);
	}


	//vector-vector multiplication
	template <typename ftype>
	Vector<ftype, 2> operator*(const Vector<ftype, 2>& v1, const Vector<ftype, 2>& v2)
	{
		return Vector<ftype, 2>(v1.x * v2.x, v1.y * v2.y);
	}


	//vector-scalar multiplication
	template <typename ftype>
	Vector<ftype, 2> operator*(const Vector<ftype, 2>& v1, const ftype scalar)
	{
		return Vector<ftype, 2>(v1.x * scalar, v1.y * scalar);
	}

	//scalar - vector multiplication
	template <typename ftype>
	Vector<ftype, 2> operator*(const ftype& scalar, const Vector<ftype, 2>& v2)
	{
		return Vector<ftype, 2>(v2.x * scalar, v2.y * scalar);
	}

	//vector-vector division
	template <typename ftype>
	Vector<ftype, 2> operator/(const Vector<ftype, 2>& v1, const Vector<ftype, 2>& v2)
	{
		return Vector<ftype, 2>(v1.x / v2.x, v1.y / v2.y);
	}


	//vector-scalar division
	template <typename ftype>
	Vector<ftype, 2> operator/(const Vector<ftype, 2>& v1, const ftype& scalar)
	{
		const ftype inv = ftype(1) / scalar;
		return Vector<ftype, 2>(v1*inv);
	}

	//scalar - vector division
	template <typename ftype>
	Vector<ftype, 2> operator/(const ftype& scalar, const Vector<ftype, 2>& v2)
	{
		return Vector<ftype, 2>(scalar / v2.x, scalar / v2.y);
	}

	//Vector - specific functions

	//dot product
	template<typename ftype>
	ftype dot(const Vector<ftype, 2>& v1, const Vector<ftype, 2>& v2)
	{
		return v1.x * v2.x + v1.y * v2.y;
	}


	//magnitude^2
	template<typename ftype>
	ftype mag2(const Vector<ftype, 2>& vec)
	{
		return vec.x * vec.x + vec.y * vec.y;
	}

	//Vector-3 specialisation
	template<typename ftype>
	struct Vector<ftype, 3>
	{
		union
		{
			struct
			{
				ftype x;
				ftype y;
				ftype z;
			};

			ftype values[3];
		};

		//default constructor - initialize all values to 0.0
		Vector(const ftype value = 0): x(value), y(value), z(value){}

		Vector(const ftype xval, const ftype yval, const ftype zval):
			x(xval), y(yval), z(zval){}

		//from an array
		Vector(const ftype data[])
		{
			for (size_t i = 0; i < 3; i++)
			{
				values[i] = data[i];
			}
		}

		Vector(const std::initializer_list<ftype>& list)
		{
			size_t i = 0;
			for (ftype item : list)
			{
				values[i] = item;
				i++;
				if (i == 3) { break; }
			}
		}

		//from another vector
		/*
		Vector(const Vector& other)
		{
			memcpy(values, other.values, sizeof(values));
		}
		*/

		//destructor
		~Vector() {}

		bool operator==(const Vector& other)
		{
			return (x == other.x) && (y == other.y) && (z == other.z);
		}

		//copy assignment
		/*
		Vector& operator=(const Vector& other)
		{
			x = other.x;
			y = other.y;
			z = other.z;
			return *this;
		}
		*/

		//+= assignment 
		Vector& operator+= (const Vector& other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
			return *this;
		}

		// += assignment 
		Vector& operator+= (const ftype& other)
		{
			x += other;
			y += other;
			z += other;
			return *this;
		}

		//-= assignment 
		Vector& operator-= (const Vector& other)
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
			return *this;
		}

		// -= assignment 
		Vector& operator-= (const ftype& other)
		{
			x -= other;
			y -= other;
			z -= other;
			return *this;
		}

		//*= assignment 
		Vector& operator*= (const Vector& other)
		{
			x *= other.x;
			y *= other.y;
			z *= other.z;
			return *this;
		}

		// *= assignment 
		Vector& operator*= (const ftype& other)
		{
			x *= other;
			y *= other;
			z *= other;
			return *this;
		}

		///= assignment 
		Vector& operator/= (const Vector& other)
		{
			x /= other.x;
			y /= other.y;
			z /= other.z;
			return *this;
		}

		// *= assignment 
		Vector& operator/= (const ftype& other)
		{
			const ftype inverse(1 / other);
			return (*this) *= inverse;
		}

		//any
		const bool any()const
		{
			return x || y || z;
		}

		//all
		const bool all()const
		{
			return x && y && z;
		}

		ftype sum()const
		{
			return x + y + z;
		}

		ftype product()const
		{
			return x * y * z;
		}

		void normalise()
		{
			(*this) /= sqrt(x*x + y*y + z*z);
		};

		ftype& operator[](const size_t index)
		{
			return values[index];
		}

		const ftype& operator[](const size_t index)const
		{
			return values[index];
		}
	};


	//vector-vector addition
	template <typename ftype>
	Vector<ftype, 3> operator+(const Vector<ftype, 3>& v1, const Vector<ftype, 3>& v2)
	{
		return Vector<ftype, 3>(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
	}

	//vector-scalar addition
	template <typename ftype>
	Vector<ftype, 3> operator+(const Vector<ftype, 3>& v1, const ftype scalar)
	{
		return Vector<ftype, 3>(v1.x + scalar, v1.y + scalar, v1.z + scalar);
	}

	//scalar - vector addition
	template <typename ftype>
	Vector<ftype, 3> operator+(const ftype& scalar, const Vector<ftype, 3>& v2)
	{
		return Vector<ftype, 3>(v2.x + scalar, v2.y + scalar, v2.z + scalar);
	}

	//vector-vector subtraction
	template <typename ftype>
	Vector<ftype, 3> operator-(const Vector<ftype, 3>& v1, const Vector<ftype, 3>& v2)
	{
		return Vector<ftype, 3>(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
	}

	//vector-scalar subtraction
	template <typename ftype>
	Vector<ftype, 3> operator-(const Vector<ftype, 3>& v1, const ftype& scalar)
	{
		return Vector<ftype, 3>(v1.x - scalar, v1.y - scalar, v1.z - scalar);
	}

	//scalar - vector subtraction
	template <typename ftype>
	Vector<ftype, 3> operator-(const ftype& scalar, const Vector<ftype, 3>& v2)
	{
		return Vector<ftype, 3>(scalar - v2.x, scalar - v2.y, scalar - v2.z);
	}


	//vector-vector multiplication
	template <typename ftype>
	Vector<ftype, 3> operator*(const Vector<ftype, 3>& v1, const Vector<ftype, 3>& v2)
	{
		return Vector<ftype, 3>(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
	}


	//vector-scalar multiplication
	template <typename ftype>
	Vector<ftype, 3> operator*(const Vector<ftype, 3>& v1, const ftype scalar)
	{
		return Vector<ftype, 3>(v1.x * scalar, v1.y * scalar, v1.z * scalar);
	}

	//scalar - vector multiplication
	template <typename ftype>
	Vector<ftype, 3> operator*(const ftype& scalar, const Vector<ftype, 3>& v2)
	{
		return Vector<ftype, 3>(v2.x * scalar, v2.y * scalar, v2.z * scalar);
	}

	//vector-vector division
	template <typename ftype>
	Vector<ftype, 3> operator/(const Vector<ftype, 3>& v1, const Vector<ftype, 3>& v2)
	{
		return Vector<ftype, 3>(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
	}


	//vector-scalar division
	template <typename ftype>
	Vector<ftype, 3> operator/(const Vector<ftype, 3>& v1, const ftype& scalar)
	{
		const ftype inv = ftype(1) / scalar;
		return Vector<ftype, 3>(v1 * inv);
	}

	//scalar - vector division
	template <typename ftype>
	Vector<ftype, 3> operator/(const ftype& scalar, const Vector<ftype, 3>& v2)
	{
		return Vector<ftype, 3>(scalar / v2.x, scalar / v2.y, scalar/ v2.z);
	}

	//Vector - specific functions

	//dot product
	template<typename ftype>
	ftype dot(const Vector<ftype, 3>& v1, const Vector<ftype, 3>& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z*v2.z;
	}


	//magnitude^2
	template<typename ftype>
	ftype mag2(const Vector<ftype, 3>& vec)
	{
		return (vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z);
	}

	//Vector-4 specialisation
	template<typename ftype>
	struct Vector<ftype, 4>
	{
		union
		{
			struct
			{
				ftype x;
				ftype y;
				ftype z;
                ftype t;
			};

			ftype values[4];
		};

		//default constructor - initialize all values to 0.0
		Vector(const ftype value = 0): x(value), y(value), z(value), t(value) {}

		Vector(const ftype xval, const ftype yval, const ftype zval, const ftype tval):
			x(xval), y(yval), z(zval), t(tval){}

		//from an array
		Vector(const ftype data[]): x(data[0]), y(data[1]), z(data[2]), t(data[3]){}

		Vector(const std::initializer_list<ftype>& list)
		{
			size_t i = 0;
			for (ftype item : list)
			{
				values[i] = item;
				i++;
				if (i == 4) { break; }
			}
		}

		//from another vector
		// Vector(const Vector& other): x(other.x), y(other.y), z(other.z), t(other.t){}

		//destructor
		~Vector() {}

		bool operator==(const Vector& other)
		{
			return (x == other.x) && (y == other.y) && (z == other.z) && (t == other.t);
		}

		//copy assignment
		/*
		Vector& operator=(const Vector& other)
		{
			x = other.x;
			y = other.y;
			z = other.z;
            t = other.t;
			return *this;
		}*/

		//+= assignment 
		Vector& operator+= (const Vector& other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
            t += other.t;
			return *this;
		}

		// += assignment 
		Vector& operator+= (const ftype& other)
		{
			x += other;
			y += other;
			z += other;
            t += other;
			return *this;
		}

		//-= assignment 
		Vector& operator-= (const Vector& other)
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
            t -= other.t;
			return *this;
		}

		// -= assignment 
		Vector& operator-= (const ftype& other)
		{
			x -= other;
			y -= other;
			z -= other;
            t -= other;
			return *this;
		}

		//*= assignment 
		Vector& operator*= (const Vector& other)
		{
			x *= other.x;
			y *= other.y;
			z *= other.z;
            t *= other.t;
			return *this;
		}

		// *= assignment 
		Vector& operator*= (const ftype& other)
		{
			x *= other;
			y *= other;
			z *= other;
            t *= other;
			return *this;
		}

		///= assignment 
		Vector& operator/= (const Vector& other)
		{
			x /= other.x;
			y /= other.y;
			z /= other.z;
            t /= other.t;
			return *this;
		}

		// *= assignment 
		Vector& operator/= (const ftype& other)
		{
			const ftype inverse(1 / other);
			return (*this) *= inverse;
		}

		//any
		const bool any()const
		{
			return x || y || z || t;
		}

		//all
		const bool all()const
		{
			return x && y && z && t;
		}

		ftype sum()const
		{
			return x + y + z + t;
		}

		ftype product()const
		{
			return x * y * z * t;
		}

		void normalise()
		{
			(*this) /= sqrt(x*x + y*y + z*z + t*t);
		};

		ftype& operator[](const size_t index)
		{
			return values[index];
		}

		const ftype& operator[](const size_t index)const
		{
			return values[index];
		}
	};


	//vector-vector addition
	template <typename ftype>
	Vector<ftype, 4> operator+(const Vector<ftype, 4>& v1, const Vector<ftype, 4>& v2)
	{
		return Vector<ftype, 4>(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.t + v2.t);
	}

	//vector-scalar addition
	template <typename ftype>
	Vector<ftype, 4> operator+(const Vector<ftype, 4>& v1, const ftype scalar)
	{
		return Vector<ftype, 4>(v1.x + scalar, v1.y + scalar, v1.z + scalar, v1.t + scalar);
	}

	//scalar - vector addition
	template <typename ftype>
	Vector<ftype, 4> operator+(const ftype& scalar, const Vector<ftype, 4>& v2)
	{
		return Vector<ftype, 4>(v2.x + scalar, v2.y + scalar, v2.z + scalar, v2.t + scalar);
	}

	//vector-vector subtraction
	template <typename ftype>
	Vector<ftype, 4> operator-(const Vector<ftype, 4>& v1, const Vector<ftype, 4>& v2)
	{
		return Vector<ftype, 4>(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.t - v2.t);
	}

	//vector-scalar subtraction
	template <typename ftype>
	Vector<ftype, 4> operator-(const Vector<ftype, 4>& v1, const ftype& scalar)
	{
		return Vector<ftype, 4>(v1.x - scalar, v1.y - scalar, v1.z - scalar, v1.t - scalar);
	}

	//scalar - vector subtraction
	template <typename ftype>
	Vector<ftype, 4> operator-(const ftype& scalar, const Vector<ftype, 4>& v2)
	{
		return Vector<ftype, 4>(scalar - v2.x, scalar - v2.y, scalar - v2.z, scalar - v2.t);
	}


	//vector-vector multiplication
	template <typename ftype>
	Vector<ftype, 4> operator*(const Vector<ftype, 4>& v1, const Vector<ftype, 4>& v2)
	{
		return Vector<ftype, 4>(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.t * v2.t);
	}


	//vector-scalar multiplication
	template <typename ftype>
	Vector<ftype, 4> operator*(const Vector<ftype, 4>& v1, const ftype scalar)
	{
		return Vector<ftype, 4>(v1.x * scalar, v1.y * scalar, v1.z * scalar, v1.t * scalar);
	}

	//scalar - vector multiplication
	template <typename ftype>
	Vector<ftype, 4> operator*(const ftype& scalar, const Vector<ftype, 4>& v2)
	{
		return Vector<ftype, 4>(v2.x * scalar, v2.y * scalar, v2.z * scalar, v2.t*scalar);
	}

	//vector-vector division
	template <typename ftype>
	Vector<ftype, 4> operator/(const Vector<ftype, 4>& v1, const Vector<ftype, 4>& v2)
	{
		return Vector<ftype, 4>(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.t/v2.t);
	}


	//vector-scalar division
	template <typename ftype>
	Vector<ftype, 4> operator/(const Vector<ftype, 4>& v1, const ftype& scalar)
	{
		const ftype inv = ftype(1) / scalar;
		return Vector<ftype, 4>(v1 * inv);
	}

	//scalar - vector division
	template <typename ftype>
	Vector<ftype, 4> operator/(const ftype& scalar, const Vector<ftype, 4>& v2)
	{
		return Vector<ftype, 4>(scalar / v2.x, scalar / v2.y, scalar/ v2.z, scalar/v2.t);
	}

	//Vector - specific functions

	//dot product
	template<typename ftype>
	ftype dot(const Vector<ftype, 4>& v1, const Vector<ftype, 4>& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z*v2.z + v1.t*v2.t;
	}

	//magnitude^2
	template<typename ftype>
	ftype mag2(const Vector<ftype, 4>& vec)
	{
		return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.t*vec.t;
	}
}

//printing for io purposes
template <typename dtype, size_t dim>
std::ostream& operator<<(std::ostream& out, const Maths::Vector<dtype, dim>& vec)
{
	out << "[";
	for (size_t i = 0; i < dim; i++)
	{
		out << vec.values[i] << (i < dim - 1 ? ", " : "");
	}
	out << "]";
	return out;
}

typedef Maths::Vector<float, 2> Vector2f;
typedef Maths::Vector<float, 3> Vector3f;
typedef Maths::Vector<float, 4> Vector4f;

typedef Maths::Vector<double, 2> Vector2d;
typedef Maths::Vector<double, 3> Vector3d;
typedef Maths::Vector<double, 4> Vector4d;

#endif