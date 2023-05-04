#include "simple_logger.h"
#include "boss_royal_slime.h"

#include "player.h"
#include "slime.h"
#include "level.h"
#include "projectile.h"

#include "entity_common.h"
#include "enemy_glowfly.h"
#include "enemy_sentient_drill.h"

static float internal_timer = 0;
static float spawn_timer = 0;
static float proj_timer = 0;

void boss_slime_think(Entity* self);
void boss_slime_draw(Entity* self);

Entity* boss_slime_new(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)return NULL;
    ent->sprite = gf2d_sprite_load_image("images/royal_slime.png");
    ent->think = boss_slime_think;
    ent->draw = boss_slime_draw;
    ent->takeDamage = entity_damage;
    vector2d_copy(ent->position, position);
    ent->drawOffset = vector2d(128, 128);
    ent->speed = 0.2;
    ent->rotation = 0;
    ent->rotatable = false;

    //body/collision stuff
    ent->shape = gfc_shape_circle(0, -20, 96);
    ent->body.shape = &ent->shape;
    ent->body.team = 2;
    vector2d_copy(ent->body.position, position);

    ent->health = 465;

    gfc_line_cpy(ent->name, "boss_slime");
    return ent;
}

void boss_slime_draw(Entity* self) {
    if (!self)return;
    //this is temp, its just cuz player sprite is a still image
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

    Sprite* bar = gf2d_sprite_load_image("images/boss_bar.png");

    //health
    if (self->health > 0) {
        gf2d_draw_rect_filled(gfc_rect(380, 650, self->health, 35), GFC_COLOR_RED);
    }
    gf2d_sprite_draw_image(bar, vector2d(350, 650));
}

void boss_slime_think(Entity* self)
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
    spawn_timer += 0.1;
    proj_timer += 0.1;

    if (internal_timer > 3.0) {
        entity_do_contact_damage(self);
        
        internal_timer = 0;
    }

    //this doesnt work?
    if (proj_timer > 11.0) {
        projectile_new(self, vector2d(self->position.x+50, self->position.y-100), dir, 5, 12.0, 17, 40);
        proj_timer = 0;
    }

    if (spawn_timer > 40.0) {
        int rand_num = rand() % 2;

        if (rand_num < 1) {
            level_add_entity(level_get_active_level(), glowfly_new(self->position));
        }
        else {
            level_add_entity(level_get_active_level(), sentient_drill_new(self->position));
        }

        spawn_timer = 0;
    }

    if (self->isPoisoned) {
        self->health -= 0.1;
    }

    if (self->isBurned) {
        self->health -= 0.1;
    }

    if (self->health <= 0)
    {
        Vector2D offset_pos;
        offset_pos = vector2d(self->position.x + 40, self->position.y + 40);
        level_add_entity(level_get_active_level(), slime_new(self->position));
        level_add_entity(level_get_active_level(), slime_new(offset_pos));

        entity_clear_from_level(self);
        entity_free(self);
    }
}

/*eol@eof*/