#include "../includes/canvas.h"
#include "../includes/tuple.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static double
canvas_linear_to_srgb(double linear);

struct canvas canvas_new(int width, int height) {
    struct canvas c;
    c.height = height;
    c.width = width;
    c.grid = calloc(c.height * c.width, sizeof(struct tuple));
    if (!c.grid) {
	fprintf(stderr, "Failed to allocate memory for canvas\n");
	exit(1);
    }

    for (int i = 0; i < c.height; i++)
	for (int j = 0; j < c.width; j++)
	    c.grid[INDEX(i, j, c.width)].w = 2;

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

    c->grid[INDEX(i, j, c->width)] = color;
}

void canvas_to_ppm(struct canvas* c) {
    int scale = 256;
    int ri;
    int gi;
    int bi;
    FILE* f = fopen("output.ppm", "w");
    if (!f) {
	fprintf(stderr, "Failed to load output image: output.ppm");
	exit(1);
    }

    double r;
    double g;
    double b;

    fprintf(f, "P3\n%d %d\n255\n", c->width, c->height);
    for (int i = 0; i < c->height; i++) {
	for (int j = 0; j < c->width; j++) {
	    r = c->grid[INDEX(i, j, c->width)].x;
	    g = c->grid[INDEX(i, j, c->width)].y;
	    b = c->grid[INDEX(i, j, c->width)].z;
	    //ri = (int) (scale * r);
	    ri = (int) (scale * canvas_linear_to_srgb(r));
	    if (ri > 255)
		ri = 255;
	    else if (ri < 0)
		ri = 0;
	    //gi = (int) (scale * g);
	    gi = (int) (scale * canvas_linear_to_srgb(g));
	    if (gi > 255)
		gi = 255;
	    else if (gi < 0)
		gi = 0;
	    //bi = (int) (scale * b);
	    bi = (int) (scale * canvas_linear_to_srgb(b));
	    if (bi > 255)
		bi = 255;
	    else if (bi < 0)
		bi = 0;
	    if (j < c->width - 1)
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

static double
canvas_linear_to_srgb(double linear) {
    double a = 0.055;
    if (linear <= 0.0031308)
	return 12.92 * linear;

    return (1 + a) * pow(linear, 1/2.2) - a;
}
