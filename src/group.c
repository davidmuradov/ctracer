#include "../includes/group.h"
#include "bounding_box.h"
#include "cone.h"
#include "intersection.h"
#include "object_utils.h"
#include "sphere.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define INIT_MAX_NB_CHILDREN_GROUP	1

static int UNIQUE_ID_GROUP = 0;

struct group*
group_new_group(void) {
    struct group* g = malloc(sizeof(struct group));
    if (!g) {
	fprintf(stderr, "Error allocating memory for sphere\n");
	exit(1);
    }

    g->_transform = matrix_make_identity4();
    g->_inv_transform = matrix_make_identity4();
    g->_transp_inv_transform = matrix_make_identity4();
    g->type = GROUP;
    g->id = UNIQUE_ID_GROUP;
    UNIQUE_ID_GROUP++;
    g->box = NULL;

    g->list_children = calloc(INIT_MAX_NB_CHILDREN_GROUP, sizeof(void*));
    if (!g->list_children) {
	fprintf(stderr, "Failed to allocate memory for world's list of objects\n");
	exit(1);
    }
    g->max_nb_children = INIT_MAX_NB_CHILDREN_GROUP;
    g->nb_children = 0;

    return g;
}

void
group_delete_group(struct group* g) {
    if (g->box)
	bbox_delete_box(g->box);

    t_object type = UNKNOWN_OBJECT;

    for (int i = 0; i < g->nb_children; i++) {
	type = object_utils_get_object_type(g->list_children[i]);
	switch (type) {
	    case SPHERE:
		sphere_delete_sphere((struct sphere*) g->list_children[i]);
		break;
	    case PLANE:
		plane_delete_plane((struct plane*) g->list_children[i]);
		break;
	    case CUBE:
		cube_delete_cube((struct cube*) g->list_children[i]);
		break;
	    case CYLINDER:
		cylinder_delete_cylinder((struct cylinder*) g->list_children[i]);
		break;
	    case CONE:
		cone_delete_cone((struct cone*) g->list_children[i]);
		break;
	    case TRIANGLE:
		//triangle_delete_triangle((struct triangle*) g->list_children[i]);
		break;
	    case GROUP:
		group_delete_group((struct group*) g->list_children[i]);
		break;
	    case CSG:
		//csg_delete_csg((struct triangle*) g->list_children[i]);
		break;
	    default:
		// something
		break;
	}
    }

    free(g->list_children);
    g->list_children = NULL;
    free(g);
    g = NULL;
}

void
group_add_object(struct group* group, void* object) {
    if (group->nb_children < group->max_nb_children) {
	group->list_children[group->nb_children] = object;
	group->nb_children++;
    }
    else {
	group->list_children = realloc(group->list_children, group->max_nb_children * 2 * sizeof(void*));
	if (!group->list_children) {
	    fprintf(stderr, "Failed to allocate new memory for list of objects in world\n");
	    exit(1);
	}
	group->max_nb_children *= 2;
	group->list_children[group->nb_children] = object;
	group->nb_children++;
    }
}

struct intersection_list
group_intersect_ray(struct group* g, struct ray* ray) {
    struct intersection_list full_list = intersection_new_intersection_list();

    // Ray does not hit groups bounding box, return empty list
    if (!bbox_intersect_ray(g->box, ray))
	return full_list;

    struct intersection_list current_list;
    t_object object_type = UNKNOWN_OBJECT;
    void* current_object;

    for (int i = 0; i < g->nb_children; i++) {
	current_object = g->list_children[i];
	object_type = object_utils_get_object_type(current_object);
	switch (object_type) {
	    case SPHERE:
		current_list = sphere_intersect_ray((struct sphere*) current_object, ray);
		for (int j = 0; j < current_list.nb_intersections; j++) {
		    intersection_add_intersection_to_list(&full_list, current_list.list[j]);
		}
		break;
	    case PLANE:
		current_list = plane_intersect_ray((struct plane*) current_object, ray);
		for (int j = 0; j < current_list.nb_intersections; j++) {
		    intersection_add_intersection_to_list(&full_list, current_list.list[j]);
		}
		break;
	    case CUBE:
		current_list = cube_intersect_ray((struct cube*) current_object, ray);
		for (int j = 0; j < current_list.nb_intersections; j++) {
		    intersection_add_intersection_to_list(&full_list, current_list.list[j]);
		}
		break;
	    case CYLINDER:
		current_list = cylinder_intersect_ray((struct cylinder*) current_object, ray);
		for (int j = 0; j < current_list.nb_intersections; j++) {
		    intersection_add_intersection_to_list(&full_list, current_list.list[j]);
		}
		break;
	    case CONE:
		current_list = cone_intersect_ray((struct cone*) current_object, ray);
		for (int j = 0; j < current_list.nb_intersections; j++) {
		    intersection_add_intersection_to_list(&full_list, current_list.list[j]);
		}
		break;
	    case GROUP:
		current_list = group_intersect_ray((struct group*) current_object, ray);
		for (int j = 0; j < current_list.nb_intersections; j++) {
		    intersection_add_intersection_to_list(&full_list, current_list.list[j]);
		}
		break;
	    default:
		// The rest will come later
		break;
	}
	intersection_clear_intersection_list(&current_list);
    }

    return full_list;
}

void
group_add_transform(struct group* g, struct matrix4 m) {
    //g->_transform = matrix_mult_matrix4(m, g->_transform);

    t_object object_type = UNKNOWN_OBJECT;
    void* current_object;

    for (int i = 0; i < g->nb_children; i++) {
	current_object = g->list_children[i];
	object_type = object_utils_get_object_type(current_object);
	switch (object_type) {
	    case SPHERE:
		sphere_add_transform((struct sphere*) current_object, m);
		break;
	    case PLANE:
		plane_add_transform((struct plane*) current_object, m);
		break;
	    case CUBE:
		cube_add_transform((struct cube*) current_object, m);
		break;
	    case CYLINDER:
		cylinder_add_transform((struct cylinder*) current_object, m);
		break;
	    case CONE:
		cone_add_transform((struct cone*) current_object, m);
		break;
	    case GROUP:
		group_add_transform((struct group*) current_object, m);
		break;
	    default:
		// The rest will come later
		break;
	}
    }
}

void
group_make_inv_transform(struct group* g) {
    //matrix_inverse_matrix4(g->_transform, &(g->_inv_transform));

    t_object object_type = UNKNOWN_OBJECT;
    void* current_object;

    for (int i = 0; i < g->nb_children; i++) {
	current_object = g->list_children[i];
	struct sphere* maybe_sphere = (struct sphere*) current_object;
	struct plane* maybe_plane = (struct plane*) current_object;
	struct cube* maybe_cube = (struct cube*) current_object;
	struct cylinder* maybe_cylinder = (struct cylinder*) current_object;
	struct cone* maybe_cone = (struct cone*) current_object;
	object_type = object_utils_get_object_type(current_object);
	switch (object_type) {
	    case SPHERE:
		sphere_make_inv_transform(maybe_sphere);
		pattern_make_inv_transform(&maybe_sphere->material.pattern);
		break;
	    case PLANE:
		plane_make_inv_transform(maybe_plane);
		pattern_make_inv_transform(&maybe_plane->material.pattern);
		break;
	    case CUBE:
		cube_make_inv_transform(maybe_cube);
		pattern_make_inv_transform(&maybe_cube->material.pattern);
		break;
	    case CYLINDER:
		cylinder_make_inv_transform(maybe_cylinder);
		pattern_make_inv_transform(&maybe_cylinder->material.pattern);
		break;
	    case CONE:
		cone_make_inv_transform(maybe_cone);
		pattern_make_inv_transform(&maybe_cone->material.pattern);
		break;
	    case GROUP:
		group_make_inv_transform((struct group*) current_object);
		break;
	    default:
		// The rest will come later
		break;
	}
    }
}

void
group_make_transp_inv_transform(struct group* g) {
    //g->_transp_inv_transform = matrix_transpose4(g->_inv_transform);

    t_object object_type = UNKNOWN_OBJECT;
    void* current_object;

    for (int i = 0; i < g->nb_children; i++) {
	current_object = g->list_children[i];
	object_type = object_utils_get_object_type(current_object);
	switch (object_type) {
	    case SPHERE:
		sphere_make_transp_inv_transform((struct sphere*) current_object);
		break;
	    case PLANE:
		plane_make_transp_inv_transform((struct plane*) current_object);
		break;
	    case CUBE:
		cube_make_transp_inv_transform((struct cube*) current_object);
		break;
	    case CYLINDER:
		cylinder_make_transp_inv_transform((struct cylinder*) current_object);
		break;
	    case CONE:
		cone_make_transp_inv_transform((struct cone*) current_object);
		break;
	    case GROUP:
		group_make_transp_inv_transform((struct group*) current_object);
		break;
	    default:
		// The rest will come later
		break;
	}
    }
}

void
group_make_bounding_box(struct group* g) {
    g->box = bbox_bounds_of_shape(g);
    struct group* child_group;

    for (int i = 0; i < g->nb_children; i++) {
	child_group = g->list_children[i];
	if (child_group->type == GROUP)
	    group_make_bounding_box(child_group);
    }
}

void
group_remove_object_at(struct group* g, const int i) {
    if (i < 0 || i > g->nb_children - 1) {
	fprintf(stderr, "Failed to remove children from group: invalid range\n");
	exit(1);
    }

    g->list_children[i] = NULL;

    for (int j = i; j < g->nb_children - 1; j++)
	g->list_children[j] = g->list_children[j + 1];
    g->list_children[g->nb_children - 1] = NULL;

    g->nb_children--;
}

void
group_partition_children(struct group* g, struct group* partition[2]) {
    partition[0] = group_new_group();
    partition[1] = group_new_group();

    struct bbox* split[2] = {0};
    bbox_split_bounds(g->box, split);
    struct bbox* bbox_child;

    for (int i = 0; i < g->nb_children; i++) {
	bbox_child = bbox_bounds_of_shape(g->list_children[i]);
	if (bbox_contains_bbox(split[0], bbox_child)) {
	    group_add_object(partition[0], g->list_children[i]);
	    group_remove_object_at(g, i);
	    i--;
	}
	else if (bbox_contains_bbox(split[1], bbox_child)) {
	    group_add_object(partition[1], g->list_children[i]);
	    group_remove_object_at(g, i);
	    i--;
	}
	bbox_delete_box(bbox_child);
    }

    partition[0]->box = split[0];
    partition[1]->box = split[1];
}

void
group_make_sub_group(struct group* g, struct group* sub_partition) {
    /*
    struct group* sub_g = group_new_group();

    for (int i = 0; i < sub_partition->nb_children; i++) {
	group_add_object(sub_g, sub_partition->list_children[i]);
    }
    */

    group_add_object(g, sub_partition);
    free(g->box);
    free(sub_partition->box);
    group_make_bounding_box(g);
}

void
group_divide_group(struct group* g, const int threshold) {
    t_object type = UNKNOWN_OBJECT;

    struct group* partition[2] = {0};

    if (threshold <= g->nb_children) {

	group_partition_children(g, partition);

	if (partition[0]->nb_children)
	    group_make_sub_group(g, partition[0]);
	else {
	    free(partition[0]->box);
	    free(partition[0]->list_children);
	    free(partition[0]);
	}
	if (partition[1]->nb_children)
	    group_make_sub_group(g, partition[1]);
	else {
	    free(partition[1]->box);
	    free(partition[1]->list_children);
	    free(partition[1]);
	}
    }

    for (int i = 0; i < g->nb_children; i++) {
	type = object_utils_get_object_type(g->list_children[i]);
	if (type == GROUP)
	    group_divide_group(g->list_children[i], threshold);
    }
}
