#include "simple_logger.h"
#include "enemy_crystal_guy.h"

#include "player.h"
#include "projectile.h"

#include "entity_common.h"

#include "gf2d_draw.h"

static float internal_timer = 0;

void crystal_guy_think(Entity* self);
void crystal_guy_draw(Entity* self);

Entity* crystal_guy_new(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)return NULL;
    ent->sprite = gf2d_sprite_load_image("images/crystal_guy.png");

    ent->think = crystal_guy_think;
    ent->draw = crystal_guy_draw;
    ent->takeDamage = entity_damage;

    vector2d_copy(ent->position, position);
    ent->drawOffset = vector2d(32, 36);
    ent->speed = 0;
    ent->rotation = 0;
    ent->rotatable = false;

    //body/collision stuff
    ent->shape = gfc_shape_circle(0, 0, 28);
    ent->body.shape = &ent->shape;
    ent->body.team = 2;
    vector2d_copy(ent->body.position, position);

    ent->health = 50;

    gfc_line_cpy(ent->name, "crystal_guy");
    return ent;
}

void crystal_guy_draw(Entity* self) {
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

    gf2d_draw_pixel(self->position, gfc_color8(255, 255, 255, 160));
    gf2d_draw_circle(self->position, 10, gfc_color8(255, 255, 255, 160));
}

void crystal_guy_think(Entity* self)
{
    Vector2D m, dir;
    int mx, my;
    Vector2D player_pos = player_get_position();

    if (!self)return;
    SDL_GetMouseState(&mx, &my);
    m.x = mx;
    m.y = my;
    vector2d_sub(dir, player_pos, self->position);

    internal_timer += 0.1;

    if (internal_timer > 8.0) {
        projectile_new(self, self->position, dir, 5, 12.0, 17, 40);
        internal_timer = 0;
    }

    if (self->isPoisoned) {
        self->health -= 0.1;
    }

    if (self->health <= 0)
    {
        entity_clear_from_level(self);
        entity_drop_loot(self);
        entity_free(self);
    }
}

/*eol@eof*/