#include <raylib.h>
#include <raymath.h>
#define W 800
#define H 600
//vector operations and helping functions
Vector3 operator+(Vector3 a, Vector3 b){return {a.x+b.x, a.y+b.y, a.z+b.z};}
Vector3 operator*(Vector3 a, float b){return {a.x*b, a.y*b, a.z*b};}
float mag(Vector3 a){return sqrt(a.x*a.x + a.y*a.y+ a.z*a.z);}
Vector3 norm(Vector3 a){return a*(1/mag(a));}
Vector3 cross(Vector3 a, Vector3 b){return {a.y*b.z- a.z*b.y, b.x*a.z- a.x*b.z, a.x*b.y- b.x*a.y};}
void DrawArrow(Vector3 start, Vector3 dirn);
Vector3 ScreenToWorld(Camera cam);

class Wire{
    public:
    Vector3  points[1000]={};
    int currentPointsSize;
    Wire(){
        currentPointsSize=0;
    }
    void addPoints(Vector3 v){
        if(currentPointsSize<1000){
            points[currentPointsSize]=v;
            currentPointsSize+=1;}

    }
    void removePoints(){}
        void drawModel(){
            for(int i=0; i<this->currentPointsSize-1; i++){
                DrawCylinderWiresEx(points[i],points[i+1], 1, 1, 10, WHITE);
            }
        }
        Vector3 getField(Vector3 &r){
            float K=10;
            Vector3 field={0,0,0};
            for(int i=0; i<this->currentPointsSize-1; i++){
                Vector3 _r= r+(points[i]*-1);
                field=field+ cross(points[i+1]+(points[i]*-1), _r)*(K/pow(mag(_r),3));
            }
            return field;
        }
}
;


int main(){

    //Initialize Window
    InitWindow(W,H,"Magnetic Field");

    //Setting up the camera
    Camera cam;
    cam.position={0,0,100};
    cam.target={0,0,0};
    cam.up = { 0.0f, 1.f, 0.0f };
    cam.fovy = 45.0f;
    cam.projection = CAMERA_PERSPECTIVE;

    //Wire Object
    Wire w;
    w.addPoints({-10,0,0});
    w.addPoints({-9,0,0});
    //Positions
    int position_index=0;
    Vector3 positions[1000];
    const char* warning= "Maximum number of segments allowed=1000.\nCurrent number of Segments: %i";
    //fill up the array
    for(float x=-50; x<50; x+=10 ){
        for(float y=-50; y<50; y+=10){
            for(float z=-10; z<90; z+=10){
                positions[position_index]=(Vector3){x,y,z};
                position_index+=1;
                }
            }
    } 

    while (!WindowShouldClose()) {
        SetTargetFPS(100);
        //Events
        if(IsKeyDown(KEY_X)){
        UpdateCamera(&cam, CAMERA_THIRD_PERSON);}

        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                w.addPoints(ScreenToWorld(cam));
           }
        if(IsKeyPressed(KEY_DELETE)||GetTouchPointCount()==2){
            if(w.currentPointsSize>0){
            w.currentPointsSize-=1;}
        }

        ClearBackground(BLACK);
        //drawing
        BeginDrawing();
            BeginMode3D(cam);
            if(w.currentPointsSize!=0){
                w.drawModel();
                //magnetic field
                for(int i=0; i<position_index; i++){
                    DrawArrow(positions[i],w.getField(positions[i]));
                }
                //drawing wires
                DrawCylinderWiresEx(w.points[w.currentPointsSize-1], ScreenToWorld(cam), 1,1,10,ColorFromNormalized({1,1,0,0.5}));
            }
            EndMode3D();
        
        EndDrawing();
        // GUI Elements
        DrawText("Hold X and use mouse to move around", 10,10,20, RAYWHITE);
        
        DrawText(TextFormat(warning, w.currentPointsSize),10,H-50, 10, YELLOW);

    }

    CloseWindow();
}



void DrawArrow(Vector3 start, Vector3 dirn){
    float length=mag(dirn)*5;
    if(length>5){length=5;}
    Vector3 dir=norm(dirn);
    if(length>0.1){
        DrawCylinderEx(start,start+dir*length,0.5 , 0.5, 10, ColorFromNormalized({length/5,0.5,0.5,1.0}));
        DrawCylinderEx(start+dir*length, start+dir*(length+(length*0.4f)), 0.75, 0, 10, ColorFromNormalized({length/5,0.5,0.5,1.0}));
    }
}
    

Vector3 ScreenToWorld(Camera cam){
    Ray r= GetMouseRay(GetMousePosition(),cam);
    Vector3 target=cam.target;
    float dist=mag(r.position+(target*-1));
    return  cam.position+r.direction*dist;
}


