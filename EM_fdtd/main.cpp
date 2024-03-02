#include <raylib.h>
#include <raymath.h>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600
#define GUI_PANEL_WIDTH 200
#define xSize 100
#define ySize 100


struct
{   //Multiplying these with Screen size will provide absolute position and size
    Vector2 relative_position={0.5,0.5};
    float relative_width=0.2;
    float relative_height=0.8;
} INTERFACE;

struct {
    float relative_position_x=0.15;
    float relative_aperture_position_y=0.5; //hole position_y
    float relative_aperture_width=0.3; //hole width
} APERTURE;

// GUI elements
struct
{
    Vector2 position = {SCREEN_WIDTH+ 10, 40};
    float width = GUI_PANEL_WIDTH - 20;
    float height = 20;
} GUI_ITEM_SIZE;

int main(){
    InitWindow(SCREEN_WIDTH+GUI_PANEL_WIDTH, SCREEN_HEIGHT, "Waves");
    //wave parameters
    float a=2;
    Color waveColor=RED;
    float c=10, mu=1.7, omega=0.5;

    //Elementary lengths
    float dx=SCREEN_WIDTH/xSize, dy= SCREEN_HEIGHT/ySize;
    float E[xSize][ySize][3];
    float dt=1.f/200;

    //initial Conditions
    for(int i=0; i<xSize; i++){
            for(int j=0; j<ySize; j++){
                E[i][j][1]=0;
                E[i][j][0]=0;
                E[i][j][2]=0;
            }
    }
    
    float t=0;
    while (!WindowShouldClose())
    {
    
    // Initial Conditions
    for(int i=0; i<ySize; i++){
        if(i>5 && i<ySize-5){
            E[0][i][0]=a*sin(omega*(t-dt));
            E[0][i][1]=a*sin(omega*t);
        }
        if(i<APERTURE.relative_aperture_position_y*ySize|| i>APERTURE.relative_aperture_position_y*ySize+APERTURE.relative_aperture_width*ySize){
            E[int(APERTURE.relative_position_x*xSize)][i][2]=0;
        }
    }
    
        // Calculate E
        for(int i=1; i<xSize-1; i++){
            for(int j=1; j<ySize-1; j++){
                //interface
                if(pow((float(i)-xSize*INTERFACE.relative_position.x)/(xSize*INTERFACE.relative_width*0.5),2)+pow((float(j)-ySize*INTERFACE.relative_position.y)/(ySize*INTERFACE.relative_height*0.5),2) < 1){c=10/mu;}else{c=10;}
                E[i][j][0]=E[i][j][1];
                E[i][j][1]=E[i][j][2];
                E[i][j][2]=pow(c*dt/dx,2)*(E[i+1][j][1]+E[i-1][j][1]-2*E[i][j][1])+pow(c*dt/dy, 2)*(E[i][j-1][1]+E[i][j+1][1]
                -2*E[i][j][1]) + 2*E[i][j][1]-E[i][j][0];
            }
        }

        
        BeginDrawing();
        ClearBackground(BLACK);
        //Drawing interface
        DrawEllipse(SCREEN_WIDTH*INTERFACE.relative_position.x, SCREEN_HEIGHT*INTERFACE.relative_position.y, 0.5*SCREEN_WIDTH*INTERFACE.relative_width, 0.5*SCREEN_HEIGHT*INTERFACE.relative_height, ColorFromNormalized({0.6,0.6,0.6,0.5}));
    
        // Drawing grid points that represents E field
            for(int i=0; i<xSize; i++){
                for(int j=0; j<ySize; j++){
                    float alpha=sqrt(pow(E[i][j][2],2));
                    if(alpha>1){alpha=1;}
                    if(alpha>0.1){
                    DrawRectangle(i*SCREEN_WIDTH/xSize, j*SCREEN_HEIGHT/ySize, SCREEN_WIDTH/xSize,SCREEN_HEIGHT/ySize, ColorAlpha(waveColor, alpha));}
                }
            }
        

        DrawRectangle(APERTURE.relative_position_x*SCREEN_WIDTH,0, SCREEN_WIDTH/xSize, SCREEN_HEIGHT*APERTURE.relative_aperture_position_y, LIGHTGRAY );
        DrawRectangle(APERTURE.relative_position_x*SCREEN_WIDTH, SCREEN_HEIGHT*(APERTURE.relative_aperture_position_y+APERTURE.relative_aperture_width), SCREEN_WIDTH/xSize, SCREEN_HEIGHT*(1-APERTURE.relative_aperture_position_y-APERTURE.relative_aperture_width), LIGHTGRAY );
        DrawText(TextFormat("FPS: %i", GetFPS()), 0,10,20,WHITE);

        //GUI Elements
        DrawRectangle(SCREEN_WIDTH, 0, GUI_PANEL_WIDTH,SCREEN_HEIGHT, ColorFromNormalized({0.1,0.6,0.3,0.3}));
        DrawRectangleLines(SCREEN_WIDTH, 0, GUI_PANEL_WIDTH,SCREEN_HEIGHT, ColorFromNormalized({0.1,0.6,0.3,1}));
        
        //aperture
        GuiDrawText("Aperture Parameters:",(Rectangle){GUI_ITEM_SIZE.position.x, GUI_ITEM_SIZE.position.y,GUI_ITEM_SIZE.width, GUI_ITEM_SIZE.height},TEXT_ALIGN_LEFT,LIGHTGRAY);

        GuiSlider((Rectangle){GUI_ITEM_SIZE.position.x+10, GUI_ITEM_SIZE.position.y+GUI_ITEM_SIZE.height+5, GUI_ITEM_SIZE.width-20, GUI_ITEM_SIZE.height}, "X:", "W", &APERTURE.relative_position_x,0.1,0.95);

        GuiSlider((Rectangle){GUI_ITEM_SIZE.position.x+10, GUI_ITEM_SIZE.position.y+GUI_ITEM_SIZE.height*2+10,GUI_ITEM_SIZE.width-20, GUI_ITEM_SIZE.height},"Y:","H", &APERTURE.relative_aperture_position_y,0,0.99);

        GuiSlider((Rectangle){GUI_ITEM_SIZE.position.x+10, GUI_ITEM_SIZE.position.y+GUI_ITEM_SIZE.height*3+15,GUI_ITEM_SIZE.width-20, GUI_ITEM_SIZE.height}, "W:","u", &APERTURE.relative_aperture_width, 0.01,0.95);

        //Interface
        GuiDrawText("Lens Parameters:",(Rectangle){GUI_ITEM_SIZE.position.x, GUI_ITEM_SIZE.position.y+GUI_ITEM_SIZE.height*4+20,GUI_ITEM_SIZE.width, GUI_ITEM_SIZE.height},TEXT_ALIGN_LEFT,LIGHTGRAY);

        GuiSlider((Rectangle){GUI_ITEM_SIZE.position.x+10, GUI_ITEM_SIZE.position.y+GUI_ITEM_SIZE.height*5+25,GUI_ITEM_SIZE.width-20, GUI_ITEM_SIZE.height},"X:","W",&INTERFACE.relative_position.x,0.1,0.95);

        GuiSlider((Rectangle){GUI_ITEM_SIZE.position.x+10, GUI_ITEM_SIZE.position.y+GUI_ITEM_SIZE.height*6+30,GUI_ITEM_SIZE.width-20, GUI_ITEM_SIZE.height},"Y:","W",&INTERFACE.relative_position.y,0.1,0.95);

        GuiSlider((Rectangle){GUI_ITEM_SIZE.position.x+10, GUI_ITEM_SIZE.position.y+GUI_ITEM_SIZE.height*7+35,GUI_ITEM_SIZE.width-20, GUI_ITEM_SIZE.height},"A:","W",&INTERFACE.relative_width,0.01,0.95);

        GuiSlider((Rectangle){GUI_ITEM_SIZE.position.x+10, GUI_ITEM_SIZE.position.y+GUI_ITEM_SIZE.height*8+40,GUI_ITEM_SIZE.width-20, GUI_ITEM_SIZE.height},"B:","W",&INTERFACE.relative_height,0.01,0.95);

        //WAVE properties
        GuiDrawText("Wave Parameters",(Rectangle){GUI_ITEM_SIZE.position.x, GUI_ITEM_SIZE.position.y+GUI_ITEM_SIZE.height*9+45,GUI_ITEM_SIZE.width, GUI_ITEM_SIZE.height},TEXT_ALIGN_LEFT, LIGHTGRAY);

        GuiColorPicker((Rectangle){GUI_ITEM_SIZE.position.x, GUI_ITEM_SIZE.position.y+GUI_ITEM_SIZE.height*10+50,GUI_ITEM_SIZE.width-30, GUI_ITEM_SIZE.width-50},"",&waveColor);

        GuiSlider((Rectangle){GUI_ITEM_SIZE.position.x+10, GUI_ITEM_SIZE.position.y+GUI_ITEM_SIZE.height*11+55+GUI_ITEM_SIZE.width-50,GUI_ITEM_SIZE.width-30, GUI_ITEM_SIZE.height},"f:", "r/s",&omega,0.1,2);

        if(GuiButton((Rectangle){GUI_ITEM_SIZE.position.x+10, SCREEN_HEIGHT-GUI_ITEM_SIZE.height-20, GUI_ITEM_SIZE.width-10,GUI_ITEM_SIZE.height},"Clear")){
            for(int i=0; i<xSize; i++){
                for(int j=0; j<ySize; j++){
                    E[i][j][2]=0;
                    E[i][j][1]=0;
                    E[i][j][0]=0;
                }
            }
        }

        EndDrawing();
        t+=dt;
        
    }
    CloseWindow();

}