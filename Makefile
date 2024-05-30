MKDIR = mkdir -p
CC = gcc

CFLAGS = -std=c99
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -g3
CFLAGS += -pedantic
CFLAGS += -fsanitize=address

SRCS = $(wildcard src/*.c) $(wildcard src/game/*.c)
OBJS = $(patsubst src/%.c,outputs/%.o,$(SRCS))
DEPS = $(SRCS:.c=.h)

TEST_SRCS = $(wildcard test/*.c)

TEST_OBJS = $(patsubst test/%.c,outputs/test/%.o,$(TEST_SRCS))
TEST_TARGETS = $(patsubst test/%.c,%,$(TEST_SRCS))

LIBS = -lSDL2 -lSDL2_image -lSDL2_ttf -lm

UNITY_ROOT = Unity
INC_DIRS = -Isrc -I$(UNITY_ROOT)/
SYMBOLS = -DUNITY_FIXTURE_NO_EXTRAS

TARGETS = main test_all

all: $(TARGETS)

main: $(OBJS)
	$(CC) $(CFLAGS) -o outputs/$@ $^ $(LIBS)

outputs/%.o: src/%.c
	@$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

outputs/test/%.o: test/%.c
	@$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

test_all: $(TEST_TARGETS)

test_main: outputs/test/test_main.o $(UNITY_ROOT)/unity.c
	@$(MKDIR) outputs/test/game
	$(CC) $(CFLAGS) $(INC_DIRS) $(SYMBOLS) -o outputs/test/$@ $^ $(LIBS)
	./outputs/test/$@

test_%: outputs/test/test_%.o $(UNITY_ROOT)/unity.c src/%.c
	@$(MKDIR) outputs/test/game
	$(CC) $(CFLAGS) $(INC_DIRS) $(SYMBOLS) -o outputs/test/$@ $^ $(LIBS)
	./outputs/test/$@

run: main
	./outputs/main

run_test: test_all
	./outputs/test/*

clean:
	rm -rf outputs
