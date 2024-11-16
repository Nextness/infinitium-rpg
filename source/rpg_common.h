#ifndef RPG_COMMON_H_
#define RPG_COMMON_H_

#include <gmp.h>

typedef struct {
    mpz_t base;
    mpz_t bonus_base;
    mpz_t max;
    mpz_t bonus_max;
    mpz_t current;
} rpg_common_vitality_t;

void rpg_common_init_bar_calc(
    rpg_common_vitality_t *result,
    const char *base,
    const char *bonus_base,
    const char *max,
    const char *bonus_max,
    const char *cur
);

typedef struct {
    mpz_t base;
    mpz_t bonus_base;
    mpz_t max;
    mpz_t bonus_max;
    float regen_interval;
} rpg_common_vitality_regen_t;

rpg_common_vitality_regen_t rpg_common_vitality_regen_init_set(
    char *base,
    char *bonus_base,
    char *max,
    char *bonus_max,
    float regen_interval
);

typedef enum {
    MELEE    =             1,
    MAGIC    = MELEE    << 1,
    DISTANCE = MAGIC    << 1,
    SUPPORT  = DISTANCE << 1,
    DEFENCE  = SUPPORT  << 1,
} rpg_common_skill_types_e;

typedef enum {
    NO_SKILL_FIGHTING = 0,
    SWORD_FIGHTING,
    AXE_FIGHTING,
    DAGGER_FIGHTING,
    MACE_FIGHTING,
    SHORT_STAFF_FIGHTING,
    STICK_FIGHTING,
    BARE_HAND_FIGHTING,
    SHORT_BOW_FIGHTING,
    SHIELD_PROTECTION,
    ELEMENTAL_PROTECETION,
    WILD_MAGIC_PROTECTION,
    CHAOTIC_MAGIC_PROTECTION,
    WAND_FIGHTING,
    ROD_FIGHTING,
    LONG_STAFF_FIGHTING,
    CANE_FIGHTING,
    SCEPTER_FIGHTING,
    LONG_BOW_FIGHTING,
    LANCE_FIGHTING,
    CROSSBOW_FIGHTING,
    BOOK_EMPOWERMENT,
    TOME_EMPOWERMENT,
    JOURNAL_EMPOWERMENT,
    RELIC_EMPOWERMENT,
    COUNT_SKILL_IDS,
} rpg_common_skill_id_e;

typedef struct {
    char *name;
    mpz_t bonus_level;
    mpz_t current_level;
    mpz_t current_experience;
    mpz_t experience_requirement;
    rpg_common_skill_types_e type;
    rpg_common_skill_id_e id;
} rpg_common_skill_attr_t;

rpg_common_skill_attr_t rpg_common_skill_attr_init_set(
    char *name,
    char *bonus_level,
    char *current_level,
    char *current_experience,
    char *experience_requirement,
    rpg_common_skill_types_e type,
    rpg_common_skill_id_e id
);

typedef struct {
    // General melee skills
    rpg_common_skill_attr_t sword_fighting;
    rpg_common_skill_attr_t axe_fighting;
    rpg_common_skill_attr_t dagger_fighting;
    rpg_common_skill_attr_t mace_fighting;
    rpg_common_skill_attr_t short_staff_fighting;
    rpg_common_skill_attr_t stick_fighting;
    rpg_common_skill_attr_t bare_hand_fighting;
    rpg_common_skill_attr_t short_bow_fighting;

    // General defence skills
    rpg_common_skill_attr_t shield_protection;
    rpg_common_skill_attr_t elemental_protecetion;
    rpg_common_skill_attr_t wild_magic_protection;
    rpg_common_skill_attr_t chaotic_magic_protection;

    // General distance skills
    rpg_common_skill_attr_t wand_fighting;
    rpg_common_skill_attr_t rod_fighting;
    rpg_common_skill_attr_t long_staff_fighting;
    rpg_common_skill_attr_t cane_fighting;
    rpg_common_skill_attr_t scepter_fighting;
    rpg_common_skill_attr_t long_bow_fighting;
    rpg_common_skill_attr_t lance_fighting;
    rpg_common_skill_attr_t crossbow_fighting;

    // General support skills
    rpg_common_skill_attr_t book_empowerment;
    rpg_common_skill_attr_t tome_empowerment;
    rpg_common_skill_attr_t journal_empowerment;
    rpg_common_skill_attr_t relic_empowerment;
} rpg_common_skills_t;

rpg_common_skills_t rpg_common_starter_skills(void);

typedef struct {
    rpg_common_vitality_t life;
    rpg_common_vitality_t mana;
    rpg_common_vitality_t energy_shield;
    rpg_common_vitality_t soul;
    rpg_common_skills_t skills;
} rpg_common_entity_t;

#endif // RPG_COMMON_H_
