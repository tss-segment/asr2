targets	= exercice1/exercice1
cflags	= -Wall -Wextra -std=c11 -O2

all: $(targets)

%: %.c
	gcc $< -o $@ $(cflags)

clean:
	rm -f $(targets)

.PHONY: all clean
