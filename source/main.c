#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <gmp.h>
#include <raylib.h>

#include "ui_config.c"

#define FRAME_RATE 60

typedef struct Bar_Calc {
    mpz_t base;
    mpz_t bonus_base;
    mpz_t max;
    mpz_t bonus_max;
    mpz_t current;
} Bar_Calc;

void init_bar_calc(Bar_Calc *result, const char *base, const char *bbase, const char *max, const char *bmax, const char *cur)
{
    mpz_init_set_str(result->base, base, 10);
    mpz_init_set_str(result->bonus_base, bbase, 10);
    mpz_init_set_str(result->max, max, 10);
    mpz_init_set_str(result->bonus_max, bmax, 10);
    mpz_init_set_str(result->current, cur, 10);
}

typedef struct Class {
    char *name;
    Bar_Calc initial_life;
    Bar_Calc initial_mana;
    Bar_Calc initial_energy_shield;
    //  TODO: Make new struct specifically for regeneration...
    Bar_Calc initial_life_regeneration;
    Bar_Calc initial_mana_regeneration;
    Bar_Calc initial_energy_shield_regeneration;
    float initial_soul;
} Class;

typedef struct Promotion {
    char *name;
    uint64_t life_gain_per_level;
    uint64_t mana_gain_per_level;
    uint64_t energy_shield_gain_per_level;
    float damage_increase_per_level;
    float soul_gain;
} Promotion;

typedef struct Player {
    Class class;
    Promotion promotion;
    Bar_Calc life;
    Bar_Calc mana;
    Bar_Calc energy_shield;
    float max_soul;
    mpz_t protection;
    mpf_t max_damage_increase;
    mpf_t damage_penetration;
    uint64_t current_rank;
} Player;

typedef struct {
    Class class;
    Promotion promotion;
    Bar_Calc life;
    Bar_Calc mana;
    Bar_Calc energy_shield;
    float max_soul;
    mpz_t protection;
    mpf_t max_damage_increase;
    mpf_t damage_penetration;
    uint64_t current_rank;
} rpg_player_t;

void init_set_player(Player *player, Class *class) {
    // Player life
    mpz_t player_max_life;
    mpz_init_set_ui(player_max_life, 100);
    mpz_init_set_ui(player->life.max, 0);
    mpz_add(player->life.max, player->life.max, class->initial_life.max);
}

typedef struct {
    rpg_ui_config_t ui_config;
    rpg_player_t player;

} rpg_game_state_t;

rpg_game_state_t * rpg_game_setup() {
    rpg_game_state_t *gs = (rpg_game_state_t *) malloc(sizeof(rpg_game_state_t));
    if (gs == NULL) {
        printf("Buy more RAM for game state allocation...\n");
        exit(0);
    }

    // Set up user interface configuration
    rpg_init_set_ui_config(&gs->ui_config, 1280, 720);

    return gs;
}

// void rpg_game_init(rpg_game_state_t *gs) {
    // InitWindow(gs->ui_config.screen_width, gs->ui_config.screen_height, "First Window");
    // SetTargetFPS(FRAME_RATE);
// }

int main(void) {
    Bar_Calc result;
    init_bar_calc(&result, "0", "0", "100", "0", "0");
    
    rpg_game_state_t *gs = rpg_game_setup();

    InitWindow(1280, 720, "First Window");
    SetTargetFPS(FRAME_RATE);

    mpz_t some_value;
    mpz_init_set_ui(some_value, 10);

    mpz_t another_value;
    mpz_init_set_ui(another_value, 1);

    mpz_t buy_shit;
    mpz_init_set_ui(buy_shit, 20);

    char *val = mpz_get_str(NULL, 10, some_value);
    float dt = 0.0f;
    char buffer[4096];

    sprintf(buffer, "Value is %s\n", val);
    while (!WindowShouldClose()) {
        dt += GetFrameTime();
        if (dt >= 1.0f) {
            gmp_printf("Current max life: %Zd\n", result.max);
            mpz_add(some_value, some_value, another_value);
            val = mpz_get_str(NULL, 10, some_value);
            sprintf(buffer, "Value is %s", val);
            dt = 0.0f;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText(buffer, 190, 200, 20, DARKGRAY);
        DrawRectangleV((Vector2){100, 100}, (Vector2){200, 50}, (mpz_cmp(some_value, buy_shit) > 0)? GREEN : RED);
        EndDrawing();
    }

    CloseWindow();
    free(gs);
    return 0;
}

