/***********************************************************
	
	Starter code for Assignment 3

***********************************************************/

#include <cstdlib>
#include "raytracer.h"
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

Material env_m (Color(0.3, 0.3, 0.3), Color(0.75164, 0.60648, 0.22648),
                Color(0.628281, 0.555802, 0.366065),
                51.2, 0.0);
Material texture_m (Color(0.3, 0.3, 0.3), Color(0.75164, 0.60648, 0.22648),
                Color(0.628281, 0.555802, 0.366065),
                51.2, 0.0);
int texture_width, texture_height,env_width, env_height;
GLubyte *wood,*env_texture;

// Inspired by https://en.wikibooks.org/wiki/OpenGL_Programming/Intermediate/Textures
// -A simple libpng example
// This c++ code snippet is an example of loading a png image file into an opengl 
// texture object. It requires libpng and opengl to work. To compile with gcc, 
// link png glu32 and opengl32 . Most of this is taken right out of the libpng manual.
// There is no checking or conversion of the png format to the opengl texture format. 
// This just gives the basic idea
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

    int width = 640;
    int height = 480;
    
	if (argc == 3) {
		width = atoi(argv[1]);
		height = atoi(argv[2]);
	}


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
		12.8,0.7);

	// Defines a Square light source.
    // when light width is 0,light source is point light
     SquareLight* sLight = new SquareLight(Point3D(0,0,5), Color(0.9,0.9,0.9),3);
     light_list.push_back(sLight);

//    PointLight* pLight = new PointLight(Point3D(0,0,5), Color(0.9,0.9,0.9));
//    light_list.push_back(pLight);

	
	// Add a unit square into the scene with material mat.
     SceneNode* cone = new SceneNode(new UnitCone(), &gold);
     scene.push_back(cone);

    // SceneNode* sphere = new SceneNode(new UnitSphere(), &env_m);
    // scene.push_back(sphere);

    SceneNode* plane = new SceneNode(new UnitSquare(), &jade);
    scene.push_back(plane);
    // SceneNode* sphere = new SceneNode(new UnitSphere(), &env_m);
    // scene.push_back(sphere);
    // SceneNode* spheresec = new SceneNode(new UnitSphere(), &env_m);
    // scene.push_back(spheresec);


    // Cube texture mapping (produce effect -- cube1/cube2)
    // SceneNode* plane = new SceneNode(new UnitSquare(), &texture_m);
    // scene.push_back(plane);

    // SceneNode* planefront = new SceneNode(new UnitSquare(), &texture_m);
    // scene.push_back(planefront);

    // SceneNode* planeleft = new SceneNode(new UnitSquare(), &texture_m);
    // scene.push_back(planeleft);
    
    // SceneNode* planeright = new SceneNode(new UnitSquare(), &texture_m);
    // scene.push_back(planeright);

    // SceneNode* planeback = new SceneNode(new UnitSquare(), &texture_m);
    // scene.push_back(planeback);
    
	// Apply some transformations to the sphere and unit square.
     double factor1[3] = { 1.0, 2.0, 1.0 };
//    double factor1[3] = { 1.0, 1.0, 1.0 }; //unit
    // sphere->translate(Vector3D(0, 0, -5));
    // sphere->rotate('x', -45);
    // sphere->rotate('z', 45);
    // sphere->scale(Point3D(0, 0, 0), factor1);

     cone->translate(Vector3D(0, 0, -6));

    double factor2[3] = { 6.0, 6.0, 6.0 };
    plane->translate(Vector3D(0, 0, -7));
    plane->rotate('z', 45);
    plane->scale(Point3D(0, 0, 0), factor2);


    // Add cube in the scene
    // double factor7[3] = { 0.5, 0.5, 0.5 }; //unit
    // spheresec->translate(Vector3D(2, 0.5, -5));
    // spheresec->rotate('x', -70);
    // spheresec->rotate('z', 45);
    // spheresec->scale(Point3D(0, 0, 0), factor7);

    // double factor2[3] = { 6.0, 6.0, 6.0 };
    // plane->translate(Vector3D(0, 0, -7));
    // // plane->rotate('z', 45);
    // plane->scale(Point3D(0, 0, 0), factor2);

    // double factor3[3] = { 6.0, 6.0, 6.0 };
    // planefront->translate(Vector3D(0,3, -7));
    // // planefront->rotate('z', 45);
    // planefront->rotate('x', 90);
    // planefront->scale(Point3D(0, 0, 0), factor3);

    // double factor4[3] = { 6.0, 6.0, 6.0 };
    // planeleft->translate(Vector3D(3,0, -7));
    // // planefront->rotate('z', 45);
    // planeleft->rotate('y', 90);
    // planeleft->scale(Point3D(0, 0, 0), factor4);

    // double factor5[3] = { 6.0, 6.0, 6.0 };
    // planeright->translate(Vector3D(-3,0, -7));
    // // planefront->rotate('z', 45);
    // planeright->rotate('y', -90);
    // planeright->scale(Point3D(0, 0, 0), factor5);

    // double factor6[3] = { 6.0, 6.0, 6.0 };
    // planeback->translate(Vector3D(0,-3, -7));
    // // planefront->rotate('z', 45);
    // planeback->rotate('x', 90);
    // planeback->scale(Point3D(0, 0, 0), factor6);

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


