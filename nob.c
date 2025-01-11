#define NOB_IMPLEMENTATION
#include "nob.h"

#include <string.h>
#include <stdbool.h>
#include <stdio.h>

int main(int argc, char **argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);

    const char *program_name = nob_shift_args(&argc, &argv);
    bool run_bin = false;
    bool run_new = false;

    while (argc > 0) {
        const char *sub_command = nob_shift_args(&argc, &argv);
        if (strcmp(sub_command, "run") == 0) {
            run_bin = true;
        } else if (strcmp(sub_command, "new") == 0) {
            run_new = true;
        } else {
            printf("ERROR: Invalid arugment '%s'\n", sub_command);
            return -1;
        }
    }

    const char *bin_location;
    const char *bin_path = "./bin";
    const char *game_name = "inifinitium_rpg";
    if (!nob_mkdir_if_not_exists(bin_path)) return 1;
    nob_temp_reset();

    Nob_Cmd cmd = {0};
    if (run_new) {
        game_name = "infinitium_rpg_new";
        bin_location = nob_temp_sprintf("%s/%s", bin_path, game_name);
        nob_cmd_append(&cmd, "gcc", "-DDEVELOPER_BUILD", "-O3");
        nob_cmd_append(&cmd, "./new_src/main.c");
        nob_cmd_append(&cmd, "-o", bin_location);
        nob_cmd_append(&cmd, "-Wall", "-Wextra", "-Wpedantic", "-lraylib", "-lGL", "-lm", "-pthread", "-ldl", "-lgmp");
        if (!nob_cmd_run_sync_and_reset(&cmd)) return 1;
            
        nob_cmd_append(&cmd, bin_location);
        if (!nob_cmd_run_sync_and_reset(&cmd)) return 1;
    } else {
        bin_location = nob_temp_sprintf("%s/%s", bin_path, game_name);
        nob_cmd_append(&cmd, "gcc", "-DDEVELOPER_BUILD", "-O3");
        nob_cmd_append(&cmd, "./source/main.c", "./source/rpg_common.c", "./source/utils_common.c",
                             "./source/player.c", "./source/ui_components.c", "./source/callbacks.c",
                             "./source/ui_state.c");
        nob_cmd_append(&cmd, "-o", bin_location);
        nob_cmd_append(&cmd, "-Wall", "-Wextra", "-Wpedantic", "-lraylib", "-lGL", "-lm", "-pthread",
                             "-ldl", "-lgmp");
        if (!nob_cmd_run_sync_and_reset(&cmd)) return 1;

        if (run_bin) {
            nob_cmd_append(&cmd, bin_location);
            if (!nob_cmd_run_sync_and_reset(&cmd)) return 1;
        }
    }

    return 0;
}

