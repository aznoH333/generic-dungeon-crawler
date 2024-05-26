#ifndef G_FRAMEWORK
#define G_FRAMEWORK

#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
//------------------------------------------------------
// Conf
//------------------------------------------------------
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const char* WINDOW_NAME = "template window";
const int DEFAULT_SPRITE_SIZE = 32;
const float DEFAULT_CAMERA_ZOOM = 2.0f;


//------------------------------------------------------------------------------------
// UTILITY
//------------------------------------------------------------------------------------
bool checkBoxCollisions(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2){
    return x1 + w1 > x2 &&
           x1 < x2 + w2 &&
           y1 + h1 > y2 &&
           y1 < y2 + h2;
}

float lerp(float a, float b, float w){
    return a * (1.0 - w) + (b * w);
}

float pythagoras(float x1, float y1, float x2, float y2){
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

float dirTowards(int x1, int y1, int x2, int y2){
    return atan2(x1 - x2, y1 - y2);
}

int min(int a, int b){
    if (a < b){
        return a;
    }
    return b;
}

float sign(float input){
	if (input == 0){
		return 0;
	}else if (input > 0){
		return 1;
	}
	return -1;
}

void printMatrix(Matrix* matrix){

		printf("{%f, %f, %f, %f} \n {%f, %f, %f, %f} \n {%f, %f, %f, %f} \n {%f, %f, %f, %f} \n", matrix->m0, matrix->m1, matrix->m2, matrix->m3, matrix->m4, matrix->m5, matrix->m6, matrix->m7, matrix->m8, matrix->m9, matrix->m10, matrix->m11, matrix->m12, matrix->m13, matrix->m14, matrix->m15);

}

//------------------------------------------------------
// sprites
//------------------------------------------------------
struct FrameworkSpriteSheet{
	Texture2D spriteSheetTexture;
	int width;
	int height;

};
typedef struct FrameworkSpriteSheet FrameworkSpriteSheet;

FrameworkSpriteSheet mainSpriteSheet;
FrameworkSpriteSheet initSpriteSheet(){
	FrameworkSpriteSheet out;
	out.spriteSheetTexture = LoadTexture("resources/spritesheet.png");
	out.width = out.spriteSheetTexture.width / DEFAULT_SPRITE_SIZE;
	out.height = out.spriteSheetTexture.height / DEFAULT_SPRITE_SIZE;

	return out;
}

void unloadSpriteSheet(FrameworkSpriteSheet spriteSheet){
	UnloadTexture(spriteSheet.spriteSheetTexture);
}



//------------------------------------------------------
// logging functions
//------------------------------------------------------

#define LOG_LEVEL_WARN 2
#define LOG_LEVEL_INFO 1
#define LOG_LEVEL_ERROR 0
#define CURRENT_LOG_LEVEL 2

void fLog(int level, const char* message){
	if (level <= CURRENT_LOG_LEVEL){
		printf("%s", message);
	}
}

void logError(const char* error){
	fLog(LOG_ERROR, error);
}


//------------------------------------------------------
// Variables
//------------------------------------------------------
FrameworkSpriteSheet loadedSheet;
RenderTexture2D renderTexture;
Camera camera = { 0 };
float scalingFactor;
int renderTextureOffset;
float screenShakeAmmount = 0.0f;
int fTimer = 0;

//------------------------------------------------------
// camera
//------------------------------------------------------
void screenShake(float ammount){
	screenShakeAmmount += ammount;
}


//------------------------------------------------------
// textures
//------------------------------------------------------
#define MAX_TEXTURE_ARR_SIZE 256
Texture2D loadedTextures[MAX_TEXTURE_ARR_SIZE];
int nextTextureIndex = 0;
void loadTexture(const char* path){
	loadedTextures[nextTextureIndex] = LoadTexture(path);
	nextTextureIndex++;
	if (nextTextureIndex >= MAX_TEXTURE_ARR_SIZE){
		exit(-1);
		logError("Maximum texture limit exceeded");
	}
}

void initTextures(){
	#define TEXTURE_FLOOR 0
	loadTexture("resources/debug_0001.png");

	#define TEXTURE_WALL 1
	loadTexture("resources/debug_0002.png");

	#define TEXTURE_CEILING 2
	loadTexture("resources/debug_0003.png");
}

void unloadTextures(){
	for (int i = 0; i <= nextTextureIndex; i++){
		UnloadTexture(loadedTextures[i]);
	}
}


Texture2D* getTexture(int textureId){
	return &loadedTextures[textureId];
}

//------------------------------------------------------
// rotation lookup
//------------------------------------------------------


#define ROTATION_COUNT 6
Matrix* rotationLookup;

#define ROTATION_FLOOR 0
#define ROTATION_CEILING 1
#define ROTATION_WEST 2
#define ROTATION_SOUTH 3
#define ROTATION_EAST 4
#define ROTATION_NORTH 5

void initRotations(){
	rotationLookup = malloc(sizeof(Matrix) * ROTATION_COUNT);
	rotationLookup[ROTATION_FLOOR] = (Matrix)	{
												1.0f, 0.0f, 0.0f, 0.0f,
												0.0f, 1.0f, 0.0f, 0.0f,
												0.0f, 0.0f, 1.0f, 0.0f,
												0.0f, 0.0f, 0.0f, 1.0f
												};

	rotationLookup[ROTATION_CEILING] = (Matrix)	{
												1.0f, 0.0f, 0.0f, 0.0f,
												0.0f, -1.0f, 0.0f, 1.0f,
												0.0f, 0.0f, -1.0f, 0.0f,
												0.0f, 0.0f, 0.0f, 1.0f
												};
	rotationLookup[ROTATION_NORTH] = (Matrix)	{
												0.0f, 0.0f, 0.0f, 0.5f,
												0.0f, 0.0f, -1.0f, 0.5f,
												1.0f, 0.0f, 0.0f, 0.0f,
												0.0f, 0.0f, 0.0f, 1.0f
												};
	rotationLookup[ROTATION_WEST] = (Matrix)	{
												1.0f, 0.0f, 0.0f, 0.0f,
												0.0f, 0.0f, -1.0f, 0.5f,
												0.0f, -1.0f, 0.0f, -0.5f,
												0.0f, 0.0f, 0.0f, 1.0f
												};
	rotationLookup[ROTATION_EAST] = (Matrix)	{
												-1.0f, 0.0f, 0.0f, 0.0f,
												0.0f, 0.0f, -1.0f, 0.5f,
												0.0f, -1.0f, 0.0f, 0.5f,
												0.0f, 0.0f, 0.0f, 1.0f
												};
	rotationLookup[ROTATION_SOUTH] = (Matrix)	{
												0.0f, 1.0f, 0.0f, -0.5f,
												0.0f, 0.0f, -1.0f, 0.5f,
												-1.0f, 0.0f, 0.0f, 0.0f,
												0.0f, 0.0f, 0.0f, 1.0f
												};


}

void disposeRotations(){
	free(rotationLookup);
}



//------------------------------------------------------
// polygonal drawing
//------------------------------------------------------
Mesh plane;
Model planeModel;
Vector3 DEFAULT_SCALE = {1, 1, 1};
void initPolygons(){
	// generate mesh
    plane = GenMeshPlane(1, 1, 1, 1);
    // convert to model
    planeModel = LoadModelFromMesh(plane);
}

void disposePolygons(){
	UnloadMesh(plane);
    UnloadModel(planeModel);
}

void drawTexturedPlane(int texture, Vector3 position, int rotation){
	// set texture
    planeModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = *getTexture(texture);

	//printMatrix(&planeModel.transform);
	planeModel.transform = rotationLookup[rotation];
	DrawModel(planeModel, position, 1.0f, WHITE);
}

//------------------------------------------------------
// drawing
//------------------------------------------------------


void fDrawBegin(){
	BeginTextureMode(renderTexture);
    BeginMode3D(camera);
	UpdateCamera(&camera, CAMERA_FREE);
	fTimer++;
}

void fDrawEnd(){
	EndMode3D();
    EndTextureMode();

    BeginDrawing();
    ClearBackground(BLACK);
    Rectangle r = { 0, 0, (float)(renderTexture.texture.width), (float)(-renderTexture.texture.height) };
    Rectangle r2 = { renderTextureOffset, 0, (float)(GetScreenWidth()) * scalingFactor, (float)(GetScreenHeight()) };
    Vector2 v = {0, 0};
    DrawTexturePro(renderTexture.texture,r,r2,v,0,WHITE);

    EndDrawing();
}

//------------------------------------------------------
// init
//------------------------------------------------------
void initFramework(){
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_NAME);
	SetTargetFPS(60);
	renderTexture = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
	loadedSheet = initSpriteSheet();
	scalingFactor = SCREEN_WIDTH /(float)(GetScreenWidth());
	renderTextureOffset = ((GetScreenWidth()) / 2) - (SCREEN_WIDTH / 2);
	ToggleFullscreen();
	camera.position = (Vector3){0.0f, 0.2f, 0.0f};
	camera.position = (Vector3){ 0.0f, 2.0f, 4.0f };
    camera.target = (Vector3){ 0.0f, 2.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	HideCursor();
	DisableCursor();
	initRotations();
	initPolygons();
	initTextures();
}

//------------------------------------------------------
// dispose
//------------------------------------------------------
void disposeFramework(){
	CloseWindow();
	unloadSpriteSheet(loadedSheet);
	UnloadRenderTexture(renderTexture);
	disposePolygons();
	unloadTextures();
	disposeRotations();
}


#endif
