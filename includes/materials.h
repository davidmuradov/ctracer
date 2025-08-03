#ifndef INCLUDES_MATERIALS_H
#define INCLUDES_MATERIALS_H

#include "tuple.h"

struct material {
    struct tuple color;
    double ambient;
    double diffuse;
    double specular;
    double shininess;
};

struct material
materials_new_material(void);

#endif
