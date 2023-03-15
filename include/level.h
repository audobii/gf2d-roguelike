#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "gfc_text.h"
#include "gfc_shape.h"
#include "gfc_list.h"

#include "gf2d_sprite.h"
#include "entity.h"

typedef struct
{
    int tileFrame;
    int solid;  // if true the tile cannot be walked through
}TileInfo;

typedef struct
{
    TextLine    name;
    Sprite* tileSet;
    Sprite* tileSetAlt;
    Vector2D    tileSize;    //the width and height of the individual tile?
    Vector2D    mapSize;    //the width and height of the tile map
    int* tileMap;    
    Sprite* tileLayer;  
    List* staticShapes;      //list of static shapes to clip against in the game world
    List* activeEntities;
    List* activeBodies;
}Level;

/**
 * @brief get the set active level pointer if it is set
 * @return NULL if no active level, or a pointer to it
 */
Level* level_get_active_level();

/**
 * @brief check if a shape clips any of the level static shapes
 * @param level the level to check
 * @param shape the shape to check with
 * @return 0 if there is no overlap, 1 if there is
 */
int level_shape_clip(Level* level, Shape shape);

/**
 * @brief set the active level (or turn it off)
 * @param level the level to set it too (or NULL to turn off the level)
 */
void level_set_active_level(Level* level);

/**
 * @brief load a level from a json file
 * @param filename path to the json file containing level data
 * @return NULL on not found or other error, or the loaded level
 */
Level* level_load(const char* filename);

/**
 * @brief draw a level
 * @param level the level to draw
 */
void level_draw(Level* level);

/**
 * @brief allocate a blank level struct
 * @return NULL on error or an intialized level
 */
Level* level_new();

/**
 * @brief clean up a previously allocated level
 */
void level_free(Level* level);

/**
 * @brief make static shapes list for level 
 */
void level_build_static_collision_layer(Level* level);

/**
 * @brief draw static shapes list for level
 */
void level_draw_static_shapes(Level* level);

/**
 * @brief draw the shapes for each active entity's body (should be magenta circle)
 */
void level_draw_active_entities_bodies(Level* level);

/**
 * @brief add an entity to the list of active entities of the level
 * @param entity the entity to add
 */
void level_add_entity(Level* level, Entity* entity);

/**
 * @brief get list of current level's active bodies (derived from active entities)
 * @param level the level to get list from
 * @return list of active bodies for given level
 */
List* level_get_active_bodies(Level* level);

#endif