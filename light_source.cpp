/***********************************************************
	
	Starter code for Assignment 3

	Implements light_source.h

***********************************************************/

#include <cmath>
#include "light_source.h"

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
