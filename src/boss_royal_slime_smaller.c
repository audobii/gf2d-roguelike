#include "simple_logger.h"
#include "boss_royal_slime_smaller.h"
#include "enemy_bigger_slime.h"

#include "player.h"
#include "slime.h"
#include "level.h"

#include "entity_common.h"

static float internal_timer = 0;

void boss_slime_smaller_think(Entity* self);
void boss_slime_smaller_draw(Entity* self);

Entity* boss_slime_smaller_new(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)return NULL;
    ent->sprite = gf2d_sprite_load_image(
        "images/royal_slime_phase2.png");
    ent->think = boss_slime_smaller_think;
    ent->draw = boss_slime_smaller_draw;
    ent->takeDamage = entity_damage;
    vector2d_copy(ent->position, position);
    ent->drawOffset = vector2d(64, 64);
    ent->speed = 0.6;
    ent->rotation = 0;
    ent->rotatable = false;

    //body/collision stuff
    ent->shape = gfc_shape_circle(0, 0, 58);
    ent->body.shape = &ent->shape;
    ent->body.team = 2;
    vector2d_copy(ent->body.position, position);

    ent->health = 100;

    gfc_line_cpy(ent->name, "boss_slime_smaller");
    return ent;
}

void boss_slime_smaller_draw(Entity* self) {
    if (!self)return;
    gf2d_sprite_draw(
        self->sprite,
        self->position,
        NULL,
        &self->drawOffset,
        &self->rotation,
        NULL,
        NULL,
        0);

    //gf2d_draw_pixel(self->position, gfc_color8(255, 255, 255, 160));
    //gf2d_draw_circle(self->position, 10, gfc_color8(255, 255, 255, 160));
}

void boss_slime_smaller_think(Entity* self)
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

    internal_timer += 0.1;

    if (internal_timer > 5.0) {
        entity_do_contact_damage(self);
        internal_timer = 0;
    }

    if (self->isPoisoned) {
        self->health -= 0.1;
    }

    if (self->isBurned) {
        self->health -= 0.1;
    }

    if (self->health <= 0)
    {
        level_add_entity(level_get_active_level(), bigger_slime_new(self->position));

        entity_clear_from_level(self);
        entity_free(self);
    }
}

/*eol@eof*/