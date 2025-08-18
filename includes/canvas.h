#ifndef INCLUDES_CANVAS_H
#define INCLUDES_CANVAS_H

#define HIGH_RES	2

#if HIGH_RES == 2
#define CANVAS_WIDTH		3840
#define CANVAS_HEIGHT		2160
#elif HIGH_RES == 1
#define CANVAS_WIDTH		1920
#define CANVAS_HEIGHT		1080
#elif HIGH_RES == 0
#define CANVAS_WIDTH		960
#define CANVAS_HEIGHT		540
#endif

#define INDEX(i, j, w)		((i * w) + j)

struct canvas {
    int width;
    int height;
    struct tuple* grid;
};

struct canvas canvas_new(int width, int height);

void canvas_write_pixel(struct canvas* c, int i, int j, struct tuple color);

void canvas_to_ppm(struct canvas* c);

void canvas_free_canvas(struct canvas* c);

#endif
