#include "../includes/intersection.h"
#include "../includes/object_utils.h"
#include "ct_math.h"
#include "object_utils.h"
#include "ray.h"
#include "sphere.h"
#include "tuple.h"
#include "world.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

struct intersection
intersection_new_intersection(double t, void* object) {
    struct intersection inter;
    inter.t = t;
    inter.object = object;
    return inter;
}

struct intersection_list
intersection_new_intersection_list(void) {
    struct intersection_list inter_list;
    inter_list.max_nb_intersections = INIT_MAX_INTERSECTIONS;
    inter_list.nb_intersections = 0;
    inter_list.list = calloc(inter_list.max_nb_intersections, sizeof(struct intersection));
    if (!inter_list.list) {
	fprintf(stderr, "Failed to allocate memory for intersection list\n");
	exit(1);
    }
    return  inter_list;
}

void
intersection_add_intersection_to_list(struct intersection_list* inter_list, struct intersection inter) {
    if (!inter_list->list) {
	inter_list->list = calloc(inter_list->max_nb_intersections, sizeof(struct intersection));
	if (!inter_list->list) {
	    fprintf(stderr, "Failed to allocate memory for intersection list\n");
	    exit(1);
	}
    }
    if (inter_list->nb_intersections < inter_list->max_nb_intersections) {
	inter_list->list[inter_list->nb_intersections] = inter;
	inter_list->nb_intersections++;
    }
    else {
	inter_list->list = realloc(inter_list->list, inter_list->max_nb_intersections * 2 * sizeof(struct intersection));
	if (!inter_list->list) {
	    fprintf(stderr, "Failed to allocate new memory for intersection list\n");
	    exit(1);
	}
	inter_list->max_nb_intersections *= 2;
	inter_list->list[inter_list->nb_intersections] = inter;
	inter_list->nb_intersections++;
    }
}

void
intersection_clear_intersection_list(struct intersection_list* inter_list) {
    inter_list->nb_intersections = 0;
    free(inter_list->list);
    inter_list->list = NULL;
}

struct intersection
intersection_hit(struct intersection_list* inter_list) {
    double s_time = UNDEF_TIME;
    struct intersection to_return = (struct intersection) {UNDEF_TIME, NULL};

    if (!inter_list->nb_intersections)
	return to_return;

    for (int i = 0; i < inter_list->nb_intersections; i++) {
	if (inter_list->list[i].t > 0) {
	    s_time = inter_list->list[i].t;
	    to_return = inter_list->list[i];
	    break;
	}
    }

    for (int i = 0; i < inter_list->nb_intersections; i++) {
	if (inter_list->list[i].t < s_time && inter_list->list[i].t > 0) {
	    s_time = inter_list->list[i].t;
	    to_return = inter_list->list[i];
	}
    }

    return to_return;
}

struct precompute
intersection_prepare_computations(struct intersection* inter, struct ray* ray, struct intersection_list* inter_list) {
    struct precompute comps;

    comps.t = inter->t;
    comps.object = inter->object;
    comps.point = ray_position_at(ray, comps.t);
    comps.eyev = tuple_scalar_mult(ray->dir, -1);

    // Get object type
    t_object obj_type = world_get_object_type(inter->object);

    switch (obj_type) {
	case SPHERE:
	    comps.normalv = sphere_normal_at((struct sphere*) comps.object, comps.point);
	    break;
	case PLANE:
	    comps.normalv = plane_normal_at((struct plane*) comps.object, comps.point);
	    break;
	case CUBE:
	    comps.normalv = cube_normal_at((struct cube*) comps.object, comps.point);
	    break;
	case CYLINDER:
	    comps.normalv = cylinder_normal_at((struct cylinder*) comps.object, comps.point);
	    break;
	case CONE:
	    comps.normalv = cone_normal_at((struct cone*) comps.object, comps.point);
	    break;
	default:
	    break;
    }

    comps.in_shadow = 0;

    if (tuple_dot(comps.normalv, comps.eyev) < 0) {
	comps.inside = 1;
	comps.normalv = tuple_scalar_mult(comps.normalv, -1);
    }
    else
	comps.inside = 0;

    comps.over_point = tuple_add(comps.point, tuple_scalar_mult(comps.normalv, EPSILON));
    comps.under_point = tuple_sub(comps.point, tuple_scalar_mult(comps.normalv, EPSILON));
    comps.reflectv = sphere_reflect(ray->dir, comps.normalv); // Very bad function name

    if (inter_list) {
	void* containers[inter_list->nb_intersections];

	for (int i = 0; i < inter_list->nb_intersections; i++)
	    containers[i] = NULL;

	int nb_elements_containers = 0;
	int last_index = -1;

	for (int i = 0; i < inter_list->nb_intersections; i++) {
	    if (inter_list->list[i].object == inter->object && ctm_floats_equal(inter_list->list[i].t, inter->t)) {
		if (nb_elements_containers == 0)
		    comps.n1 = 1;
		else {
		    struct material mat = object_utils_get_material(containers[last_index]);
		    comps.n1 = mat.refractive_index;
		}
	    }

	   if (object_utils_is_object_in_containers(inter_list->list[i].object, containers, inter_list->nb_intersections)) {
	       object_utils_remove_object_from_containers(inter_list->list[i].object, containers, inter_list->nb_intersections);
	       nb_elements_containers--;
	       last_index--;
	   }
	   else {
	       object_utils_add_object_to_containers(inter_list->list[i].object, containers, inter_list->nb_intersections);
	       nb_elements_containers++;
	       last_index++;
	   }

	    if (inter_list->list[i].object == inter->object && ctm_floats_equal(inter_list->list[i].t, inter->t)) {
		if (nb_elements_containers == 0)
		    comps.n2 = 1;
		else {
		    struct material mat = object_utils_get_material(containers[last_index]);
		    comps.n2 = mat.refractive_index;
		}
		break;
	    }
	}
    }

    return comps;
}

double
intersection_schlick(struct precompute* comps) {
    double cos = tuple_dot(comps->eyev, comps->normalv);

    if (comps->n1 > comps->n2) {
	double n = comps->n1 / comps->n2;
	double sin2_t = n * n * (1 - cos * cos);
	if (sin2_t > 1)
	    return 1;

	double cos_t = sqrt(1 - sin2_t);
	cos = cos_t;
    }

    double r0 = ((comps->n1 - comps->n2) / (comps->n1 + comps->n2)) *
	((comps->n1 - comps->n2) / (comps->n1 + comps->n2));

    double fac5 = (1 - cos) * (1 - cos) * (1 - cos) * (1 - cos) * (1 - cos);

    return r0 + (1 - r0) * fac5;
}
