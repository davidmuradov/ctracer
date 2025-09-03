#include "../includes/world.h"
#include "ct_math.h"
#include "cube.h"
#include "cylinder.h"
#include "group.h"
#include "intersection.h"
#include "lights.h"
#include "materials.h"
#include "../includes/object_utils.h"
#include "pattern.h"
#include "ray.h"
#include "sphere.h"
#include "tuple.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define INIT_NB_OBJECTS		1
#define INIT_NB_LIGHTS		1

struct world {
    void** object_list;
    void** light_list;
    int nb_objects;
    int max_nb_objects;
    int nb_lights;
    int max_nb_lights;
};

int
world_compare_intersections(const void* a, const void* b);

struct world*
world_new_world(void) {
    struct world* w = calloc(1, sizeof(struct world));
    if (!w) {
	fprintf(stderr, "Failed to allocate memory for new world\n");
	exit(1);
    }

    w->object_list = calloc(INIT_NB_OBJECTS, sizeof(void*));
    if (!w->object_list) {
	fprintf(stderr, "Failed to allocate memory for world's list of objects\n");
	exit(1);
    }
    w->max_nb_objects = INIT_NB_OBJECTS;
    for (int i = 0; i < INIT_NB_OBJECTS; i++) {
	w->object_list[i] = NULL;
    }

    w->light_list = calloc(INIT_NB_LIGHTS, sizeof(void*));
    if (!w->light_list) {
	fprintf(stderr, "Failed to allocate memory for world's list of lights\n");
	exit(1);
    }
    w->max_nb_lights = INIT_NB_LIGHTS;
    for (int i = 0; i < INIT_NB_LIGHTS; i++) {
	w->light_list[i] = NULL;
    }

    w->nb_objects = w->nb_lights = 0;

    return w;
}

int
world_get_nb_objects(struct world* w) {return w->nb_objects;}

int
world_get_nb_lights(struct world* w) {return w->nb_lights;}

int
world_get_max_nb_objects(struct world* w) {return w->max_nb_objects;}

int
world_get_max_nb_lights(struct world* w) {return w->max_nb_lights;}

void
world_free_world(struct world* w) {
#if(WORLD_MEM_CLEANUP)
    t_object obj_type = UNKNOWN_OBJECT;
    for (int i = 0; i < w->nb_objects; i++) {
	obj_type = object_utils_get_object_type(w->object_list[i]);
	switch (obj_type) {
	    case SPHERE:
		sphere_delete_sphere((struct sphere*) w->object_list[i]);
		break;
	    case PLANE:
		plane_delete_plane((struct plane*) w->object_list[i]);
		break;
	    case CUBE:
		cube_delete_cube((struct cube*) w->object_list[i]);
		break;
	    case CYLINDER:
		cylinder_delete_cylinder((struct cylinder*) w->object_list[i]);
		break;
	    case CONE:
		cone_delete_cone((struct cone*) w->object_list[i]);
		break;
	    case GROUP:
		group_delete_group((struct group*) w->object_list[i]);
		break;
	    case TRIANGLE:
		//triangle_delete_triangle((struct triangle*) w->object_list[i]);
		break;
	    case CSG:
		//csg_delete_csg((struct csg*) w->object_list[i]);
		break;
	    default:
		break;
	}
    }

    free(w->object_list);
    w->object_list = NULL;
    free(w->light_list);
    w->light_list = NULL;
    free(w);
    w = NULL;
#endif
}

void
world_add_sphere(struct world* world, struct sphere* sphere) {
    sphere_make_inv_transform(sphere);
    sphere_make_transp_inv_transform(sphere);
    pattern_make_inv_transform(&(sphere->material.pattern));
    if (world->nb_objects < world->max_nb_objects) {
	world->object_list[world->nb_objects] = (void*) sphere;
	world->nb_objects++;
    }
    else {
	world->object_list = realloc(world->object_list, world->max_nb_objects * 2 * sizeof(void*));
	if (!world->object_list) {
	    fprintf(stderr, "Failed to allocate new memory for list of objects in world\n");
	    exit(1);
	}
	world->max_nb_objects *= 2;
	world->object_list[world->nb_objects] = (void*) sphere;
	world->nb_objects++;
    }
}

void
world_add_plane(struct world* world, struct plane* plane) {
    plane_make_inv_transform(plane);
    plane_make_transp_inv_transform(plane);
    pattern_make_inv_transform(&(plane->material.pattern));
    if (world->nb_objects < world->max_nb_objects) {
	world->object_list[world->nb_objects] = (void*) plane;
	world->nb_objects++;
    }
    else {
	world->object_list = realloc(world->object_list, world->max_nb_objects * 2 * sizeof(void*));
	if (!world->object_list) {
	    fprintf(stderr, "Failed to allocate new memory for list of objects in world\n");
	    exit(1);
	}
	world->max_nb_objects *= 2;
	world->object_list[world->nb_objects] = (void*) plane;
	world->nb_objects++;
    }
}

void
world_add_cube(struct world* world, struct cube* cube) {
    cube_make_inv_transform(cube);
    cube_make_transp_inv_transform(cube);
    pattern_make_inv_transform(&(cube->material.pattern));
    if (world->nb_objects < world->max_nb_objects) {
	world->object_list[world->nb_objects] = (void*) cube;
	world->nb_objects++;
    }
    else {
	world->object_list = realloc(world->object_list, world->max_nb_objects * 2 * sizeof(void*));
	if (!world->object_list) {
	    fprintf(stderr, "Failed to allocate new memory for list of objects in world\n");
	    exit(1);
	}
	world->max_nb_objects *= 2;
	world->object_list[world->nb_objects] = (void*) cube;
	world->nb_objects++;
    }
}

void
world_add_cylinder(struct world* world, struct cylinder* cylinder) {
    cylinder_make_inv_transform(cylinder);
    cylinder_make_transp_inv_transform(cylinder);
    pattern_make_inv_transform(&(cylinder->material.pattern));
    if (world->nb_objects < world->max_nb_objects) {
	world->object_list[world->nb_objects] = (void*) cylinder;
	world->nb_objects++;
    }
    else {
	world->object_list = realloc(world->object_list, world->max_nb_objects * 2 * sizeof(void*));
	if (!world->object_list) {
	    fprintf(stderr, "Failed to allocate new memory for list of objects in world\n");
	    exit(1);
	}
	world->max_nb_objects *= 2;
	world->object_list[world->nb_objects] = (void*) cylinder;
	world->nb_objects++;
    }
}

void
world_add_cone(struct world* world, struct cone* cone) {
    cone_make_inv_transform(cone);
    cone_make_transp_inv_transform(cone);
    pattern_make_inv_transform(&(cone->material.pattern));
    if (world->nb_objects < world->max_nb_objects) {
	world->object_list[world->nb_objects] = (void*) cone;
	world->nb_objects++;
    }
    else {
	world->object_list = realloc(world->object_list, world->max_nb_objects * 2 * sizeof(void*));
	if (!world->object_list) {
	    fprintf(stderr, "Failed to allocate new memory for list of objects in world\n");
	    exit(1);
	}
	world->max_nb_objects *= 2;
	world->object_list[world->nb_objects] = (void*) cone;
	world->nb_objects++;
    }
}

void
world_add_group(struct world* world, struct group* group) {
    group_make_inv_transform(group);
    group_make_transp_inv_transform(group);
    group_make_bounding_box(group);
    group_divide_group(group, GROUP_SUBDIV_THRESH);
    if (world->nb_objects < world->max_nb_objects) {
	world->object_list[world->nb_objects] = (void*) group;
	world->nb_objects++;
    }
    else {
	world->object_list = realloc(world->object_list, world->max_nb_objects * 2 * sizeof(void*));
	if (!world->object_list) {
	    fprintf(stderr, "Failed to allocate new memory for list of objects in world\n");
	    exit(1);
	}
	world->max_nb_objects *= 2;
	world->object_list[world->nb_objects] = (void*) group;
	world->nb_objects++;
    }
}

void
world_add_point_light(struct world* world, struct point_light* light) {
    if (world->nb_lights < world->max_nb_lights) {
	world->light_list[world->nb_lights] = (void*) light;
	world->nb_lights++;
    }
    else {
	world->light_list = realloc(world->light_list, world->max_nb_lights * 2 * sizeof(void*));
	if (!world->light_list) {
	    fprintf(stderr, "Failed to allocate new memory for list of objects in world\n");
	    exit(1);
	}
	world->max_nb_lights *= 2;
	world->light_list[world->nb_lights] = (void*) light;
	world->nb_lights++;
    }

}

void*
world_get_object_at(struct world* world, int i) {
    if (i < 0 || i > world->nb_objects - 1) {
	fprintf(stderr, "Out of bound reading on world->object_list\n");
	exit(1);
    }
    return world->object_list[i];
}

void*
world_get_light_at(struct world* world, int i) {
    if (i < 0 || i > world->nb_lights - 1) {
	fprintf(stderr, "Out of bound reading on world->light_list\n");
	exit(1);
    }
    return world->light_list[i];
}

struct intersection_list
world_intersect_world(struct world* world, struct ray* ray) {
    struct intersection_list full_list = intersection_new_intersection_list();
    struct intersection_list current_list;
    t_object object_type = UNKNOWN_OBJECT;
    void* current_object;

    for (int i = 0; i < world->nb_objects; i++) {
	current_object = world_get_object_at(world, i);
	object_type = world_get_object_type(current_object);
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

    struct intersection inter_array[full_list.nb_intersections];
    for (int i = 0; i < full_list.nb_intersections; i++) {
	inter_array[i] = full_list.list[i];
    }
    qsort(inter_array, full_list.nb_intersections, sizeof(struct intersection), world_compare_intersections);
    for (int i = 0; i < full_list.nb_intersections; i++) {
	full_list.list[i] = inter_array[i];
    }

    return full_list;
}

int
world_compare_intersections(const void* a, const void* b) {
    struct intersection arg1 = *(struct intersection*) a;
    struct intersection arg2 = *(struct intersection*) b;

    if (arg1.t < arg2.t) return -1;
    if (arg1.t > arg2.t) return 1;

    return 0;
}

t_object
world_get_object_type(void* object) {
    struct sphere* obj = (struct sphere*) object;
    return obj->type;
}

struct tuple
world_shade_hit(struct world* world, struct precompute* comps, int remaining_calls) {
    struct material mat = object_utils_get_material(comps->object);

    struct tuple color = tuple_new_color(0, 0, 0);
    int shadowed = 0;

    for (int i = 0; i < world->nb_lights; i++) {
	shadowed = world_is_shadowed(world, i, comps->over_point);
	color = tuple_add(color, lights_lighting_sphere(mat, comps->object,
		    *(struct point_light*) world->light_list[i], comps->point, comps->eyev,
		    comps->normalv, shadowed));
    }

    struct tuple reflected = world_reflected_color(world, comps, remaining_calls);
    struct tuple refracted = world_refracted_color(world, comps, remaining_calls);

    if (mat.reflective > 0 && mat.transparency > 0) {
	double reflectance = intersection_schlick(comps);
	struct tuple t1 = tuple_scalar_mult(reflected, reflectance);
	struct tuple t2 = tuple_scalar_mult(refracted, (1 - reflectance));
	return tuple_add(tuple_add(color, t1), t2);
    }

    return tuple_add(tuple_add(color, reflected), refracted);
}

struct tuple
world_color_at(struct world* world, struct ray* ray, int remaining_calls) {
    struct tuple color = tuple_new_color(0, 0, 0);
    struct intersection_list list = world_intersect_world(world, ray);
    struct intersection hit = intersection_hit(&list);

    if (ctm_floats_equal(hit.t, UNDEF_TIME)) {
	intersection_clear_intersection_list(&list);
	return color;
    }

    struct precompute comps = intersection_prepare_computations(&hit, ray, &list);
    color = world_shade_hit(world, &comps, remaining_calls);
    intersection_clear_intersection_list(&list);

    return color;
}

int
world_is_shadowed(struct world* world, int i, struct tuple point) {
    struct point_light* l = (struct point_light*) world->light_list[i];
    struct tuple v = tuple_sub(l->position, point);
    double distance = tuple_mag(v);
    struct tuple direction = tuple_normalize(v);
    int is_shadowed = 0;

    struct ray r = ray_new_ray(point, direction);
    struct intersection_list intersections = world_intersect_world(world, &r);
    struct intersection hit = intersection_hit(&intersections);
    intersection_clear_intersection_list(&intersections);

    // Here we want to get the object casts_shadows property in it's material field
    // to see if an object should cast shadows.
    struct material mat;
    int can_cast_shadows = 1;

    if (hit.object != NULL) {
	mat = object_utils_get_material(hit.object);
	can_cast_shadows = mat.casts_shadows;
    }

    if (hit.t < distance && hit.object != NULL && can_cast_shadows)
	return is_shadowed = 1;

    return is_shadowed;
}

struct tuple
world_reflected_color(struct world* world, struct precompute* comps, int remaining_calls) {
    struct material mat = object_utils_get_material(comps->object);

    if (ctm_floats_equal(mat.reflective, 0))
	return tuple_new_color(0, 0, 0);
    if (remaining_calls < 1)
	return tuple_new_color(0, 0, 0);

    struct ray reflect_ray = ray_new_ray(comps->over_point, comps->reflectv);
    struct tuple color = world_color_at(world, &reflect_ray, remaining_calls - 1);

    return tuple_scalar_mult(color, mat.reflective);
}

struct tuple
world_refracted_color(struct world* world, struct precompute* comps, int remaining_calls) {
    struct material mat = object_utils_get_material(comps->object);

    if (ctm_floats_equal(mat.transparency, 0))
	return tuple_new_color(0, 0, 0);
    if (remaining_calls < 1)
	return tuple_new_color(0, 0, 0);

    double n_ratio = comps->n1 / comps->n2;
    double cos_i = tuple_dot(comps->eyev, comps->normalv);
    double sin2_t = n_ratio * n_ratio * (1 - (cos_i * cos_i));

    if (sin2_t > 1)
	return tuple_new_color(0, 0, 0);

    double cos_t = sqrt(1 - sin2_t);
    struct tuple direction = tuple_sub(tuple_scalar_mult(comps->normalv, (n_ratio * cos_i - cos_t)),
	    tuple_scalar_mult(comps->eyev, n_ratio));
    struct ray refract_ray = ray_new_ray(comps->under_point, direction);
    struct tuple color = tuple_scalar_mult(world_color_at(world, &refract_ray, remaining_calls - 1), mat.transparency);

    return color;
}
