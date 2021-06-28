/*
we can have a struct that is edited whilst editing that tells us the mi and max intensity values for use in processing the images.
*/

#ifndef CAMERA_H
#define CAMERA_H

#include "Maths/Linalg.h"
#include "Optics/SpectrumArray.h"
#include "Surfaces/Surface.h"
#include "Canvas.h"

#include <mutex>

#define CAMERA_DEBUG

template<typename ftype>
class Camera
{

public:		
	enum class CameraErrors
	{
		CanvasComplete
	};

	typedef Maths::Vector<ftype, 3> fvector;
	typedef Maths::Vector<ftype, 2> fvector2;
	typedef Geometry::Space<ftype, 1, 3> linef;

	struct SurfaceCoords
	{
		Surface<ftype>* surface;
		ftype distance;
		ftype distance2;
		union
		{
			struct
			{
				ftype min_longitude;
				ftype max_longitude;
			};
			fvector2 h_bounds;
		};
		union
		{
			struct
			{
				ftype min_latitude;
				ftype max_latitude;
			};
			fvector2 v_bounds;
		};
	};

private:
	const uint16_t h_res;
	const uint16_t v_res;
	uint16_t h_counter = 0;
	uint16_t v_counter = 0;

	const ftype h_angle;   //in radians, ALWAYS
	const ftype tan_h;
	const ftype aspect_ratio;
	const ftype v_angle;   //in radians, ALWAYS
	const ftype tan_v;
	const ftype c_h_angle; //cos(h_angle)
	const ftype c_v_angle; //cos(v_angle)
	
	fvector m_position;
	fvector m_rotation; //in radians, ALWAYS
	Maths::Matrix<ftype, 3, 3> directions;

	//Set<SurfaceCoords> coords;  //we have no use for this yet
	
	//where we send all our intensity data
	Canvas<ftype> canvas;

	void compute_rotation()
	{
		directions = Maths::matmul(
			Maths::rotation_matrix(m_rotation, true),
			Maths::identity_matrix<ftype, 3>());
	}

	//we can get the ray by using the angle off the center that we look at
	const linef get_ray_line()const 
	{
		const ftype delta_y = (ftype(h_counter << 1)/(h_res - 1) - 1)*tan_h; //replaced * 2 by  << 1
        const ftype delta_z = (ftype(v_counter << 1)/(v_res - 1) - 1)*tan_v;

        const fvector axis = Maths::unit(directions[0] + delta_y*directions[1] + delta_z*directions[2]);
        return linef(m_position, axis);
	}

public:
	Camera(
		const uint16_t horizontal_res,
		const ftype aspect_ratio_,
		const ftype viewing_angle_deg,
		const fvector& position,
		const fvector& rotation_deg
	):
	h_res(horizontal_res),
	v_res(h_res/aspect_ratio_),
	aspect_ratio(aspect_ratio_),
	h_angle(Maths::degtorad<ftype>*viewing_angle_deg),
	tan_h(tan(h_angle*ftype(0.5))),
	v_angle(h_angle/aspect_ratio),
	tan_v(tan(v_angle*0.5)),
	c_h_angle(cos(h_angle)),
	c_v_angle(cos(v_angle)),
	m_position(position),
	m_rotation(Maths::degtorad<ftype>*rotation_deg),
	canvas(h_res, v_res)
	{
		compute_rotation();
		//coords.reserve(Surface<ftype>::surface_count());
		//find_surface_coordinates();
#ifdef CAMERA_DEBUG
#define PRINT_MEMBER(member)\
std::cout << "\n" #member ": " << member;

PRINT_MEMBER(h_res)
PRINT_MEMBER(v_res)
PRINT_MEMBER(aspect_ratio)
PRINT_MEMBER(h_angle)
PRINT_MEMBER(v_angle)
PRINT_MEMBER(m_position)
PRINT_MEMBER(m_rotation)
PRINT_MEMBER(directions)
PRINT_MEMBER(tan_h)
PRINT_MEMBER(tan_v)

#endif
	}

	const uint16_t get_horizontal_resolution()const
	{
		return h_res;
	}

	const uint16_t get_vertical_resolution()const
	{
		return v_res;
	}

	//define some useful getters
	const ftype& get_horizontal_angle()const
	{
		return (ftype(h_counter << 1)/(h_res - 1) -  1) * ftype(0.5)*h_angle;
	}
	
	const ftype& get_vertical_angle()const
	{
		return (ftype(v_counter << 1)/(v_res - 1) -  1) * ftype(0.5)*v_angle;
	}

	const Canvas<ftype>& get_canvas()const
	{
		return canvas;
	}

	const Set<SurfaceCoords>& get_surface_coords()const
	{
		return coords;
	}

	const size_t n_pixels()const
	{
		return size_t(v_res) * size_t(h_res);
	}

	//we could also get the line for the ray by specifying the x, y
	const RayInfo<ftype> spawn_ray(size_t& rel_address, const Optics::SpectrumInt bits_set = Optics::all_colours)
	{
		const linef line = get_ray_line();
		rel_address = size_t(h_counter) + (size_t(h_res) * size_t(v_counter));
		if (h_counter == h_res)
		{
			if (v_counter == v_res)
			{
				throw CameraErrors::CanvasComplete;
			}
			else
			{
				h_counter = 0;
				v_counter++;
			}
		}
		else
		{
			h_counter++;
		}
		RayInfo<ftype> out(line, bits_set, 0, ftype(1.0));
		return out;
	}

	inline void write_to_canvas(const ftype* input_data, const size_t relative_address)
	{
#ifdef CAMERA_DEBUG
		if (relative_address > h_res * v_res)
		{
			static std::mutex m;
			m.lock();
			cout << "\nrelative address:" << relative_address;
			cout << "\nh_counter" << h_counter;
			cout << "\nv_counter" << v_counter;
			m.unlock();
		}
#endif CAMERA_DEBUG
		canvas.write_to_canvas(input_data, relative_address);
	}

	/*
	we should use this to construct the relative angular bounds. can run at initalialisation
	and when position/rotation changes
	as planes are the only objects with infs in their suurface, we can use that information
	to downcast and find the actual angular co-ordinates.
	*/
	void find_surface_coordinates()
	{
		//loop over all surfaces
		Set<Surface<ftype>*> out;
		const size_t n_surfaces = Surface<ftype>::surface_count();
	}

	//we can call this to loop over the surfaces when it's ready lol.
	Set<Surface<ftype>*> ray_cull() const
	{
		typedef Maths::Vector<ftype, 3> fvector;
		// we know the displacement from each surface sphere to the camera, we just need to find the
		//angle to the transformed y & z axes.

		//loop over all surfaces
		Set<Surface<ftype>*> out;
		const size_t n_surfaces = Surface<ftype>::surface_count();
		for (size_t i = 0; i < n_surfaces; i++)
		{
			//find the displacement vector and things
			auto info = Surface<ftype>::get_surface_infos()[i];
			if(info.m_sphere.get_radius() == INFINITY)
			{
				out.add(info.m_surface)
			}

			const fvector displacement = info.m_sphere.get_center() - m_position;
			const fvector on_z = displacement - directions[1]*(Maths::dot(directions[1], displacement));
			const fvector on_y = displacement - directions[2]*(Maths::dot(directions[2], displacement));

			//see if any of this object would be in view:
		}
		return out;
	}

	void reset()
	{
		h_counter = 0;
		v_counter = 0;
		canvas.reset();
	}

};

#endif // !PHYSICAL_CAMERA_H
