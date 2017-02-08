/*
 The Disarray 
 by jrs0ul(jrs0ul ^at^ gmail ^dot^ com) 2010
 -------------------------------------------
 Collision stuff
 mod. 2011.08.31
 */

#include "Collisions.h"
#include <cmath>
#include <cstdio>


//----------------------------------------

bool    RectanglesColide(Rectangle_ & a, Rectangle_ & b){

       if(((a.x + a.width) < b.x) ||
       (a.x > (b.x + b.width)) ||
       (a.y > (b.y + b.height)) ||
       ((a.y + a.height) < b.y)){
        /*printf("%f < %f || %f > %f || %f < %f || %f > %f\n", 
                a.x+a.width, b.x,
                a.x, b.x + b.width,
                a.y, b.y + b.height,
                a.y + a.height, b.y
                );*/
        return false;
    }else{
        puts("true");
        return true;
    }
    


}



//--------------------------------------------------------------

bool CirclesColide(float x1,float y1,float radius1, float x2, float y2, float radius2){
    
    float difx = (float) fabs (x1 - x2);
    float  dify = (float) fabs (y1 - y2);
    float   distance = (float) sqrt (difx * difx + dify * dify);
    
    if   (distance < (radius1 + radius2))
        return   true;
    
    return   false;
    
}
//-----------------------------------------------
bool    CollisionSphere_Sphere(Vector3D pos1, float radius1,
                               Vector3D pos2, float radius2){
    
    float  difx = (float) fabs (pos1.v[0] - pos2.v[0]);
    float  dify = (float) fabs (pos1.v[1] - pos2.v[1]);
    float  difz = (float) fabs (pos1.v[2] - pos2.v[2]);
    
    float   distance = (float) sqrt (difx * difx + dify * dify + difz * difz);
    
    if   (distance < (radius1 + radius2))
        return   true;
    
    return   false;
    
}
//------------------------------------------------
int CollisionRay_Traingle( Vector3D rayOrigin, Vector3D rayDir,
                          Vector3D vert0,Vector3D vert1,
                          Vector3D vert2, float &t){
    
    float u,v;
    
    Vector3D tvec, pvec, qvec;
    float det,inv_det;
    
    Vector3D edge1 = vert1 - vert0;
    Vector3D edge2 = vert2 - vert0;
    
    pvec = rayDir ^ edge2;
    
    det = edge1 * pvec;
    
    if (det > -0.000001 && det < 0.000001)
        return 0;
    inv_det = 1.0f/det;
    tvec = rayOrigin - vert0;
    u = tvec * pvec;
    u  *= inv_det;
    if (u <0.0 || u>1.0f)
        return 0;
    
    qvec = tvec ^ edge1;
    
    v = rayDir * qvec;
    v *= inv_det;
    if (v < 0.0 || u + v > 1.0)
        return 0;
    
    t = edge2 * qvec;
    t *= inv_det;
    
    return 1;
}

//-----------------------------------------------

float   CalcFrustumSphereRadius(float _near, float _far,
                                float fov){
    float viewLen = _far - _near;
    float hviewLen = viewLen * 0.5f;
    float frustumHeight = viewLen * tan((fov * 0.0174532925f) * 0.5f);
    Vector3D P(0.0f, 0.0f, _near + hviewLen);
    Vector3D Q(frustumHeight, frustumHeight, viewLen);
    Vector3D Diff = P - Q;
    
    return Diff.length();
}
//--------------------------------------------------------
Vector3D    CalcFrustumSphereCenter(float _near, float _far,
                                    Vector3D camPos, Vector3D camLook){
    
    
    float hviewLen = (_far - _near) * 0.5f;
    float distanceFromCam = hviewLen + _near;
    // calculate the center of the sphere
    Vector3D temp = Vector3D(camLook.v[0] * distanceFromCam,
                             camLook.v[1] * distanceFromCam,
                             camLook.v[2] * distanceFromCam);
    return camPos + temp;
}

//----------------------------------------------


bool CollisionRay_AABB(Vector3D rayorigin, Vector3D raydir, Vector3D min, Vector3D max, float t0, float t1){
    
    float tmin, tmax, tymax, tymin, tzmin, tzmax;
    if (raydir.x()>=0){
        tmin = (min.x() - rayorigin.x())/raydir.x();
        tmax = (max.x() - rayorigin.x())/raydir.x();
    }
    else{
        tmin = (max.x() - rayorigin.x())/raydir.x();
        tmax = (min.x() - rayorigin.x())/raydir.x();
    }
    
    if (raydir.y()>=0){
        tymin = (min.y() - rayorigin.y())/raydir.y();
        tymax = (max.y() - rayorigin.y())/raydir.y();
    }
    else{
        tymin = (max.y() - rayorigin.y())/raydir.y();
        tymax = (min.y() - rayorigin.y())/raydir.y();
    }
    if ((tmin>tymax)||(tymin>tmax))
        return false;
    
    if (tymin >tmin)
        tmin=tymin;
    if (tymax < tmax)
        tmax = tymax;
    if (raydir.z()>=0){
        tzmin = (min.z() - rayorigin.z())/raydir.z();
        tzmax = (max.z() - rayorigin.z())/raydir.z();
    }
    else{
        tzmin = (max.z() - rayorigin.z())/raydir.z();
        tzmax = (min.z() - rayorigin.z())/raydir.z();
    }
    if ((tmin>tzmax)||(tzmin > tmax))
        return false;
    if (tzmin> tmin)
        tmin = tzmin;
    if (tzmax < tmax)
        tmax=tzmax;
    //printf("%.2f %.2f %.2f %.2f\n", tmin, tmax, t1, t0);
    return ((tmin<t1)&&(tmax>t0));
    return true;
    
    
    
}

//-------------------------------------

bool CollisionLine_Sphere (Vector3D scenter, float radius, Vector3D begin, Vector3D end){
    
    float A = end.x() * end.x() + end.y() * end.y() + end.z() * end.z();
    float B = 2 * (end.x() * (begin.x() - scenter.x()) + end.y() * (begin.y() -
                                                                    scenter.y()) + end.z() * (begin.z() - scenter.z()));
    float C = pow((begin.x() - scenter.x()),2.0f) + pow((begin.y() - scenter.y()),2) +
    pow((begin.z() - scenter.z()),2.0f) - radius*radius;
    
    float D = B * B - 4 * A * C;
    if (D < 0.0)
        return false;
    else
        return true;
}


//---------------------------------------------
//vertices[0 1 2 3] for base of the prism and [4 5] for the top
bool    CollisionTriangularPrism_Sphere(Vector3D* vertices, Vector3D sphereCenter, float radius){
    
    if ((CollisionLine_Sphere(sphereCenter, radius, vertices[0], vertices[1]))||
        (CollisionLine_Sphere(sphereCenter, radius, vertices[0], vertices[2]))||
        (CollisionLine_Sphere(sphereCenter, radius, vertices[1], vertices[3]))||
        (CollisionLine_Sphere(sphereCenter, radius, vertices[0], vertices[4]))||
        (CollisionLine_Sphere(sphereCenter, radius, vertices[1], vertices[5]))||
        (CollisionLine_Sphere(sphereCenter, radius, vertices[2], vertices[3]))||
        (CollisionLine_Sphere(sphereCenter, radius, vertices[4], vertices[2]))||
        (CollisionLine_Sphere(sphereCenter, radius, vertices[5], vertices[3]))||
        (CollisionLine_Sphere(sphereCenter, radius, vertices[4], vertices[5]))){
        
        printf("%f %f %f\n", sphereCenter.v[0], sphereCenter.v[1], sphereCenter.v[2]);
        return true;
    }
    return false;
}


