/***********************************************************
 
 Starter code for Assignment 3
 
 Implements scene_object.h
 
 ***********************************************************/

#include <cmath>
#include <algorithm>
#include "scene_object.h"

bool UnitSquare::intersect(Ray3D& ray, const Matrix4x4& worldToModel,
                           const Matrix4x4& modelToWorld) {
    // TODO: implement intersection code for UnitSquare, which is
    // defined on the xy-plane, with vertices (0.5, 0.5, 0),
    // (-0.5, 0.5, 0), (-0.5, -0.5, 0), (0.5, -0.5, 0), and normal
    // (0, 0, 1).
    //
    // Your goal here is to fill ray.intersection with correct values
    // should an intersection occur.  This includes intersection.point,
    // intersection.normal, intersection.none, intersection.t_value.
    //
    // HINT: Remember to first transform the ray into object space
    // to simplify the intersection test.
    
    //adapted from tutorial slide
    
    //Transform the ray (origin, direction) to object space
    
    Point3D origin = worldToModel * ray.origin;
    Vector3D direction = worldToModel * ray.dir;
    double t = -origin[2] / direction[2];
    //invalid intersection
    if (t < 0 || direction[2] ==0){
        return false;
    }
    
    Point3D p = origin + t * direction;

    Vector3D normal = Vector3D(0,0,1);
    if (ray.intersection.none || t < ray.intersection.t_value){
        if (p[0] >= -0.5 && p[0] <= 0.5 && p[1] >= -0.5 && p[1] <= 0.5) {
            ray.intersection.none = false;
            ray.intersection.t_value = t;
            ray.intersection.point = modelToWorld * p;
            ray.intersection.transformed_point = p;
            ray.intersection.normal = worldToModel.transpose() * normal;
            ray.intersection.normal.normalize();
            return true;
        }
    }
    return false;
    
}

bool UnitSphere::intersect(Ray3D& ray, const Matrix4x4& worldToModel,
                           const Matrix4x4& modelToWorld) {
    // TODO: implement intersection code for UnitSphere, which is centred
    // on the origin.
    //
    // Your goal here is to fill ray.intersection with correct values
    // should an intersection occur.  This includes intersection.point,
    // intersection.normal, intersection.none, intersection.t_value.
    //
    // HINT: Remember to first transform the ray into object space
    // to simplify the intersection test.
    
    //Transform the ray (origin, direction) to object space
    
    Point3D origin = worldToModel * ray.origin;
    Vector3D direction = worldToModel * ray.dir;

    double A = direction.dot(direction);
    double B = 2 * direction.dot(origin - Point3D(0,0,0));
    double C = (origin - Point3D(0,0,0)).dot(origin - Point3D(0,0,0))-1;
    double delta = pow(B,2)-4*A*C;
    if (delta >= 0){
        
        double t1 = (-B + sqrt(delta))/(2*A);
        double t2 = (-B - sqrt(delta))/(2*A);
        double t_value;
        
        if (t1 < 0 && t2 < 0){
            return false;
        } else if (t1 > 0 && t2 >0) {
            if (t1 < t2) {
                t_value = t1;
            }else{
                t_value = t2;
            }
        }else if (t1 > 0 && t2 <0){
            t_value = t1;
        }else{
            t_value = t2;
        }
        if (ray.intersection.none || t_value < ray.intersection.t_value){
            
            Point3D p = origin + t_value * direction;
            Vector3D normal = Vector3D(p[0],p[1],p[2]);
            ray.intersection.none = false;
            ray.intersection.t_value = t_value;
            ray.intersection.point = modelToWorld * p;
            ray.intersection.transformed_point = p;
            ray.intersection.normal = worldToModel.transpose() * normal;
            ray.intersection.normal.normalize();
            return true;
        }
    }
    return false;
}

bool UnitCone::intersect(Ray3D& ray, const Matrix4x4& worldToModel,
        const Matrix4x4& modelToWorld) {

    Vector3D direction = worldToModel * ray.dir;
    Point3D origin = worldToModel * ray.origin;
    
    double A = pow(direction[0],2) + pow(direction[2],2) - pow(direction[1],2);
    double B = 2*(origin[0] * direction[0] + origin[2] * direction[2] - origin[1] * direction[1]);
    double C = pow(origin[0],2) + pow(origin[2],2) -pow(origin[1],2);
    double delta = pow(B,2)-4*A*C;
    
    if (delta > 0) {
        double t1 = (-B - sqrt(delta)) / (2 * A);
        double t2 = (-B + sqrt(delta)) / (2 * A);
        double t3 = -(origin[1]-1) / direction[1];
        double t4 = -(origin[1]-(-1)) / direction[1];
        
        double d1 = (origin + t1 * direction)[1];
        double d2 = (origin + t2 * direction)[1];
        double d3 = pow((origin + t3 * direction)[0],2)+ pow((origin + t3 * direction)[2],2) +1;
        double d4 = pow((origin + t4 * direction)[0],2)+ pow((origin + t4 * direction)[2],2) +1;

        double inf = std::numeric_limits<double>::infinity();
        std::vector<double> v = {inf};
        if (d1 > 1 || d1 < -1) {
            t1 = -inf;
        }
        else if(t1>0){
            v.push_back(t1);
        }
        if (d2 > 1 || d2 < -1) {
            t2 = -inf;
        }
        else if(t2>0){
            v.push_back(t2);
        }
        if (d3>2) {
            t3= -inf;
        }
        else if(t3>0){
            v.push_back(t3);
        }
        if (d4>2) {
            t4= -inf;
        }
        else if(t4>0){
            v.push_back(t4);
        }
        auto min = std::min_element(std::begin(v), std::end(v));
        Point3D point = origin + *min * direction;
        if ((!ray.intersection.none && *min > ray.intersection.t_value) || *min == -inf || *min == inf || point[1]>0 ) {
            return false;
        }
        ray.intersection.normal = transNorm(worldToModel, Vector3D(point[0], 0, point[2]));
        ray.intersection.normal.normalize();
        ray.intersection.point = modelToWorld * point;
        ray.intersection.t_value = *min;
        ray.intersection.none = false;
        return true;
    }
    return false;
}


void SceneNode::rotate(char axis, double angle) {
    Matrix4x4 rotation;
    double toRadian = 2*M_PI/360.0;
    int i;
    
    for (i = 0; i < 2; i++) {
        switch(axis) {
            case 'x':
                rotation[0][0] = 1;
                rotation[1][1] = cos(angle*toRadian);
                rotation[1][2] = -sin(angle*toRadian);
                rotation[2][1] = sin(angle*toRadian);
                rotation[2][2] = cos(angle*toRadian);
                rotation[3][3] = 1;
                break;
            case 'y':
                rotation[0][0] = cos(angle*toRadian);
                rotation[0][2] = sin(angle*toRadian);
                rotation[1][1] = 1;
                rotation[2][0] = -sin(angle*toRadian);
                rotation[2][2] = cos(angle*toRadian);
                rotation[3][3] = 1;
                break;
            case 'z':
                rotation[0][0] = cos(angle*toRadian);
                rotation[0][1] = -sin(angle*toRadian);
                rotation[1][0] = sin(angle*toRadian);
                rotation[1][1] = cos(angle*toRadian);
                rotation[2][2] = 1;
                rotation[3][3] = 1;
                break;
        }
        if (i == 0) {
            this->trans = this->trans*rotation;
            angle = -angle;
        }
        else {
            this->invtrans = rotation*this->invtrans;
        }
    }
}

void SceneNode::translate(Vector3D trans) {
    Matrix4x4 translation;
    
    translation[0][3] = trans[0];
    translation[1][3] = trans[1];
    translation[2][3] = trans[2];
    this->trans = this->trans*translation;
    translation[0][3] = -trans[0];
    translation[1][3] = -trans[1];
    translation[2][3] = -trans[2];
    this->invtrans = translation*this->invtrans;
}

void SceneNode::scale(Point3D origin, double factor[3] ) {
    Matrix4x4 scale;
    
    scale[0][0] = factor[0];
    scale[0][3] = origin[0] - factor[0] * origin[0];
    scale[1][1] = factor[1];
    scale[1][3] = origin[1] - factor[1] * origin[1];
    scale[2][2] = factor[2];
    scale[2][3] = origin[2] - factor[2] * origin[2];
    this->trans = this->trans*scale;
    scale[0][0] = 1/factor[0];
    scale[0][3] = origin[0] - 1/factor[0] * origin[0];
    scale[1][1] = 1/factor[1];
    scale[1][3] = origin[1] - 1/factor[1] * origin[1];
    scale[2][2] = 1/factor[2];
    scale[2][3] = origin[2] - 1/factor[2] * origin[2];
    this->invtrans = scale*this->invtrans;
}


