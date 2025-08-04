CFLAGS = -g -Wall -Wextra -Wconversion -Wsign-conversion -Wpedantic -pedantic-errors
EXECDIR = Debug
INCLUDES_PATH = includes
SRC_PATH = src

$(shell mkdir -p Debug)

#main: $(SRC_PATH)/*.c $(INCLUDES_PATH)/*.h
#	clang $(CFLAGS) -o Debug/ctracer -I $(INCLUDES_PATH) -lm $(SRC_PATH)/*.c main.c
#
#$(SRC_PATH)/*.c:
#
#$(INCLUDES_PATH)/*.h:

test1: tests/chap1/*.c $(INCLUDES_PATH)/*.h
	clang $(CFLAGS) -o Debug/test_chap1 -I $(INCLUDES_PATH) -lm $(SRC_PATH)/*.c tests/chap1/test_chap1.c

test2: tests/chap2/*.c $(INCLUDES_PATH)/*.h
	clang $(CFLAGS) -o Debug/test_chap2 -I $(INCLUDES_PATH) -lm $(SRC_PATH)/*.c tests/chap2/test_chap2.c

test3: tests/chap3/*.c $(INCLUDES_PATH)/*.h
	clang $(CFLAGS) -o Debug/test_chap3 -I $(INCLUDES_PATH) -lm $(SRC_PATH)/*.c tests/chap3/test_chap3.c

test4: tests/chap4/*.c $(INCLUDES_PATH)/*.h
	clang $(CFLAGS) -o Debug/test_chap4 -I $(INCLUDES_PATH) -lm $(SRC_PATH)/*.c tests/chap4/test_chap4.c

test5: tests/chap5/*.c $(INCLUDES_PATH)/*.h
	clang $(CFLAGS) -o Debug/test_chap5 -I $(INCLUDES_PATH) -lm $(SRC_PATH)/*.c tests/chap5/test_chap5.c

test6: tests/chap6/*.c $(INCLUDES_PATH)/*.h
	clang $(CFLAGS) -o Debug/test_chap6 -I $(INCLUDES_PATH) -lm $(SRC_PATH)/*.c tests/chap6/test_chap6.c

test7: tests/chap7/*.c $(INCLUDES_PATH)/*.h
	clang $(CFLAGS) -o Debug/test_chap7 -I $(INCLUDES_PATH) -lm $(SRC_PATH)/*.c tests/chap7/test_chap7.c

.PHONY: clean
clean:
	rm -rf $(EXECDIR)
