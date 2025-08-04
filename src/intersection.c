#include "../includes/intersection.h"
#include "ray.h"
#include "sphere.h"
#include "tuple.h"
#include <stdio.h>
#include <stdlib.h>

struct intersection
intersection_new_intersection(double t, void* object) {
    struct intersection inter;
    inter.t = t;
    inter.object = object;
    return inter;
}

t_object
intersection_get_object_type(struct intersection* inter) {
    struct sphere* type1 = (struct sphere*) inter->object;
    if (type1->type == SPHERE)
	return SPHERE;

    /* every other type will be checked later when we get there */

    return UNKNOWN_OBJECT;
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
intersection_prepare_computations(struct intersection* inter, struct ray* ray) {
    struct precompute comps;

    comps.t = inter->t;
    comps.object = inter->object;
    comps.point = ray_position_at(ray, comps.t);
    comps.eyev = tuple_scalar_mult(ray->dir, -1);
    comps.normalv = sphere_normal_at((struct sphere*) comps.object, comps.point);

    if (tuple_dot(comps.normalv, comps.eyev) < 0) {
	comps.inside = 1;
	comps.normalv = tuple_scalar_mult(comps.normalv, -1);
    }
    else
	comps.inside = 0;

    return comps;
}
