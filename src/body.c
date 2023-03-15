#include <stdlib.h>
#include "simple_logger.h"

#include "gfc_config.h"

#include "gf2d_draw.h"
#include "body.h"

Uint8 body_shape_collide(Body* a, Shape* s, Vector2D* poc, Vector2D* normal);

void body_draw(Body* body, Vector2D offset)
{
    Color color;
    Shape shape;
    Vector2D center;
    if (!body)return;
    color = gfc_color8(0, 255, 255, 255);
    // draw center point
    vector2d_add(center, body->position, offset);
    gf2d_draw_circle(center, 2, color);

    color = gfc_color8(255, 0, 255, 255);
    gfc_shape_copy(&shape, *body->shape);
    gfc_shape_move(&shape, body->position);
    gf2d_draw_shape(shape, color, offset);
}

void body_update(Body* body) {
    if (!body)return;
    //TODO

}

Shape body_to_shape(Body* a)
{
    Shape aS = { 0 };
    if (!a)return aS;
    gfc_shape_copy(&aS, *a->shape);
    gfc_shape_move(&aS, a->position);
    return aS;
}

Uint8 body_body_collide(Body* a, Body* b) {
    if ((!a) || (!b))
    {
        slog("missing body in collision check");
        return 0;
    }
    return gfc_shape_overlap(body_to_shape(a), body_to_shape(b));
}