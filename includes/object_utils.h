#ifndef INCLUDES_OBJECT_UTILS_H
#define INCLUDES_OBJECT_UTILS_H

#include "materials.h"
#include "matrix.h"
#include "world.h"
#include "intersection.h"
#include <stdlib.h>

/**
 * Returns the material property of an object.
 */
struct material
object_utils_get_material(const void* object);

/**
 * Returns the transform property of an object.
 */
struct matrix4
object_utils_get_transform(const void* object);

/**
 * Returns 1 if an object is included in a containers of size sz. Returns 0 otherwise.
 */
int
object_utils_is_object_in_containers(const void* object, void* containers[], const int sz);

/**
 * Removes object from containers of size sz.
 */
void
object_utils_remove_object_from_containers(const void* object, void* containers[], const int sz);

/**
 * Adds object to containers of size sz.
 */
void
object_utils_add_object_to_containers(void* object, void* containers[], const int sz);

#endif
