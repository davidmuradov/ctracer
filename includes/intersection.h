#ifndef INCLUDES_INTERSECTION_H
#define INCLUDES_INTERSECTION_H

#include "tuple.h"
#include "ray.h"

typedef enum {UNKNOWN_OBJECT, SPHERE, PLANE, CUBE, CYLINDER, CONE, GROUP, TRIANGLE, CSG} t_object;

#define INIT_MAX_INTERSECTIONS	16

struct intersection {
    double t;
    void* object;
};

struct intersection_list {
    int max_nb_intersections;
    int nb_intersections;
    struct intersection* list;
};

struct precompute {
    double t;
    void* object;
    struct tuple point;
    struct tuple eyev;
    struct tuple normalv;
    int inside;
    int in_shadow;
    struct tuple over_point;
    struct tuple under_point;
    struct tuple reflectv;
    double n1;
    double n2;
};

struct intersection
intersection_new_intersection(double t, void* object);

t_object
intersection_get_object_type(struct intersection* inter);

struct intersection_list
intersection_new_intersection_list(void);

void
intersection_add_intersection_to_list(struct intersection_list* inter_list, struct intersection inter);

void
intersection_clear_intersection_list(struct intersection_list* inter_list);

struct intersection
intersection_hit(struct intersection_list* inter_list);

struct precompute
intersection_prepare_computations(struct intersection* inter, struct ray* ray, struct intersection_list* inter_list);

double
intersection_schlick(struct precompute* comps);

#endif
