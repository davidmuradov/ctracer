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

struct point_light
lights_new_point_light(struct tuple p, struct tuple i);

struct tuple
lights_lighting_sphere(struct material material, void* object, struct point_light light,
	struct tuple point, struct tuple eyev, struct tuple normalv, double intensity);


#endif
