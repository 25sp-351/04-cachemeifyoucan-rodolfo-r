CC = gcc
CFLAGS = -Wall -Wextra -g -std=c11
OBJS = rodcut.o piece_values.o cut_list.o cache_lifo.o vec.o

all: rodcut

rodcut: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

rodcut.o: rodcut.c piece_values.h cut_list.h cache.h vec.h
	$(CC) $(CFLAGS) -c $

piece_values.o: piece_values.c piece_values.h vec.h
	$(CC) $(CFLAGS) -c $

cut_list.o: cut_list.c cut_list.h cache.h vec.h
	$(CC) $(CFLAGS) -c $

cache_lifo.o: cache_lifo.c cache.h cut_list.h
	$(CC) $(CFLAGS) -c $

vec.o: vec.c vec.h
	$(CC) $(CFLAGS) -c $

clean:
	rm -f rodcut *.o

test: rodcut
	@echo "Testing with sample values..."
	@echo "25\n50\n75\n100\n25\n50\n75\n100\nquit" | ./rodcut sample_values.txt

.PHONY: all clean test