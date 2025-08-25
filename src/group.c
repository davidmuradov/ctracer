#include "../includes/group.h"
#include "object_utils.h"
#include <stdlib.h>
#include <stdio.h>

#define INIT_MAX_NB_CHILDREN_GROUP	1

static int UNIQUE_ID_GROUP = 0;

struct group
group_new_group(void) {
    struct group g;

    g._transform = matrix_make_identity4();
    g._inv_transform = matrix_make_identity4();
    g._transp_inv_transform = matrix_make_identity4();
    g.type = GROUP;
    g.id = UNIQUE_ID_GROUP;
    UNIQUE_ID_GROUP++;

    g.list_children = calloc(INIT_MAX_NB_CHILDREN_GROUP, sizeof(void*));
    if (!g.list_children) {
	fprintf(stderr, "Failed to allocate memory for world's list of objects\n");
	exit(1);
    }
    g.max_nb_children = INIT_MAX_NB_CHILDREN_GROUP;
    g.nb_children = 0;

    return g;
}

void
group_free_group(struct group* g) {
    free(g->list_children);
    g->list_children = NULL;
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
	object_type = object_utils_get_object_type(current_object);
	switch (object_type) {
	    case SPHERE:
		sphere_make_inv_transform((struct sphere*) current_object);
		break;
	    case PLANE:
		plane_make_inv_transform((struct plane*) current_object);
		break;
	    case CUBE:
		cube_make_inv_transform((struct cube*) current_object);
		break;
	    case CYLINDER:
		cylinder_make_inv_transform((struct cylinder*) current_object);
		break;
	    case CONE:
		cone_make_inv_transform((struct cone*) current_object);
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
