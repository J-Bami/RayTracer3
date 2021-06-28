#ifndef GEOMETRIC_SPHERICAL_COORDINATE_SYSTEM_H
#define GEOMETRIC_SPHERICAL_COORDINATE_SYSTEM_H

#include "Maths/Vector.h"
/*
defines the coordinate system struct for the camera-centered coordinates...
we should be able to take in one of:
- a bounding box
- a bounding sphere
- a Space

use a position & direction representing the camera, and define the:

*/

namespace Geometry
{
	template<typename ftype>
	struct SphericalCoordinates
	{
		union
		{
			struct 
			{
				ftype r2;
				ftype h_angle;
				ftype v_angle;
			};

			struct
			{
				ftype dist2;
				ftype longitude;
				ftype latitude;
			};

			Maths::Vector<ftype, 3> data;
		};
	};

	template<typename ftype>
	struct SphericalBounds
	{
		typedef Maths::Vector<ftype, 2> fvector2;
		ftype dist2;
		union
		{
			struct
			{
				ftype min_longitude;
				ftype max_longitude;
			};
			fvector2 longitudes;
		};

		union
		{
			struct
			{
				ftype min_latitude;
				ftype max_latitude;
			};
			fvector2 latitudes;
		};
	};
}

#endif