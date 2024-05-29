CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic -g3 -fsanitize=address
LDFLAGS = -fsanitize=address

SRCS = src/main.c src/sdl.c src/utils.c src/structs.c src/game/map.c src/game/player.c src/game/game.c src/game/interact.c src/game/objects.c src/text.c src/game/homepage.c
OBJS = $(patsubst src/%.c,outputs/%.o,$(SRCS))
DEPS = $(SRCS:.c=.h)
TARGETS = main main_test

LIBS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm

all: $(TARGETS)

main: $(OBJS)
	$(CC) -o $@ $(LDFLAGS) $^ $(LIBS)

main_test: outputs/main_test.o outputs/unity.o Unity/unity.h
	$(CC) -o $@ Unity/unity.o $(LDFLAGS) $^

outputs/main.o: src/main.c
	@mkdir -p $(@D)
	$(CC) -o $@ $(CFLAGS) -c $<

outputs/main_test.o: src/main_test.c Unity/unity.h
	@mkdir -p $(@D)
	$(CC) -o $@ $(CFLAGS) -c $<


outputs/%.o: src/%.c src/%.h
	@mkdir -p $(@D)
	$(CC) -o $@ $(CFLAGS) -c $<

outputs/unity.o: Unity/unity.c Unity/unity.h
	 $(CC) -c $< -o $@ 

run: main
	./main

test: main_test
	./main_test

clean:
	rm -rf $(TARGETS) outputs
