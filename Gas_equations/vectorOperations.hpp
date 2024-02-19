#pragma once
#include<raylib.h>
#include <math.h>

namespace Vectors{
    Vector3 create(float x, float y, float z){return {x,y,z};}
    Vector3 Null(){return{0,0,0};}
    Vector3 operator+(Vector3 a, Vector3 b){return {a.x+b.x, a.y+b.y, a.z+b.z};}
    Vector3 operator-(Vector3 a, Vector3 b){return {a.x-b.x, a.y-b.y, a.z-b.z};}
    Vector3 operator*(Vector3 a, float b){ return {b*a.x, b*a.y, b*a.z};}
    Vector3 dot(Vector3 a, Vector3 b){return {a.x*b.x, a.y*b.y, a.z*b.z};}
    Vector3 cross(Vector3 a, Vector3 b){
        return {
            (a.y*b.z)-(a.z*b.y), (b.x*a.z)-(a.x*b.z), (a.x*b.y)-(a.y*b.x)
        };
    }
    float mag(Vector3 a){return sqrt((a.x*a.x)+(a.y*a.y)+(a.z*a.z));}
    Vector3 transForm(Vector3 a, int W, int H){return {a.x+W/2, H/2-a.y, a.z};}
}