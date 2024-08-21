#include <gmp.h>

#include "class.c"
#include "promotion.c"

typedef struct {
    rpg_class_t class;
    rpg_promotion_t promotion;
    common_vitality_t life;
    common_vitality_t mana;
    common_vitality_t energy_shield;
    float max_soul;
    mpz_t protection;
    mpf_t max_damage_increase;
    mpf_t damage_penetration;
    uint64_t current_rank;
} rpg_player_t;


void init_set_player(rpg_player_t *player, rpg_class_t *class)
{
    // Player life
    mpz_t player_max_life;
    mpz_init_set_ui(player_max_life, 100);
    mpz_init_set_ui(player->life.max, 0);
    mpz_add(player->life.max, player->life.max, class->initial_life.max);
}
