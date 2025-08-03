#ifndef INCLUDES_CANVAS_H
#define INCLUDES_CANVAS_H

#define HIGH_RES	1

#if HIGH_RES
#define CANVAS_WIDTH		1920
#define CANVAS_HEIGHT		1080
#else
#define CANVAS_WIDTH		320
#define CANVAS_HEIGHT		180
#endif

#define INDEX(i, j)		((i * CANVAS_WIDTH) + j)

struct canvas {
    int width;
    int height;
    struct tuple* grid;
};

struct canvas canvas_new(void);

void canvas_write_pixel(struct canvas* c, int i, int j, struct tuple color);

void canvas_to_ppm(struct canvas* c);

void canvas_free_canvas(struct canvas* c);

#endif
