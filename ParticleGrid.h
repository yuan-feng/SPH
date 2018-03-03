#ifndef ParticleGrid_H_sph
#define ParticleGrid_H_sph

#include <cstdlib>
#include <iostream>
#include <vector>
#include "Particle.h"

namespace sph{
  template<typename T_dat>
  class ParticleGrid
  {
    using ParticleVector = std::vector<Particle<T_dat>>;    // Particles in one cell
    using ParticleVectorGrid = std::vector<ParticleVector>; // Particles in all cells

  private:
    int _xCellMax;
    int _yCellMax;
    int _zCellMax;
    int _totalCellMax;
    ParticleVectorGrid* _particlesGrid ;
  
  public:
    ParticleGrid()
    : _xCellMax(0), _yCellMax(0), _zCellMax(0)
    , _totalCellMax(0), _particlesGrid{nullptr}
    {};
  
    ParticleGrid(int xCellMax, int yCellMax, int zCellMax)
    : _xCellMax(xCellMax), _yCellMax(yCellMax)
    , _zCellMax(zCellMax), _totalCellMax(xCellMax*yCellMax*zCellMax)
    {
      _particlesGrid = new ParticleVectorGrid( _totalCellMax , ParticleVector() );
    };
  
    ~ParticleGrid(){
      if ( _particlesGrid ){ delete _particlesGrid;}
    };
  
    inline ParticleVector& operator()( int x, int y, int z){
  	  return (*_particlesGrid)[ x + y * _xCellMax + z * _xCellMax * _yCellMax];
    }
    
    int xCellMax(){ return _xCellMax; };
    int yCellMax(){ return _yCellMax; };
    int zCellMax(){ return _zCellMax; };
    int totalCellMax(){ return _totalCellMax; };
    ParticleVectorGrid* allParticles() const {return _particlesGrid;}
  };

}



#endif