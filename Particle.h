#ifndef PARTICLE_H_
#define PARTICLE_H_

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include "Vec3.h"
#include "basicDefinitions.h"

namespace sph{
  template<typename T_dat>
  class Particle
  {
    using T_Vec3 = Vec3<T_dat>;

  public:
    static size_t _count ;

  private:  
    static float _particleDrawRadius  ;

    T_Vec3 _position;
    T_Vec3 _velocity;
    T_Vec3 _force;
    T_Vec3 _acceleration;
    T_dat _density;
    T_dat _pressure;
    bool _flag;
    size_t _id;
  
  
  public:
    Particle(){};
    Particle(const T_Vec3& position_)
    : 
    _position( position_ ),
    _id{ _count++ }
    {};
  
    Particle(T_Vec3&& position_)
    : 
    _position( std::move(position_) ),
    _id{ _count++ }
    {};
  
    Particle(const T_Vec3& position_, const T_Vec3& vel_)
    : 
    _position(position_),
    _velocity(vel_),
    _id{ _count++ }
    {};
  
    Particle(T_Vec3&& position_, T_Vec3&& vel_)
    : 
    _position(std::move(position_)),
    _velocity(std::move(vel_)),
    _id{ _count++ }
    {};
  
    void draw()
    {
      if ( _flag ){
        glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
      }else{
        glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
      }
      glPushMatrix();
      glTranslated( _position[0], _position[1], _position[2] );
      glutSolidSphere(_particleDrawRadius, 9, 9);
      glPopMatrix();
    };
  
    void clearParameters(){
      _count = 0 ; 
      _position = T_Vec3();
      _velocity = T_Vec3();
      _acceleration = T_Vec3();
      _density = 0.0;
      _pressure = 0.0;
    };
    void clearForce(){_force *= 0. ;};
    void addForce(T_Vec3 other){_force += other;};
  
    T_Vec3& position(){ return _position; };
    T_Vec3& velocity(){ return _velocity; };
    T_Vec3& acceleration(){ return _acceleration;};
    T_Vec3& force(){ return _force;};
    T_dat& density() {return _density;};
    T_dat& pressure() {return _pressure;};
    bool& flag(){ return _flag;};
    size_t& id(){ return _id;};
  
  };
  
  template<typename T_dat>
  size_t Particle<T_dat>::_count = 0 ; 
  
  template<typename T_dat>
  float Particle<T_dat>::_particleDrawRadius = 0.02 ; 
}


#endif