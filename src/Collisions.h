/*
 The Disarray 
 by jrs0ul(jrs0ul ^at^ gmail ^dot^ com) 2010
 -------------------------------------------
 Collision stuff
 mod. 2011.08.31
 */

#ifndef COLLISIONS_H
#define COLLISIONS_H

#include "Vectors.h"


struct Rectangle_{

    float x;
    float y;
    float width;
    float height;

    Rectangle_(float nx, float ny, float w, float h)
                {x = nx; y = ny; width = w; height = h;}
    Rectangle_(){};
};


bool    RectanglesColide(Rectangle_ & a, Rectangle_ & b);

bool    CirclesColide(float x1,float y1,float radius1, float x2, float y2, float radius2);

bool    CollisionSphere_Sphere(Vector3D pos1, float radius1,
                               Vector3D pos2, float radius2);
int     CollisionRay_Traingle( Vector3D rayOrigin, Vector3D rayDir,
                              Vector3D vert0, Vector3D vert1,
                              Vector3D vert2, float &t);

float       CalcFrustumSphereRadius(float near, float far, float fov);
Vector3D    CalcFrustumSphereCenter(float near, float far,
                                    Vector3D camPos, Vector3D camLook);

bool    CollisionRay_AABB(Vector3D rayorigin, Vector3D raydir, Vector3D min, Vector3D max, float t0, float t1);

bool    CollisionLine_Sphere (Vector3D scenter, float radius, Vector3D begin, Vector3D end);

bool    CollisionSphere_Triangle(Vector3D sCenter, float radius, 
                                 Vector3D v0, Vector3D v1, Vector3D v2);

bool    CollisionTriangularPrism_Sphere(Vector3D* vertices, Vector3D sphereCenter, float radius);



#endif //COLLISIONS_H
