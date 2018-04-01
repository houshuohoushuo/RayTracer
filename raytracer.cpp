/***********************************************************
 
 Starter code for Assignment 3
 
 Implementations of functions in raytracer.h,
 and the main function which specifies the scene to be rendered.
 
 ***********************************************************/


#include "raytracer.h"
#include <cmath>
#include <iostream>
#include <cstdlib>

void Raytracer::traverseScene(Scene& scene, Ray3D& ray)  {
    for (size_t i = 0; i < scene.size(); ++i) {
        SceneNode* node = scene[i];
        
        if (node->obj->intersect(ray, node->worldToModel, node->modelToWorld)) {
            ray.intersection.mat = node->mat;
        }
    }
}

void Raytracer::computeTransforms(Scene& scene) {
    // right now this method might seem redundant. But if you decide to implement
    // scene graph this is where you would propagate transformations to child nodes
    
    for (size_t i = 0; i < scene.size(); ++i) {
        SceneNode* node = scene[i];
        
        node->modelToWorld = node->trans;
        node->worldToModel = node->invtrans;
    }
}

void Raytracer::computeShading(Ray3D& ray, LightList& light_list,Scene& scene) {
    int sample_num = 10;
    for (int i = 0; i < light_list.size(); i++) {
        LightSource* light = light_list[i];
        
        // Each lightSource provides its own shading function.
        // Implement shadows here if needed.
        light->shade(ray);
        int hit = 0;
        //n samples of light from the light source within the light area
        for (int i = 0; i < sample_num; i++)
        {
            Vector3D area = Vector3D(light->get_width(), 0, light->get_width());
            //get a random light ray within the area
            Point3D rand_light_pos = light->get_position() + ((double)rand() / RAND_MAX - 1 / 2) * area;
            Ray3D r;
            r.origin = ray.intersection.point - 0.0001 * ray.dir;
            r.dir = rand_light_pos - r.origin;
            r.dir.normalize();
            traverseScene(scene,r);
            if (!r.intersection.none) {
                hit++;
            }
        }
        double ratio = (double)hit / sample_num;
        ray.col = (1 - ratio) * ray.col + ratio* ray.intersection.mat->ambient;
    }
}

Color Raytracer::shadeRay(Ray3D& ray, Scene& scene, LightList& light_list, int refl_depth) {
    Color col(0.0, 0.0, 0.0);
    traverseScene(scene, ray);
    
    // Don't bother shading if the ray didn't hit
    // anything.
    if (!ray.intersection.none) {
        computeShading(ray, light_list,scene);
        col = ray.col;
        
        if (refl_depth < MAX_DEPTH){
            // reflection
            Ray3D r;
            r.origin = ray.intersection.point + 0.0001 * ray.intersection.normal;
            r.dir = ray.dir + 2*(ray.intersection.normal.dot(-ray.dir))* ray.intersection.normal;
            r.dir.normalize();
            traverseScene(scene, r);
            if (!r.intersection.none){
                col = (1-ray.intersection.mat->ref_coef)*col + ray.intersection.mat->ref_coef*shadeRay(r,scene,light_list,refl_depth+1);
            }
        }
        
    }
    // You'll want to call shadeRay recursively (with a different ray,
    // of course) here to implement reflection/refraction effects.
    col.clamp();
    return col;
}	

void Raytracer::render(Camera& camera, Scene& scene, LightList& light_list, Image& image) {
    computeTransforms(scene);
    Matrix4x4 viewToWorld;
    double factor = (double(image.height)/2)/tan(camera.fov*M_PI/360.0);
    
    viewToWorld = camera.initInvViewMatrix();
    //jitter matrix from computergraphics.stackexchange.com/questions/4248/how-is-anti-aliasing-implemented-in-ray-tracing
    float jitterMatrix[4 * 2] = {
        -1.0/4.0,  3.0/4.0,
        3.0/4.0,  1.0/3.0,
        -3.0/4.0, -1.0/4.0,
        1.0/4.0, -3.0/4.0,
    };
    
    // Construct a ray for each pixel.
    for (int i = 0; i < image.height; i++) {
        for (int j = 0; j < image.width; j++) {
            // Sets up ray origin and direction in view space,
            // image plane is at z = -1.
            Color col = Color(0.0,0.0,0.0);
            //Anti aliasing
            //Accumulate light for 4 samples
            for(int k = 0; k < 4; k++){
                Point3D origin(0, 0, 0);
                Point3D imagePlane;
                imagePlane[0] = (-double(image.width)/2 + 0.5 + j + jitterMatrix[2*k+1])/factor;
                imagePlane[1] = (-double(image.height)/2 + 0.5 + i + jitterMatrix[2*k])/factor;
                imagePlane[2] = -1;
                Ray3D ray;
                //Ray Direction
                Vector3D direction = imagePlane - origin;
                //Convert to world-space
                direction = viewToWorld * direction;
                direction.normalize();
                origin = viewToWorld * origin;
                ray.origin = origin;
                ray.dir = direction;
                col = col + shadeRay(ray, scene, light_list,0);
            }
            //average of 4 samples
            Color col1 = Color(col[0]/4.0,col[1]/4.0,col[2]/4.0);
            image.setColorAtPixel(i, j, col1);
        }
    }
}

