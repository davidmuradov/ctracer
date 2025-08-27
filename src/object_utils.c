#include "../includes/object_utils.h"
#include "materials.h"
#include "matrix.h"
#include "group.h"

struct material
object_utils_get_material(void* object) {
    t_object obj_type = world_get_object_type(object);
    struct material mat;
    struct sphere* maybe_sphere = (struct sphere*) object;
    struct plane* maybe_plane = (struct plane*) object;
    struct cube* maybe_cube = (struct cube*) object;
    struct cylinder* maybe_cylinder = (struct cylinder*) object;
    struct cone* maybe_cone = (struct cone*) object;
    struct group* maybe_group = (struct group*) object;

    switch (obj_type) {
	case SPHERE:
	    mat = maybe_sphere->material;
	    break;
	case PLANE:
	    mat = maybe_plane->material;
	    break;
	case CUBE:
	    mat = maybe_cube->material;
	    break;
	case CYLINDER:
	    mat = maybe_cylinder->material;
	    break;
	case CONE:
	    mat = maybe_cone->material;
	    break;
	case GROUP:
	    mat = materials_new_material(); // maybe_group->material; // Group does not have a material for now
	    break;
	default:
	    mat = materials_new_material();
	    break;
    }

    return mat;
}

struct matrix4
object_utils_get_transform(void* object) {
    t_object obj_type = world_get_object_type(object);
    struct matrix4 transform;
    struct sphere* maybe_sphere = (struct sphere*) object;
    struct plane* maybe_plane = (struct plane*) object;
    struct cube* maybe_cube = (struct cube*) object;
    struct cylinder* maybe_cylinder = (struct cylinder*) object;
    struct cone* maybe_cone = (struct cone*) object;
    struct group* maybe_group = (struct group*) object;

    switch (obj_type) {
	case SPHERE:
	    transform = maybe_sphere->transform;
	    break;
	case PLANE:
	    transform = maybe_plane->transform;
	    break;
	case CUBE:
	    transform = maybe_cube->transform;
	    break;
	case CYLINDER:
	    transform = maybe_cylinder->transform;
	    break;
	case CONE:
	    transform = maybe_cone->transform;
	    break;
	case GROUP:
	    transform = maybe_group->_transform;
	    break;
	default:
	    transform = matrix_make_identity4();
	    break;
    }

    return transform;
}

struct matrix4
object_utils_get_inv_transform(void* object) {
    t_object obj_type = world_get_object_type(object);
    struct matrix4 transform;
    struct sphere* maybe_sphere = (struct sphere*) object;
    struct plane* maybe_plane = (struct plane*) object;
    struct cube* maybe_cube = (struct cube*) object;
    struct cylinder* maybe_cylinder = (struct cylinder*) object;
    struct cone* maybe_cone = (struct cone*) object;
    struct group* maybe_group = (struct group*) object;

    switch (obj_type) {
	case SPHERE:
	    transform = maybe_sphere->inv_transform;
	    break;
	case PLANE:
	    transform = maybe_plane->inv_transform;
	    break;
	case CUBE:
	    transform = maybe_cube->inv_transform;
	    break;
	case CYLINDER:
	    transform = maybe_cylinder->inv_transform;
	    break;
	case CONE:
	    transform = maybe_cone->inv_transform;
	    break;
	case GROUP:
	    transform = maybe_group->_inv_transform;
	    break;
	default:
	    transform = matrix_make_identity4();
	    break;
    }

    return transform;
}

struct matrix4
object_utils_get_transp_inv_transform(void* object) {
    t_object obj_type = world_get_object_type(object);
    struct matrix4 transform;
    struct sphere* maybe_sphere = (struct sphere*) object;
    struct plane* maybe_plane = (struct plane*) object;
    struct cube* maybe_cube = (struct cube*) object;
    struct cylinder* maybe_cylinder = (struct cylinder*) object;
    struct cone* maybe_cone = (struct cone*) object;
    struct group* maybe_group = (struct group*) object;

    switch (obj_type) {
	case SPHERE:
	    transform = maybe_sphere->transp_inv_transform;
	    break;
	case PLANE:
	    transform = maybe_plane->transp_inv_transform;
	    break;
	case CUBE:
	    transform = maybe_cube->transp_inv_transform;
	    break;
	case CYLINDER:
	    transform = maybe_cylinder->transp_inv_transform;
	    break;
	case CONE:
	    transform = maybe_cone->transp_inv_transform;
	    break;
	case GROUP:
	    transform = maybe_group->_transp_inv_transform;
	    break;
	default:
	    transform = matrix_make_identity4();
	    break;
    }

    return transform;
}

int
object_utils_is_object_in_containers(void* object, void* containers[], const int sz) {
    for (int i = 0; i < sz; i++)
	if (object == containers[i])
	    return 1;
    return 0;
}

void
object_utils_remove_object_from_containers(void* object, void* containers[], const int sz) {
    for (int i = 0; i < sz; i++) {
	if(object == containers[i]) {
	    containers[i] = NULL;
	    for (int j = i; j < sz - 1; j++)
		containers[j] = containers[j + 1];
	    containers[sz - 1] = NULL;
	    return;
	}
    }
}

void
object_utils_add_object_to_containers(void* object, void* containers[], const int sz) {
    for (int i = 0; i < sz; i++)
	if (containers[i] == NULL) {
	    containers[i] = object;
	    return;
	}
}

t_object
object_utils_get_object_type(void* object) {
    struct sphere* obj = (struct sphere*) object;
    return obj->type;
}
