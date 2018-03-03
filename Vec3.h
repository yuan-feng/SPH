#ifndef Vec3_H_
#define Vec3_H_

#include <iostream>
#include <cmath>

namespace sph{

  template <typename T_dat>
  class Vec3{
  public:
    union{
      struct {T_dat x,y,z;};
      struct {T_dat r,g,b;};
      T_dat _elem[3];  
    };
    
  
    Vec3(T_dat x_=0, T_dat y_=0, T_dat z_=0)
    : x{x_}, y{y_}, z{z_}
    {}
  
    Vec3( Vec3 const& other){*this=other;}
    Vec3( Vec3 && other){*this=std::move(other);}
    Vec3& operator=( Vec3 const& other){
      _elem[0]=other._elem[0] ;
      _elem[1]=other._elem[1] ;
      _elem[2]=other._elem[2] ;
      return (*this);
    }
    Vec3& operator=( Vec3 && other){
      _elem[0]=std::move(other._elem[0]) ;
      _elem[1]=std::move(other._elem[1]) ;
      _elem[2]=std::move(other._elem[2]) ;
      return (*this);
    }
  
    Vec3& operator=( T_dat other){
      _elem[0]=other;
      _elem[1]=other;
      _elem[2]=other;
      return (*this);
    }
    operator const T_dat*() { return _elem;}
    T_dat& operator[](int id){return _elem[id];}
  
    Vec3& operator+=(const Vec3& v)     { x += v.x; y += v.y; z += v.z; return *this; }
    Vec3& operator+=(Vec3&& v)     { x += std::move(v.x); y += std::move(v.y); z += std::move(v.z); return *this; }
    Vec3& operator-=(const Vec3& v)     { x -= v.x; y -= v.y; z -= v.z; return *this; }
    Vec3& operator-=(Vec3&& v)     { x -= std::move(v.x); y -= std::move(v.y); z -= std::move(v.z); return *this; }
    Vec3& operator/=(const Vec3& v)     { x /= v.x; y /= v.y; z /= v.z; return *this; }
    Vec3& operator/=(Vec3&& v)     { x /= std::move(v.x); y /= std::move(v.y); z /= std::move(v.z); return *this; }
    Vec3& operator/=(const float b)     { x /= b; y /= b; z /= b; return *this; }
    Vec3& operator*=(const float b)     { x *= b; y *= b; z *= b;   return *this; }
    Vec3 operator+(const Vec3 &b) const { return Vec3(x+b.x,y+b.y,z+b.z); }
    Vec3 operator-(const Vec3 &b) const { return Vec3(x-b.x,y-b.y,z-b.z); }
    Vec3 operator*(const float b) const { return Vec3(x*b,y*b,z*b); }
    Vec3 operator/(const float b) const { return Vec3(x/b,y/b,z/b); }
  
    // cross product
    Vec3 operator^(const Vec3 &v) const { return Vec3(y*v.z-v.y*z,-x*v.z+v.x*z,x*v.y-v.x*y); }
  
    // dot product
    T_dat dot(const Vec3 &b) const      { return x*b.x+y*b.y+z*b.z; };
  
    // naive element-wise multiply.
    Vec3 operator*(const Vec3& v) const { return Vec3(x*v.x,y*v.y,z*v.z); }
    Vec3& operator*=(const Vec3& v)     { x *= v.x; y *= v.y; z *= v.z; return *this; }
    Vec3& operator*=(Vec3&& v)     { x *= std::move(v.x); y *= std::move(v.y); z *= std::move(v.z); return *this; }
  
    T_dat magnitude(){
      return sqrt( x*x + y*y + z*z);
    }
  
    Vec3 normalize(){
      T_dat denom = x*x + y*y + z*z;
      if (denom <= 0. ){
        printf("ERROR in Vec3.h normalize() denom<=0. denom = %f\n", denom);
      }
      else if (denom !=1.0){
        T_dat inv = 1. / sqrt(denom) ;
        (*this) *= inv ; 
      }
      return (*this);
    }
  
    Vec3 normal(){
      Vec3 a = *this;
      a.normalize();
      return a;
    }
  
    T_dat maxVal(){
      return x > y && x > z ? x : y > z ? y : z; 
    }
  
  };
  
  template<typename T_dat>
  inline std::istream &operator>>(std::istream &in, Vec3<T_dat>& v){
    return in >> v[0] >> v[1] >> v[2];
  }
  
  template<typename T_dat>
  inline std::ostream &operator<<(std::ostream &out, Vec3<T_dat>& v){
    return out << v[0] << " " << v[1] << " " << v[2] ;
  }
  
  template <typename T_dat, typename T_other>
  inline auto operator*(const T_other a, const Vec3<T_dat>& b){
    return Vec3<T_dat>( b.x*a, b.y*a, b.z*a );
  }

}


#endif