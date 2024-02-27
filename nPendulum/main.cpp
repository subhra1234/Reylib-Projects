#include<math.h>
#include <raylib.h>

//vector operators
Vector2 operator+(Vector2 a, Vector2 b){return {a.x+b.x, a.y+b.y};}
Vector2 operator*(Vector2 a, float b){return {a.x*b, a.y*b};}
float mag(Vector2 a){return sqrt(a.x*a.x + a.y*a.y);}
Vector2 norm(Vector2 a){return a*(1/mag(a));}


class Pendululm{
    public:
    Vector2 origin, position, velocity; float length, theta, mass, K=10;
        Pendululm(){}
        Pendululm(Vector2 origin, float mass, float length, float theta){
            this->origin=origin; this->length=length; this->theta=theta;this->mass=mass;
        }
        Pendululm(Vector2 origin, Vector2 position, float mass){
            this->origin=origin; this->position=position; this->mass=mass;
            this->length=sqrt(pow(this->position.x-this->origin.x,2)+ pow(this->position.y-this->origin.y, 2));
            this->theta=atan((this->position.x-this->origin.x)/(this->position.y- this->origin.y)); 
        }
        void resetPosition(){
            this->position.x= this->origin.x+ this->length*sin(theta);
            this->position.y= this->origin.y+ this->length*cos(theta);
        }
        void swing(Vector2 F, float dt){
            theta=atan((position.x-origin.x)/(position.y-origin.y));
            this->velocity=this->velocity+ F*(dt/this->mass);
            this->position=this->position+ this->velocity*dt;
        }
};

int main(){
    float W=800, H=600;
    float textPosX=20, textPosY=25;
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(W, H, "N pendulum");
    
    Pendululm pendulums[20];
    int pendulumSize=1;
    pendulums[0]=Pendululm({W/2,0},1,100,PI/12);
    pendulums[0].resetPosition();
    const char* EventText;
   
    float dt=1.f/100;
    SetTargetFPS(100);
    //mode
    bool isInputMode=true;

    //trail
    int trailPointNo=100;
    int trailCurrentSize=0;
    Vector2 trail[trailPointNo];
    
    while (!WindowShouldClose())
    { 
        for(int i=pendulumSize; i>0;i--){
            pendulums[i].origin=pendulums[i-1].position;
        }
        //dynamics
        if(!isInputMode){
            EventText= "Simulating.....";
            for(int i=0; i<pendulumSize; i++){
                pendulums[i].swing({0, pendulums[i].mass*500}, dt);
                Vector2 dist=pendulums[i].position+pendulums[i].origin*-1;
                float dl=mag(dist)-pendulums[i].length;
                float wi=1/pendulums[i].mass; float wp=0;
                if(i>0){wp=1/pendulums[i-1].mass;}
                Vector2 toMove=dist*(dl/(pendulums[i].length*(wi+wp)));
                pendulums[i].position=pendulums[i].position+toMove*(-1*wi);
                pendulums[i].velocity=pendulums[i].velocity+toMove*(-wi/dt);
                if(i>0){
                    pendulums[i-1].position=pendulums[i-1].position+ toMove*wp;
                    pendulums[i-1].velocity=pendulums[i-1].velocity+toMove*(wp/dt);
                }
                
            }

        // fill up the trail
        if(trailCurrentSize<trailPointNo){
            if(trailCurrentSize!=0){
                if(mag(pendulums[pendulumSize-1].position+trail[trailCurrentSize-1]*-1)>2){
                    trail[trailCurrentSize]=pendulums[pendulumSize-1].position;
                    trailCurrentSize+=1;
                }
            }
            else{trail[trailCurrentSize]=pendulums[pendulumSize-1].position;trailCurrentSize+=1;}
        }
        else{
            for(int i=0; i<trailCurrentSize-1; i++){
                trail[i]=trail[i+1];
            }
            trailCurrentSize-=1;
            }
        }
        if(IsKeyPressed(KEY_I)){isInputMode=!isInputMode;}
        if(IsKeyPressed(KEY_DELETE)){
            for(int i=1; i<pendulumSize;i++){pendulumSize-=1;}
             pendulums[0].theta=PI/12; pendulums[0].velocity={0,0}; pendulums[0].resetPosition();}

        ClearBackground(BLACK);
        //drawing
        BeginDrawing();
        //draw pendulums
            for(int i=0; i<pendulumSize; i++){
                DrawCircle(pendulums[i].origin.x, pendulums[i].origin.y, 5, YELLOW);
                DrawLine(pendulums[i].origin.x, pendulums[i].origin.y, pendulums[i].position.x, pendulums[i].position.y, WHITE);
                DrawCircle(pendulums[i].position.x, pendulums[i].position.y, 20, BLUE);
            }
        //drawing Mode
            if(isInputMode){
                    EventText="Input Mode";
                    DrawLine(pendulums[pendulumSize-1].position.x, pendulums[pendulumSize-1].position.y,GetMousePosition().x, GetMousePosition().y, DARKGRAY);
                    DrawCircle(GetMousePosition().x, GetMousePosition().y, 20, GRAY);
                    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                        Vector2 origin=pendulums[pendulumSize-1].position;
                        Pendululm p(origin, GetMousePosition(), 1);
                        pendulums[pendulumSize]=p;
                        pendulumSize+=1;
                        EventText="Clicked";
                    }
                }


        // draw trail
        if(trailCurrentSize>1){
        for(int i=1; i<trailCurrentSize; i++){
            DrawLineEx(trail[i-1],trail[i], 10*(float(i)/trailPointNo), ColorFromNormalized({0,0.5,1,float(i)/trailPointNo}));
        }}

        //draw text
        DrawRectangleLines(0,0,200,50, WHITE);
        DrawText("Event Tracker", 20,0, 14, WHITE);  
        DrawText(EventText, textPosX, textPosY, 20, LIGHTGRAY);  
        EndDrawing();
        if(isInputMode){
            DrawText("Left-Click to add a new Bob.\nPress Delete to remove Bobs.\nPress I to simulate", 20, H-50,20, LIGHTGRAY);
        }
        
    }

    CloseWindow();
}