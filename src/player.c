#include <SDL_ttf.h>
#include "simple_logger.h"

#include "gfc_input.h"

#include "gf2d_draw.h"
#include "gf2d_mouse.h"
#include "gf2d_graphics.h"

#include "level.h"
#include "player.h"
#include "projectile.h"

#include "gf2d_collision.h"
#include "entity_common.h"

void player_think(Entity* self);
void player_draw(Entity* self);
void player_free(Entity* self);

static Entity* ThePlayer = NULL;
static float internal_timer = 0;
static float ability_timer = 0;

typedef struct {
	Uint32 mana;
    int currentAbility; //-1 if none; 1/2/3/4/5 for each ability
    Bool abilityActive;
    int money;
    int room_score; //how many rooms player went through
    int score; //for enemy killings
}PlayerData;

Entity* player_get() {
    return ThePlayer;
}

Uint32 player_get_mana() {
    PlayerData* pdata;
    if (ThePlayer == NULL)return;
    pdata = ThePlayer->data;

    return pdata->mana;
}

void player_set_mana(Uint32 newMana) {
    PlayerData* pdata;
    if (ThePlayer == NULL)return;
    pdata = ThePlayer->data;

    pdata->mana = newMana;
}

int player_get_money() {
    PlayerData* pdata;
    if (ThePlayer == NULL)return;
    pdata = ThePlayer->data;

    return pdata->money;
}

void player_set_money(int newMoney) {
    PlayerData* pdata;
    if (ThePlayer == NULL)return;
    pdata = ThePlayer->data;

    pdata->money = newMoney;
}

Vector2D player_get_position() {
    if (ThePlayer == NULL)return;
    return ThePlayer->position;
}

void player_set_position(Vector2D newPos) {
    if (ThePlayer == NULL)return;
    vector2d_copy(ThePlayer->position, newPos);
    vector2d_copy(ThePlayer->body.position, newPos);
}

void player_room_inc() {
    PlayerData* pdata;
    if (ThePlayer == NULL)return;
    pdata = ThePlayer->data;

    pdata->room_score += 1;
}

void player_score_inc(int points) {
    PlayerData* pdata;
    if (ThePlayer == NULL)return;
    pdata = ThePlayer->data;

    pdata->score += points;
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

    ent->health = 350;

	vector2d_copy(ent->position, position);
	ent->speed = 3.5;

	data = gfc_allocate_array(sizeof(PlayerData), 1);
	if (data) {
		data->mana = 350;
        data->currentAbility = -1;
        data->abilityActive = false;
        data->money = 20;
        data->room_score = 0;
        data->score = 0;
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
    
    if (data->abilityActive) {
        if (data->currentAbility == 1) { //ABILITY 1: triple shot
            slog("TRIPLE");
            projectile_new(self, self->body.position, dir, 5, 10.0, 5, 100);
            dir1.x = dir.x + 150;
            dir1.y = dir.y;
            dir2.x = dir.x - 150;
            dir2.y = dir.y;
            projectile_new(self, self->body.position, dir1, 5, 10.0, 5, 100);
            projectile_new(self, self->body.position, dir2, 5, 10.0, 5, 100);
        }

        else if (data->currentAbility == 3) { //ABILITY 3: self destruct
            slog("SELF DESTRUCT");
            Vector2D temp, dir3, dir4;
            vector2d_negate(temp, dir);
            //a temporary mess
            projectile_new(self, self->body.position, dir, 5, 20.0, 20, 20);
            projectile_new(self, self->body.position, temp, 5, 20.0, 20, 20);
            
            dir1.x = dir.x + 350;
            dir1.y = dir.y;
            dir2.x = dir.x - 350;
            dir2.y = dir.y;

            dir3.x = temp.x + 350;
            dir3.y = temp.y;
            dir4.x = temp.x - 350;
            dir4.y = temp.y;

            projectile_new(self, self->body.position, dir1, 5, 20.0, 20, 20);
            projectile_new(self, self->body.position, dir2, 5, 20.0, 20, 20);
            projectile_new(self, self->body.position, dir3, 5, 20.0, 20, 20);
            projectile_new(self, self->body.position, dir4, 5, 20.0, 20, 20);
            ThePlayer->health -= 50;
        }

        else if (data->currentAbility == 5) { //ABILITY 5: poison darts
            projectile_new(self, self->body.position, dir, 5, 10.0, 2, 100);
        }

    }
    else { //normal atk
        projectile_new(self, self->body.position, dir, 5, 10.0, 5, 100);
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

    //TEMPORARY BC I CANT FIGURE OUT HOW TO PRINT TEXT TO SCREEN...
    if (gfc_input_key_down("m")) {
        char str[20];
        sprintf(str, "%i", player_get_money());
        slog(str);
    }

    if (gfc_input_key_down("p")) {
        char str[20];
        sprintf(str, "%f", player_get_position().y);
        slog(str);
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
    //but also might be breaking something? when doing something (usually a free?) while under timer > 0, seg fault...
    //MAYBE FIX with global timer for every entity??
    internal_timer += 0.1;
    
    if (data->abilityActive) {
        ability_timer += 0.1;
        
        if (data->currentAbility == 2) { //ABILITY 2: heal over time
            if ((int)ability_timer % 5 == 0 && ThePlayer->health < 347.5) {
                ThePlayer->health += 2.5;
            }
        }

        else if (data->currentAbility == 4) { //ABILITY 4: rage mode
            //copied over from projectile update
            List* activeEnts;
            Entity* other;
            Collision* collision;

            activeEnts = level_get_active_level()->activeEntities;

            if (!activeEnts)return;

            for (int i = 0; i < gfc_list_get_count(activeEnts); i++) {
                other = gfc_list_get_nth(activeEnts, i);

                if (other == self)continue;
                //if (!gfc_line_cmp(other->body.name, "proj"))continue;
                if (other->body.team == self->body.team)continue;
                if (other->body.team == 0)continue;

                collision = gf2d_collision_body_body(&self->body, &other->body);

                if (!collision)continue;
                //slog("owie");

                //do damage
                if (other->takeDamage)other->takeDamage(other, 30, self);
                //slog(other->name);
            }
        }
    }
    
    /*
    if (gfc_input_command_down("attack") && internal_timer > 5.0 && player_get_mana() >= 10) {
        player_attack(self);
        internal_timer = 0;
        //player_set_mana(player_get_mana() - 10);
    }

    if (gfc_input_command_down("activateAbility") && player_get_mana() >= 25 && data->currentAbility > 0)
    {
        player_set_mana(player_get_mana() - 25);
        player_activate_ability();
    }
    */

    if (gfc_input_command_down("activateAbility") && player_get_mana() >= 25 && player_get_ability() > 0)
    {
        player_activate_ability();
    }

    if (internal_timer > 5.0) {
        if (gfc_input_command_down("attack")) {
            player_attack(self);
        }

        internal_timer = 0;
    }

    if (ability_timer > 16.0) { //cooldown sorta
        player_deactivate_ability();
        ability_timer = 0;
    }

    //maybe causing seg fault
    
    if (self->health <= 0 && ThePlayer != NULL) {
        slog("in player.c");
        player_game_over(self);
        entity_clear_from_level(self);
        entity_free(self);
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
    //gf2d_draw_pixel(self->position, gfc_color8(255, 255, 255, 160));
    //gf2d_draw_circle(self->position, 10, gfc_color8(255, 255, 255, 160));
}

void player_draw_hud(Entity* self) {
    TTF_Init();
    if (!self) {
        Sprite* gameOverSprite = gf2d_sprite_load_image("images/game_over.png");
        gf2d_sprite_draw_image(gameOverSprite, vector2d(500, 250));
    }
    else {
        Sprite* bar = gf2d_sprite_load_image("images/bar.png");
        Sprite* ability_blank = gf2d_sprite_load_image("images/ability_blank.png");
        Sprite* curr_ability = NULL;
        //??? how to write text
        TTF_Font* font = TTF_OpenFont("fonts/arial.ttf", 25);
        //slog(TTF_GetError());
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

        gf2d_sprite_draw_image(ability_blank, vector2d(-20, 550));

        if (player_get_ability() != NULL || player_get_ability() != -1) {
            switch (player_get_ability()) {
            case 1:
                curr_ability = gf2d_sprite_load_image("images/triple_shot_ability.png");
                break;
            case 2:
                curr_ability = gf2d_sprite_load_image("images/heal_ability.png");
                break;
            case 3:
                curr_ability = gf2d_sprite_load_image("images/self_destruct_ability.png");
                break;
            case 4:
                curr_ability = gf2d_sprite_load_image("images/rage_mode_ability.png");
                break;
            case 5:
                curr_ability = gf2d_sprite_load_image("images/poison_dart_ability.png");
                break;
            }
            
            if(curr_ability)gf2d_sprite_draw_image(curr_ability, vector2d(40, 600));
        }

        SDL_Rect dstrect = { 0, 0, 0, 0 };
        SDL_RenderCopy(gf2d_graphics_get_renderer(), texture, NULL, &dstrect);
        SDL_RenderPresent(gf2d_graphics_get_renderer());

        TTF_CloseFont(font);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }
}

void player_set_ability(Entity* self, int ability) {
    PlayerData* pdata;
    if (ThePlayer == NULL)return;
    pdata = ThePlayer->data;

    pdata->currentAbility = ability;
}

int player_get_ability() {
    PlayerData* pdata;
    if (ThePlayer == NULL)return;
    pdata = ThePlayer->data;

    return pdata->currentAbility;
}

void player_activate_ability() {
    PlayerData* pdata;
    if (ThePlayer == NULL)return;
    pdata = ThePlayer->data;

    if (pdata->abilityActive)return;

    player_set_mana(player_get_mana() - 25);
    pdata->abilityActive = true;
}

void player_deactivate_ability() {
    PlayerData* pdata;
    if (ThePlayer == NULL)return;
    pdata = ThePlayer->data;

    pdata->abilityActive = false;
}

Bool player_ability_is_active() {
    PlayerData* pdata;
    if (ThePlayer == NULL)return;
    pdata = ThePlayer->data;

    return pdata->abilityActive;
}

void player_game_over(Entity* self) {
    SJson* json, *new_json, *new_scores, *obj, *old_high_score, *old_highest_rooms;
    PlayerData* pdata;
    int old_rooms=0, old_score=0;

    if (!self)return;

    slog("game over...");

    //load scores into high score json
    //write to new json if scores>previous; if not, dont do anything
    //and then save as high_score.json?
    json = sj_load("config/high_score.json");
    if (!json)slog("could not read high score list");

    obj = sj_object_get_value(json, "current_high_scores");

    if (!obj)
    {
        slog("file missing current high scores object");
        sj_free(json);
    }

    old_high_score = sj_object_get_value(obj, "high_score");
    old_highest_rooms = sj_object_get_value(obj, "highest_rooms");

    sj_get_integer_value(old_high_score, &old_score);
    sj_get_integer_value(old_highest_rooms, &old_rooms);

    new_scores = sj_object_new();
    new_json = sj_object_new();

    pdata = ThePlayer->data;
    /*
    //TESTING PURPOSES
    sj_object_insert(new_scores, "player_name", sj_new_str("test_player"));
    sj_object_insert(new_scores, "highest_rooms", sj_new_int(4));
    sj_object_insert(new_scores, "high_score", sj_new_int(20));
    */

    char str[40];
    sprintf(str, "room score: %i // score: %i", pdata->room_score, pdata->score);
    slog(str);

    char str2[40];
    sprintf(str2, "old room score: %i // old score: %i", old_rooms, old_score);
    slog(str2);

    if (pdata->room_score > old_rooms && pdata->score > old_score) {
        sj_object_insert(new_scores, "player_name", sj_new_str(ThePlayer->name));
        sj_object_insert(new_scores, "highest_rooms", sj_new_int(pdata->room_score));
        sj_object_insert(new_scores, "high_score", sj_new_int(pdata->score));

        sj_object_insert(new_json, "current_high_scores", new_scores);
        sj_save(new_json, "config/high_score.json");
    }
    
    sj_free(json);
    //COMMENTING THIS OUT FIXED SEG FAULTS? idk
    //sj_free(new_json);
    //sj_free(new_scores);
    slog("about 2 free player");
    player_free(self);
}

void player_free(Entity* self)
{
    slog("freeing player");
	//if (!self)return;
	ThePlayer = NULL;
}