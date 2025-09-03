#ifndef INCLUDES_LIGHTS_H
#define INCLUDES_LIGHTS_H

#include "materials.h"
#include "tuple.h"

typedef enum {POINT_LIGHT, AREA_LIGHT_RECT} t_light;

struct point_light {
    t_light type;
    struct tuple position;
    struct tuple intensity;
};

struct area_light_rect {
    t_light type;
    struct tuple position;
    struct tuple intensity;
    struct tuple corner;
    struct tuple uvec;
    int usteps;
    struct tuple vvec;
    int vsteps;
    int samples;
};

struct point_light
lights_new_point_light(struct tuple p, struct tuple i);

struct area_light_rect
lights_new_area_light_rect(struct tuple corner, struct tuple full_uvec, int usteps,
	struct tuple full_vvec, int vsteps, struct tuple intensity);

struct tuple
lights_lighting_sphere(struct material material, void* object, struct point_light light,
	struct tuple point, struct tuple eyev, struct tuple normalv, double intensity);

struct tuple
lights_point_on_area_light_rect(struct area_light_rect* light, const int u, const int v);

#endif
