#include "simple_logger.h"
#include "cave_hole.h"

#include "player.h"

#include "entity_common.h"
#include "gf2d_collision.h"

#include "level.h"

static const char* rooms[] = {"rooms/blockRoom.json", "rooms/enemyRoom1.json", "rooms/fliesncoins.json", "rooms/mazeRoom.json", "rooms/shop.json", "rooms/enemyRoom2.json", "rooms/trickRoom.json"};

void cave_hole_think(Entity* self);
void cave_hole_draw(Entity* self);

Entity* cave_hole_new(Vector2D position) {
    Entity* ent;
    ent = entity_new();
    if (!ent)return NULL;

    ent->sprite = gf2d_sprite_load_image("images/cave_hole.png");

    ent->think = cave_hole_think;
    ent->draw = cave_hole_draw;

    vector2d_copy(ent->position, position);

    ent->drawOffset = vector2d(32, 32);
    ent->speed = 0;

    //body/collision stuff
    ent->shape = gfc_shape_rect(-30, -12, 60, 20);
    ent->body.shape = &ent->shape;
    ent->body.team = 0;
    vector2d_copy(ent->body.position, position);

    gfc_line_cpy(ent->name, "cave_hole");
    return ent;
}

void cave_hole_draw(Entity* self) {
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

void cave_hole_think(Entity* self)
{
    //just keep checking if it makes contact with player
    List* activeEnts;
    Entity* player;
    Collision* collision;
    Uint32 p_mana;
    Level* old_level, *level;

    player = player_get();

    if (!player)return;
    collision = gf2d_collision_body_body(&self->body, &player->body);

    if (!collision)return;

    old_level = level_get_active_level();
    if (!old_level)return;

    //get rid of old entities - maybe put this in a function?
    for (int i = 0; i < gfc_list_get_count(old_level->enemiesToSpawn);i++) {
        entity_clear_from_level(gfc_list_get_nth(old_level->enemiesToSpawn, i));
        entity_free(gfc_list_get_nth(old_level->enemiesToSpawn, i));
    }

    //do a similar thing as above^ to entities such as shop and drops
    for (int i = 0; i < gfc_list_get_count(old_level->existing_entities); i++) {
        entity_free(gfc_list_get_nth(old_level->existing_entities, i));
    }

    level_free(old_level);

    int rand_num = rand() % 7;

    //temporary bad way to do this bc loading from rooms array not working
    /*
    switch (rand_num)
    {
    case 0:
        level = level_load("rooms/blockRoom.json");
        break;
    case 1:
        level = level_load("rooms/enemyRoom1.json");
        break;
    case 2:
        level = level_load("rooms/fliesncoins.json");
        break;
    case 3:
        level = level_load("rooms/mazeRoom.json");
        break;
    case 4:
        level = level_load("rooms/shop.json");
        break;
    default:
        level = level_load("rooms/startRoom.json");
        break;
    }
    */
    
    //slog("here");
    level = level_load(rooms[rand_num]);
    level_set_active_level(level);
   
    level_add_entity(level, player_get());
    player_set_position(vector2d(600, 555));
    player_room_inc();
    
    //level_free(old_level); //this breaks it... but i want to free the old level? or do i have to
    //entity_clear_from_level(self);
    entity_free(self);
    cave_hole_new(vector2d(600, 200));
}

/*eol@eof*/
