#! /usr/bin/make -f

targets	= exercice1/exercice1 \
	  exercice2/bruiteur exercice2/code_ascii exercice2/programme_2 \
	  exercice2/programme_3
cflags	= -Wall -Wextra -std=c11 -O2 -lpng

all: $(targets)

%: %.c
	gcc $< -o $@ $(cflags)

clean:
	rm -f $(targets)

.PHONY: all clean
