#include "../includes/world.h"
#include "ct_math.h"
#include "intersection.h"
#include "lights.h"
#include "sphere.h"
#include "tuple.h"
#include <stdio.h>
#include <stdlib.h>

#define INIT_NB_OBJECTS		1
#define INIT_NB_LIGHTS		1

struct world {
    void** object_list;
    void** light_list;
    int nb_objects;
    int max_nb_objects;
    int nb_lights;
    int max_nb_lights;
};

int
world_compare_intersections(const void* a, const void* b);

struct world*
world_new_world(void) {
    struct world* w = calloc(1, sizeof(struct world));
    if (!w) {
	fprintf(stderr, "Failed to allocate memory for new world\n");
	exit(1);
    }

    w->object_list = calloc(INIT_NB_OBJECTS, sizeof(void*));
    if (!w->object_list) {
	fprintf(stderr, "Failed to allocate memory for world's list of objects\n");
	exit(1);
    }
    w->max_nb_objects = INIT_NB_OBJECTS;
    for (int i = 0; i < INIT_NB_OBJECTS; i++) {
	w->object_list[i] = NULL;
    }

    w->light_list = calloc(INIT_NB_LIGHTS, sizeof(void*));
    if (!w->light_list) {
	fprintf(stderr, "Failed to allocate memory for world's list of lights\n");
	exit(1);
    }
    w->max_nb_lights = INIT_NB_LIGHTS;
    for (int i = 0; i < INIT_NB_LIGHTS; i++) {
	w->light_list[i] = NULL;
    }

    w->nb_objects = w->nb_lights = 0;

    return w;
}

int
world_get_nb_objects(struct world* w) {return w->nb_objects;}

int
world_get_nb_lights(struct world* w) {return w->nb_lights;}

int
world_get_max_nb_objects(struct world* w) {return w->max_nb_objects;}

int
world_get_max_nb_lights(struct world* w) {return w->max_nb_lights;}

void
world_free_world(struct world* w) {
    free(w->object_list);
    w->object_list = NULL;
    free(w->light_list);
    w->light_list = NULL;
    free(w);
    w = NULL;
}

void
world_add_sphere(struct world* world, struct sphere* sphere) {
    if (world->nb_objects < world->max_nb_objects) {
	world->object_list[world->nb_objects] = (void*) sphere;
	world->nb_objects++;
    }
    else {
	world->object_list = realloc(world->object_list, world->max_nb_objects * 2 * sizeof(void*));
	if (!world->object_list) {
	    fprintf(stderr, "Failed to allocate new memory for list of objects in world\n");
	    exit(1);
	}
	world->max_nb_objects *= 2;
	world->object_list[world->nb_objects] = (void*) sphere;
	world->nb_objects++;
    }
}

void
world_add_point_light(struct world* world, struct point_light* light) {
    if (world->nb_lights < world->max_nb_lights) {
	world->light_list[world->nb_lights] = (void*) light;
	world->nb_lights++;
    }
    else {
	world->light_list = realloc(world->light_list, world->max_nb_lights * 2 * sizeof(void*));
	if (!world->light_list) {
	    fprintf(stderr, "Failed to allocate new memory for list of objects in world\n");
	    exit(1);
	}
	world->max_nb_lights *= 2;
	world->light_list[world->nb_lights] = (void*) light;
	world->nb_lights++;
    }

}

void*
world_get_object_at(struct world* world, int i) {
    if (i < 0 || i > world->nb_objects - 1) {
	fprintf(stderr, "Out of bound reading on world->object_list\n");
	exit(1);
    }
    return world->object_list[i];
}

void*
world_get_light_at(struct world* world, int i) {
    if (i < 0 || i > world->nb_lights - 1) {
	fprintf(stderr, "Out of bound reading on world->light_list\n");
	exit(1);
    }
    return world->light_list[i];
}

struct intersection_list
world_intersect_world(struct world* world, struct ray* ray) {
    struct intersection_list full_list = intersection_new_intersection_list();
    struct intersection_list current_list;
    t_object object_type = UNKNOWN_OBJECT;
    void* current_object;

    for (int i = 0; i < world->nb_objects; i++) {
	current_object = world_get_object_at(world, i);
	object_type = world_get_object_type(current_object);
	if (object_type == SPHERE) {
	    current_list = sphere_intersect_ray((struct sphere*) current_object, ray);
	    for (int j = 0; j < current_list.nb_intersections; j++) {
		intersection_add_intersection_to_list(&full_list, current_list.list[j]);
	    }
	}
	intersection_clear_intersection_list(&current_list);
    }

    struct intersection inter_array[full_list.nb_intersections];
    for (int i = 0; i < full_list.nb_intersections; i++) {
	inter_array[i] = full_list.list[i];
    }
    qsort(inter_array, full_list.nb_intersections, sizeof(struct intersection), world_compare_intersections);
    for (int i = 0; i < full_list.nb_intersections; i++) {
	full_list.list[i] = inter_array[i];
    }

    return full_list;
}

int
world_compare_intersections(const void* a, const void* b) {
    struct intersection arg1 = *(struct intersection*) a;
    struct intersection arg2 = *(struct intersection*) b;

    if (arg1.t < arg2.t) return -1;
    if (arg1.t > arg2.t) return 1;

    return 0;
}

t_object
world_get_object_type(void* object) {
    t_object obj_type = UNKNOWN_OBJECT;
    struct sphere* maybe_sphere = (struct sphere*) object;

    if (maybe_sphere->type == SPHERE) {
	return SPHERE;
    }

    return obj_type;
}

struct tuple
world_shade_hit(struct world* world, struct precompute* comps) {
    struct sphere* obj = (struct sphere*) comps->object;
    struct material mat = obj->material;
    struct tuple color = tuple_new_color(0, 0, 0);

    for (int i = 0; i < world->nb_lights; i++)
	color = tuple_add(color, lights_lighting(mat, comps->point,
		    *(struct point_light*) world->light_list[i], comps->eyev,
		    comps->normalv));

    return color;
}

struct tuple
world_color_at(struct world* world, struct ray* ray) {
    struct tuple color = tuple_new_color(0, 0, 0);
    struct intersection_list list = world_intersect_world(world, ray);
    struct intersection hit = intersection_hit(&list);

    if (ctm_floats_equal(hit.t, UNDEF_TIME)) {
	intersection_clear_intersection_list(&list);
	return color;
    }

    struct precompute comps = intersection_prepare_computations(&hit, ray);
    color = world_shade_hit(world, &comps);
    intersection_clear_intersection_list(&list);

    return color;
}
