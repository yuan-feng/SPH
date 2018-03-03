#ifndef WALL_H_sph_
#define WALL_H_sph_


#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include "Particle.h"
#include "Vec3.h"

namespace sph{
  template<typename T_dat> 
  class Wall
  {
    using T_Vec3 = Vec3<T_dat>;
  private:
    T_Vec3 _normal;
    T_Vec3 _point;
    constexpr static float _thickness = 0.03;
  
  public:
    Wall(){};
    Wall(T_Vec3 const& normal, T_Vec3 const& point)
    : _normal(normal), _point(point)
    {
      _normal.normalize();
    } ;
  
    void draw(){
      glPushMatrix();
      glTranslated(_point[0], _point[1], _point[2]);
      double angle1 = asin(_normal[0]) / (2 * M_PI) * 360.;
      double angle2 = asin(_normal[1]) / (2 * M_PI) * 360.;
      glRotatef(-angle1, 0, 1, 0);
      glRotatef(-angle2, 1, 0, 0);
      glTranslated(0, 0, _thickness/2.);
      glScalef(20,20,1);
      glutSolidCube(_thickness);
      glPopMatrix();
    };
  
    T_Vec3& normal(){return _normal;};
    T_Vec3& point(){return _point;};
  };

}



#endif