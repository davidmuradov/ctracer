#include "../includes/lights.h"
#include "pattern.h"
#include "sphere.h"
#include "tuple.h"
#include <math.h>

struct point_light
lights_new_point_light(struct tuple p, struct tuple i) {
    return (struct point_light) {POINT_LIGHT, p, i};
}

struct area_light_rect
lights_new_area_light_rect(struct tuple corner, struct tuple full_uvec, int usteps,
	struct tuple full_vvec, int vsteps, struct tuple intensity) {
    struct area_light_rect l;

    l.type = AREA_LIGHT_RECT;
    l.corner = corner;
    l.usteps = usteps;
    l.uvec = tuple_scalar_div(full_uvec, usteps);
    l.vsteps = vsteps;
    l.vvec = tuple_scalar_div(full_vvec, vsteps);
    l.samples = usteps  * vsteps;
    l.intensity = intensity;
    l.position = tuple_add(tuple_scalar_div(full_uvec, 2), tuple_scalar_div(full_vvec, 2));

    return l;
}

struct tuple
lights_lighting_sphere(struct material material, void* object, struct point_light light,
	struct tuple point, struct tuple eyev, struct tuple normalv, double intensity) {
    struct tuple material_color;

    switch (material.pattern.type) {
	case STRIPE:
	    // Stripe pattern color
	    material_color = pattern_stripe_at_object(material.pattern, object, point);
	    break;
	case GRADIENT:
	    // Gradient pattern color
	    material_color = pattern_gradient_at_object(material.pattern, object, point);
	    break;
	case RING:
	    // Ring pattern color
	    material_color = pattern_ring_at_object(material.pattern, object, point);
	    break;
	case CHECKER:
	    // Checker pattern color
	    material_color = pattern_checker_at_object(material.pattern, object, point);
	    break;
	case TEST:
	    // Checker pattern color
	    material_color = pattern_test_at_object(material.pattern, object, point);
	    break;
	default:
	    material_color = material.color;
    }

    struct tuple effec_color = tuple_color_mult(material_color, light.intensity);
    struct tuple light_v = tuple_normalize(tuple_sub(light.position, point));
    struct tuple ambient = tuple_scalar_mult(effec_color, material.ambient);
    struct tuple diffuse;
    struct tuple specular;

    double light_dot_normal = tuple_dot(light_v, normalv);

    diffuse = tuple_scalar_mult(effec_color, material.diffuse * light_dot_normal);
    struct tuple reflect_v = sphere_reflect(tuple_scalar_mult(light_v, -1), normalv); // Bad function name
    double reflect_dot_eye = tuple_dot(reflect_v, eyev);
    if (reflect_dot_eye <= 0) {
	specular = tuple_new_color(0, 0, 0);
    }
    else {
	double factor = pow(reflect_dot_eye, material.shininess);
	specular = tuple_scalar_mult(light.intensity, material.specular * factor);
    }
    diffuse = tuple_scalar_mult(diffuse, intensity);
    specular = tuple_scalar_mult(specular, intensity);
    /*
    if (light_dot_normal < 0 || in_shadow) {
	diffuse = tuple_new_color(0, 0, 0);
	specular = tuple_new_color(0, 0, 0);
    }
    else {
	diffuse = tuple_scalar_mult(effec_color, material.diffuse * light_dot_normal);
	struct tuple reflect_v = sphere_reflect(tuple_scalar_mult(light_v, -1), normalv); // Bad function name
	double reflect_dot_eye = tuple_dot(reflect_v, eyev);
	if (reflect_dot_eye <= 0) {
	    specular = tuple_new_color(0, 0, 0);
	}
	else {
	    double factor = pow(reflect_dot_eye, material.shininess);
	    specular = tuple_scalar_mult(light.intensity, material.specular * factor);
	}
    }
    */

    return tuple_add(ambient, tuple_add(diffuse, specular));
}

struct tuple
lights_point_on_area_light_rect(struct area_light_rect* light, const int u, const int v) {
    struct tuple point = tuple_add(light->corner, tuple_add(tuple_scalar_mult(light->uvec, u + 0.5),
		tuple_scalar_mult(light->vvec, v + 0.5)));
    return point;
}
