CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic -g3 -fsanitize=address
LDFLAGS = -fsanitize=address

SRCS = src/main.c src/sdl.c src/utils.c src/structs.c src/game/map.c src/game/player.c src/game/game.c src/game/interact.c src/game/objects.c src/text.c src/game/homepage.c src/game/init.c
OBJS = $(patsubst src/%.c,outputs/%.o,$(SRCS))
DEPS = $(SRCS:.c=.h)
TARGETS = main

LIBS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm

all: $(TARGETS)

main: $(OBJS)
	$(CC) -o $@ $(LDFLAGS) $^ $(LIBS)

main_test: outputs/main_test.o
	$(CC) -o $@ $(LDFLAGS) $^

outputs/main.o: src/main.c
	@mkdir -p $(@D)
	$(CC) -o $@ $(CFLAGS) -c $<

outputs/%.o: src/%.c src/%.h
	@mkdir -p $(@D)
	$(CC) -o $@ $(CFLAGS) -c $<

run: main
	./main

clean:
	rm -rf $(TARGETS) outputs
