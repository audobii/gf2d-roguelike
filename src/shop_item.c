#include "simple_logger.h"
#include "shop_item.h"

#include "player.h"

#include "entity_common.h"
#include "gf2d_collision.h"

void shop_item_think(Entity* self);
void shop_item_draw(Entity* self);

//TODO: maybe consolidate this into pickup class?

typedef struct {
    int price;
    int itemType;
}ShopItemData;

//1=triple; 2=heal; 3=self destruct; 4=rage; 5=poison; 6=heart; 7=mana;
Entity* shop_item_new(Vector2D position, int type)
{
    Entity* ent;
    ShopItemData* data;

    ent = entity_new();
    if (!ent)return NULL;

    switch (type) {
    case 1:
        ent->sprite = gf2d_sprite_load_image("images/shop_triple_shot_ability.png");
        gfc_line_cpy(ent->name, "ab_triple");
        break;
    case 2:
        ent->sprite = gf2d_sprite_load_image("images/shop_heal_ability.png");
        gfc_line_cpy(ent->name, "ab_heal");
        break;
    case 3:
        ent->sprite = gf2d_sprite_load_image("images/shop_self_destruct_ability.png");
        gfc_line_cpy(ent->name, "ab_destruct");
        break;
    case 4:
        ent->sprite = gf2d_sprite_load_image("images/shop_rage_mode_ability.png");
        gfc_line_cpy(ent->name, "ab_rage");
        break;
    case 5:
        ent->sprite = gf2d_sprite_load_image("images/shop_poison_dart_ability.png");
        gfc_line_cpy(ent->name, "ab_poison");
        break;
    case 6:
        ent->sprite = gf2d_sprite_load_image("images/shop_heart.png");
        gfc_line_cpy(ent->name, "heart");
        break;
    case 7:
        ent->sprite = gf2d_sprite_load_image("images/shop_mana_drop.png");
        gfc_line_cpy(ent->name, "mana");
        break;
    default:
        return;
        break;
    }

    ent->think = shop_item_think;
    ent->draw = shop_item_draw;

    vector2d_copy(ent->position, position);

    if (type <= 5) {
        ent->drawOffset = vector2d(28, 32);
    }
    else {
        ent->drawOffset = vector2d(16, 16);
    }
    
    ent->speed = 0;

    //body/collision stuff
    ent->shape = gfc_shape_circle(0, 0, 22);
    ent->body.shape = &ent->shape;
    ent->body.team = 0;
    vector2d_copy(ent->body.position, position);

    data = gfc_allocate_array(sizeof(ShopItemData), 1);
    if (data) {
        if (type <= 5) {
            data->price = 20;
        } if (type > 5) {
            data->price = 5;
        }

        data->itemType = type;
        ent->data = data;
    }

    //gfc_line_cpy(ent->name, "shop_item");
    return ent;
}

void shop_item_draw(Entity* self) {
    Sprite* price;
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

    if (shop_item_get_type(self) <= 5) {
        price = gf2d_sprite_load_image("images/temp_price.png");
    }
    else {
        price = gf2d_sprite_load_image("images/temp_price_5.png");
    }
    
    gf2d_sprite_draw_image(price, vector2d(self->position.x - 20, self->position.y + 20));
}

void shop_item_think(Entity* self)
{
    //just keep checking if it makes contact with player
    List* activeEnts;
    Entity* player;
    Collision* collision;
    int p_current_money;
    int price;
    Uint32 p_mana;

    player = player_get();

    if (!player)return;
    collision = gf2d_collision_body_body(&self->body, &player->body);
    p_mana = player_get_mana();

    if (!collision)return;

    p_current_money = player_get_money();
    price = shop_item_get_price(self);
    
    if (p_current_money - price >= 0) {
        if (!gfc_line_cmp(self->name, "heart")) {
            if (player->health < 330) {
                player->health += 20;
            }
            else {
                player->health = 350;
            }
        }
        else if (!gfc_line_cmp(self->name, "mana")) {
            if (p_mana < 330) {
                player_set_mana(player_get_mana() + 20);
            }
            else {
                player_set_mana(350);
            }
        }
        else if (!gfc_line_cmp(self->name, "ab_triple")) {
            player_set_ability(player_get(), 1);
        }
        else if (!gfc_line_cmp(self->name, "ab_heal")) {
            player_set_ability(player_get(), 2);
        }
        else if (!gfc_line_cmp(self->name, "ab_destruct")) {
            player_set_ability(player_get(), 3);
        }
        else if (!gfc_line_cmp(self->name, "ab_rage")) {
            player_set_ability(player_get(), 4);
        }
        else if (!gfc_line_cmp(self->name, "ab_poison")) {
            player_set_ability(player_get(), 5);
        }

        player_set_money(p_current_money - price);
        //entity_clear_from_level(self);
        entity_free(self);
    }
    else {
        return;
    }
    
}

int shop_item_get_price(Entity* item) {
    ShopItemData* sdata;

    sdata = item->data;

    if (!sdata)return;
    return sdata->price;
}

int shop_item_get_type(Entity* item) {
    ShopItemData* sdata;

    sdata = item->data;

    if (!sdata)return;
    return sdata->itemType;
}

/*eol@eof*/