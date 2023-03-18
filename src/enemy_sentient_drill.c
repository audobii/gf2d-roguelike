#include "simple_logger.h"
#include "enemy_sentient_drill.h"

#include "player.h"

#include "entity_common.h"

static float internal_timer = 0;

void sentient_drill_think(Entity* self);

Entity* sentient_drill_new(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)return NULL;
    ent->sprite = gf2d_sprite_load_all(
        "images/drill.png",
        32,
        32,
        16,
        0);
    ent->think = sentient_drill_think;
    ent->takeDamage = entity_damage;
    vector2d_copy(ent->position, position);
    ent->drawOffset = vector2d(16, 16);
    ent->speed = 2.0;
    ent->rotatable = true;

    //body/collision stuff
    ent->shape = gfc_shape_circle(0, 0, 14);
    ent->body.shape = &ent->shape;
    ent->body.team = 2;
    vector2d_copy(ent->body.position, position);

    ent->health = 40;

    gfc_line_cpy(ent->name, "sentient_drill");
    return ent;
}

void sentient_drill_think(Entity* self)
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

    if (self->isPoisoned) {
        self->health -= 0.1;
    }

    internal_timer += 0.1;

    if (internal_timer > 5.0) {
        if (self->speed >= 2.0) {
            self->speed = 1.0;
        }
        else {
            self->speed = 2.0;
        }
        entity_do_contact_damage(self);
        internal_timer = 0;
    }

    if (self->health <= 0)
    {
        slog("died...");
        entity_clear_from_level(self);
        entity_free(self);
    }
}

/*eol@eof*/