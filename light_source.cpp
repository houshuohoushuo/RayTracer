/***********************************************************
	
	Starter code for Assignment 3

	Implements light_source.h

***********************************************************/

#include <cmath>
#include "light_source.h"
#include <algorithm>
#include <cassert>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

extern int env_width;
extern int env_height;
extern GLubyte *env_texture;
extern GLubyte *wood;

double max(double a, double b){
    if (a > b) {
        return a;
    }else{
        return b;
    }
}

void PointLight::shade(Ray3D& ray) {
	// TODO: implement this function to fill in values for ray.col 
	// using phong shading.  Make sure your vectors are normalized, and
	// clamp colour values to 1.0.
	//
	// It is assumed at this point that the intersection information in ray 
	// is available.  So be sure that traverseScene() is called on the ray 
	// before this function.
    /*
    Vector3D normal = ray.intersection.normal;
    normal.normalize();
    
    Color ambient = ray.intersection.mat->ambient * col_ambient;
    
    Vector3D s = pos - ray.intersection.point;
    s.normalize();
    Color diffuse = max(0.0,s.dot(normal)) * ray.intersection.mat->diffuse * col_diffuse;
    
    Vector3D viewVec = -ray.dir;
    viewVec.normalize();
    
    Vector3D r = -s + 2.0 * (normal.dot(s)) * normal;
    Color specular = pow(max(0.0,r.dot(viewVec)),ray.intersection.mat->specular_exp) * ray.intersection.mat->specular * col_specular;

    ray.col = ambient + diffuse + specular;;
    ray.col.clamp();
    */
    auto point = ray.intersection.point;
    auto material = *(ray.intersection.mat);
    auto normal = ray.intersection.normal;
    auto viewDir = -ray.dir;
    auto reflectDir = 2.0 * viewDir.dot(normal)*normal - viewDir;
    reflectDir.normalize();

    int x = env_width  * (std::atan2(reflectDir[0], reflectDir[2]) / (2 * M_PI) + 0.5);
    int y = env_height * (std::asin (reflectDir[1]) / M_PI + 0.5);

    float r = env_texture[y * 3 * env_width + x * 3 + 0] * 1.0 / 255;
    float g = env_texture[y * 3 * env_width + x * 3 + 1] * 1.0 / 255;
    float b = env_texture[y * 3 * env_width + x * 3 + 2] * 1.0 / 255;

    ray.col = ray.col + Color(r, g, b);

    return;

}
void SquareLight::shade(Ray3D& ray) {
    // TODO: implement this function to fill in values for ray.col
    // using phong shading.  Make sure your vectors are normalized, and
    // clamp colour values to 1.0.
    //
    // It is assumed at this point that the intersection information in ray
    // is available.  So be sure that traverseScene() is called on the ray
    // before this function.
    
    Vector3D normal = ray.intersection.normal;
    normal.normalize();
    
    Color ambient = ray.intersection.mat->ambient * col_ambient;
    
    Vector3D s = pos - ray.intersection.point;
    s.normalize();
    Color diffuse = max(0.0,s.dot(normal)) * ray.intersection.mat->diffuse * col_diffuse;
    
    Vector3D viewVec = -ray.dir;
    viewVec.normalize();
    
    Vector3D r = -s + 2.0 * (normal.dot(s)) * normal;
    Color specular = pow(max(0.0,r.dot(viewVec)),ray.intersection.mat->specular_exp) * ray.intersection.mat->specular * col_specular;
    
    ray.col = ambient + diffuse + specular;;
    ray.col.clamp();
}
