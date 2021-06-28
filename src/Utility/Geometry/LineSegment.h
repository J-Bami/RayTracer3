#ifndef LINE_SEGMENT_H
#define LINE_SEGMENT_H

#include "Maths/Vector.h"
#include "AxisAlignedBoundingBox.h"
namespace Geometry
{
	template<typename ftype, unsigned char dimension>
	class LineSegment
	{
		typedef Maths::Vector<ftype, dimension> fvector;
	private:
		fvector p1;
		fvector p2;
	public:
		LineSegment(const fvector& pt1, const fvector& pt2) : p1(pt1), p2(pt2) {}

		LineSegment(const LineSegment& other) : p1(other.p1), p2(other.p2) {}

		const AxisAlignedBoundingBox<ftype, dimension> make_aabb()const
		{
			return AxisAlignedBoundingBox<ftype, dimension>(pt1, pt2);
		}

		void set_p1(const fvector& pt) { p1 = pt; }
		void set_p2(const fvector& pt) { p2 = pt; }

		const fvector& get_p1()const { return p1; }
		const fvector& get_p2()const { return p2; }
	};
}




#endif // !LINE_SEGMENT_H



