all: compile run

compile:
	clang ./source/main.c ./source/rpg_common.c -o ./bin/infinitium_rpg -Wall -Wextra -lraylib -lGL -lm -pthread -ldl -lgmp

run:
	./bin/infinitium_rpg

clean:
	rm ./bin/infinitium_rpg


