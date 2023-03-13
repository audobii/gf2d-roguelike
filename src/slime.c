#include "simple_logger.h"
#include "slime.h"

void slime_think(Entity* self);


Entity* slime_new(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)return NULL;
    ent->sprite = gf2d_sprite_load_all(
        "images/tiny_slime.png",
        32,
        32,
        16,
        0);
    ent->think = slime_think;
    vector2d_copy(ent->position, position);
    ent->drawOffset = vector2d(16, 16);
    ent->speed = 1.3;
    ent->rotation = 0;
    ent->rotatable = false;

    //body/collision stuff
    ent->shape = gfc_shape_circle(0, 0, 12);
    ent->body.shape = &ent->shape;
    ent->body.team = 1;
    vector2d_copy(ent->body.position, position);
    return ent;
}

void slime_think(Entity* self)
{
    Vector2D m, dir;
    int mx, my;
    if (!self)return;
    SDL_GetMouseState(&mx, &my);
    m.x = mx;
    m.y = my;
    vector2d_sub(dir, m, self->position);
    if (vector2d_magnitude_compare(dir, 10) > 0)
    {
        vector2d_set_magnitude(&dir, self->speed);
        vector2d_copy(self->velocity, dir);
        vector2d_copy(self->body.position, self->position);
    }
    else
    {
        vector2d_clear(self->velocity);
    }
}

/*eol@eof*/