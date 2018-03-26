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

extern int texture_width;
extern int texture_height;
extern int env_width;
extern int env_height;
extern GLubyte *env_texture;
extern GLubyte *wood;

extern Material EnvMapping;
extern Material TextureMapping;

double max(double a, double b){
    if (a > b) {
        return a;
    }else{
        return b;
    }
}

Color sphere_texture_color (Ray3D& ray, int width, int height, GLubyte *image, int type){
    auto normal = ray.intersection.normal;
    auto viewDir = -ray.dir;
    auto reflectDir = 2.0 * viewDir.dot(normal)*normal - viewDir;
    reflectDir.normalize();

    int u,v;
    // u = 0.5 + atan2(z, x) / 2π
    // v = 0.5 - asin(y) / π

    if (type==0){
        // u = width  * (std::atan2(reflectDir[0], reflectDir[2]) / (2 * M_PI) + 0.5);
        // v = height * (std::asin (reflectDir[1]) / M_PI + 0.5);

        if (abs(ray.intersection.transformed_point[0])==1){
            u = texture_width*(ray.intersection.transformed_point[2]+1)/2;
            v = texture_height*(ray.intersection.transformed_point[1]+1)/2;
        }
        else if (abs(ray.intersection.transformed_point[1])==1){
            u = texture_width*(ray.intersection.transformed_point[0]+1)/2;
            v = texture_height*(ray.intersection.transformed_point[2]+1)/2;
        }
        else{
            u = texture_width*(ray.intersection.transformed_point[0]+1)/2;
            v = texture_height*(ray.intersection.transformed_point[1]+1)/2;
        }
    }

    if (type==1){
        u = width  * (std::atan2(ray.intersection.transformed_point[0], ray.intersection.transformed_point[2]) / (2 * M_PI) + 0.5);
        v = 0.5*height - height * (std::asin (ray.intersection.transformed_point[1]) / M_PI );
    }
    

    float r = image[v * 3 * width + u * 3 + 0]  / 255.0;
    float g = image[v * 3 * width + u * 3 + 1]  / 255.0;
    float b = image[v * 3 * width + u * 3 + 2]  / 255.0;

    return Color(r,g,b);

}

void PointLight::shade(Ray3D& ray) {
	// TODO: implement this function to fill in values for ray.col 
	// using phong shading.  Make sure your vectors are normalized, and
	// clamp colour values to 1.0.
	//
	// It is assumed at this point that the intersection information in ray 
	// is available.  So be sure that traverseScene() is called on the ray 
	// before this function.
    
    auto point = ray.intersection.point;
    auto material = *(ray.intersection.mat);

    if (ray.intersection.mat == &EnvMapping){
        ray.col = ray.col + sphere_texture_color(ray, env_width, env_height,env_texture, 0);
        return;
    }
    if (ray.intersection.mat == &TextureMapping){
        ray.col = ray.col + sphere_texture_color(ray, texture_width, texture_height,wood, 1);
        return;
    }
    // plane texture mapping
    /*
    if (ray.intersection.mat == &TextureMapping){
        // UV mapping of a unit cube
        // if |x| == 1:
        //  u = (z + 1) / 2
        //  v = (y + 1) / 2
        // elif |y| == 1:
        //  u = (x + 1) / 2
        //  v = (z + 1) / 2
        // else:
        //  u = (x + 1) / 2
        //  v = (y + 1) / 2
        int x,y;
        if (abs(ray.intersection.transformed_point[0])==1){
            x = texture_width*(ray.intersection.transformed_point[2]+1)/2;
            y = texture_height*(ray.intersection.transformed_point[1]+1)/2;
        }
        else if (abs(ray.intersection.transformed_point[1])==1){
            x = texture_width*(ray.intersection.transformed_point[0]+1)/2;
            y = texture_height*(ray.intersection.transformed_point[2]+1)/2;
        }
        else{
            x = texture_width*(ray.intersection.transformed_point[0]+1)/2;
            y = texture_height*(ray.intersection.transformed_point[1]+1)/2;
        }

        float r = wood[y * 3 * texture_width + x * 3 + 0] * 1.0 / 255;
        float g = wood[y * 3 * texture_width + x * 3 + 1] * 1.0 / 255;
        float b = wood[y * 3 * texture_width + x * 3 + 2] * 1.0 / 255;

        ray.col = ray.col + Color(r, g, b);

        return; 
    }
    */

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

    ray.col = ambient + diffuse + specular;
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
