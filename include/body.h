#ifndef __BODY_H__
#define __BODY_H__

#include "gfc_list.h"
#include "gfc_text.h"
#include "gfc_shape.h"

/**
 * @purpose bodies are used to track physics enabled objects in the 2d collisions space system
 */

typedef struct Body_S
{
	TextLine name;
	Uint32 team;
	Vector2D position;
	Vector2D velocity;

	Shape* shape;
}Body;

/**
 * @brief draw a body to the screen.  Shape will be magenta, center point will be a green pixel
 * @param body the body to draw, a no-op if this is NULL
 * @param offset to adjust for camera or other position change relative to the body center
 */
void body_draw(Body* body, Vector2D offset);

void body_update(Body* body);

/**
 * @brief check if the two bodies provided are overlapping in any way
 * @param a one body
 * @param b the other body
 * @return 0 on error or no collision, 1 otherwise
 */
Uint8 body_body_collide(Body* a, Body* b);

/**
 * @brief get the shape, adjusted for position for the provided body
 * @param a the body to get the shape for
 * @return an empty {0} shape on error, or the body shape information otherwise
 */
Shape body_to_shape(Body* a);


#endif