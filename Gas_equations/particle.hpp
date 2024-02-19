#pragma once
#include "vectorOperations.hpp"
#include <math.h>
using namespace Vectors;

class Particle{
    public:
    float mass,radius;
    Vector3 pos, vel, acc={0,0,0};
    Particle(){}
    Particle(Vector3 pos, Vector3 vel,Vector3 acc, float mass, float radius){
        this->pos=pos; this->vel=vel; this->mass=mass; this->radius=radius; this->acc=acc;
    }
    void draw(Color color){
        DrawSphere(pos, radius, color);
    }
    void move(Vector3 F, float dt){
        this->acc=F*(1/this->mass);
        this->vel= this->vel+(this->acc*dt);
        this->pos= this->pos+(this->vel*dt);
    }
    Vector3 getWallCollision(int x, int y, int z){
        Vector3 change=Null();
        if (abs(pos.x)>x){vel.x=-vel.x;change.x=-2*(vel.x);}
        if (abs(pos.y)>y){vel.y=-vel.y;change.y=-2*(vel.y);}
        if (abs(pos.z)>z){vel.z=-vel.z;change.z=-2*(vel.z);}
        return change;
    }
};