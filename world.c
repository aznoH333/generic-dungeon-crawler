#ifndef WORLD
#define WORLD

#include "gframework3d.c"


//------------------------------------------------------
// pre dec
//------------------------------------------------------
void generateLevel();



//------------------------------------------------------
// world blocks
//------------------------------------------------------
#define WORLD_SIZE 30
#define BLOCK_NOTHING -1
struct WorldBlock{
    int north;
    int west;
    int south;
    int east;
    bool isFilled;
};typedef struct WorldBlock WorldBlock;

WorldBlock initEmptyWorldBlock(){
    WorldBlock out = {BLOCK_NOTHING, BLOCK_NOTHING, BLOCK_NOTHING, BLOCK_NOTHING, false };
    return out;
}

WorldBlock initBlockBasedOnNeighbors(int x, int y, int worldPregenData[WORLD_SIZE][WORLD_SIZE]){
    WorldBlock out;
    out.north = BLOCK_NOTHING;
    out.east = BLOCK_NOTHING;
    out.west = BLOCK_NOTHING;
    out.south = BLOCK_NOTHING;


    if (worldPregenData[x + 1][y] != 0){
        out.north = TEXTURE_WALL;
    }
    if (worldPregenData[x - 1][y] != 0){
        out.south = TEXTURE_WALL;
    }
    if (worldPregenData[x][y - 1] != 0){
        out.west = TEXTURE_WALL;
    }
    if (worldPregenData[x][y + 1] != 0){
        out.east = TEXTURE_WALL;
    }

    out.isFilled = true;

    return out;
}


void drawWorldBlock(WorldBlock* block, int x, int y){

    if (block->isFilled){

        drawTexturedPlane(TEXTURE_FLOOR, (Vector3){x,0,y}, ROTATION_FLOOR);
        drawTexturedPlane(TEXTURE_CEILING, (Vector3){x,0,y}, ROTATION_CEILING);

        if (block->north != BLOCK_NOTHING){
            drawTexturedPlane(block->north, (Vector3){x,0,y}, ROTATION_NORTH);
        }
        if (block->south != BLOCK_NOTHING){
            drawTexturedPlane(block->south, (Vector3){x,0,y}, ROTATION_SOUTH);
        }
        if (block->west != BLOCK_NOTHING){
            drawTexturedPlane(block->west, (Vector3){x,0,y}, ROTATION_WEST);
        }
        if (block->east != BLOCK_NOTHING){
            drawTexturedPlane(block->east, (Vector3){x,0,y}, ROTATION_EAST);
        }

    }

}

WorldBlock world[WORLD_SIZE][WORLD_SIZE];





//------------------------------------------------------
// controll
//------------------------------------------------------
void worldInit(){
    generateLevel();
}

void worldDispose(){

}

//------------------------------------------------------
// world generation
//------------------------------------------------------
void generateTileData(int worldPregenData[WORLD_SIZE][WORLD_SIZE]){
    for (int x = 0; x < WORLD_SIZE; x++){
            for (int y = 0; y < WORLD_SIZE; y++){

                if (x == 0 || y == 0 || x == WORLD_SIZE - 1 || y == WORLD_SIZE - 1){
                    worldPregenData[x][y] = 1;
                }
                else {

                    if (GetRandomValue(0, 10) < 3){
                        worldPregenData[x][y] = 1;
                    }else {
                        worldPregenData[x][y] = 0;
                    }
                }
            }
        }
}



void generateLevel(){

    int worldPregenData[WORLD_SIZE][WORLD_SIZE];
    { // pregenete world
        generateTileData(worldPregenData);
    }

    { // build world blocks
        for (int x = 0; x < WORLD_SIZE; x++){
            for (int y = 0; y < WORLD_SIZE; y++){

                int tile = worldPregenData[x][y];

                if (tile == 0){
                    world[x][y] = initBlockBasedOnNeighbors(x, y, worldPregenData);
                }else {
                    world[x][y] = initEmptyWorldBlock();
                }

            }
        }
    }
}


//------------------------------------------------------
// update
//------------------------------------------------------
void worldUpdate(){
    for (int x = 0; x < WORLD_SIZE; x++){
        for (int y = 0; y < WORLD_SIZE; y++){
            drawWorldBlock(&world[x][y], x, y);
        }
    }

}

#endif
