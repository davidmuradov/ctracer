#include "../includes/camera.h"
#include "../includes/print_image.h"
#include "vec3.h"
#include "../includes/ray.h"
#include <stdio.h>
#include <math.h>

static void camera_init(struct camera* camera);

static inline struct pixel ray_color(struct ray* r);
static inline float hit_sphere(struct vec3 center, float radius, struct ray* r);

void camera_render(struct camera* camera, void* nil) {

    camera_init(camera);

    struct vec3 px_center;
    struct vec3 ray_dir;
    struct ray r;
    struct pixel color = {0};
    struct vec3 inter1;
    struct vec3 inter2;
    struct vec3 inter3;

    printf("P3\n%d %d\n255\n", W, H);
    for (int i = 0; i < H; i++) {
	for (int j = 0; j < W; j++) {
	    inter1 = vec3_mult(camera->px_del_u, j);
	    inter2 = vec3_mult(camera->px_del_v, i);
	    inter3 = vec3_add(inter1, inter2);
	    px_center = vec3_add(camera->px00_loc, inter3);
	    ray_dir = vec3_sub(px_center, camera->center);
	    r.o = camera->center;
	    r.dir = ray_dir;
	    color = ray_color(&r);
	    write_color(&color);
	}
    }
}

static void camera_init(struct camera* camera) {

    // Initialise fields
    camera->image_height = H;
    camera->image_width = W;

    //struct vec3 cam_center = {0, 0, 0};
    camera->center = (struct vec3) {0, 0, 0};

    float focal_len = 1.0;
    float vp_height = 2.0;
    float vp_width = vp_height * (((float)W) / H);

    struct vec3 vp_u = {vp_width, 0, 0};
    struct vec3 vp_v = {0, -vp_height, 0};

    //struct vec3 vp_u_del = vec3_div(vp_u, W);
    camera->px_del_u = vec3_div(vp_u, W);
    //struct vec3 vp_v_del = vec3_div(vp_v, H);
    camera->px_del_v = vec3_div(vp_v, H);

    struct vec3 t_focal_len = {0, 0, focal_len};

    // Calculate vp_up_left
    struct vec3 inter1 = vec3_sub(camera->center, t_focal_len);
    struct vec3 inter2 = vec3_div(vp_u, 2);
    struct vec3 inter3 = vec3_div(vp_v, 2);
    struct vec3 inter4 = vec3_sub(inter1, inter2);
    struct vec3	vp_up_left = vec3_sub(inter4, inter3);

    // Calculate px00_loc
    inter1 = vec3_add(camera->px_del_u, camera->px_del_v);
    inter2 = vec3_mult(inter1, 0.5);
    //struct vec3 px00_loc = vec3_add(vp_up_left, inter2);
    camera->px00_loc = vec3_add(vp_up_left, inter2);

    /*
    struct vec3 px_center;
    struct vec3 ray_dir;
    struct ray r;
    struct pixel color = {0};
    */
}

static inline float hit_sphere(struct vec3 center, float radius, struct ray* r) {

    struct vec3 oc = vec3_sub(center, r->o);
    float a = vec3_len_sqrd(r->dir);
    float h = vec3_dot(r->dir, oc);
    float c = vec3_len_sqrd(oc) - radius * radius;

    float delta = h * h - a * c;

    if (delta < 0)
	return -1;
    else
	return (h - sqrt(delta)) / (a);
}

static struct pixel ray_color(struct ray* r) {

    struct vec3 center = {0, 0, -1};
    float t = hit_sphere(center, 0.5, r);
    if (t > 0) {
	struct vec3 inter1 = vec3_sub(ray_at(*r, t), center);
	struct vec3 normal = vec3_unitv(inter1);
	struct pixel c = {0.5*(normal.x + 1), 0.5*(normal.y + 1), 0.5*(normal.z + 1)};
	return c;
    }

    struct vec3 unit_dir = vec3_unitv(r->dir);
    float a = 0.5 * (unit_dir.y + 1);
    struct pixel c = {(1 - a) * 1, (1 - a) * 1, (1 - a) * 1};
    c.r += a * 0.5;
    c.g += a * 0.7;
    c.b += a * 1.0;
    return c;
}
