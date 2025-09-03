CFLAGS = -g -Wall -Wextra -Wpedantic -pedantic-errors -std=c99
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

all: test1 test2 test3 test4 test5 test6 test7 test8 test9 test10 test11 test12

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

test8: tests/chap8/*.c $(INCLUDES_PATH)/*.h
	clang $(CFLAGS) -o Debug/test_chap8 -I $(INCLUDES_PATH) -lm $(SRC_PATH)/*.c tests/chap8/test_chap8.c

test8_perf: tests/chap8/*.c $(INCLUDES_PATH)/*.h
	clang $(CFLAGS) -O3 -o Debug/test_chap8 -I $(INCLUDES_PATH) -lm $(SRC_PATH)/*.c tests/chap8/test_chap8.c

test9: tests/chap9/*.c $(INCLUDES_PATH)/*.h
	clang $(CFLAGS) -o Debug/test_chap9 -I $(INCLUDES_PATH) -lm $(SRC_PATH)/*.c tests/chap9/test_chap9.c

test9_perf: tests/chap9/*.c $(INCLUDES_PATH)/*.h
	clang $(CFLAGS) -O3 -o Debug/test_chap9 -I $(INCLUDES_PATH) -lm $(SRC_PATH)/*.c tests/chap9/test_chap9.c

test10: tests/chap10/*.c $(INCLUDES_PATH)/*.h
	clang $(CFLAGS) -o Debug/test_chap10 -I $(INCLUDES_PATH) -lm $(SRC_PATH)/*.c tests/chap10/test_chap10.c

test10_perf: tests/chap10/*.c $(INCLUDES_PATH)/*.h
	clang $(CFLAGS) -O3 -o Debug/test_chap10 -I $(INCLUDES_PATH) -lm $(SRC_PATH)/*.c tests/chap10/test_chap10.c

test11: tests/chap11/*.c $(INCLUDES_PATH)/*.h
	clang $(CFLAGS) -o Debug/test_chap11 -I $(INCLUDES_PATH) -lm $(SRC_PATH)/*.c tests/chap11/test_chap11.c

test11_perf: tests/chap11/*.c $(INCLUDES_PATH)/*.h
	clang $(CFLAGS) -O3 -o Debug/test_chap11 -I $(INCLUDES_PATH) -lm $(SRC_PATH)/*.c tests/chap11/test_chap11.c

test12: tests/chap12/*.c $(INCLUDES_PATH)/*.h
	clang $(CFLAGS) -o Debug/test_chap12 -I $(INCLUDES_PATH) -lm $(SRC_PATH)/*.c tests/chap12/test_chap12.c

test12_perf: tests/chap12/*.c $(INCLUDES_PATH)/*.h
	clang $(CFLAGS) -O3 -o Debug/test_chap12 -I $(INCLUDES_PATH) -lm $(SRC_PATH)/*.c tests/chap12/test_chap12.c

test12_prof: tests/chap12/*.c $(INCLUDES_PATH)/*.h
	clang $(CFLAGS) -O3 -pg -o Debug/test_chap12 -I $(INCLUDES_PATH) -lm $(SRC_PATH)/*.c tests/chap12/test_chap12.c

test13: tests/chap13/*.c $(INCLUDES_PATH)/*.h
	clang $(CFLAGS) -o Debug/test_chap13 -I $(INCLUDES_PATH) -lm $(SRC_PATH)/*.c tests/chap13/test_chap13.c

test13_perf: tests/chap13/*.c $(INCLUDES_PATH)/*.h
	clang $(CFLAGS) -O3 -o Debug/test_chap13 -I $(INCLUDES_PATH) -lm $(SRC_PATH)/*.c tests/chap13/test_chap13.c

test14: tests/chap14/*.c $(INCLUDES_PATH)/*.h
	clang $(CFLAGS) -o Debug/test_chap14 -I $(INCLUDES_PATH) -lm $(SRC_PATH)/*.c tests/chap14/test_chap14.c

test14_perf: tests/chap14/*.c $(INCLUDES_PATH)/*.h
	clang $(CFLAGS) -O3 -o Debug/test_chap14 -I $(INCLUDES_PATH) -lm $(SRC_PATH)/*.c tests/chap14/test_chap14.c

test_bbox: tests/bbox/*.c $(INCLUDES_PATH)/*.h
	clang $(CFLAGS) -o Debug/test_bbox -I $(INCLUDES_PATH) -lm $(SRC_PATH)/*.c tests/bbox/test_bbox.c

test_bbox_perf: tests/bbox/*.c $(INCLUDES_PATH)/*.h
	clang $(CFLAGS) -O3 -o Debug/test_bbox -I $(INCLUDES_PATH) -lm $(SRC_PATH)/*.c tests/bbox/test_bbox.c

test_soft_shadows: tests/soft_shadows/*.c $(INCLUDES_PATH)/*.h
	clang $(CFLAGS) -o Debug/test_soft_shadows -I $(INCLUDES_PATH) -lm $(SRC_PATH)/*.c tests/soft_shadows/test_soft_shadows.c

test_soft_shadows_perf: tests/soft_shadows/*.c $(INCLUDES_PATH)/*.h
	clang $(CFLAGS) -O3 -o Debug/test_soft_shadows -I $(INCLUDES_PATH) -lm $(SRC_PATH)/*.c tests/soft_shadows/test_soft_shadows.c

.PHONY: clean
clean:
	rm -rf $(EXECDIR)
