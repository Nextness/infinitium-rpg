all: compile run

compile:
	clang -O3 ./source/main.c ./source/rpg_common.c ./source/utils_common.c -o ./bin/infinitium_rpg -Wall -Wextra -lraylib -lGL -lm -pthread -ldl -lgmp

run:
	./bin/infinitium_rpg

clean:
	rm ./bin/infinitium_rpg


