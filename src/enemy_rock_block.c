#include "simple_logger.h"
#include "enemy_rock_block.h"

#include "player.h"

#include "entity_common.h"

static float internal_timer = 0;

void rock_block_think(Entity* self);
void rock_block_draw(Entity* self);

Entity* rock_block_new(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)return NULL;
    ent->sprite = gf2d_sprite_load_image("images/angry_block.png");
    
    ent->think = rock_block_think;
    ent->draw = rock_block_draw;

    ent->takeDamage = entity_damage;
    vector2d_copy(ent->position, position);
    ent->drawOffset = vector2d(32, 34);
    ent->speed = 2.0;
    ent->rotatable = false;

    //body/collision stuff
    ent->shape = gfc_shape_circle(0, 0, 30);
    ent->body.shape = &ent->shape;
    ent->body.team = 2;
    vector2d_copy(ent->body.position, position);

    ent->health = 60;

    gfc_line_cpy(ent->name, "rock_block");
    return ent;
}

void rock_block_draw(Entity* self) {
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

//FOR SOME REASON blockRoom.json is NOT REALLY WORKING - idk if its cuz of here
//two of the blocks in the middle and right dont hit player/no collision
//is it because of multiple instances...?
void rock_block_think(Entity* self)
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
        //vector2d_copy(self->velocity, dir);
        //vector2d_copy(self->body.velocity, dir);

        self->velocity.x = dir.x;
        self->body.velocity.x = dir.x;
    }
    else
    {
        vector2d_clear(self->velocity);
        vector2d_clear(self->body.velocity);
    }

    //cant be poisoned
    /*
    if (self->isPoisoned) {
        self->health -= 0.1;
    }
    */

    internal_timer += 0.1;

    if (internal_timer > 5.0) {
        entity_do_contact_damage(self);
        internal_timer = 0;
    }

    if (self->health <= 0)
    {
        slog("died...");
        entity_clear_from_level(self);
        entity_drop_loot(self);
        entity_free(self);
    }
}

/*eol@eof*/