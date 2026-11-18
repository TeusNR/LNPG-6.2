CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -g
OUT = build
PROGRAMAS = l1 l2 l3 l4 l5

.PHONY: all clean

all: $(addprefix $(OUT)/,$(PROGRAMAS))

$(OUT):
	mkdir -p $(OUT)

$(OUT)/%: parte_b/%.c | $(OUT)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OUT)
