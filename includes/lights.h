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
lights_lighting(struct material m, struct tuple p, struct point_light l,
	struct tuple e, struct tuple n, int in_shadow);

#endif
