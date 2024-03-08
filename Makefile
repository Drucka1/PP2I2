CFLAGS = -std=c99 -Wall -Wextra -pedantic -fsanitize=address
LDFLAGS = -fsanitize=address
LIBS = -lSDL2
CC = gcc

SRCS = src/struct.c src/game.c src/aux.c src/init.c 
OBJS = $(patsubst src/%.c,outputs/%.o,$(SRCS))
DEPS = $(SRCS:.c=.h)
TARGETS = game

all: $(TARGETS)

game: $(OBJS)
	$(CC) -o $@ $(LDFLAGS) $^ $(LIBS)

outputs/%.o: src/%.c
	@mkdir -p $(@D)
	$(CC) -o $@ $(CFLAGS) -c $< 

test: src/test.c
	$(CC) -o test src/test.c

run: game 
	./game
	
clean: 
	rm -rf $(TARGETS) outputs
 	




