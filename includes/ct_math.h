#ifndef INCLUDES_CT_MATH_H
#define INCLUDES_CT_MATH_H

#define EPSILON		(0.00001)
#define PI		(3.141592654)

#define UNDEF_TIME	(-9999)

double
ctm_abs(const double a);

int
ctm_floats_equal(const double a, const double b);

double
ctm_max(const double a, const double b);

double
ctm_random_jitter(void);

#endif

