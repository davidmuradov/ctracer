#ifndef INCLUDES_TUPLE_H
#define INCLUDES_TUPLE_H

struct tuple {
    double x;
    double y;
    double z;
    int w;
};

struct tuple tuple_new_point(double x, double y, double z);

struct tuple tuple_new_vector(double x, double y, double z);

int tuple_equals(struct tuple a, struct tuple b);

struct tuple tuple_add(struct tuple a, struct tuple b);

struct tuple tuple_sub(struct tuple a, struct tuple b);

struct tuple tuple_negate(struct tuple a);

struct tuple tuple_scalar_mult(struct tuple a, double b);

struct tuple tuple_scalar_div(struct tuple a, double b);

double tuple_mag(struct tuple a);

struct tuple tuple_normalize(struct tuple a);

double tuple_dot(struct tuple a, struct tuple b);

struct tuple tuple_cross(struct tuple a, struct tuple b);

void tuple_print(struct tuple a);

struct tuple tuple_new_color(double r, double g, double b);

struct tuple tuple_color_mult(struct tuple a, struct tuple b);

#endif
