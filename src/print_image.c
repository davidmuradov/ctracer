#include "../includes/print_image.h"
#include "../includes/vec3.h"
#include "pixels.h"
#include <stdio.h>
#include <stdlib.h>

struct pixel* init_image(int h, int w) {

    struct pixel* im = malloc(h * w * sizeof(struct pixel));

    for (int i = 0; i < h; i++) {
	for (int j = 0; j < w; j++) {
	    im[INDEX(i, j)].r = 1;
	    im[INDEX(i, j)].g = 0;
	    im[INDEX(i, j)].b = 0;
	}
    }

    return im;
}

struct vec3* init_image_vec3(int h, int w) {

    float scale = 255.999f;

    struct vec3* im_vec = malloc(h * w * sizeof(struct pixel));

    for (int i = 0; i < H; i++) {
	for (int j = 0; j < W; j++) {
	    im_vec[INDEX(i, j)].x = (scale * (float)j) / (H - 1);
	    im_vec[INDEX(i, j)].y = (scale * (float)i) / (H - 1);
	    im_vec[INDEX(i, j)].z = 0;
	}
    }

    return im_vec;
}

void print_image(struct pixel* im) {

    float scale = 255.9f;

    printf("P3\n%d %d\n255\n", W, H);

    for (int i = 0; i < H; i++) {
	for (int j = 0; j < W; j++) {
	    printf("%d %d %d\n", (int) (im[INDEX(i, j)].r * scale), 
		    (int) (im[INDEX(i, j)].g * scale), 
		    (int) (im[INDEX(i, j)].b * scale));
	}
    }
}

void print_image_vec3(struct vec3* im_vec) {

    printf("P3\n%d %d\n255\n", W, H);

    for (int i = 0; i < H; i++) {
	for (int j = 0; j < W; j++) {
	    printf("%d %d %d\n", (int) im_vec[INDEX(i, j)].x, (int) im_vec[INDEX(i, j)].y, (int) im_vec[INDEX(i, j)].z);
	}
    }
}

void write_color(struct pixel* px) {

    int ri = (255.9 * px->r);
    int gi = (255.9 * px->g);
    int bi = (255.9 * px->b);

    printf("%d %d %d\n", ri, gi, bi);
}
