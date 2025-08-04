#include "../includes/lights.h"
#include "sphere.h"
#include "tuple.h"
#include <math.h>

struct point_light
lights_new_point_light(struct tuple p, struct tuple i) {
    return (struct point_light) {p, i};
}

struct tuple
lights_lighting(struct material m, struct tuple p, struct point_light l,
	struct tuple e, struct tuple n, int in_shadow) {
    struct tuple effec_color = tuple_color_mult(m.color, l.intensity);
    struct tuple light_v = tuple_normalize(tuple_sub(l.position, p));
    struct tuple ambient = tuple_scalar_mult(effec_color, m.ambient);
    struct tuple diffuse;
    struct tuple specular;

    double light_dot_normal = tuple_dot(light_v, n);
    if (light_dot_normal < 0 || in_shadow) {
	diffuse = tuple_new_color(0, 0, 0);
	specular = tuple_new_color(0, 0, 0);
    }
    else {
	diffuse = tuple_scalar_mult(effec_color, m.diffuse * light_dot_normal);
	struct tuple reflect_v = sphere_reflect(tuple_scalar_mult(light_v, -1), n);
	double reflect_dot_eye = tuple_dot(reflect_v, e);
	if (reflect_dot_eye <= 0) {
	    specular = tuple_new_color(0, 0, 0);
	}
	else {
	    double factor = pow(reflect_dot_eye, m.shininess);
	    specular = tuple_scalar_mult(l.intensity, m.specular * factor);
	}
    }

    return tuple_add(ambient, tuple_add(diffuse, specular));
}
