CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic -fsanitize=address
LDFLAGS = -fsanitize=address

SRCS = src/struct.c src/game.c src/utils.c src/aux.c src/init.c src/main.c 
OBJS = $(patsubst src/%.c,outputs/%.o,$(SRCS))
DEPS = $(SRCS:.c=.h)
TARGETS = main

LIBS = -lSDL2

all: $(TARGETS)

main: $(OBJS)
	$(CC) -o $@ $(LDFLAGS) $^ $(LIBS)

outputs/%.o: src/%.c
	@mkdir -p $(@D)
	$(CC) -o $@ $(CFLAGS) -c $< 

map: src/create_map.c
	$(CC) -o map src/create_map.c

run: main
	./main ./assets/test_10_20.txt
	
clean: 
	rm -rf $(TARGETS) outputs
 	




