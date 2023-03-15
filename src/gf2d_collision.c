#include <stdlib.h>
#include "simple_logger.h"

#include "gf2d_draw.h"
#include "gf2d_collision.h"

#include "level.h"

Collision* collision_new() {
    Collision* collision = NULL;
    collision = (Collision*)malloc(sizeof(Collision));
    if (!collision)
    {
        slog("failed to allocate data for a collision object");
        return NULL;
    }
    memset(collision, 0, sizeof(Collision));
    return collision;
}

void gf2d_collision_free(Collision* collision)
{
    if (!collision)return;
    free(collision);
}

void gf2d_collision_list_clear(List* list)
{
    int i, count;
    Collision* collision;
    if (!list)return;
    count = gfc_list_get_count(list);
    for (i = 0; i < count; i++)
    {
        collision = (Collision*)gfc_list_get_nth(list, i);
        if (!collision)continue;
        gf2d_collision_free(collision);
    }
    gfc_list_clear(list);
}

void gf2d_collision_list_free(List* list)
{
    int i, count;
    Collision* collision;
    if (!list)return;
    count = gfc_list_get_count(list);
    for (i = 0; i < count; i++)
    {
        collision = (Collision*)gfc_list_get_nth(list, i);
        if (!collision)continue;
        gf2d_collision_free(collision);
    }
    gfc_list_delete(list);
}

Collision* gf2d_collision_body_shape(Shape s, Body* b) {
    Shape b_shape;
    Collision* collision;

    Vector2D poc, normal;
    if (!b)return NULL;

    b_shape = body_to_shape(b);

    if (!gfc_shape_overlap_poc(s, b_shape, &poc, &normal))
    {
        return NULL;
    }

    collision = collision_new();
    collision->collided = 1;
    collision->blocked = 1;
    vector2d_copy(collision->pointOfContact, poc);
    vector2d_copy(collision->normal, normal);
    collision->shape = *b->shape;
    collision->body = b;
    collision->bounds = 0;
    collision->timeStep = 0;
    return collision;
}

void gf2d_collision_update(List* list) {
    Body* b;

    int i, count;
    Collision* collision;
    if (!list)return;
    count = gfc_list_get_count(list);
    for (i = 0; i < count; i++)
    {
        collision = (Collision*)gfc_list_get_nth(list, i);
        if (!collision)continue;

        //TODO: math here about messing w position/velocity of body??
        b = collision->body;
        vector2d_sub(b->position, b->position, b->velocity);

        //check if projectile hitting wall - delete body if so
        if (!gfc_line_cmp(b->name, "proj")) {
            gfc_list_delete_data(level_get_active_level()->activeBodies, b);
            gfc_line_cpy(b->name, "FREEME");
        }

        //slog("oof! a collision happened");
    }

    gf2d_collision_list_clear(list);
}

List* gf2d_collision_build_list(List* collisions, List* staticShapes, List* activeBodies) {
    Collision* collision;
    Shape* s;
    Body* b;

    for (int j = 0; j < staticShapes->count; j++) {
        for (int i = 0; i < activeBodies->count; i++) {

            s = gfc_list_get_nth(staticShapes, j);
            b = gfc_list_get_nth(activeBodies, i);
            collision = gf2d_collision_body_shape(*s, b);

            if (collision == NULL)continue;
            collisions = gfc_list_append(collisions, (void*)collision);
        }
    }

    return collisions;
}