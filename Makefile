CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic -g3 -fsanitize=address
LDFLAGS = -fsanitize=address

SRCS = src/main.c src/game/init.c src/utils.c src/structs.c src/game/map.c src/game/player.c src/game/game.c
OBJS = $(patsubst src/%.c,outputs/%.o,$(SRCS))
DEPS = $(SRCS:.c=.h)
TARGETS = main main_test

LIBS = -lSDL2 -lSDL2_image

all: $(TARGETS)

main: $(OBJS)
	$(CC) -o $@ $(LDFLAGS) $^ $(LIBS)

main_test: outputs/main_test.o 
	$(CC) -o $@ $(LDFLAGS) $^

outputs/%.o: src/%.c
	@mkdir -p $(@D)
	$(CC) -o $@ $(CFLAGS) -c $< 

run: main 
	./main
	
test: main_test
	./main_test

clean: 
	rm -rf $(TARGETS) outputs
