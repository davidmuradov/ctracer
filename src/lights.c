#include "../includes/lights.h"
#include "ct_math.h"
#include "object_utils.h"
#include "pattern.h"
#include "sphere.h"
#include "tuple.h"
#include <math.h>

static struct tuple
lights_point_light_lighting(struct tuple material_color, struct material material,
	struct point_light* light, struct tuple point, struct tuple eyev,
	struct tuple normalv, double intensity);

static struct tuple
lights_area_light_rect_lighting(struct tuple material_color, struct material material,
	struct area_light_rect* light, struct tuple point, struct tuple eyev,
	struct tuple normalv, double intensity);

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
    l.position.w = 1;

    return l;
}

struct tuple
lights_lighting_sphere(struct material material, void* object, void* light,
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

    struct tuple final_color;

    t_light type = object_utils_get_light_type(light);
    switch (type) {
	case POINT_LIGHT:
	    final_color = lights_point_light_lighting(material_color, material,
		    light, point, eyev, normalv, intensity);
	    break;
	case AREA_LIGHT_RECT:
	    final_color = lights_area_light_rect_lighting(material_color, material,
		    light, point, eyev, normalv, intensity);
	    break;
    }
    /*

    struct tuple ambient = tuple_scalar_mult(effec_color, material.ambient);
    struct tuple diffuse;
    struct tuple specular;

    double light_dot_normal = tuple_dot(light_v, normalv);

    if (light_dot_normal < 0 || ctm_floats_equal(0, intensity)) {
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
	    switch (type) {
		case POINT_LIGHT:
		    specular = tuple_scalar_mult(((struct point_light*) light)->intensity, material.specular * factor);
		case AREA_LIGHT_RECT:
		    specular = tuple_scalar_mult(((struct area_light_rect*) light)->intensity, material.specular * factor);
	    }
	}
	diffuse = tuple_scalar_mult(diffuse, intensity);
	specular = tuple_scalar_mult(specular, intensity);
    }
    ///////////////////
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
    ////////////////
    */

    return final_color;
}

struct tuple
lights_point_on_area_light_rect(struct area_light_rect* light, const int u, const int v) {
    struct tuple point = tuple_add(light->corner, tuple_add(tuple_scalar_mult(light->uvec, u + ctm_random_jitter()),
		tuple_scalar_mult(light->vvec, v + ctm_random_jitter())));
    return point;
}

static struct tuple
lights_point_light_lighting(struct tuple material_color, struct material material,
	struct point_light* light, struct tuple point, struct tuple eyev,
	struct tuple normalv, double intensity) {

    struct tuple effec_color = tuple_color_mult(material_color, light->intensity);
    struct tuple light_v = tuple_normalize(tuple_sub(light->position, point));
    struct tuple ambient = tuple_scalar_mult(effec_color, material.ambient);

    struct tuple diffuse;
    struct tuple specular;

    double light_dot_normal = tuple_dot(light_v, normalv);

    if (light_dot_normal < 0 || ctm_floats_equal(0, intensity)) {
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
	    specular = tuple_scalar_mult(light->intensity, material.specular * factor);
	}
    }
    diffuse = tuple_scalar_mult(diffuse, intensity);
    specular = tuple_scalar_mult(specular, intensity);

    return tuple_add(ambient, tuple_add(diffuse, specular));
}

static struct tuple
lights_area_light_rect_lighting(struct tuple material_color, struct material material,
	struct area_light_rect* light, struct tuple point, struct tuple eyev,
	struct tuple normalv, double intensity) {

    struct tuple light_v;
    struct tuple effec_color = tuple_color_mult(material_color, light->intensity);
    struct tuple ambient = tuple_scalar_mult(effec_color, material.ambient);

    struct tuple diffuse;
    struct tuple specular;
    double light_dot_normal;
    struct tuple sum = tuple_new_color(0, 0, 0);
    struct tuple sample_position;

    // For every sample do ..
    for (int v = 0; v < light->vsteps; v++) {
	for (int u = 0; u < light->usteps; u++) {
	    sample_position = lights_point_on_area_light_rect(light, u, v);
	    light_v = tuple_normalize(tuple_sub(sample_position, point));
	    light_dot_normal = tuple_dot(light_v, normalv);

	    if (light_dot_normal < 0 || ctm_floats_equal(0, intensity)) {
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
		    specular = tuple_scalar_mult(light->intensity, material.specular * factor);
		}
	    }

	    sum = tuple_add(sum, diffuse);
	    sum = tuple_add(sum, specular);
	}
    }

    return tuple_add(ambient, tuple_scalar_mult(tuple_scalar_div(sum, light->samples), intensity));
}
