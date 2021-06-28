#ifndef MATRIX_H
#define MATRIX_H

#include "Vector.h"
#include "MathUtil.h"

#include <iostream>
#include <tgmath.h>

namespace Maths
{
	template <typename ftype, size_t ncols, size_t nrows>
	class Matrix
	{
	private:
		//Vector<ftype, ncols> rows[nrows];  //essentially indirection but not. this class linearly houses the values in the matrix
		Vector<Vector<ftype, ncols>, nrows> rows;
	public:
		//initialise to zeros?
		Matrix():rows({}){}

		Matrix(const ftype value): rows(value) {}

		//nested array
		Matrix(const ftype** data)
		{
			for (size_t i = 0; i < nrows; i++)
			{
				for (size_t j = 0; j < ncols; j++)
				{
					rows[i][j] = data[i][j];
				}
			}
		}

		//nested array
		Matrix(const ftype data[nrows][ncols]): rows({})
		{
			for (size_t i = 0; i < nrows; i++)
			{
				for (size_t j = 0; j < ncols; j++)
				{
					rows[i][j] = data[i][j];
				}
			}
		}


		Matrix(const ftype data[nrows*ncols])
		{
			for (size_t i = 0; i < nrows; i++)
			{
				for (size_t j = 0; j < ncols; j++)
				{
					rows[i][j] = data[i*ncols + j];
				}
			}
		}


		//arry of vectors
		Matrix(const Vector<ftype, ncols> mrows[])
		{
			for (size_t i = 0; i < nrows; i++)
			{
				// use the assignment of the vector class
				rows[i] = mrows[i];
			}
		}

		Matrix(const std::initializer_list<Vector<ftype, ncols>> &list): rows({})
		{
			size_t index = 0;
			for (auto &&i : list)
			{
				rows[index] = i;
				index ++;
				if(index == nrows){break;}
			}
			
		}

		//another matrix
		Matrix(const Matrix& other): rows(other.rows){}

		//booleans

		//returns true if any elements are non-zero
		bool any()const
		{
			for (size_t i = 0; i < nrows; i++)
			{
				if(rows[i].any()){return true;}
			}
			return false;
		}


		//returns true if all elements are non-zero
		bool all()const
		{
			for (size_t i = 0; i < nrows; i++)
			{
				if(!rows[i].all()){return false;}
			}
			return true;
		}


		//operators
		Matrix& operator=(const Matrix& other)
		{
			rows = other.rows;
			return *this;
		}


		Vector<ftype, ncols>& operator[](const size_t index)
		{
			return rows[index];
		}

		const Vector<ftype, ncols>& operator[](const size_t index)const
		{
			return rows[index];
		}

		//+=
		Matrix& operator+=(const ftype scalar)
		{
			for (size_t i = 0; i < nrows; i++)
			{
				rows[i] += scalar;
			}
			return *this;
		}

		Matrix& operator+=(const Matrix& other)
		{
			for (size_t i = 0; i < nrows; i++)
			{
				rows[i] += other.rows[i];
			}
			return *this;
		}

		//-=
		Matrix& operator-=(const ftype scalar)
		{
			for (size_t i = 0; i < nrows; i++)
			{
				rows[i] -= scalar;
			}
			return *this;
		}

		Matrix& operator-=(const Matrix& other)
		{
			for (size_t i = 0; i < nrows; i++)
			{
				rows[i] -= other.rows[i];
			}
			return *this;
		}

		//*=
		Matrix& operator*=(const ftype scalar)
		{
			for (size_t i = 0; i < nrows; i++)
			{
				rows[i] *= scalar;
			}
			return *this;
		}

		Matrix& operator*=(const Matrix& other)
		{
			for (size_t i = 0; i < nrows; i++)
			{
				rows[i] *= other.rows[i];
			}
			return *this;
		}

		// /=
		Matrix& operator/=(const ftype scalar)
		{
			ftype multiplier = 1 / scalar;
			for (size_t i = 0; i < nrows; i++)
			{
				rows[i] *= multiplier;
			}
			return *this;
		}

		Matrix& operator/=(const Matrix& other)
		{
			for (size_t i = 0; i < nrows; i++)
			{
				rows[i] /= other.rows[i];
			}
			return *this;
		}

		//transposes this matrix
		Matrix<ftype, nrows, ncols> transpose()const
		{
			Matrix<ftype, nrows, ncols> out;
			for (size_t i = 0; i < nrows; i++)
			{
				for (size_t j = 0; j < ncols; j++)
				{
					out[j][i] = rows[i][j];
				}	
			}
		}
	};


	//do some matrix mathematics
	//matrix addition
	template<typename ftype, size_t nrows, size_t ncols>
	Matrix<ftype, ncols, nrows> operator+(const Matrix<ftype, ncols, nrows>& m1, const Matrix<ftype, ncols, nrows>& m2)
	{
		Matrix<ftype, ncols, nrows> out;
		for (size_t i = 0; i < nrows; i++)
		{
			out[i] = m1[i] + m2[i];
		}
		return out;
	}

	//Matrix -scalar addition
	template<typename ftype, size_t nrows, size_t ncols>
	Matrix<ftype, ncols, nrows> operator+(const Matrix<ftype, ncols, nrows>& m1, const ftype& scalar)
	{
		Matrix<ftype, ncols, nrows> out = m1;
		out += scalar;
		return out;
	}

	//scalar - Matrix addition
	template<typename ftype, size_t nrows, size_t ncols>
	Matrix<ftype, ncols, nrows> operator+(const ftype& scalar, const Matrix<ftype, ncols, nrows>& m2)
	{
		Matrix<ftype, ncols, nrows> out = m2;
		out += scalar;
		return out;
	}


	//Matrix subtraction
	template<typename ftype, size_t nrows, size_t ncols>
	Matrix<ftype, ncols, nrows> operator-(const Matrix<ftype, ncols, nrows>& m1, const Matrix<ftype, ncols, nrows>& m2)
	{
		Matrix<ftype, ncols, nrows> out;
		for (size_t i = 0; i < nrows; i++)
		{
			out[i] = m1[i] - m2[i];
		}
		return out;
	}


	template<typename ftype, size_t nrows, size_t ncols>
	Matrix<ftype, ncols, nrows> operator-(const Matrix<ftype, ncols, nrows>& m1, const ftype& scalar)
	{
		Matrix<ftype, ncols, nrows> out = m1;
		out -= scalar;
		return out;
	}


	template<typename ftype, size_t nrows, size_t ncols>
	Matrix<ftype, ncols, nrows> operator-(const ftype& scalar, const Matrix<ftype, ncols, nrows>& m2)
	{
		Matrix<ftype, ncols, nrows> out;

		for (size_t i = 0; i < nrows; i++)
		{
			out[i] = scalar - m2[i];
		}
		return out;
	}



	//element-wise multiplication
	template<typename ftype, size_t nrows, size_t ncols>
	Matrix<ftype, ncols, nrows> operator*(const Matrix<ftype, ncols, nrows>& m1, const Matrix<ftype, ncols, nrows>& m2)
	{
		Matrix<ftype, ncols, nrows> out;
		for (size_t i = 0; i < nrows; i++)
		{
			out[i] = m1[i] * m2[i];
		}
		return out;
	}


	template<typename ftype, size_t nrows, size_t ncols>
	Matrix<ftype, ncols, nrows> operator*(const Matrix<ftype, ncols, nrows>& m1, const ftype& scalar)
	{
		Matrix<ftype, ncols, nrows> out = m1;
		out *= scalar;
		return out;
	}

	template<typename ftype, size_t nrows, size_t ncols>
	Matrix<ftype, ncols, nrows> operator*(const ftype& scalar, const Matrix<ftype, ncols, nrows>& m2)
	{
		Matrix<ftype, ncols, nrows> out = m2;
		out *= scalar;
		return out;
	}

	//element-wise multiplication
	template<typename ftype, size_t nrows, size_t ncols>
	Matrix<ftype, ncols, nrows> operator/(const Matrix<ftype, ncols, nrows>& m1, const Matrix<ftype, ncols, nrows>& m2)
	{
		Matrix<ftype, ncols, nrows> out;
		for (size_t i = 0; i < nrows; i++)
		{
			out[i] = m1[i] / m2[i];
		}
		return out;
	}

	template<typename ftype, size_t nrows, size_t ncols>
	Matrix<ftype, ncols, nrows> operator/(const Matrix<ftype, ncols, nrows>& m1, const ftype& scalar)
	{
		Matrix<ftype, ncols, nrows> out = m1;
		out /= scalar;
		return out;
	}

	template<typename ftype, size_t nrows, size_t ncols>
	Matrix<ftype, ncols, nrows> operator/(const ftype& scalar, const Matrix<ftype, ncols, nrows>& m2)
	{
		Matrix<ftype, ncols, nrows> out = m2;

		for (size_t i = 0; i < nrows; i++)
		{
			out[i] = scalar / m2[i];
		}
		return out;
	}

	template <typename ftype, size_t dimension>
	Matrix<ftype, dimension, dimension> identity_matrix()
	{
		Matrix<ftype, dimension, dimension> out(0.0);
		for (size_t i = 0; i < dimension; i++)
		{
			out[i][i] = 1.0;
		}
		return out;
	}


	template <typename ftype>
	Matrix<ftype, 2, 2> rotation_matrix(const ftype& angle, const bool is_radians=true)
	{
		angle = is_radians ? angle : angle*degtorad<ftype>;
		const ftype data[4] = { cos(angle), sin(angle), -sin(angle), cos(angle) };
		return Matrix<double, 2, 2>(data);
	}

	template <typename ftype>
	Matrix<ftype, 3, 3> rotation_matrix(Vector<ftype, 3> angles, const bool is_radians=true)
	{
		angles = is_radians ? angles : angles * degtorad<ftype>;
		ftype cx = cos(angles[0]); ftype sx = sin(angles[0]);
		ftype cy = cos(angles[1]); ftype sy = sin(angles[1]);
		ftype cz = cos(angles[2]); ftype sz = sin(angles[2]);

		ftype data[3][3] = { {cy * cz, sz * cx - sx * sy * cx, sz * sx + cx * sy * cz},
							{-sz * cy, cx * cz + sx * sy * sz, cz * sx - sy * sz * cx},
							{-sy, -sx * cy, cy * cx} };

		return Matrix<ftype, 3, 3>(data);
	}
}

template<typename ftype, size_t nrows, size_t ncols>
std::ostream& operator<<(std::ostream& out, const Maths::Matrix<ftype, ncols, nrows>& matrix)
{
	out << "\n[";
	for (size_t i = 0; i < nrows; i++)
	{
		out << matrix[i];
		out << (i < nrows - 1 ?  ",\n" : "");
	}
	out << "]\n";
	return out;
}

typedef Maths::Matrix<float, 2, 2> Matrix22f;
typedef Maths::Matrix<float, 3, 3> Matrix33f;
typedef Maths::Matrix<float, 4, 4> Matrix44f;

typedef Maths::Matrix<double, 2, 2> Matrix22d;
typedef Maths::Matrix<double, 3, 3> Matrix33d;
typedef Maths::Matrix<double, 4, 4> Matrix44d;

int a = sizeof(Matrix33f);

#endif