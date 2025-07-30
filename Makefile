CFLAGS = -g -Wall -Wextra -Wconversion -Wsign-conversion -Wpedantic -pedantic-errors
EXECDIR = Debug
INCLUDES_PATH = includes
SRC_PATH = src

$(shell mkdir -p Debug)

main: $(SRC_PATH)/*.c $(INCLUDES_PATH)/*.h
	clang $(CFLAGS) -o Debug/ctracer -I $(INCLUDES_PATH) -lm $(SRC_PATH)/*.c main.c

$(SRC_PATH)/*.c:

$(INCLUDES_PATH)/*.h:

.PHONY: clean
clean:
	rm -rf $(EXECDIR)
