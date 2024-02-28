#include <raylib.h>
#include "particle.hpp"
#include <rlgl.h>
#include <iostream>
#include <math.h>
#include <sys/types.h>
#include <vector>
float randMToN(float M, float N);
Vector3 randomMizeV(float V);
Color getColor(float strength);


int main(){
    int W=640, H=600, N=200;
    float boxLength=10;
    Vector3 boxPosition=Null();
    InitWindow(W, H,"Gas eqn");

    Camera3D cam;
    cam.position={20,20,20};
    cam.target={0,0,0};
    cam.up = { 0.0f, 1.f, 0.0f };
    cam.fovy = 45.0f;
    cam.projection = CAMERA_PERSPECTIVE;

    //Particles
    Particle ps[N];
    int T=300;
    float c_rms=sqrt(3*T);
    //initialising position and momentum
    for(int i=0; i<N;i++){
        Vector3 pos={randMToN(-boxLength/2,boxLength/2),randMToN(-boxLength/2,boxLength/2),randMToN(-boxLength/2,boxLength/2)};
        Vector3 vel=randomMizeV(c_rms);
        ps[i]= Particle(pos, vel, Null(), 1,0.1);
    }
    SetTargetFPS(60);
    float t=0;

    int noOfItr=1;
    float avgPressure=0;
    while(!WindowShouldClose()){
        //variables
        float netMomChange=0;
        float dt= 1.f/60.f;
        float Pressure=0;
        float avgVel=0;

        std::vector col={0} ;
        ClearBackground(RAYWHITE);
        BeginDrawing();
        DrawText(TextFormat("FPS: %02.02f", 1.f/GetFrameTime()), 10,10,20, BLUE);
            BeginMode3D(cam);
            
            //Drawing cube
            DrawCubeWiresV(boxPosition,{boxLength, boxLength, boxLength},BLUE);  
   
            //Drawing Particles
            for(int i=0; i<N; i++){
                Vector3 netForce={0,0,0};
                Vector3 dv=ps[i].getWallCollision(boxLength/2, boxLength/2, boxLength/2);
                netMomChange+=mag(dv)*ps[i].mass;
                avgVel+=mag(ps[i].vel)/N;

                ps[i].draw(RED);
                ps[i].move(netForce,dt);
            }
            Pressure=netMomChange/(dt*6.f*pow(boxLength,2));
            DrawGrid(20,10);
            EndMode3D();

        //avg calculations    
        avgPressure+=Pressure;
        // if(noOfItr>=2){avgPressure/=2;}
        DrawText(TextFormat("P : %02.02f", Pressure), 10,30,20, BLUE);
        // Inputs
        if(IsKeyDown(KEY_LEFT)){cam.position.x+=1;}
        else if(IsKeyDown(KEY_RIGHT)){cam.position.x-=1;}
        if(IsKeyDown(KEY_UP)){cam.position.y+=1;}
        else if(IsKeyDown(KEY_DOWN)){cam.position.y-=1;}
        
        EndDrawing();
        t+=dt;
        noOfItr+=1;
    }

    avgPressure/=noOfItr;

    //// Results
    std::cout<<"\n\n\n\n";
    std::cout<<"Theoretical Results: \n";
    std::cout<<"P=(1/3)mnc^2= "<<N*c_rms*c_rms/(3*pow(boxLength,3))<<" KT= "<<N*T<<'\n';
    std::cout<<"\nSimulated Results :"<<'\n';
    std::cout<<"Avg Pressure= "<<avgPressure<<" PV= "<<avgPressure*pow(boxLength,3)<<'\n';
    std::cout<<"\n\n\n\n";
    

}
float randMToN(float M, float N)
{
    return M + (rand() / ( RAND_MAX / (N-M) ) ) ;  
}

//initiates random velocity accoring to c_rms
Vector3 randomMizeV(float V){
    float target= V+randMToN(-20,20);
    float vx= randMToN(-target, target);
    float target2= sqrt(pow(target,2)-pow(vx,2));
    float vy= randMToN(-target2, target2);
    float vz= sqrt(pow(target,2)-pow(vx,2)-pow(vy,2));
    return {vx, vy, vz};
}

Color getColor(float strength){
    if (strength>255){strength=255;}
    return {static_cast<unsigned char>(strength*3), static_cast<unsigned char>(255-3*strength),0,255};
}