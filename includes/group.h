#ifndef INCLUDES_GROUP_H
#define INCLUDES_GROUP_H

#include "matrix.h"
//#include "object_utils.h"
#include "intersection.h"
#include "bounding_box.h"
#include "sphere.h"
#include "plane.h"
#include "cube.h"
#include "cylinder.h"
#include "cone.h"

#define GROUP_SUBDIV_THRESH	1

struct group {
    t_object type;
    int id;
    // Do not manually change the transforms
    struct matrix4 _transform;
    struct matrix4 _inv_transform;
    struct matrix4 _transp_inv_transform;
    struct bbox* box;
    void** list_children;
    int nb_children;
    int max_nb_children;
};

struct group*
group_new_group(void);

void
group_delete_group(struct group* g);

void
group_add_object(struct group* group, void* object);

struct intersection_list
group_intersect_ray(struct group* g, struct ray* ray);

void
group_add_transform(struct group* g, struct matrix4 m);

void
group_make_inv_transform(struct group* g);

void
group_make_transp_inv_transform(struct group* g);

void
group_make_bounding_box(struct group* g);

void
group_remove_object_at(struct group* g, const int i);

void
group_partition_children(struct group* g, struct group* partition[2]);

void
group_make_sub_group(struct group* g, struct group* sub_partition);

void
group_divide_group(struct group* g, const int threshold);

#endif
