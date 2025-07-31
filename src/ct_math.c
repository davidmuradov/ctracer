#include "../includes/ct_math.h"

double ctm_abs(double a) {
    return ((a < 0) ? (-a):(a));
}

int ctm_floats_equal(const double a, const double b) {
    return (ctm_abs(a - b) < EPSILON) ? (1):(0);
}
