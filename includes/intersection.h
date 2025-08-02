#ifndef INCLUDES_INTERSECTION_H
#define INCLUDES_INTERSECTION_H

typedef enum {UNKNOWN_OBJECT, SPHERE, PLANE, CUBE, CYLINDER, GROUP, TRIANGLE, CSG} t_object;

#define INIT_MAX_INTERSECTIONS	2

struct intersection {
    double t;
    void* object;
};

struct intersection_list {
    int max_nb_intersections;
    int nb_intersections;
    struct intersection* list;
};

void
intersection_new_intersection(struct intersection* inter, double t, void* object);

t_object
intersection_get_object_type(struct intersection* inter);

void
intersection_new_intersection_list(struct intersection_list* inter_list);

void
intersection_add_intersection_to_list(struct intersection_list* inter_list, struct intersection inter);

void
intersection_clear_intersection_list(struct intersection_list* inter_list);

struct intersection
intersection_hit(struct intersection_list* inter_list);

#endif
