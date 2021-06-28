#ifndef GEOMETRIC_TRIANGLE_H
#define GEOMETRIC_TRIANGLE_H

#include "Maths/Vector.h"
#include "AxisAlignedBoundingBox.h"
#include "Sphere.h"
#include <iostream>


namespace Geometry
{
	template<typename ftype, unsigned char dimension>
	class Triangle
	{
		typedef Maths::Vector<ftype, dimension> fvector;
	private:
		union
		{
			struct
			{
				fvector p1;
				fvector p2;
				fvector p3;
			};

			fvector vertices[3];
		};
	public:
		Triangle() = delete;

		Triangle(const fvector& pt1, const fvector &pt2, const fvector &pt3) : p1(pt1), p2(pt2), p3(pt3) {}

		Triangle(const fvector* points)
		{
			vertices[0] = points[0];
			vertices[1] = points[1];
			vertices[2] = points[2];
		}

		Triangle(const Triangle& tri) : p1(tri.p1), p3(tri.p3), p2(tri.p2) {}

		~Triangle() {}

		const fvector get_center()const
		{
			return (p1 + p2 + p3) / 3;
		}

		inline const fvector& get_vertex(const size_t index)const
		{ 
			assert(index < 3);
			return vertices[index];
		}

		inline void set_vertex(const fvector vertex, const size_t index)
		{
			assert(index < 3);
			vertices[index] = vertex;
		}

		const AxisAlignedBoundingBox<ftype, dimension> make_aabb()const 
		{
			Maths::Vector<ftype, dimension> upper(-INFINITY);
			Maths::Vector<ftype, dimension> lower(INFINITY);

			for (size_t i = 0; i < dimension; i++)
			{
				for (size_t j = 0; j < 3; j++)
				{
					upper[j] = Maths::max(m_triangle.get_vertex(i)[j], upper[j]);
					lower[j] = Maths::min(m_triangle.get_vertex(i)[j], lower[j]);
				}
			}
			return AxisAlignedBoundingBox<ftype, dimension>(lower, upper);
		}
	};

	template<typename ftype>
	class Triangle<ftype, 3>
	{
		typedef Maths::Vector<ftype, 3> fvector;
	private:
		union
		{
			struct
			{
				fvector p1;
				fvector p2;
				fvector p3;
			};

			fvector vertices[3];
		};
	public:
		Triangle() = delete;

		Triangle(const fvector& pt1, const fvector& pt2, const fvector& pt3) : p1(pt1), p2(pt2), p3(pt3) {}

		Triangle(const fvector* points)
		{
			vertices[0] = points[0];
			vertices[1] = points[1];
			vertices[2] = points[2];
		}

		Triangle(const Triangle& tri) : p1(tri.p1), p2(tri.p2), p3(tri.p3) {}

		~Triangle() {}

		fvector get_center()const
		{
			return (p1 + p2 + p3) / ftype(3);
		}

		inline const fvector& get_vertex(const size_t index)const { return vertices[index]; }
		inline fvector& get_normal()const
		{
			return Maths::unit(Maths::cross(p1 - p2, p1 - p3));
		}

		
		const AxisAlignedBoundingBox<ftype, 3> make_aabb()const 
		{
			Maths::Vector<ftype, 3> upper(-INFINITY);
			Maths::Vector<ftype, 3> lower(INFINITY);

			for (size_t i = 0; i < 3; i++)
			{
				for (size_t j = 0; j < 3; j++)
				{
					upper[j] = Maths::max(get_vertex(i)[j], upper[j]);
					lower[j] = Maths::min(get_vertex(i)[j], lower[j]);
				}
			}
			return AxisAlignedBoundingBox<ftype, 3>(lower, upper);
		}

		const Sphere<ftype> make_bounding_sphere()const 
		{
			//find the center
			const fvector center = get_center();

			//find the max radius squared
			const ftype radius2 = Maths::max(Maths::mag2(center - p1), Maths::max(Maths::mag2(center - p2), Maths::mag2(center - p3)));
			return Sphere<ftype>(sqrt(radius2), center);
		}
	};

	template<typename ftype>
	class Triangle<ftype*, 3>
	{
		typedef Maths::Vector<ftype, 3> fvector;
	private:
		union
		{
			struct
			{
				fvector* p1;
				fvector* p2;
				fvector* p3;
			};

			fvector* vertices[3];
		};
	public:
		Triangle() = delete;

		Triangle(const fvector* pt1, const fvector* pt2, const fvector* pt3) : p1(pt1), p2(pt2), p3(pt3) {}

		Triangle(const fvector** points)
		{
			vertices[0] = points[0];
			vertices[1] = points[1];
			vertices[2] = points[2];
		}

		Triangle(const Triangle& tri) : p1(tri.p1), p2(tri.p2), p3(tri.p3) {}

		~Triangle() {}

		fvector get_center()const
		{
			return (*p1 + *p2 + *p3) / 3;
		}

		inline fvector& get_vertex(const size_t index) { return *(vertices[index]); }
		inline fvector& get_normal()const
		{
			return Maths::unit(Maths::cross(*p1 - *p2, *p1 - *p3));
		}
		
		const AxisAlignedBoundingBox<ftype, 3> make_aabb()const 
		{
			Maths::Vector<ftype, 3> upper(-INFINITY);
			Maths::Vector<ftype, 3> lower(INFINITY);

			for (size_t i = 0; i < 3; i++)
			{
				for (size_t j = 0; j < 3; j++)
				{
					upper[j] = Maths::max(m_triangle.get_vertex(i)[j], upper[j]);
					lower[j] = Maths::min(m_triangle.get_vertex(i)[j], lower[j]);
				}
			}
			return AxisAlignedBoundingBox<ftype, dimension>(lower, upper);
		}
		
		const Sphere<ftype> make_bounding_sphere()const
		{
			//find the center
			const fvector center = get_center();

			//find the max radius squared
			const ftype radius2 = Maths::max(Maths::mag2(center - *p1), Maths::max(Maths::mag2(center - *p2), Maths::mag2(center - *p3)));
			return Sphere<ftype>(sqrt(radius2), center);
		}
	};
}


template<class ftype, unsigned char dim>
std::ostream& operator<<(std::ostream& out, const Geometry::Triangle<ftype, dim>& tri)
{
	out << int(dim) << "-Triangle: " << tri.get_vertex(0) << ", " << tri.get_vertex(1) << ", " << tri.get_vertex(2);
	return out;
}


#endif