#include <SDL_ttf.h>
#include "simple_logger.h"

#include "gfc_input.h"

#include "gf2d_draw.h"
#include "gf2d_mouse.h"
#include "gf2d_graphics.h"

#include "level.h"
#include "player.h"
#include "projectile.h"

#include "entity_common.h"

void player_think(Entity* self);
void player_draw(Entity* self);
void player_free(Entity* self);

static Entity* ThePlayer = NULL;
static float internal_timer = 0;
static float ability_timer = 0;

typedef struct {
	Uint32 mana;
    Uint8 currentAbility; //-1 if none; 1/2/3/4/5 for each ability
    Bool abilityActive;
    Uint32 money;
}PlayerData;

Entity* player_get() {
    return ThePlayer;
}

Uint32 player_get_mana() {
    PlayerData* pdata;

    pdata = ThePlayer->data;

    return pdata->mana;
}

void player_set_mana(Uint32 newMana) {
    PlayerData* pdata;

    pdata = ThePlayer->data;

    pdata->mana = newMana;
}

Vector2D player_get_position() {
    if (ThePlayer == NULL)return;
    return ThePlayer->position;
}

Entity* player_new(Vector2D position) {
	PlayerData* data;
	Entity* ent;

	ent = entity_new();
	if (!ent)return NULL;

	gfc_line_cpy(ent->name, "player");

	ent->sprite = gf2d_sprite_load_image("images/player_temp.png");
    ent->drawOffset = vector2d(28, 32);

	ent->think = player_think;
	ent->draw = player_draw;
	ent->free_entity = player_free;
    ent->takeDamage = entity_damage;

    ent->health = 150;

	vector2d_copy(ent->position, position);
	ent->speed = 2.5;

	data = gfc_allocate_array(sizeof(PlayerData), 1);
	if (data) {
		data->mana = 350;
        data->currentAbility = 3;
        data->abilityActive = false;
        data->money = 0;
		ent->data = data;
	}

    ent->shape = gfc_shape_circle(0,0, 22);
    ent->body.shape = &ent->shape;
    ent->body.team = 1;
    vector2d_copy(ent->body.position, position);

	ThePlayer = ent;
	return ent;
}

void player_attack(Entity* self) {
    //slog("atk");
    Vector2D m, dir, dir1, dir2;
    int mx, my;

    PlayerData* data;
    if ((!self) || (!self->data))return;
    data = self->data;

    SDL_GetMouseState(&mx, &my);
    m.x = mx;
    m.y = my;
    vector2d_sub(dir, m, self->position);

    //dir = vector2d_from_angle(self->rotation);
    
    projectile_new(self, self->body.position, dir, 5, 10.0);
    
    if (data->abilityActive) {
        if (data->currentAbility == 1) { //triple shot
            slog("TRIPLE");
            dir1.x = dir.x + 150;
            dir1.y = dir.y;
            dir2.x = dir.x - 150;
            dir2.y = dir.y;
            projectile_new(self, self->body.position, dir1, 5, 10.0);
            projectile_new(self, self->body.position, dir2, 5, 10.0);
        }

        else if (data->currentAbility == 3) { //self destruct
            slog("SELF DESTRUCT");
            //TODO: spawn projectiles in ring around player

            ThePlayer->health -= 50;
        }
    }
}

void player_think(Entity* self) {
    int mx, my;
    float axis = 0;
    Vector2D dir;
    Vector2D m = { 0 };
    Vector2D walk = { 0 };
    PlayerData* data;

    if (!self)return;

    data = ThePlayer->data;

    SDL_GetMouseState(&mx, &my);
    m.x = mx;
    m.y = my;
    vector2d_sub(dir, m, self->position);

    if (gfc_input_command_down("walkup"))
    {
        walk.y = -1;
    }
    if (gfc_input_command_down("walkdown"))
    {
        walk.y += 1;
    }
    if (gfc_input_command_down("walkleft"))
    {
        walk.x = -1;
    }
    if (gfc_input_command_down("walkright"))
    {
        walk.x += 1;
    }

    if (gfc_input_command_down("activateAbility"))
    {
        player_activate_ability();
    }

    if ((walk.x) || (walk.y))
    {
        vector2d_normalize(&walk);
        vector2d_scale(walk, walk, self->speed);
        vector2d_copy(self->velocity, walk);
        //might be a hacky way to do bodies
        //vector2d_copy(self->body.position,self->position);
        vector2d_copy(self->body.velocity, walk);
    }
    else
    {
        vector2d_clear(self->velocity);
        vector2d_clear(self->body.velocity);
    }

    //internal timer is a temporary fix to input_key_pressed not working...
    internal_timer += 0.1;
    
    if (data->abilityActive) {
        ability_timer += 0.1;
        
        if (data->currentAbility == 2) { //2 = heal over time
            if ((int)ability_timer % 5 == 0) {
                ThePlayer->health += 2.5;
            }
        }

        else if (data->currentAbility == 3) { //3 - bomb; do big damage around yourself but take some health/chip dmg

        }
    }
    
    if (gfc_input_command_down("attack") && internal_timer > 5.0 && player_get_mana() >= 10) {
        player_attack(self);
        internal_timer = 0;
        player_set_mana(player_get_mana() - 10);
    }

    if (ability_timer > 16.0) { //cooldown sorta
        player_deactivate_ability();
        ability_timer = 0;
    }
}

void player_draw(Entity* self) {
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

    //draw body for collisions at this pos? idk
    gf2d_draw_pixel(self->position, gfc_color8(255, 255, 255, 160));
    gf2d_draw_circle(self->position, 10, gfc_color8(255, 255, 255, 160));
}

void player_draw_hud(Entity* self) {
    if (!self)return;
    Sprite* bar = gf2d_sprite_load_image("images/bar.png");
    //??? how to write text
    TTF_Font* font = TTF_OpenFont("fonts/arial.ttf", 25);
    SDL_Color white = { 255,255,255 };

    SDL_Surface* surface = TTF_RenderText_Solid(font, "this is a TEST", white);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), surface);

    //health
    if (self->health > 0) {
        gf2d_draw_rect_filled(gfc_rect(10, 10, self->health, 25), GFC_COLOR_RED);
    }
    gf2d_sprite_draw_image(bar, vector2d(10, 10));

    //mana
    if (player_get_mana() > 0) {
        gf2d_draw_rect_filled(gfc_rect(25, 35, player_get_mana(), 25), GFC_COLOR_BLUE);
    }
    gf2d_sprite_draw_image(bar, vector2d(25, 35));

    TTF_CloseFont(font);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void player_set_ability(Entity* self, Uint8 ability) {
    PlayerData* pdata;
    pdata = ThePlayer->data;

    pdata->currentAbility = ability;
}

void player_activate_ability() {
    PlayerData* pdata;
    pdata = ThePlayer->data;

    pdata->abilityActive = true;
}

void player_deactivate_ability() {
    PlayerData* pdata;
    pdata = ThePlayer->data;

    pdata->abilityActive = false;
}

void player_free(Entity* self)
{
	if (!self)return;
	ThePlayer = NULL;
}