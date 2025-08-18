#ifndef INCLUDES_PATTERN_H
#define INCLUDES_PATTERN_H

#include "tuple.h"
#include "matrix.h"

typedef enum {NO_PATTERN, STRIPE, GRADIENT, RING, CHECKER} t_pattern;

struct pattern {
    t_pattern type;
    struct tuple color_a;
    struct tuple color_b;
    struct matrix4 default_transformation;
};

struct pattern
pattern_stripe(struct tuple color_a, struct tuple color_b);

struct tuple
pattern_stripe_at(struct pattern pattern, struct tuple point);

struct tuple
pattern_stripe_at_object(struct pattern pattern, void* object, struct tuple world_point);

struct pattern
pattern_gradient(struct tuple color_a, struct tuple color_b);

struct tuple
pattern_gradient_at(struct pattern pattern, struct tuple point);

struct tuple
pattern_gradient_at_object(struct pattern pattern, void* object, struct tuple world_point);

struct pattern
pattern_ring(struct tuple color_a, struct tuple color_b);

struct tuple
pattern_ring_at(struct pattern pattern, struct tuple point);

struct tuple
pattern_ring_at_object(struct pattern pattern, void* object, struct tuple world_point);

struct pattern
pattern_checker(struct tuple color_a, struct tuple color_b);

struct tuple
pattern_checker_at(struct pattern pattern, struct tuple point);

struct tuple
pattern_checker_at_object(struct pattern pattern, void* object, struct tuple world_point);

#endif
