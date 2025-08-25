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
object_utils_get_material(void* object);

/**
 * Returns the transform property of an object.
 */
struct matrix4
object_utils_get_transform(void* object);

/**
 * Returns the inverse transform property of an object.
 */
struct matrix4
object_utils_get_inv_transform(void* object);

/**
 * Returns the transpose of the inverse transform property of an object.
 */
struct matrix4
object_utils_get_transp_inv_transform(void* object);

/**
 * Returns 1 if an object is included in a containers of size sz. Returns 0 otherwise.
 */
int
object_utils_is_object_in_containers(void* object, void* containers[], const int sz);

/**
 * Removes object from containers of size sz.
 */
void
object_utils_remove_object_from_containers(void* object, void* containers[], const int sz);

/**
 * Adds object to containers of size sz.
 */
void
object_utils_add_object_to_containers(void* object, void* containers[], const int sz);

/**
 * Return the object type of a shape primitive.
 */
t_object
object_utils_get_object_type(void* object);

#endif
