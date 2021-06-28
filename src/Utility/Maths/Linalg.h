#ifndef LINALG_H
#define LINALG_H

#include <iostream>
#include "Matrix.h"
#include "MathUtil.h"
//matrix multiplication

//minimum limit...

static constexpr float tol = 1e-6f;

enum LINALG_ERRORS
{
	SINGULAR_MATRIX
};

//this is also defined in Utility.
template <typename type>
void inline swap(type& a, type& b)
{
    type temp = a;
    a = b;
    b = temp;
    return;
}


namespace Maths
{
	template <typename ftype, size_t dim1, size_t dim2, size_t dim3>
	Matrix<ftype, dim3, dim1> matmul(const Matrix<ftype, dim2, dim1>& m1, const Matrix<ftype, dim3, dim2>& m2)
	{
		Matrix<ftype, dim3, dim1> out;
		for (size_t i = 0; i < dim1; i++)
		{
			for (size_t j = 0; j < dim3; j++)
			{
				ftype result = 0;
				for (size_t k = 0; k < dim2; k++)
				{
					result += m1[i][k] * m2[k][j];
				}
				out[i][j] = result;
			}
		}
		return out;
	}

	//finds the determinant of the n*n matrix
	template <typename ftype, size_t dimension>
	ftype det(Matrix<ftype, dimension, dimension> matrix)
	{
		//we actually want to copy the matrix in, because this function will swap rows.
		//we convert this matrix to be lower-triangular...
		ftype determinant = ftype(1.0);
		
		//before we do the calculation, determine if this matrix is singular.
		for (size_t i = 0; i < dimension; i++)
		{
			//check to see if the first element is non zero:
			if (!matrix[i][i])
			{
				//for each row below this
				for (size_t j = i; j < dimension; j++)
				{
					//if this entry is non-zero
					if (matrix[j][i])
					{
						//swap the rows
						swap(matrix[i], matrix[j]);
						determinant = -determinant;
						break;
					}
					else
					{
						continue;
					}
					//if all entries are zero, return 0.0
					return 0.0;
				}
			}

			// now compute the determinant
			ftype divisor = matrix[i][i];
			determinant *= divisor;
			if (i == dimension - 1)
			{
				return determinant;
			}
			for (size_t j = i + 1; j < dimension; j++)
			{
				ftype multiplier = matrix[j][i];
				matrix[j] -= multiplier * matrix[i] / divisor;
			}
		}
		return determinant;
	}

	//finds the inverse of this matrix, explicitly by copying it in
	template<typename ftype, size_t dimension>
	Matrix<ftype, dimension, dimension> inv(Matrix<ftype, dimension, dimension> matrix)
	{
		Matrix<ftype, dimension, dimension> dummy = identity_matrix<ftype, dimension>();

		for (size_t i = 0; i < dimension; i++)
		{
			if (!matrix[i][i])
			{
			// looking for a non-zero entry in (i, j)
				if (i == dimension - 1)
				{
					//printf("matrix is singular. linalg error\n");
					throw LINALG_ERRORS::SINGULAR_MATRIX;
				}

				for (size_t j = 0; j < dimension; j++)
				{
					if (i == j) { continue; }
					if (matrix[j][i])
					{
						// std::cout << " swapping rows " << i << " and " << j << " for " << matrix << dummy;

						swap(matrix[i], matrix[j]);
						swap(dummy[i], dummy[j]);
						// std::cout << matrix << dummy;
						break;
					}
				}
			}


			//the actual inverse part
			const ftype inverse_divisor = 1/(matrix[i][i]);
			//divide row by i
			matrix[i] *= inverse_divisor;
			dummy[i] *= inverse_divisor;

			//for all rows
			for (size_t j = 0; j < dimension; j++)
			{
				if (j != i)
				{
					const ftype multiplier = matrix[j][i];
					matrix[j] -= multiplier * matrix[i];
					dummy[j] -= multiplier * dummy[i];
				}
			}
		}
		return dummy;
	}

	template <typename ftype, size_t dimension>
	Vector<ftype, dimension> cross(const Vector<ftype, dimension> vectors[dimension - 1])
	{
		Vector<ftype, dimension> out(0.0);
		for (size_t i = 0; i < dimension; i++)
		{
			Matrix<ftype, dimension-1, dimension-1> det_matrix; 
			//construct the det matrix

			//for each vector
			for (size_t j = 0; j < dimension-1; j++)
			{
				//for each element in vector
				for (size_t k = 0; k < dimension; k++)
				{
					if (k < i) { det_matrix[j][k] = vectors[j][k]; }
					else if (k == i) { continue; }
					else { det_matrix[j][k - 1] = vectors[j][k]; }
				}
			}
			out[i] = det(det_matrix);
		}
		return out;
	}

	template <typename ftype>
	Vector<ftype, 3> cross(const Vector<ftype, 3>& v1, const Vector<ftype, 3>& v2)
	{
		return Vector<ftype, 3>{v1[1]*v2[2] - v1[2]*v2[1], v1[2]*v2[0] - v2[2]*v1[0], v1[0]*v2[1] - v2[0]*v1[1]};
	}
}
#endif