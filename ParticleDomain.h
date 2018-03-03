
#ifndef sph_ParticleDomain_H_
#define sph_ParticleDomain_H_

#include "Wall.h"
#include "ParticleGrid.h"
#include "basicDefinitions.h"



#define SCENARIO_DAM 1
#define SCENARIO_CUBE 2
#define SCENARIO_FAUCET 3

namespace sph{

  template<typename T_dat>
  class ParticleDomain
  {
    using T_Vec3 = Vec3<T_dat>;
    using T_Wall = Wall<T_dat>;
    using T_WallVector = std::vector<T_Wall>;
    using T_Particle = Particle<T_dat>;
    using T_ParticleVector = std::vector<T_Particle>;

    constexpr static double DIAMETER             = 0.046 ; 
    constexpr static double GAS_STIFFNESS        = 3.0   ; // Nm/kg is gas constant of water vapor
    constexpr static double REST_DENSITY         = 1000  ; // kg/m^3 is rest density of water particle
    constexpr static double PARTICLE_MASS        = 0.02  ; // kg
    constexpr static double VISCOSITY            = 3.5   ; // Ns/m^2 or Pa*s viscosity of water
    constexpr static double SURFACE_TENSION      = 0.073 ; // N/m
    constexpr static double SURFACE_THRESHOLD    = 7.07  ;
    constexpr static double KERNEL_PARTICLES     = 20.0  ;
    constexpr static double GRAVITY_ACCELERATION = -9.8  ; 
    constexpr static double WALL_K               = 10000 ;
    constexpr static double WALL_DAMPING         = -0.9  ; 
    constexpr static double BOX_SIDE             = 0.4   ;
    constexpr static double MAX_PARTICLES        = 1500  ;
    constexpr static double INITIAL_SCENARIO     = SCENARIO_DAM;

  private:
    bool _isGridVisible;
    
    int _scenario ;

    double _surfaceThreshold;
    
    T_Vec3 _boxSize;
    T_Vec3 _gravityVector;
  
    ParticleGrid<T_dat>* _grid;
    T_ParticleVector* _particles;
    T_WallVector*     _walls;
    
  
  public:
    ParticleDomain();
    ~ParticleDomain();
    
    void updateGrid();
  
    void draw();

    void stepVerlet(T_dat dt);

    void switchGridVisble();
    void switchTumble();
    
    void loadScenario(int scenario);
    

  private:
    void generateFaucetParticleSet();
    void addWalls(T_Vec3 const& boxSize_, T_WallVector* wall_);
    void clear();

    void addParticle(const T_Vec3& position);
    void addParticle(const T_Vec3& position, const T_Vec3& velocity);
    void addFacucetParticle();
    void calculateAcceleration();
    void collisionForce(T_Particle& particle, T_Vec3& f_collision);

    T_dat Wpoly6(T_dat radiusSquared);
    void Wpoly6Gradient(T_Vec3& diffPosition, T_dat radiusSquared, T_Vec3& gradient);
    T_dat Wpoly6Laplacian(T_dat radiusSquared); 
    void WspikyGradient(T_Vec3& diffPosition, T_dat radiusSquared, T_Vec3& gradient);
    T_dat WviscosityLaplacian(T_dat radiusSquared);

  };
}






#include "ParticleDomain.cpp"

#endif


