#ifndef INCLUDES_CANVAS_H
#define INCLUDES_CANVAS_H

#define CANVAS_WIDTH		320
#define CANVAS_HEIGHT		180
#define INDEX(i, j)		((i * CANVAS_WIDTH) + j)

struct canvas {
    int width;
    int height;
    struct tuple* grid;
};

struct canvas canvas_new(void);

void canvas_write_pixel(struct canvas* c, int i, int j, struct tuple color);

void canvas_to_ppm(struct canvas* c);

#endif
