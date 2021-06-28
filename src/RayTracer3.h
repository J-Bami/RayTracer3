// RayTracer3.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include "Optics/Material.h"
#include "Optics/Conversions.h"
#include "Physics/MaterialComponents/UniformMaterial.h"
#include "Physics/MaterialComponents/DualCheckeredComponent.h"
#include "Physics/Interaction.h"
#include "Physics/Surfaces/Sphere.h"
#include "Physics/Surfaces/Plane.h"
#include "Physics/Surfaces/Triangle.h"
#include "Physics/LightSources/DirectionalLight.h"
#include "Physics/LightSources/PointLight.h"
#include "Scene/Canvas.h"
#include "Scene/Camera.h"
#include "Scene/Scene.h"
#include "File/bitmap.h"

#include "SDL.h"
#undef main

#include <iostream>
#include <thread>
#include <mutex>


template <typename ftype>
void camera_tests(
	const char filename[],
	const unsigned char n_threads,
	const unsigned short n_spheres,
	const ftype sphere_radius = ftype(1.0),
	const uint16_t h = 256,
	const uint16_t v = 256)
{
	//we'll make a lovely test environment 
	//initialise an rgb spectrum:
	typedef Maths::Vector<ftype, 3> fvector;
	make_rgb_spectrum();

	//make a sun at the equator at equinox
	DirectionalLight<ftype> sun0({0, 0.5, 1}, 1400.f);
	//PointLight<ftype> sun({ 9, 0, 10.f }, 1400.f);

	//make a couple of uniform material components:
	ftype glass_alpha[3] = { 0.05, 0.05, 0.05 };
	ftype glass_sigma[3] = { 0.1, 0.1, 0.1 };
	ftype glass_delta[3] = { 0.0, 0.0, 0.0 };
	ftype glass_tau[3] = { 0.85, 0.85, 0.85 };
	ftype glass_ns[3] = { 1.50, 1.53, 1.56 };

	Optics::Material<ftype> reflective(0, 0, 1, 0, 1);
	Optics::Material<ftype> diffusive(0, 1, 0, 0, 1);
	Optics::Material<ftype> glossy(0, 0.5, 0.5, 0, 1);
	Optics::Material<ftype> dark_glossy(0.5, 0.0, 0.5, 0, 1);
	Optics::Material<ftype> glass(glass_alpha, glass_delta, glass_sigma, glass_tau, glass_ns);

	Optics::Material<ftype> red_gloss = Optics::Coloured<ftype>(0, 0.5, 0.5, 0, 1, 0);
	Optics::Material<ftype> yellow_gloss = Optics::MultiColoured<ftype>(0, 0.5, 0.5, 0, 1, 1 | 2);
	Optics::Material<ftype> green_gloss = Optics::Coloured<ftype>(0, 0.5, 0.5, 0, 1, 1);
	Optics::Material<ftype> cyan_gloss = Optics::MultiColoured<ftype>(0, 0.5, 0.5, 0, 1, 4 | 2);
	Optics::Material<ftype> blue_gloss = Optics::Coloured<ftype>(0, 0.5, 0.5, 0, 1, 2);
	Optics::Material<ftype> magenta_gloss = Optics::MultiColoured<ftype>(0, 0.5, 0.5, 0, 1, 4 | 1);

	Optics::Material<ftype> red_transparent = Optics::Coloured<ftype>(0, 0.5, 0.5, 0.5, 1, 0);
	Optics::Material<ftype> green_transparent = Optics::Coloured<ftype>(0, 0.5, 0.5, 0.5, 1, 1);
	Optics::Material<ftype> blue_transparent = Optics::Coloured<ftype>(0, 0.5, 0.5, 0.5, 1, 2);

	const UniformComponent<ftype> reflective_component(&reflective);
	const UniformComponent<ftype> diffusive_component(&diffusive);
	const UniformComponent<ftype> glassy_component(&glass);
	const UniformComponent<ftype> glossy_component(&glossy);
	const UniformComponent<ftype> dark_glossy_component(&dark_glossy);

	const UniformComponent<ftype> red_gloss_component(&red_gloss);
	const UniformComponent<ftype> yellow_gloss_component(&yellow_gloss);
	const UniformComponent<ftype> green_gloss_component(&green_gloss);
	const UniformComponent<ftype> cyan_gloss_component(&cyan_gloss);
	const UniformComponent<ftype> blue_gloss_component(&blue_gloss);
	const UniformComponent<ftype> magenta_gloss_component(&magenta_gloss);

	UniformComponent<ftype> red_transparent_component(&red_transparent);
	UniformComponent<ftype> green_transparent_component(&green_transparent);
	UniformComponent<ftype> blue_transparent_component(&blue_transparent);

	DualCheckeredComponent<ftype> chessboard_component(8, &glossy, &reflective);

	//make a level plane
	Geometry::Space<ftype, 2, 3> my_plane({ 0, 0, 0 }, { {0.f, 1.f, 0}, {1.f, 0, 0} });
	Plane<ftype> ground(my_plane, &chessboard_component);

	std::cout << ground.make_aabb();

	Geometry::Space<ftype, 2, 3> my_plane2({ 10, 0, 0 }, { {0.f, 1.f, 0}, {0, 0, 1.f} });
	//Plane<ftype> wall(my_plane2, &diffusive_component);

	//make a whole bunch of spheres

	//* add another slash to the beginning of this

	const ftype forward_space = ftype(6 * sphere_radius);
	const ftype sideways_space = ftype(4.5 * sphere_radius);

	ftype current_x = 0;
	ftype current_y = 0;

	unsigned int n = 0;  //number of spheres per row,
	unsigned int counter = 0;
	

	for (size_t i = 0; i < n_spheres; i++)
	{
		const UniformComponent<ftype>* mat = nullptr;
		switch (i % 12)
		{
		case 0:
			mat = &red_gloss_component; break;
		case 1:
			mat = &yellow_gloss_component; break;
		case 2:
			mat = &green_gloss_component; break;
		case 3:
			mat = &cyan_gloss_component; break;
		case 4:
			mat = &blue_gloss_component; break;
		case 5:
			mat = &magenta_gloss_component; break;
		case 6:
			mat = &diffusive_component; break;
		case 7:
			mat = &reflective_component; break;
		case 8:
			mat = &glassy_component; break;
		case 9:
			mat = &red_transparent_component; break;
		case 10:
			mat = &green_transparent_component; break;
		case 11:
			mat = &blue_transparent_component; break;
		default:
			mat = &reflective_component; break;
		}

		fvector position{ current_x, current_y, sphere_radius };

		current_y += sideways_space;
		counter++;

		if (counter > n)
		{
			counter = 0;
			n++;
			current_y = -ftype(n) * ftype(0.5) *sideways_space;
			current_x = ftype(n) * forward_space;
		}
		new Sphere<ftype>(sphere_radius, position, mat); //(i & 1) ? &glassy_component: &reflective_component);
	}
	//*/

	//make a camera to look at everything
	const size_t n_pixels = size_t(h) * size_t(v);
	const ftype ar = ftype(h) / v;
	Camera<ftype>my_camera(h, ar, 70.f, { -20.f, 0.f, 7.f }, { 0.f, -15.f, 0.f });

	//do the raytracing loop:
	Scene::render<ftype>(my_camera, n_threads);
	const Canvas<ftype>& canvas = my_camera.get_canvas();
	uint32_t* my_bitmap = new uint32_t[n_pixels];

	//convert the ftype array to pixels:
	Optics::linear_conversion(
		canvas.get_data(),
		my_bitmap,
		n_pixels,
		canvas.get_max_intensity());

	write_bitmap32(filename, h, v, reinterpret_cast<uint8_t*>(my_bitmap));
}

// TODO: Reference additional headers your program requires here.
