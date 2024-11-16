#define NOB_IMPLEMENTATION
#include "nob.h"

#include <string.h>
#include <stdbool.h>

int
main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);

    const char *program_name = nob_shift_args(&argc, &argv);
    bool run_bin = false;

    if (argc > 0) {
        const char *sub_command = nob_shift_args(&argc, &argv);
        if (strcmp(sub_command, "run") == 0) {
            run_bin = true;
        }
    }

    const char *bin_path = "./bin";
    const char *game_name = "inifinitium_rpg";
    const char *bin_location = nob_temp_sprintf("%s/%s", bin_path, game_name);
    if (!nob_mkdir_if_not_exists(bin_path)) return 1;
    nob_temp_reset();

    Nob_Cmd cmd = {0};
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

    return 0;
}
