#include <gmp.h>
#include "rpg_common.h"

void init_bar_calc(
    rpg_common_vitality_t *result,
    const char *base,
    const char *bonus_base,
    const char *max,
    const char *bonus_max,
    const char *cur
) {
    mpz_init_set_str(result->base, base, 10);
    mpz_init_set_str(result->bonus_base, bonus_base, 10);
    mpz_init_set_str(result->max, max, 10);
    mpz_init_set_str(result->bonus_max, bonus_max, 10);
    mpz_init_set_str(result->current, cur, 10);
}

rpg_common_skill_attr_t rpg_common_skill_attr_init_set(
    char *name,
    char *bonus_level,
    char *current_level,
    char *current_experience,
    char *experience_requirement,
    rpg_common_skill_types_e type,
    rpg_common_skill_id_e id
) {
    rpg_common_skill_attr_t result;

    mpz_init_set_str(result.bonus_level, bonus_level, 10);
    mpz_init_set_str(result.current_level, current_level, 10);
    mpz_init_set_str(result.current_experience, current_experience, 10);
    mpz_init_set_str(result.experience_requirement, experience_requirement, 10);

    result.name = name;
    result.type = type;
    result.id = id;

    return result;
}

rpg_common_skills_t rpg_common_starter_skills(void) {
    rpg_common_skills_t result = {
        // Melee
        .sword_fighting = rpg_common_skill_attr_init_set(
            "Sword Fighting", "0", "10", "100", "110", MELEE, SWORD_FIGHTING),
        .axe_fighting = rpg_common_skill_attr_init_set(
            "Axe Fighting", "0", "10", "100", "110", MELEE, AXE_FIGHTING),
        .dagger_fighting = rpg_common_skill_attr_init_set(
            "Dagger Fighting", "0", "10", "100", "110", MELEE, DAGGER_FIGHTING),
        .mace_fighting = rpg_common_skill_attr_init_set(
            "Mace Fighting", "0", "10", "100", "110", MELEE, MACE_FIGHTING),
        .short_staff_fighting = rpg_common_skill_attr_init_set(
            "Short Staff Fighting", "0", "10", "100", "110", MELEE, SHORT_STAFF_FIGHTING),
        .stick_fighting = rpg_common_skill_attr_init_set(
            "Stick Fighting", "0", "10", "100", "110", MELEE, STICK_FIGHTING),
        .bare_hand_fighting = rpg_common_skill_attr_init_set(
            "Bare Hand Fighting", "0", "10", "100", "110", MELEE, BARE_HAND_FIGHTING),
        .short_bow_fighting = rpg_common_skill_attr_init_set(
            "Short Bow Fighting", "0", "10", "100", "110", MELEE, SHORT_BOW_FIGHTING),

        // Defence
        .shield_protection = rpg_common_skill_attr_init_set(
            "Shield Protection", "0", "10", "100", "110", DEFENCE, SHIELD_PROTECTION),
        .elemental_protecetion = rpg_common_skill_attr_init_set(
            "Elemental Protection", "0", "10", "100", "110", DEFENCE, ELEMENTAL_PROTECETION),
        .wild_magic_protection = rpg_common_skill_attr_init_set(
            "Wild Magic Protection", "0", "10", "100", "110", DEFENCE, WILD_MAGIC_PROTECTION),
        .chaotic_magic_protection = rpg_common_skill_attr_init_set(
            "Chaotic Magic Protection", "0", "10", "100", "110", DEFENCE, CHAOTIC_MAGIC_PROTECTION),

        // Distance

        .wand_fighting = rpg_common_skill_attr_init_set(
            "Wand Fighting", "0", "10", "100", "110", DEFENCE, CHAOTIC_MAGIC_PROTECTION),
        .rod_fighting = rpg_common_skill_attr_init_set(
            "Rod Fighting Fighting", "0", "10", "100", "110", DEFENCE, CHAOTIC_MAGIC_PROTECTION),
        .long_staff_fighting = rpg_common_skill_attr_init_set(
            "Long Staff Fighting", "0", "10", "100", "110", DEFENCE, CHAOTIC_MAGIC_PROTECTION),
        .cane_fighting = rpg_common_skill_attr_init_set(
            "Cane Fighting", "0", "10", "100", "110", DEFENCE, CHAOTIC_MAGIC_PROTECTION),
        .scepter_fighting = rpg_common_skill_attr_init_set(
            "Scepter Fighting", "0", "10", "100", "110", DEFENCE, CHAOTIC_MAGIC_PROTECTION),
        .long_bow_fighting = rpg_common_skill_attr_init_set(
            "Long Bow Fighting", "0", "10", "100", "110", DEFENCE, CHAOTIC_MAGIC_PROTECTION),
        .lance_fighting = rpg_common_skill_attr_init_set(
            "Lance Fighting", "0", "10", "100", "110", DEFENCE, CHAOTIC_MAGIC_PROTECTION),
        .crossbow_fighting = rpg_common_skill_attr_init_set(
            "Crossbow Fighting", "0", "10", "100", "110", DEFENCE, CHAOTIC_MAGIC_PROTECTION),

        // Empowerment
        .book_empowerment = rpg_common_skill_attr_init_set(
            "Book Empowerment", "0", "10", "100", "110", DEFENCE, CHAOTIC_MAGIC_PROTECTION),
        .tome_empowerment = rpg_common_skill_attr_init_set(
            "Tome Empowerment", "0", "10", "100", "110", DEFENCE, CHAOTIC_MAGIC_PROTECTION),
        .journal_empowerment = rpg_common_skill_attr_init_set(
            "Journal Empowerment", "0", "10", "100", "110", DEFENCE, CHAOTIC_MAGIC_PROTECTION),
        .relic_empowerment = rpg_common_skill_attr_init_set(
            "Relic Empowerment", "0", "10", "100", "110", DEFENCE, CHAOTIC_MAGIC_PROTECTION),
    };

    return result;
}

rpg_common_vitality_regen_t rpg_common_vitality_regen_init_set(
    char *base,
    char *bonus_base,
    char *max,
    char *bonus_max,
    float regen_interval
) {
    rpg_common_vitality_regen_t regen;

    mpz_init_set_str(regen.base, base, 10);
    mpz_init_set_str(regen.bonus_base, bonus_base, 10);
    mpz_init_set_str(regen.max, max, 10);
    mpz_init_set_str(regen.bonus_max, bonus_max, 10);

    regen.regen_interval = regen_interval;

    return regen;
}

