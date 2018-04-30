#! /usr/bin/make -f

targets	= exercice2/exercice2 \
	  exercice3/bruiteur exercice3/code_ascii exercice3/programme_2 \
	  exercice3/programme_3 exercice3/programme_5 exercice3/programme_6
cflags	= -Wall -Wextra -std=c11 -O2 -lpng

all: $(targets)

%: %.c
	gcc $< -o $@ $(cflags)

clean:
	rm -f $(targets)

.PHONY: all clean
