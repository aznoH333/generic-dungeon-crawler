/*******************************************************************************************
*
*   raylib [core] example - Basic window
*
*   Welcome to raylib!
*
*   To test examples, just press F6 and execute raylib_compile_execute script
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   Example originally created with raylib 1.0, last time updated with raylib 1.0
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2013-2024 Ramon Santamaria (@raysan5)
*
********************************************************************************************/
#include "gframework3d.c"
#include "raylib.h"
#include "world.c"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    initFramework();
    Texture2D debug1 = LoadTexture("resources/debug_0002.png");
    // generate mesh
    Mesh plane = GenMeshPlane(1, 1, 1, 1);
    // convert to model
    Model m = LoadModelFromMesh(plane);
    // set texture
    m.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = debug1;

    // position
    Vector3 position = { 0.0f, 0.0f, 0.0f };
    Vector3 rotation = { 1.0f, 0.0f, 0.0f };
    Vector3 scale = {1.0f, 1.0f, 1.0f} ;


    // world stuff
    worldInit();

    // Main game loop
    while (!WindowShouldClose())
    {
        
        fDrawBegin();

            ClearBackground(RAYWHITE);
            DrawGrid(10, 1.0);

            //DrawModel(m, position, 1.0f, WHITE);
            //DrawModelEx(m, position, rotation, 90, scale, WHITE);
            worldUpdate();


            /*
            drawTexturedPlane(TEXTURE_FLOOR, (Vector3){0,0,0}, ROTATION_FLOOR);
            drawTexturedPlane(TEXTURE_WALL, (Vector3){0.0f,0.0f,0.0f}, ROTATION_WEST);
            drawTexturedPlane(TEXTURE_WALL, (Vector3){0.0f,0.0f,0.0f}, ROTATION_SOUTH);
            drawTexturedPlane(TEXTURE_CEILING, (Vector3){0.0,0.0f,0}, ROTATION_CEILING);
            drawTexturedPlane(TEXTURE_WALL, (Vector3){0.0,0.0f,0}, ROTATION_EAST);
            drawTexturedPlane(TEXTURE_WALL, (Vector3){0.0,0.0f,0}, ROTATION_NORTH);
            */



        fDrawEnd();
        
    }

	disposeFramework();
    // unload all
    UnloadTexture(debug1);
    UnloadMesh(plane);
    UnloadModel(m);
    worldDispose();

    return 0;
}
