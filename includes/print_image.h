#ifndef INCLUDES_PRINT_IMAGE_H
#define INCLUDES_PRINT_IMAGE_H

#include "pixels.h"
#include "vec3.h"

#define ASP_RAT	(16. / 9)
#define H	450 // Height of image
#define W	(int) (ASP_RAT * H) // Width of image


struct pixel* init_image(int h, int w);

struct vec3* init_image_vec3(int h, int w);

void print_image(struct pixel* im);

void print_image_vec3(struct vec3* im_vec);

void write_color(struct pixel* px);

#endif
