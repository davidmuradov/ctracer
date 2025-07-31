CFLAGS = -g -Wall -Wextra -Wconversion -Wsign-conversion -Wpedantic -pedantic-errors
EXECDIR = Debug
INCLUDES_PATH = includes
SRC_PATH = src

$(shell mkdir -p Debug)

main: $(SRC_PATH)/*.c $(INCLUDES_PATH)/*.h
	clang $(CFLAGS) -o Debug/ctracer -I $(INCLUDES_PATH) -lm $(SRC_PATH)/*.c main.c

$(SRC_PATH)/*.c:

$(INCLUDES_PATH)/*.h:

test1: tests/chap1/*.c $(INCLUDES_PATH)/*.h
	clang $(CFLAGS) -o Debug/test_chap1 -I $(INCLUDES_PATH) -lm $(SRC_PATH)/*.c tests/chap1/test_chap1.c

test2: tests/chap2/*.c $(INCLUDES_PATH)/*.h
	clang $(CFLAGS) -o Debug/test_chap2 -I $(INCLUDES_PATH) -lm $(SRC_PATH)/*.c tests/chap2/test_chap2.c

.PHONY: clean
clean:
	rm -rf $(EXECDIR)
