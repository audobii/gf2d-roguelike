#include "simple_logger.h"
#include "slime.h"

#include "player.h"

#include "entity_common.h"

static float internal_timer = 0;

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
    ent->takeDamage = entity_damage;
    vector2d_copy(ent->position, position);
    ent->drawOffset = vector2d(16, 16);
    ent->speed = 1.3;
    ent->rotation = 0;
    ent->rotatable = false;

    //body/collision stuff
    ent->shape = gfc_shape_circle(0, 0, 12);
    ent->body.shape = &ent->shape;
    ent->body.team = 2;
    vector2d_copy(ent->body.position, position);

    ent->health = 25;

    gfc_line_cpy(ent->name, "slime");
    return ent;
}

void slime_think(Entity* self)
{
    Vector2D m, dir;
    int mx, my;
    Vector2D player_pos = player_get_position();

    if (!self)return;
    SDL_GetMouseState(&mx, &my);
    m.x = mx;
    m.y = my;
    vector2d_sub(dir, player_pos, self->position);
    if (vector2d_magnitude_compare(dir, 10) > 0)
    {
        vector2d_set_magnitude(&dir, self->speed);
        vector2d_copy(self->velocity, dir);
        //vector2d_copy(self->body.position, self->position);
        vector2d_copy(self->body.velocity, dir);
    }
    else
    {
        vector2d_clear(self->velocity);
        vector2d_clear(self->body.velocity);
    }

    /*
    internal_timer += 0.1;

    if (internal_timer > 5.0) {
        char str[20];
        sprintf(str, "%f", self->health);
        slog(str);
        internal_timer = 0;
    }
    */

    if (self->isPoisoned) {
        self->health -= 0.1;
    }

    if (self->health <= 0)
    {
        slog("died...");
        entity_clear_from_level(self);
        entity_free(self);
    }
}

/*eol@eof*/