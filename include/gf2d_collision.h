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

#endif
