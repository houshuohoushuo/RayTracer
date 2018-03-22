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
    const int SHADOW_SAMPLE = 10;
    const double REFLECTION_OFFSET = 0.0001; // remove shadow acne
    
    
    for (size_t  i = 0; i < light_list.size(); ++i) {
        LightSource* light = light_list[i];
        
        // Each lightSource provides its own shading function.
        // Implement shadows here if needed.
        light->shade(ray);
        
        int hit = 0;
        for (int i = 0; i < SHADOW_SAMPLE; i++)
        {
            // sample light position
            Point3D light_rand = light->get_position() + ((double)rand() / RAND_MAX - 1.0 / 2) * Vector3D(5, 0, 5);
            Ray3D r;
            r.origin = ray.intersection.point - 0.0001 * ray.dir;
            r.dir = light_rand - r.origin;
            r.dir.normalize();
            traverseScene(scene,r);
            if (!r.intersection.none) {
                hit++;
            }
        }
        double r = (double)hit / SHADOW_SAMPLE; // 0 ~ 1
        //            ray.col = (1 - r) * ray.col + (r)* ray.intersection.mat->ambient;
        ray.col = (1 - r) * ray.col;
    }
}

Color Raytracer::shadeRay(Ray3D& ray, Scene& scene, LightList& light_list) {
    Color col(0.0, 0.0, 0.0);
    traverseScene(scene, ray);
    
    // Don't bother shading if the ray didn't hit
    // anything.
    if (!ray.intersection.none) {
        computeShading(ray, light_list,scene);
        col = ray.col;
        
        Ray3D r;
        r.origin = ray.intersection.point - 0.0001 * ray.dir;
        
        r.dir = ray.dir + 2*(ray.intersection.normal.dot(-ray.dir))* ray.intersection.normal;
        r.dir.normalize();
        
        if(ray.intersection.normal.dot(r.dir) > 0){
            col = col + 0.0*shadeRay(r,scene,light_list);
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
            
            //Anti aliasing super sampling
            for(int k = 0; k < 4; k++){
                
                Point3D origin(0, 0, 0);
                Point3D imagePlane;
                imagePlane[0] = (-double(image.width)/2 + 0.5 + j + jitterMatrix[2*k+1])/factor;
                imagePlane[1] = (-double(image.height)/2 + 0.5 + i + jitterMatrix[2*k])/factor;
                imagePlane[2] = -1;
                
                Ray3D ray;
                // TODO: Convert ray to world space
                
                //Ray Direction
                Vector3D direction = imagePlane - origin;
                //Convert to world-space
                direction = viewToWorld * direction;
                direction.normalize();
                origin = viewToWorld * origin;
                
                ray.origin = origin;
                ray.dir = direction;
                
                col = col + shadeRay(ray, scene, light_list);
            }
            Color col1 = Color(col[0]/4.0,col[1]/4.0,col[2]/4.0);
            image.setColorAtPixel(i, j, col1);
        }
    }
}

