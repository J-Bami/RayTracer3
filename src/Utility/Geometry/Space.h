#ifndef GEOMETRIC_SPACE_H
#define GEOMETRIC_SPACE_H


#include "Maths/MathUtil.h"
#include "Maths/Vector.h"

#include <cassert>

namespace Geometry
{
	enum SPACE_ERRORS
	{
		NON_NORMAL_AXES,
		NON_UNIT_VECTOR,
	};

	template<class ftype, unsigned char rank, unsigned char dim>
	class Space
	{
	public:
		static constexpr ftype tol = ftype(1e-6);
		typedef Maths::Vector<ftype, dim> fvector;
		typedef Maths::Vector<ftype, rank> rvector;
	private:
		//data
		fvector origin;
		fvector axes[rank];

		void normalise_axes()
		{
			for (size_t i = 0; i < rank; i++)
			{
				axes[i].normalise(); 
			}
		}
			
		void check_axes()const
		{
			for (size_t i = 0; i < rank; i++)
			{
				for (size_t j = i + 1; j < rank; j++)
				{
					if (Maths::modulus(dot(get_axis(j), get_axis(i))) > tol)
					{
						printf("non-normal axes\n");
						throw SPACE_ERRORS::NON_NORMAL_AXES;
					}
				}
			}
		}

		void set_axes(const fvector new_axes[rank])
		{
			for (size_t i = 0; i < rank; i++)
			{
				axes[i] = new_axes[i];
			}
			normalise_axes();
			check_axes();
		}

	public:
		Space()
		{
			fvector these_axes[rank];
			for (size_t i = 0; i < rank; i++)
			{
				these_axes[i][i] = ftype(1); 
			}
			set_axes(these_axes);
		}

		Space(const fvector& origin_, const fvector axes_[rank]) :
			origin(origin_)
		{
			set_axes(axes_);
		}

		Space(const fvector& origin_, const std::initializer_list<fvector>& list) : origin(origin_)
		{
			size_t i = 0;
			for (fvector item : list)
			{
				axes[i] = item; 
				i++; 
				if (i == rank) { break; }
			}
			normalise_axes();
			check_axes(); 
		}

		Space(const Space& other) :
			origin(other.origin)
		{
			memcpy(axes, other.axes, sizeof(axes));
		}

		Space& operator=(const Space &other)
		{
			origin = other.origin;
			memcpy(axes, other.axes, sizeof(axes));
			return *this;
		}

		//the default copy assignment & constructors are fine here.

		inline const fvector& get_origin()const { return origin; }
		inline const fvector& get_axis(const size_t index)const
		{
			assert(index < rank);
			return axes[index];
		}

		inline const fvector* get_axes(const size_t index)const
		{
			return axes;
		}

		//projects a point onto the Space, returning a point expressed in the local coordinates of the Space
		inline const rvector project(const fvector& point)const
		{
			rvector out;
			for (size_t i = 0; i < rank; i++)
			{
				out[i] = Maths::dot(axes[i], point - origin);
			}
			return out;
		}

		//turns a local coordinate into global coordinates 
		inline const fvector local_to_global(const rvector& local)const
		{
			fvector out = origin;
			for (size_t i = 0; i < rank; i++)
			{
				out += axis[i] * local[i];
			}
			return out;
		}

		inline const fvector displacement(const fvector& point)const
		{
			return point - local_to_global(project(point));
		}

		const ftype seperation2(const fvector& point)const
		{
			return Maths::mag2(displacement(point));
		}

		const ftype seperation(const fvector& point)const
		{
			return sqrt(seperation2(point));
		}
	};


	//specialisation for lines as these are very important
	template<typename ftype, unsigned char dim>
	class Space<ftype, 1, dim>
	{
	public:
		static constexpr ftype tol = ftype(1e-6);
		typedef Maths::Vector<ftype, dim> fvector;
	private:
		//data
		fvector origin;
		union
		{
			fvector axis;
			fvector axes[1];
		};
		
		inline void set_axis(const fvector& axis_)
		{
			axis = Maths::unit(axis_);
		}
	public:
		Space(): origin(), axis()
		{
			axis[0] = 1;
		};

		Space(const fvector& origin_, const fvector axis_, const bool is_unit_axis = false) :
			origin(origin_)
		{
			is_unit_axis ? (axis = axis_): set_axis(axis_);
		}

		Space(const fvector& origin_, const std::initializer_list<fvector>& list) : origin(origin_)
		{
			size_t i = 0;
			for (fvector item : list)
			{
				axes[i] = item;
				i++;
				if (i == 1) { break; }
			}
			axis.normalise();
		}

		Space(const Space& other): origin(other.origin), axis(other.axis){}

		Space& operator=(const Space& other)
		{
			origin = other.origin;
			axis = other.axis;
			return *this;
		}

		~Space(){}

		inline const fvector& get_origin()const { return origin; }
		inline const fvector& get_axis()const{return axis;}
		inline const fvector& get_axis(const size_t index) const 
		{
			assert(index == 0);
			return axis;
		}

		inline const fvector* get_axes(const size_t index)const
		{
			return axes;
		}

		//projects a point onto the Space, returning a point expressed in the local coordinates of the Space
		inline const ftype project(const fvector& point)const
		{
			return Maths::dot(axis, point - origin);
		}

		//turns a local coordinate into global coordinates 
		inline const fvector local_to_global(const ftype& local)const
		{
			return origin + local*axis;
		}

		inline const fvector displacement(const fvector& point)const
		{
			return point - local_to_global(project(point));
		}

		const ftype seperation2(const fvector& point)const
		{
			return Maths::mag2(displacement(point));
		}

		const ftype seperation(const fvector& point)const
		{
			return sqrt(seperation2(point));
		}
	};
}

template<class ftype, unsigned char rank, unsigned char dim>
std::ostream& operator<<(std::ostream& out, const Geometry::Space<ftype, rank, dim>& nspace)
{
	switch (rank)
	{
	case 0:
		out << "Node: ";
		break;
	case 1:
		out << "Line: ";
		break;
	case 2:
		out << "Plane: ";
		break;
	default:
		out << int(rank) << "-Space: ";
		break;
	}
	out << "Origin: " << nspace.get_origin();
	if (!rank) { return out; }
	out << ", Directions: ";
	for (Byte i = 0; i < rank; i++)
	{
		out << nspace.get_axis(i);
		out << (i == rank - 1 ? "" : ", ");
	}
	return out;
}

typedef Geometry::Space<float, 1, 2> Line2f;
typedef Geometry::Space<float, 1, 3> Line3f;
typedef Geometry::Space<float, 2, 3> Plane3f;

typedef Geometry::Space<double, 1, 2> Line2d;
typedef Geometry::Space<double, 1, 3> Line3d;
typedef Geometry::Space<double, 2, 3> Plane3d;


#endif // !Space_H
