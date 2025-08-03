#include "../includes/materials.h"
#include "../includes/tuple.h"

struct material
materials_new_material(void) {
    struct material m;
    m.color = tuple_new_color(1, 1, 1);
    m.ambient = 0.1;
    m.diffuse = 0.9;
    m.specular = 0.9;
    m.shininess = 200.0;
    return m;
}
