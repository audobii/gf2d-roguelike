#ifndef __GF2D_COLLISION_H__
#define __GF2D_COLLISION_H__

#include "gfc_list.h"
#include "gfc_text.h"
#include "gfc_shape.h"

#include "body.h"

typedef struct Collision_S
{
    Uint8    collided;          /**<true if the there as a collision*/
    Uint8    blocked;           /**<true if this blocked any further movement.  Default for non elastic collisions*/
    Vector2D pointOfContact;    /**<point in space that contact was made*/
    Vector2D normal;            /**<normal vector at the point of contact*/
    Shape    shape;             /**<shape information on what what contacted*/
    Body* body;              /**<body information if a body was collided with*/
    Uint8    bounds;            /**<true if this collision was with the space bounds*/
    float    timeStep;          /**<at what time step contact was made*/
}Collision;

/**
 * @brief allocate data for a new collision
 * @return a new initialized collision or NULL on error
 */
Collision* collision_new();

/**
 * @brief empty the list, without freeing the data
 * @param list the collision list to clear
 */
void gf2d_collision_list_clear(List* list);

/**
 * @brief free data allocated for a collision
 * @param collision the collision to free
 */
void gf2d_collision_free(Collision* collision);

/**
 * @brief free all the collisions and the list containing it.
 * @param list must contain a list of collisions
 */
void gf2d_collision_list_free(List* list);

/**
 * @brief check if there is overlap (collision) between body/active entity and static shape
 * @return NULL if no collision, new Collision otherwise
 */
Collision* gf2d_collision_body_shape(Shape s, Body* b);

/**
 * @brief check if there is overlap (collision) between 2 bodies/active entities
 * @return NULL if no collision, new Collision otherwise
 */
Collision* gf2d_collision_body_body(Body* b1, Body* b2);

/**
 * @brief build list of collisions based on static shapes and active bodies list
 * @param collisions the list of collisions to add to
 * @param staticShapes the list of static shapes to check against
 * @param activeBodies the list of active entities's bodies
 */
List* gf2d_collision_build_list(List* collisions, List* staticShapes, List* activeBodies);

/**
 * @brief do something to each collision and then remove the collision from the list
 * @param list must contain a list of collisions
 */
void gf2d_collision_update(List* list);

#endif
