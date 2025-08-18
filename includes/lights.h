#ifndef INCLUDES_LIGHTS_H
#define INCLUDES_LIGHTS_H

#include "materials.h"
#include "tuple.h"

struct point_light {
    struct tuple position;
    struct tuple intensity;
};

struct point_light
lights_new_point_light(struct tuple p, struct tuple i);

struct tuple
lights_lighting_sphere(struct material material, void* object, struct point_light light,
	struct tuple point, struct tuple eyev, struct tuple normalv, int in_shadow);

#endif
