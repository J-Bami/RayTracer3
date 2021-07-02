# RayTracer3
Custom variable-spectrum Raytracer

***IN PROGRESS***

This raytracer uses elementary physical principles and mathematics to (currently) render images.
It is formed mostly of templates which allow different floating point types to be used in its rendering calculations.

Features:
  - Custom spectra can be used, with up to 8 colours used per spectrum currently. This is all translated to RGB, however.
  - Additive Refraction, Diffuse and Specular reflection 

Current capabilities:
  Light Sources:
    Directional
    Point
   
   Surfaces:
    Triangles
    Spheres
    Planes
   
   Material Components:
    Uniform
    Checkered
    
   Materials:
    Any non zero combination of the properties:
      - absorption
      - diffuse reflectivity
      - specular reflectivity
      - refractivity & refractive index
      - transmissivity
     for each must be defined for a material, with respect to each colour in the spectrum
     
    Spectrum: (1-8 colours)
      - takes input of n colours with each colour defined as an RGB value

Currently compiling & running Raytracer3.cpp will attempt to write a bitmap to the hard drive
     
