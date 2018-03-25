/***********************************************************
	
	Starter code for Assignment 3

***********************************************************/

#include <cstdlib>
#include "raytracer.h"
#include "texture_handler.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

Material EnvMapping (Color(0.3, 0.3, 0.3), Color(0.75164, 0.60648, 0.22648),
                Color(0.628281, 0.555802, 0.366065),
                51.2);
Material TextureMapping (Color(0.3, 0.3, 0.3), Color(0.75164, 0.60648, 0.22648),
                Color(0.628281, 0.555802, 0.366065),
                51.2);

int main(int argc, char* argv[])
{
	// Build your scene and setup your camera here, by calling 
	// functions from Raytracer.  The code here sets up an example
	// scene and renders it from two different view points, DO NOT
	// change this if you're just implementing part one of the 
	// assignment.  
	Raytracer raytracer;
	LightList light_list;
	Scene scene;   

    int width = 320;
    int height = 240;
    
	if (argc == 3) {
		width = atoi(argv[1]);
		height = atoi(argv[2]);
	}
	ini_texture_handler();
	// Define materials for shading.
	Material gold(Color(0.3, 0.3, 0.3), Color(0.75164,0.60648,0.22648),
		Color(0.628281, 0.555802, 0.366065),
		51.2);
	Material jade(Color(0, 0, 0), Color(0.54,0.89,0.63),
		Color(0.316228,0.316228,0.316228),
		12.8);

	// Defines a Square light source.
    // when light width is 0,light source is point light
	// SquareLight* sLight = new SquareLight(Point3D(0,0,5), Color(0.9,0.9,0.9),0);
	// light_list.push_back(sLight);

	PointLight* pLight = new PointLight(Point3D(0,0,5), Color(0.9,0.9,0.9));
	light_list.push_back(pLight);
	
	// Add a unit square into the scene with material mat.
	// SceneNode* sphere = new SceneNode(new UnitSphere(), &TextureMapping);
	// scene.push_back(sphere);

	//up side for cube
	SceneNode* cube1 = new SceneNode(new UnitSquare(), &TextureMapping);
	scene.push_back(cube1);
	// side for cube
	SceneNode* cube2 = new SceneNode(new UnitSquare(), &TextureMapping);
	scene.push_back(cube2);
	//side for cube
	SceneNode* cube3 = new SceneNode(new UnitSquare(), &TextureMapping);
	scene.push_back(cube3);

	//side for cube
	SceneNode* cube4 = new SceneNode(new UnitSquare(), &TextureMapping);
	scene.push_back(cube4);

	//side for cube
	SceneNode* cube5 = new SceneNode(new UnitSquare(), &TextureMapping);
	scene.push_back(cube5);
	
	SceneNode* plane = new SceneNode(new UnitSquare(), &jade);
	scene.push_back(plane);

	// Apply some transformations to the sphere and unit square.
	// double factor1[3] = { 1.0, 2.0, 1.0 };

	// double factor1[3] = { 1.0, 1.0, 1.0 }; //unit
	// sphere->translate(Vector3D(0, 0, -5));
	// sphere->rotate('x', -45);
	// sphere->rotate('z', 45);
	// sphere->scale(Point3D(0, 0, 0), factor1);
	double factor2[3] = { 6.0, 6.0, 6.0 };
	plane->translate(Vector3D(0, 0, -7));
	plane->rotate('z', 45);
	plane->scale(Point3D(0, 0, 0), factor2);


	double factor3[3] = { 1.0, 1.0, 1.0 }; 
	cube1->translate(Vector3D(0, 0, -5));
	cube1->scale(Point3D(0, 0, 0), factor3);

	
	double factor4[3] = { 1.0, 1.0, 1.0 }; 
	cube2->translate(Vector3D(0, 0.5, -5.5));
	cube2->rotate('x', -90);
	cube2->scale(Point3D(0, 0, 0), factor4);

	double factor5[3] = { 1.0, 1.0, 1.0 }; 
	cube3->translate(Vector3D(0, -0.5, -5.5));
	cube3->rotate('x', -90);
	cube3->scale(Point3D(0, 0, 0), factor5);

	double factor6[3] = { 1.0, 1.0, 1.0 }; 
	cube4->translate(Vector3D(0.5, 0, -5.5));
	cube4->rotate('x', -90);
	cube4->rotate('y', 90);
	cube4->scale(Point3D(0, 0, 0), factor6);

	double factor7[3] = { 1.0, 1.0, 1.0 }; 
	cube5->translate(Vector3D(-0.5, 0, -5.5));
	cube5->rotate('x', -90);
	cube5->rotate('y', 90);
	cube5->scale(Point3D(0, 0, 0), factor7);


	
	// Render the scene, feel free to make the image smaller for
	// testing purposes.	
	Camera camera1(Point3D(0, 0, 1), Vector3D(0, 0, -1), Vector3D(0, 1, 0), 60.0);
	Image image1(width, height);
	raytracer.render(camera1, scene, light_list, image1); //render 3D scene to image
	image1.flushPixelBuffer("view1.bmp"); //save rendered image to file

	// Render it from a different point of view.
	Camera camera2(Point3D(4, 2, 1), Vector3D(-4, -2, -6), Vector3D(0, 1, 0), 60.0);
	Image image2(width, height);
	raytracer.render(camera2, scene, light_list, image2);
	image2.flushPixelBuffer("view2.bmp");

	// Free memory
	for (size_t i = 0; i < scene.size(); ++i) {
		delete scene[i];
	}

	for (size_t i = 0; i < light_list.size(); ++i) {
		delete light_list[i];
	}

	return 0;
}
