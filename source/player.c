#include <gmp.h>

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
