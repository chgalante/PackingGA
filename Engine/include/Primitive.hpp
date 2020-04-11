#ifndef _PRIMITIVES_H
#define _PRIMITIVES_H

#include "Geometry.hpp"

enum PrimitiveType
{
    PLANE   = 0,
    CUBE    = 1,
    SPHERE  = 2,
    LINE    = 3
};

class Primitive
{
public:
    /* World Scale */
    float Unit = 1.0f;

    /*Geometry* InitializeGrid(void);*/
    Geometry* Plane(void);
    Geometry* Cube(void);
    Geometry* Sphere(unsigned int sectors, unsigned int stacks);
    Geometry* Line(void);
};

#endif /* _PRIMITIVES_H */