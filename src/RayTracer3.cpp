// RayTracer3.cpp : Defines the entry point for the application.
//

#define TEST
#include "RayTracer3.h"
//#include "SDL.h"
#include <iostream>
using namespace std;

/*We will start off with a whole bunch of tests that we can make for each folder*/

//optics

void colour_test()
{
	cout << "Printing Red, Green, and Blue: \n";
	cout << Red << "\n";
	cout << Green << "\n";
	cout << Blue << "\n";
}

void make_rgb_spectrum()
{
	Colour my_colours[3] = { Red, Green, Blue};
	Optics::Spectrum::initialise(my_colours, 3);	
	
}

void make_roygbiv_spectrum()
{
	Colour my_colours[7] = { Red, Orange, Yellow, Green, Blue, Indigo, Violet };
	Optics::Spectrum::initialise(my_colours, 7);
}

void make_greyscale_spectrum()
{
	Colour my_colours[1] = { White};
	Optics::Spectrum::initialise(my_colours, 1);	
}

void spectrum_test() // initialises an roygbiv spectrum
{
	make_roygbiv_spectrum();

	BEGIN_SPECTRUM_LOOP(i)
		cout << '\n' << Optics::Spectrum::get_colour(i);
	END_SPECTRUM_LOOP
}

void spectrum_array_test()
{
	make_rgb_spectrum();
	using sarray = Optics::SpectrumArray<double>;
	using psarray = Optics::SpectrumArray<double*>;

	sarray a1({ 1, 1, 0 });
	double a_list[] = { 3.0, 1.0, 9.0 };
	psarray a2(a_list);
	psarray a3(a_list);
	double a = 10.0;

	//vigourously test the operator overloads...

	cout << "\ntesting the arithmetic overloads of " << a1 << " & " << a2 << "\n";
	cout << a1 << " + " << a2 << " = " << (a1 + a2) << '\n';
	cout << a2 << " + " << a1 << " = " << (a2 + a1) << '\n';
	cout << a1 << " - " << a2 << " = " << (a1 - a2) << '\n';
	cout << a2 << " - " << a1 << " = " << (a2 - a1) << '\n';


	cout << "\nTesting operator overloads of SpectrumArray a1 = " << a1 << '\n';
	cout << a1 << "+=" << a1 << ": " << (a1 += a1, a1) << '\n';
	cout << a1 << "+=" << 1 << ": " << (a1 += 1, a1) << '\n';
	cout << a1 << "-=" << a1 << ": " << (a1 -= a1, a1) << '\n';
	cout << a1 << "-=" << 1 << ": " << (a1 -= 1, a1) << '\n';
	cout << a1 << "*=" << a1 << ": " << (a1 *= a1, a1) << '\n';
	cout << a1 << "*=" << 5 << ": " << (a1 *= 5, a1) << '\n';
	cout << a1 << "/=" << 0.25 << ": " << (a1 /= 0.25, a1) << '\n';
	cout << a1 << "/=" << a1 << ": " << (a1 /= a1, a1) << '\n';

	cout << "\nTesting operator overloads of SpectrumArray* a2 = " << a2 << '\n';
	cout << a2 << "+=" << a2 << ": " << (a2 += a2, a2) << '\n';
	cout << a2 << "+=" << 1 << ": " << (a2 += 1, a2) << '\n';
	cout << a2 << "-=" << a2 << ": " << (a2 -= a2, a2) << '\n';
	cout << a2 << "-=" << 1 << ": " << (a2 -= 1, a2) << '\n';
	cout << a2 << "*=" << a2 << ": " << (a2 *= a2, a2) << '\n';
	cout << a2 << "*=" << 5 << ": " << (a2 *= 5, a2) << '\n';
	cout << a2 << "/=" << 0.25 << ": " << (a2 /= 0.25, a2) << '\n';
	cout << a2 << "/=" << a2 << ": " << (a2 /= a2, a2) << '\n';
}

void geometric_intersection_test()
{
	//we'll just test some aabbs
	aabb2f inner {{-1, -1}, {1, 1}};
	aabb2f outer {{-10, -10}, {10, 10}};
	aabb2f overlapping_inner {{0, 0},  {2, 2}};
	aabb2f touching_inner {{1, 1}, {3, 3}};

#define EXPAND(x) x
#define FIRST_ARG(x, ...) EXPAND(x)

#define RAY_INTERSECTION(shape, ...)\
std::cout << "\ntesting intersection between:\n" #shape ": " << shape << " and\n" << FIRST_ARG(__VA_ARGS__) << " :" << intersection(shape, __VA_ARGS__);

	std::cout << "\ntesting intersection between:\n" << outer << " and\n" << inner << " :\nresult = " << int(intersection(outer, inner));
	std::cout << "\ntesting intersection between:\n" << inner << " and\n" << outer << " :\nresult = " << int(intersection(inner, outer));

	//if we use a line shooting forward from the origin, we'll see if these intersections work
	Line3f my_ray({ 0, 0, 0 }, { 1, 0, 0 });
	Plane3f my_plane ({ 5, 0, 0 }, { {0, 0, 1}, {0, 1, 0} });
	Geometry::Sphere<float> my_sphere (2, { 12, 1, 1 });
	Geometry::Triangle<float, 3> my_tri({ 10, 0, -10 }, { 10, 10, 10 }, { 10, -10, 10 });

	RAY_INTERSECTION(my_plane, my_ray)
	//RAY_INTERSECTION(my_sphere, my_ray, Surface<float>::tolerance)
	RAY_INTERSECTION(my_tri, my_ray)
	
	Line3f my_other_ray({ 0, 0, 0 }, { 0, 1, 0 });

	RAY_INTERSECTION(my_plane, my_other_ray)
	//RAY_INTERSECTION(my_sphere, my_other_ray, Surface<float>::tolerance)
	RAY_INTERSECTION(my_tri, my_other_ray)

}

void materials_test()
{
	//lets define a glass thingy
	Colour my_colours[7] = { Red, Orange, Yellow, Green, Blue, Indigo, Violet };
	Optics::Spectrum::initialise(my_colours, 7);

	//mirrors tend to reflect a little more green
	float alpha[] = { 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05 };
	float sigma[] = { 0.1, 0.1, 0.1, 0.15, 0.1, 0.1, 0.1 };
	float delta[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
	float tau[] = { 0.85, 0.85, 0.85, 0.80, 0.85, 0.85, 0.85 };
	float ns[] = { 1.50, 1.51, 1.52, 1.53, 1.54, 1.55, 1.56 };

	Optics::Material<float> my_material( alpha, delta, sigma, tau, ns );
	cout << my_material;
	cout << "\n\nAbsorbent material: " << Optics::Absorber<float>();

}

void material_components_test()
{
	//lets define a glass thingy
	make_roygbiv_spectrum();

	//mirrors tend to reflect a little more green
	float alpha[] = { 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05 };
	float sigma[] = { 0.1, 0.1, 0.1, 0.15, 0.1, 0.1, 0.1 };
	float delta[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
	float tau[] = { 0.85, 0.85, 0.85, 0.80, 0.85, 0.85, 0.85 };
	float ns[] = { 1.50, 1.51, 1.52, 1.53, 1.54, 1.55, 1.56 };

	Optics::Material<float> my_material(alpha, delta, sigma, tau, ns);
	
	UniformComponent<float>my_component(&my_material);
	cout << *my_component.get_material({ 0, 0, 0 });
}


void surfaces_test()
{

	//initialise spectrum
	make_roygbiv_spectrum();

	//mirrors tend to reflect a little more green
	float alpha[] = { 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05 };
	float sigma[] = { 0.1, 0.1, 0.1, 0.15, 0.1, 0.1, 0.1 };
	float delta[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
	float tau[] = { 0.85, 0.85, 0.85, 0.80, 0.85, 0.85, 0.85 };
	float ns[] = { 1.50, 1.51, 1.52, 1.53, 1.54, 1.55, 1.56 };

	Optics::Material<float> my_material(alpha, delta, sigma, tau, ns);
	UniformComponent<float>my_component(&my_material);

	//make a ray
	Line3f ray({0, 0, 0}, {1, 0, 0});

	//make some surfaces
	Plane<float> my_plane(
		Vector3f({100, 0, 0}), 
		Vector3f({0, 1, 0}), 
		Vector3f({0, 0, 1}), 
		&my_component);

	Sphere<float> my_sphere(
		5.f,
		{10.f, 0.f, 0.f},
		&my_component);

	Triangle<float> my_tri(
		Vector3f({15.f, 0, 10}), 
		Vector3f({15.f, -5, -10}), 
		Vector3f({15.f, 5, -10}), 
		&my_component);

	Surface<float>* surfaces[] {&my_plane, &my_sphere, &my_tri};
	const size_t surfaces_length = sizeof(surfaces)/sizeof(Surface<float>*);

#define SURFACE_TEST_MACRO(function, ...)\
	std::cout << "Testing " #function "(" #__VA_ARGS__ ") on surfaces: \n"; \
	for (size_t i = 0; i < surfaces_length; i++)\
	{\
		std::cout << surfaces[i]->function(__VA_ARGS__) << " for i = " << i << "\n";\
	}

	SURFACE_TEST_MACRO(make_aabb)
	SURFACE_TEST_MACRO(make_bounding_sphere)
	SURFACE_TEST_MACRO(first_intersection, ray)
	SURFACE_TEST_MACRO(normal, Vector3f({ 0, 0, 0 }))

	
	const size_t count = 10000;
	Sphere<float>** my_spheres = new Sphere<float> * [count];
	//make a bunch of spheres
	for (size_t i = 0; i < count; i++)
	{
		my_spheres[i] = new Sphere<float>(0.1f, Vector3f({ float(i), 0, 0 }), &my_component);
	}

	//test the surface cull
	Geometry::AxisAlignedBoundingBox<float, 3>my_aabb({0, 0, 0}, {count >> 1, 3, 3});
	Set<Surface<float>*> my_set = Surface<float>::surface_cull(my_aabb);
	std::cout << "\n\nSurface cull ptrs: ";
	for (size_t i = 0; i < my_set.get_size(); i++)
	{
		std::cout << my_set[i] << " ";
	}
}

void light_tests(const size_t n)
{
	//initialise spectrum
	make_roygbiv_spectrum();

	//make two point and directional lights
	DirectionalLight<float> upper_left(Vector3f({ 0, 1, 1 }), 10);
	DirectionalLight<float> upper_right(Vector3f({ 0, -1, 1 }), 10);

	PointLight<float> left(Vector3f({ 0, 3, 0 }), 100);
	PointLight<float> right(Vector3f({ 0, -3, 0 }), 100);

	//mirrors tend to reflect a little more green
	float alpha[] = { 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05 };
	float sigma[] = { 0.1, 0.1, 0.1, 0.15, 0.1, 0.1, 0.1 };
	float delta[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
	float tau[] = { 0.85, 0.85, 0.85, 0.80, 0.85, 0.85, 0.85 };
	float ns[] = { 1.50, 1.51, 1.52, 1.53, 1.54, 1.55, 1.56 };

	Optics::Material<float> my_material(alpha, delta, sigma, tau, ns);

	UniformComponent<float>my_component(&my_material);

	//make a bunch of surfaces and do the illumination or something
	Sphere<float>** surfaces = new Sphere<float>* [n];
	for (size_t i = 0; i < n; i++)
	{
		surfaces[i] = new Sphere<float>(1, { float(i), 0, 0 }, &my_component);
	}

	Plane3f geom ({ 0.f, 0.f, 0.f }, { {1.f, 0.f, 0.f}, {0.f, 1.f, 0.f} });
	Plane<float> my_plane(geom, &my_component);

	DirectionalLight<float> light1({ 0, 0, 1.f }, 100);
	PointLight<float> light2({ 0, 0, 10 }, 100);
}

void bitmap_test(const char filename[], const uint16_t h = 256, const uint16_t v = 256)
{
	const size_t size = size_t(h) * size_t(v);
	uint32_t* bitmap = new uint32_t[size];

	for (uint32_t i = 0; i < size; i++)
	{
		bitmap[i] = Violet.m_data;
	}
	write_bitmap32(filename, h, v, reinterpret_cast<uint8_t*>(bitmap));
	delete[] bitmap;
}


int main()
{
	//colour_test();
	//spectrum_test();
	//spectrum_array_test();
	//materials_test();
	//material_components_test();
	//geometric_intersection_test();
	//surfaces_test();
	//light_tests(5);
	//bitmap_test("C:/Users/jbambigboye/Desktop/bitmaps/my_bitmap.bmp");
	camera_tests<float>("C:/Users/jbambigboye/Desktop/bitmaps/raytracer3.bmp", 1, 595, 1.0, 2*1600, 2*900);
	std::cout << "\nfinished, press enter to close window.";

	//int a = 0;
	//cin >> a;
	return 0;
}
