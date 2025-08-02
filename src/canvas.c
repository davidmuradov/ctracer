#include "../includes/canvas.h"
#include "../includes/tuple.h"
#include <stdio.h>
#include <stdlib.h>

struct canvas canvas_new(void) {
    struct canvas c;
    c.height = CANVAS_HEIGHT;
    c.width = CANVAS_WIDTH;
    c.grid = calloc(c.height * c.width, sizeof(struct tuple));
    if (!c.grid) {
	fprintf(stderr, "Failed to allocate memory for canvas\n");
	exit(1);
    }

    for (int i = 0; i < CANVAS_HEIGHT; i++)
	for (int j = 0; j < CANVAS_WIDTH; j++)
	    c.grid[INDEX(i, j)].w = 2;

    return c;
}

void canvas_write_pixel(struct canvas* c, int i, int j, struct tuple color) {
    if (i < 0 || i > CANVAS_HEIGHT - 1) {
	fprintf(stderr, "Error: Reading out of bound on canvas height (ILLEGAL)");
	exit(1);
    }
    else if (j < 0 || j > CANVAS_WIDTH - 1) {
	fprintf(stderr, "Error: Reading out of bound on canvas width (ILLEGAL)");
	exit(1);
    }

    c->grid[INDEX(i, j)] = color;
}

void canvas_to_ppm(struct canvas* c) {
    double scale = 255.99;
    int ri;
    int gi;
    int bi;
    FILE* f = fopen("output.ppm", "w");
    if (!f) {
	fprintf(stderr, "Failed to load output image: output.ppm");
	exit(1);
    }

    fprintf(f, "P3\n%d %d\n255\n", CANVAS_WIDTH, CANVAS_HEIGHT);
    for (int i = 0; i < CANVAS_HEIGHT; i++) {
	for (int j = 0; j < CANVAS_WIDTH; j++) {
	    ri = (int) (scale * c->grid[INDEX(i, j)].x);
	    gi = (int) (scale * c->grid[INDEX(i, j)].y);
	    bi = (int) (scale * c->grid[INDEX(i, j)].z);
	    if (j < CANVAS_WIDTH - 1)
		fprintf(f, "%d %d %d ", ri, gi, bi);
	    else
		fprintf(f, "%d %d %d\n", ri, gi, bi);
	}
    }

    fclose(f);
}

void canvas_free_canvas(struct canvas* c) {
    free(c->grid);
}
