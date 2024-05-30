MKDIR = mkdir -p
CC = gcc

CFLAGS = -std=c99
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -g3
CFLAGS += -pedantic
CFLAGS += -fsanitize=address

SRCS = \
	src/main.c \
	src/sdl.c \
	src/utils.c \
	src/structs.c \
	src/game/map.c \
	src/game/player.c \
	src/game/game.c \
	src/game/interact.c \
	src/game/objects.c \
	src/text.c \
	src/game/homepage.c
OBJS = $(patsubst src/%.c,outputs/%.o,$(SRCS))
DEPS = $(SRCS:.c=.h)

TEST_SRCS = \
	test/test_main.c \

TEST_OBJS = $(patsubst test/%.c,outputs/test/%.o,$(TEST_SRCS))

LIBS = -lSDL2 -lSDL2_image -lSDL2_ttf -lm

UNITY_ROOT = Unity
INC_DIRS = -Isrc -I$(UNITY_ROOT)/
SYMBOLS = -DUNITY_FIXTURE_NO_EXTRAS

TARGETS = main test_all

all: $(TARGETS)

main: $(OBJS)
	$(CC) $(CFLAGS) -o outputs/$@ $^ $(LIBS)

outputs/%.o: src/%.c
	$(MKDIR) outputs/game
	$(CC) $(CFLAGS) -c -o $@ $<

outputs/test/%.o: test/%.c
	$(MKDIR) outputs/test
	$(CC) $(CFLAGS) -c -o $@ $<

test_all: $(TEST_OBJS) $(UNITY_ROOT)/unity.c
	$(CC) $(CFLAGS) $(INC_DIRS) $(SYMBOLS) -o outputs/$@ $^ $(LIBS)

run: main
	./outputs/main

run_test: test_all
	./outputs/test/*

clean:
	rm -rf outputs
