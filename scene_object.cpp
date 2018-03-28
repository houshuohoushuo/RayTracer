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

// bool UnitCone::intersect(Ray3D& ray, const Matrix4x4& worldToModel,
//                            const Matrix4x4& modelToWorld){
//     Point3D origin = worldToModel * ray.origin;
//     Vector3D direction = worldToModel * ray.dir;

//     double x0=origin[0], y0=origin[1], z0=origin[2];
//     double dx=direction[0], dy=direction[1], dz=direction[2];
//     double t;

//     double A = pow(dx,2) + pow(dy,2) - pow(dz,2);
//     double B = 2*(x0*dx + y0*dy - z0*dz);
//     double C = pow(x0,2) + pow(y0,2) -pow(z0,2);
//     double delta = pow(B,2)-4*A*C;
//     double d = (1 - z0)/dz;

//     Point3D D = origin + d*direction;

//     bool hitbase = D[0] * D[0] + D[1] * D[1] <= 1;

//     if(fabs(delta) < 0.001) return false; 
//     if(delta < 0.0) return false;

//     if (hitbase){
//         if(delta>=0){
//             double t1 = (-B - sqrt(delta))/(2*A);
//             double t2 = (-B + sqrt(delta))/(2*A);
            
//             if (t1>t2){
//                 t=t2;
//             }
//             else{
//                 t=t1;
//             }
//         }

//     }
    
    
//     Point3D inter = origin + t * direction;
//     Vector3D normal;
//     if(t==d){
//         normal = Vector3D(0,0,1);
//     }
//     else{
//         normal =  Vector3D(inter[0], inter[1], sqrt(inter[0]*inter[0]+ inter[1]*inter[1]));
//     }

//     ray.intersection.none = false;
//     ray.intersection.point = modelToWorld * inter;
//     ray.intersection.normal = worldToModel.transpose() * normal;
//     ray.intersection.normal.normalize();
//     ray.intersection.t_value = t;
    
//     return true;

// }

bool UnitCone::intersect(Ray3D& ray, const Matrix4x4& worldToModel,
        const Matrix4x4& modelToWorld) {
    Vector3D direction = worldToModel * ray.dir;
    Point3D origin = worldToModel * ray.origin;

    double a = pow(direction[0],2) + pow(direction[2],2) - pow(direction[1],2);
    double b = 2*(origin[0] * direction[0] + origin[2] * direction[2] - origin[1] * direction[1]);
    double c = pow(origin[0],2) + pow(origin[2],2) -pow(origin[1],2);
    double delta = pow(b,2)-4*a*c;

    if (delta > 0) {
        double t1 = (-b - sqrt(delta)) / (2 * a);
        double t2 = (-b + sqrt(delta)) / (2 * a);

        if ((origin + t1 * direction)[1] > 1 || (origin + t1 * direction)[1] < -1) {
            t1 = -1;
        }
        if ((origin + t2 * direction)[1] > 1 || (origin + t2 * direction)[1] < -1) {
            t2 = -1;
        }

        //add point t3 for check intersection y = -1
        double t3 = -(origin[1]-1) / direction[1];
        double t3_0 = (origin + t3 * direction)[0]*(origin + t3 * direction)[0];
        double t3_2 = (origin + t3 * direction)[2]*(origin + t3 * direction)[2]+1;
        if ((t3_0 + t3_2) > 2) {
            t3 = -1;
        }
        //check for intersection y = 1
        double t4 = -(1 + origin[1]) / direction[1];
        double t4_0 = (origin + t4 * direction)[0]*(origin + t4 * direction)[0];
        double t4_2 = (origin + t4 * direction)[2]*(origin + t4 * direction)[2]+1;
        if (t4_0 + t4_2 > 2) {
            t4 = -1;
        }

        //find min value
        double min = 1000;
        double t_n[4] = {t1, t2, t3, t4};

        for (int i = 0; i < 4; i++) {
            if (t_n[i] > 0 && t_n[i] < min) {
                min = t_n[i];
            }
        }
        if ((!ray.intersection.none && min > ray.intersection.t_value) ||
                min < 0 || min == 1000) {
            return false;
        }
        Point3D point = origin + min * direction;
        Vector3D normal;
        if (min != t3) {
            normal = Vector3D(point[0], 0, point[2]);
        }else{
            normal = Vector3D(-point[0], 0, -point[2]);
        }

        ray.intersection.normal = transNorm(worldToModel, normal);
        ray.intersection.normal.normalize();
        ray.intersection.point = modelToWorld * point;
        ray.intersection.t_value = min;
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


