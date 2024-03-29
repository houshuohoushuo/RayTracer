/***********************************************************
	
	Starter code for Assignment 3

	Light source classes

***********************************************************/
#pragma once

#include "util.h"
#include <vector>

// Base class for a light source.  You could define different types
// of lights here, but point light is sufficient for most scenes you
// might want to render.  Different light sources shade the ray 
// differently.
class LightSource {
public:
	virtual void shade(Ray3D&) = 0;
	virtual Point3D get_position() const = 0; 
    virtual ~LightSource() {};
    virtual double get_width() const = 0;
};  

// List of all light sources in your scene
typedef std::vector<LightSource*> LightList;

// A point light is defined by its position in world space and its
// color.
class PointLight : public LightSource {
public:
	PointLight(Point3D pos, Color col) 
	: 
	pos(pos), col_ambient(col), col_diffuse(col), col_specular(col) {}
	
	PointLight(Point3D pos, Color ambient, Color diffuse, Color specular) 
	: 
	pos(pos), col_ambient(ambient), col_diffuse(diffuse), col_specular(specular) {}
	
	void shade(Ray3D& ray);
	
    Point3D get_position() const { return pos;};
        
    double get_width() const{return 0;}
	
private:
	Point3D pos;
	Color col_ambient;
	Color col_diffuse; 
	Color col_specular; 
};

class SquareLight : public LightSource {
public:
    SquareLight(Point3D pos, Color col, double w)
    :
    pos(pos), col_ambient(col), col_diffuse(col), col_specular(col), width(w){}
    
    SquareLight(Point3D pos, Color ambient, Color diffuse, Color specular, double w)
    :
    pos(pos), col_ambient(ambient), col_diffuse(diffuse), col_specular(specular), width(w){}
    
    void shade(Ray3D& ray);
    
    Point3D get_position() const { return pos; };
    
    double get_width() const{return width;}
    
private:
    Point3D pos;
    Color col_ambient;
    Color col_diffuse;
    Color col_specular;
    double width;
};
