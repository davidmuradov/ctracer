#ifndef INCLUDES_MATERIALS_H
#define INCLUDES_MATERIALS_H

#include "pattern.h"
#include "tuple.h"

struct material {
    struct tuple color;
    double ambient;
    double diffuse;
    double specular;
    double shininess;
    struct pattern pattern;
    double reflective;
    double transparency;
    double refractive_index;
};

struct material
materials_new_material(void);

#endif
