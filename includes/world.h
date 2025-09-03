#ifndef INCLUDES_WORLD_H
#define INCLUDES_WORLD_H

#define MAX_RECUR_CALLS		10
#define WORLD_MEM_CLEANUP	1

#include "intersection.h"
#include "lights.h"
#include "sphere.h"
#include "plane.h"
#include "cube.h"
#include "cylinder.h"
#include "cone.h"
#include "group.h"

struct world;

struct world*
world_new_world(void);

struct world*
world_new_default_world(void);

void
world_free_world(struct world* w);

int
world_get_nb_objects(struct world* w);

int
world_get_nb_lights(struct world* w);

int
world_get_max_nb_objects(struct world* w);

int
world_get_max_nb_lights(struct world* w);

void*
world_get_object_at(struct world* world, int i);

t_object
world_get_object_type(void* object);

void*
world_get_light_at(struct world* world, int i);

void
world_add_sphere(struct world* world, struct sphere* sphere);

void
world_add_plane(struct world* world, struct plane* plane);

void
world_add_cube(struct world* world, struct cube* cube);

void
world_add_cylinder(struct world* world, struct cylinder* cylinder);

void
world_add_cone(struct world* world, struct cone* cone);

void
world_add_group(struct world* world, struct group* group);

void
world_add_point_light(struct world* world, struct point_light* light);

void
world_add_area_light_rect(struct world* world, struct area_light_rect* light);

struct intersection_list
world_intersect_world(struct world* world, struct ray* ray);

struct tuple
world_shade_hit(struct world* world, struct precompute* comps, int remaining_calls);

struct tuple
world_color_at(struct world* world, struct ray* ray, int remaining_calls);

int
world_is_shadowed(struct world* world, struct tuple light_pos, struct tuple point);

struct tuple
world_reflected_color(struct world* world, struct precompute* comps, int remaining_calls);

struct tuple
world_refracted_color(struct world* world, struct precompute* comps, int remaining_calls);

double
world_intensity_at(void* light, struct tuple point, struct world* world);

#endif
