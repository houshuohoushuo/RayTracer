/***********************************************************
	
	Starter code for Assignment 3

***********************************************************/

#include <cstdlib>
#include "raytracer.h"
// #include "texture_handler.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <png.h>
#include <iostream>
#include <memory.h>
using namespace std;

Material EnvMapping (Color(0.3, 0.3, 0.3), Color(0.75164, 0.60648, 0.22648),
                Color(0.628281, 0.555802, 0.366065),
                51.2, 0.0);
Material TextureMapping (Color(0.3, 0.3, 0.3), Color(0.75164, 0.60648, 0.22648),
                Color(0.628281, 0.555802, 0.366065),
                51.2, 0.0);
int texture_width, texture_height,env_width, env_height;
GLubyte *wood,*env_texture;

int loadTexture(char *name, int &width, int &height, GLubyte **image) {
    int interlace_type;
    FILE *fp;
    fp = fopen(name, "rb");

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,NULL, NULL, NULL);
    if (png_ptr == NULL) {
        fclose(fp);
        return -1;
    }
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        fclose(fp);
        png_destroy_read_struct(&png_ptr, NULL, (png_infopp) NULL);
        return -1;
    }

    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info) {
      png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
      fclose(fp);
      return -1;
    }

    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 0);
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);

    png_uint_32 w, h;
    int bit_depth,color_type;
    png_get_IHDR(png_ptr, info_ptr, &w, &h, &bit_depth, &color_type, NULL, NULL, NULL);
    width = w;
    height = h;
 
    unsigned int row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    *image = (unsigned char*) malloc(row_bytes * height);
 
    png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);
 
    for (int i = 0; i < height; i++) {
        memcpy(*image+(row_bytes * (height-1-i)), row_pointers[i], row_bytes);
    }
 
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    fclose(fp);
    return 1;
}

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
	// ini_texture_handler();
	char texture[]="ball.png";
    loadTexture(texture, texture_width, texture_height,&wood);
    char env_image[]="torontodowntown.png";
    loadTexture(env_image, env_width, env_height,&env_texture);
	// Define materials for shading.
	Material gold(Color(0.3, 0.3, 0.3), Color(0.75164,0.60648,0.22648),
		Color(0.628281, 0.555802, 0.366065),
		51.2,0.0);
	Material jade(Color(0, 0, 0), Color(0.54,0.89,0.63),
		Color(0.316228,0.316228,0.316228),
		12.8,0.0);

	// Defines a Square light source.
    // when light width is 0,light source is point light
	// SquareLight* sLight = new SquareLight(Point3D(0,0,5), Color(0.9,0.9,0.9),0);
	// light_list.push_back(sLight);

	PointLight* pLight = new PointLight(Point3D(0,0,5), Color(0.9,0.9,0.9));
	light_list.push_back(pLight);

	
	// Add a unit square into the scene with material mat.
	// SceneNode* cone = new SceneNode(new UnitCone(), &gold);
	// scene.push_back(cone);

	SceneNode* sphere = new SceneNode(new UnitSphere(), &EnvMapping);
	scene.push_back(sphere);

	
	SceneNode* plane = new SceneNode(new UnitSquare(), &jade);
	scene.push_back(plane);



	// Apply some transformations to the sphere and unit square.
	// double factor1[3] = { 1.0, 2.0, 1.0 };

	double factor1[3] = { 1.0, 1.0, 1.0 }; //unit
	sphere->translate(Vector3D(0, 0, -5));
	sphere->rotate('x', -45);
	sphere->rotate('z', 45);
	sphere->scale(Point3D(0, 0, 0), factor1);

	// double factor3[3] = { 1.0, 1.0, 1.0 }; //unit
	// cone->translate(Vector3D(0, 0, -5));
	// // cone->rotate('x', -45);
	// // cone->rotate('z', 45);
	// cone->scale(Point3D(0, 0, 0), factor3);


	double factor2[3] = { 6.0, 6.0, 6.0 };
	plane->translate(Vector3D(0, 0, -7));
	plane->rotate('z', 45);
	plane->scale(Point3D(0, 0, 0), factor2);



	
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


