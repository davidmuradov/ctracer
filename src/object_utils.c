#include "../includes/object_utils.h"

struct material
object_utils_get_material(const void* object) {
    t_object obj_type = world_get_object_type(object);
    struct material mat;
    struct sphere* maybe_sphere = (struct sphere*) object;
    struct plane* maybe_plane = (struct plane*) object;
    struct cube* maybe_cube = (struct cube*) object;
    struct cylinder* maybe_cylinder = (struct cylinder*) object;

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
	default:
	    mat = materials_new_material();
	    break;
    }

    return mat;
}

int
object_utils_is_object_in_containers(const void* object, void* containers[], const int sz) {
    for (int i = 0; i < sz; i++)
	if (object == containers[i])
	    return 1;
    return 0;
}

void
object_utils_remove_object_from_containers(const void* object, void* containers[], const int sz) {
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
