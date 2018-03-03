
#ifndef ParticleDomain_impl_
#define ParticleDomain_impl_

#include "ParticleDomain.h"
  
namespace sph{  
  template<typename T_dat>
  ParticleDomain<T_dat>::ParticleDomain()
  : 
  _isGridVisible(false), 
  _boxSize(T_Vec3()),
  _gravityVector(0., GRAVITY_ACCELERATION, 0.),
  _grid(nullptr), 
  _particles(nullptr)
  {
	  _walls = new T_WallVector(5, T_Wall()) ; 
	  loadScenario(INITIAL_SCENARIO);
  }
  
  template<typename T_dat>
  void ParticleDomain<T_dat>::loadScenario(int newScenario){
    _scenario = newScenario;
    this->clear();
  
    T_Particle::_count = 0 ;
  
    switch( _scenario ){
  	  case SCENARIO_DAM:{
  	    _boxSize.x = BOX_SIDE * 2. ; 
  	    _boxSize.y = BOX_SIDE ; 
  	    _boxSize.z = BOX_SIDE / 2. ;
  
  	    int gridXCellMax = ceil(_boxSize.x/DIAMETER);
  	    int gridYCellMax = ceil(_boxSize.y/DIAMETER);
  	    int gridZCellMax = ceil(_boxSize.z/DIAMETER);
  
  	    _grid = new ParticleGrid<T_dat>(gridXCellMax, gridYCellMax, gridZCellMax);
  
  	    addWalls(_boxSize, _walls);
  
  	    T_ParticleVector& firstGridCell = (*_grid)(0,0,0);
  
  	    for (double x = -_boxSize.x/2.0; x < -_boxSize.x/4.0; x += DIAMETER/2.0) {
  	      for (double y = -_boxSize.y/2.0; y < _boxSize.y/2.0; y+= DIAMETER/2.0) {
  	        for (double z = -_boxSize.z/2.0; z < _boxSize.z/2.0; z+= DIAMETER/2.0) {
  	          firstGridCell.push_back(T_Particle(T_Vec3(x,y,z)));
  	        }
  	      }
  	    }
        break;
  	  }
  
  	  case SCENARIO_CUBE:{
        _boxSize.x = BOX_SIDE;
        _boxSize.y = BOX_SIDE;
        _boxSize.z = BOX_SIDE;

        int gridXCellMax = ceil(_boxSize.x/DIAMETER);
        int gridYCellMax = ceil(_boxSize.y/DIAMETER);
        int gridZCellMax = ceil(_boxSize.z/DIAMETER);

        _grid = new ParticleGrid<T_dat>(gridXCellMax, gridYCellMax, gridZCellMax);

        addWalls(_boxSize, _walls);

        T_ParticleVector& firstGridCell = (*_grid)(0,0,0);
        for (T_dat x = -_boxSize.x/5.0; x < _boxSize.x/5.0; x += DIAMETER/2.0) {
          for (T_dat y = 0. ; y < _boxSize.y/2.0; y+= DIAMETER/2.0) {
            for (T_dat z = -_boxSize.z/2.0; z < _boxSize.z/2.0; z+= DIAMETER/2.0) {
              firstGridCell.push_back(T_Particle(T_Vec3(x,y,z)));
            }
          }
        }
        break;
  	  }
  
  	  case SCENARIO_FAUCET:{
        _boxSize.x = BOX_SIDE;
        _boxSize.y = BOX_SIDE;
        _boxSize.z = BOX_SIDE;

        int gridXCellMax = ceil(_boxSize.x/DIAMETER);
        int gridYCellMax = ceil(_boxSize.y/DIAMETER);
        int gridZCellMax = ceil(_boxSize.z/DIAMETER);

        _grid = new ParticleGrid<T_dat>(gridXCellMax, gridYCellMax, gridZCellMax);

        addWalls(_boxSize, _walls);

        break;
  	  }
  
  	  default:{
  		  printf("case/scenario is not available!\n");
  	  }
    }
  
  
  }
  
  
  
  template<typename T_dat>
  void ParticleDomain<T_dat>::addWalls(T_Vec3 const& boxSize_, T_WallVector* wall_){
    (*wall_)[0] = T_Wall(T_Vec3(0,0, 1), T_Vec3(0,0,-boxSize_.z/2.) ) ; // back
    (*wall_)[1] = T_Wall(T_Vec3(0,0,-1), T_Vec3(0,0, boxSize_.z/2.) ) ; // front
    (*wall_)[2] = T_Wall(T_Vec3( 1,0,0), T_Vec3(-boxSize_.x/2.,0,0) ) ; // left
    (*wall_)[3] = T_Wall(T_Vec3(-1,0,0), T_Vec3( boxSize_.x/2.,0,0) ) ; // right
    (*wall_)[4] = T_Wall(T_Vec3(0, 1,0), T_Vec3(0,-boxSize_.y/2.,0) ) ; // bottom
  }
  
  
  
  template<typename T_dat>
  void ParticleDomain<T_dat>::addParticle(const T_Vec3& position, const T_Vec3& velocity) {
    (*_grid)(0,0,0).push_back(T_Particle(position, velocity));
  }
  
  template<typename T_dat>
  void ParticleDomain<T_dat>::addParticle(const T_Vec3& position) {
    addParticle(position, T_Vec3());
  }
  
  template<typename T_dat>
  ParticleDomain<T_dat>::~ParticleDomain()
  {
    this->clear();
  }
  
  template<typename T_dat>
  void ParticleDomain<T_dat>::clear(){
	  if (_grid) {delete _grid;}
	  if (_particles){delete _particles;}
  }
  
  
  template<typename T_dat>
  void ParticleDomain<T_dat>::switchGridVisble() {
    _isGridVisible = !_isGridVisible;
  }
  
  
  
  // After the calculation of the new particle positions, 
  // update the new grid of particles.
  template<typename T_dat>
  void ParticleDomain<T_dat>::updateGrid() {
  
    for (int referX = 0 ; referX < _grid->xCellMax() ;  referX++ ) {
      for (int referY = 0 ; referY < _grid->yCellMax() ;  referY++ ) {
        for (int referZ = 0 ; referZ < _grid->zCellMax() ;  referZ++ ) {
  
          T_ParticleVector& reference_particles = (*_grid)(referX,referY,referZ);
  
          for (auto it = reference_particles.begin(); it != reference_particles.end(); it++ ){
            
            int currentGridCellX = (int)floor( (it->position().x + _boxSize.x/2.) / DIAMETER );
            int currentGridCellY = (int)floor( (it->position().y + _boxSize.y/2.) / DIAMETER );
            int currentGridCellZ = (int)floor( (it->position().z + _boxSize.z/2.) / DIAMETER );
  
            if ( currentGridCellX < 0 ){
              currentGridCellX = 0 ; 
            }else if ( currentGridCellX >= _grid->xCellMax() ){
              currentGridCellX = _grid->xCellMax() - 1 ;
            }
  
            if ( currentGridCellY < 0 ){
              currentGridCellY = 0 ; 
            }else if ( currentGridCellY >= _grid->yCellMax() ){
              currentGridCellY = _grid->yCellMax() - 1 ;
            }
  
            if ( currentGridCellZ < 0 ){
              currentGridCellZ = 0 ; 
            }else if ( currentGridCellZ >= _grid->zCellMax() ){
              currentGridCellZ = _grid->zCellMax() - 1 ;
            }
  
  
  
            if( currentGridCellX != referX || currentGridCellY != referY || currentGridCellZ != referZ ){
              (*_grid)(currentGridCellX, currentGridCellY, currentGridCellZ).push_back(*it);
              reference_particles.erase(it--);
            }
  
          }
        }
      }
    }
  }
  
  
  
  template<typename T_dat>
  void ParticleDomain<T_dat>::draw() {

    static float shininess = 10.0f;
  
    glEnable(GL_LIGHTING);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialfv(GL_FRONT, GL_SHININESS, &shininess);
  
    auto particles_in_all_cells = _grid->allParticles( ) ;
  
    for (auto particles_in_one_cell = particles_in_all_cells->begin() ; 
         particles_in_one_cell != particles_in_all_cells->end() ; 
         particles_in_one_cell++ 
    ) {
      for( auto particle = particles_in_one_cell->begin(); particle != particles_in_one_cell->end(); particle++){
        particle->draw();
      }
    }
  
    glDisable(GL_LIGHTING);
  
    if ( _isGridVisible ) {
      glColor3fv(green);
      
      for (int referX = 0 ; referX < _grid->xCellMax() ;  referX++ ) {
        for (int referY = 0 ; referY < _grid->yCellMax() ;  referY++ ) {
          for (int referZ = 0 ; referZ < _grid->zCellMax() ;  referZ++ ) {
            glPushMatrix();
            glTranslated(referX*DIAMETER - _boxSize.x/2.0 + DIAMETER/2.0, 
                         referY*DIAMETER - _boxSize.y/2.0 + DIAMETER/2.0, 
                         referZ*DIAMETER - _boxSize.z/2.0 + DIAMETER/2.0);
            glutWireCube(DIAMETER);
            glPopMatrix();
          }
        }
      }
    }
  
    glColor3fv(black);
    glPopMatrix();
    glScaled(_boxSize.x, _boxSize.y, _boxSize.z);
    glutWireCube(1.);
    glPopMatrix();
  
    return;
  }
  
  
  
  template<typename T_dat>
  void ParticleDomain<T_dat>::stepVerlet(T_dat dt){

    this->calculateAcceleration();

    auto particles_in_all_cells = _grid->allParticles( ) ;
    for (auto particles_in_one_cell = particles_in_all_cells->begin() ; 
         particles_in_one_cell != particles_in_all_cells->end() ; 
         particles_in_one_cell++ 
    ) {

      for( auto particle = particles_in_one_cell->begin(); particle != particles_in_one_cell->end(); particle++){
  
        auto currentPosition = particle->position() + particle->velocity()*dt + particle->acceleration()*dt*dt;
        auto currentVelocity = ( currentPosition - particle->position()) / dt; 
  
        particle->position() = currentPosition ; 
        particle->velocity() = currentVelocity ; 
      }
    }
    if (_scenario == SCENARIO_FAUCET && T_Particle::_count < MAX_PARTICLES ){
      this->addFacucetParticle();
    }

  
    this->updateGrid();
  }
  
  
  template<typename T_dat>
  void ParticleDomain<T_dat>::addFacucetParticle(){
    static T_Vec3 initialVelocity(-1,-1,0);

    addParticle(T_Vec3(BOX_SIDE/2.0-DIAMETER/2.0, BOX_SIDE+DIAMETER* 0.6, 0), initialVelocity);
    addParticle(T_Vec3(BOX_SIDE/2.0-DIAMETER/2.0, BOX_SIDE              , 0), initialVelocity);
    addParticle(T_Vec3(BOX_SIDE/2.0-DIAMETER/2.0, BOX_SIDE+DIAMETER*-0.6, 0), initialVelocity);
    
    addParticle(T_Vec3(BOX_SIDE/2.0-DIAMETER/2.0, BOX_SIDE+DIAMETER*0.3, DIAMETER* 0.6), initialVelocity);
    addParticle(T_Vec3(BOX_SIDE/2.0-DIAMETER/2.0, BOX_SIDE+DIAMETER*0.3, DIAMETER*-0.6), initialVelocity);
    
    addParticle(T_Vec3(BOX_SIDE/2.0-DIAMETER/2.0, BOX_SIDE+DIAMETER*-0.3, DIAMETER* 0.6), initialVelocity);
    addParticle(T_Vec3(BOX_SIDE/2.0-DIAMETER/2.0, BOX_SIDE+DIAMETER*-0.3, DIAMETER*-0.6), initialVelocity);
  }

  
  // *************************************************************
  // Go over 27 neighbor cells to update acceleration, etc.
  // *************************************************************
  template<typename T_dat>
  void ParticleDomain<T_dat>::calculateAcceleration(){
    // ********************************************
    // Step 1. Update Density and Pressure
    // ********************************************
    // Iterate over cells
    for (int referX = 0 ; referX < _grid->xCellMax() ;  referX++ ) {
      for (int referY = 0 ; referY < _grid->yCellMax() ;  referY++ ) {
        for (int referZ = 0 ; referZ < _grid->zCellMax() ;  referZ++ ) {
  
          T_ParticleVector& reference_particles = (*_grid)(referX,referY,referZ);
  
          for (auto it = reference_particles.begin(); it != reference_particles.end(); it++ ){
  
            T_Particle& particle = *it ; 
  
            particle.density() = 0. ; 
  
            // Iterate over neighbor cells 
            for (int offsetX = -1; offsetX <= 1; offsetX++) {
              if (referX+offsetX < 0) {continue;}
              else if (referX+offsetX >= _grid->xCellMax()) {break;}
              else{
  
                for (int offsetY = -1; offsetY <= 1; offsetY++) {
                  if (referY+offsetY < 0) {continue;}
                  else if (referY+offsetY >= _grid->yCellMax()) {break;}
                  else{
    
                    for (int offsetZ = -1; offsetZ <= 1; offsetZ++) {
                      if (referZ+offsetZ < 0) {continue;}
                      else if (referZ+offsetZ >= _grid->zCellMax()) {break;}
                      else{
                        
                        T_ParticleVector& neighborParticles = (*_grid)(referX+offsetX, referY+offsetY, referZ+offsetZ);
  
                        for ( auto neighbor = neighborParticles.begin() ; neighbor != neighborParticles.end() ; neighbor++ ){
                          
                          auto diffPosition = particle.position() - neighbor->position() ;
  
                          double radiusSquared = diffPosition.dot(diffPosition);
  
                          if ( radiusSquared <= DIAMETER * DIAMETER){
                            particle.density() += Wpoly6( radiusSquared );
                          }
                        }
                      }
                    }
                  }
                }
              }
            } // end step 1 iterate over neighbors.
            
            particle.density() *= PARTICLE_MASS ; 
  
            particle.pressure() = GAS_STIFFNESS * ( particle.density() - REST_DENSITY ); 
  
          }
        }
      }
    } // End Step 1 Iterate over cells
  
    // ********************************************
    // Step 2. Update Forces
    // ********************************************
    // Iterate over cells
    for (int referX = 0 ; referX < _grid->xCellMax() ;  referX++ ) {
      for (int referY = 0 ; referY < _grid->yCellMax() ;  referY++ ) {
        for (int referZ = 0 ; referZ < _grid->zCellMax() ;  referZ++ ) {
  
          T_ParticleVector& reference_particles = (*_grid)(referX,referY,referZ) ;
  
          for (auto it = reference_particles.begin(); it != reference_particles.end(); it++ ){
  
            T_Particle& particle = *it ; 
  
            T_Vec3 f_pressure,
            f_viscosity,
            f_surface,
            f_gravity = _gravityVector * particle.density(),
            colorFieldNormal ; 
  
            double colorFieldLaplacian = 0. ;
  
            // Iterate over neighbor cells 
            for (int offsetX = -1; offsetX <= 1; offsetX++) {
              if (referX+offsetX < 0) {continue;}
              else if (referX+offsetX >= _grid->xCellMax()) {break;}
              else{
  
                for (int offsetY = -1; offsetY <= 1; offsetY++) {
                  if (referY+offsetY < 0) {continue;}
                  else if (referY+offsetY >= _grid->yCellMax()) {break;}
                  else{
            
                    for (int offsetZ = -1; offsetZ <= 1; offsetZ++) {
                      if (referZ+offsetZ < 0) {continue;}
                      else if (referZ+offsetZ >= _grid->zCellMax()) {break;}
                      else{
  
                        T_ParticleVector& neighborParticles = (*_grid)(referX+offsetX, referY+offsetY, referZ+offsetZ);
  
                        for ( auto neighbor = neighborParticles.begin() ; neighbor != neighborParticles.end() ; neighbor++ ){
  
                          auto diffPosition = particle.position() - neighbor->position() ;
                          double radiusSquared = diffPosition.dot(diffPosition);
  
                          if ( radiusSquared <= DIAMETER*DIAMETER ){
                            T_Vec3 poly6Gradient, spikyGradient;
                            
                            Wpoly6Gradient(diffPosition, radiusSquared, poly6Gradient);
                            
                            WspikyGradient(diffPosition, radiusSquared, spikyGradient);
  
                            if (particle.id() != neighbor->id()) {
                              
                              f_pressure += (particle.pressure()/pow(particle.density(),2)+neighbor->pressure()/pow(neighbor->density(),2))*spikyGradient;
                              
                              f_viscosity += (neighbor->velocity() - particle.velocity()) * WviscosityLaplacian(radiusSquared) / neighbor->density();
  
                            }
  
                            colorFieldNormal += poly6Gradient / neighbor->density();
                            
                            colorFieldLaplacian += Wpoly6Laplacian(radiusSquared) / neighbor->density();
  
                          }
                        } 
                      }
                    }
                  }
                }
              }
            } // End step 2 Iterate over neighbor cells 
  
  
            f_pressure *= -PARTICLE_MASS * particle.density();
            
            f_viscosity *= VISCOSITY * PARTICLE_MASS;
            
            colorFieldNormal *= PARTICLE_MASS;
            
            colorFieldLaplacian *= PARTICLE_MASS;
  
  
            // surface tension force
  
            double colorFieldNormalMagnitude = colorFieldNormal.magnitude();
            
            if (colorFieldNormalMagnitude > SURFACE_THRESHOLD) {
              particle.flag() = true;
              f_surface = -SURFACE_TENSION * colorFieldNormal / colorFieldNormalMagnitude * colorFieldLaplacian;
            }
            else {
              particle.flag() = false;
            }
  
  
            // force and acceleration
            particle.acceleration() = (f_pressure + f_viscosity + f_surface + f_gravity) / particle.density();
            
            
            T_Vec3 f_collision; 
            collisionForce(particle, f_collision);
          }
        }
      }
    } // End step 2 Iterate over cells
    return;
  }
  
  
  template <typename T_dat>
  void ParticleDomain<T_dat>::collisionForce(T_Particle& particle, T_Vec3& f_collision){
    
    for (auto it = _walls->begin(); it != _walls->end(); it++ ){
      T_Wall& wall = *it ; 
  
      double d = ( wall.point() - particle.position()).dot(wall.normal()) + 0.01 ; 
  
      if ( d > 0. ){
        particle.acceleration() += WALL_K * wall.normal() * d ;
        particle.acceleration() += WALL_DAMPING * particle.velocity().dot(wall.normal()) * wall.normal();
      }
    }
  
  }
  
  
  template <typename T_dat>
  T_dat ParticleDomain<T_dat>::Wpoly6(T_dat radiusSquared) { 
      
    static T_dat coefficient = 315.0/(64.0*M_PI*pow(DIAMETER,9));
    static T_dat hSquared = DIAMETER*DIAMETER;
    
    return coefficient * pow(hSquared-radiusSquared, 3);
  }
  
  
  template <typename T_dat>
  void ParticleDomain<T_dat>::Wpoly6Gradient(T_Vec3& diffPosition, T_dat radiusSquared, T_Vec3& gradient) {  
      
    static T_dat coefficient = -945.0/(32.0*M_PI*pow(DIAMETER,9));
    static T_dat hSquared = DIAMETER * DIAMETER ; 
      
    gradient = coefficient * pow(hSquared-radiusSquared, 2) * diffPosition;
  }
  
  template <typename T_dat>
  T_dat ParticleDomain<T_dat>::Wpoly6Laplacian(T_dat radiusSquared) {
      
    static T_dat coefficient = -945.0/(32.0*M_PI*pow(DIAMETER,9));
    static T_dat hSquared = DIAMETER*DIAMETER;
      
    return coefficient * (hSquared-radiusSquared) * (3.0*hSquared - 7.0*radiusSquared);
  }
  
  template <typename T_dat>
  void ParticleDomain<T_dat>::WspikyGradient(T_Vec3& diffPosition, T_dat radiusSquared, T_Vec3& gradient) {
    
    static T_dat coefficient = -45.0/(M_PI*pow(DIAMETER,6));
    
    T_dat radius = sqrt(radiusSquared);
    
    gradient = coefficient * pow(DIAMETER-radius, 2) * diffPosition / radius;
  }
   
  
  template <typename T_dat>
  T_dat ParticleDomain<T_dat>::WviscosityLaplacian(T_dat radiusSquared) {  
    
    static T_dat coefficient = 45.0/(M_PI*pow(DIAMETER,6));
    
    T_dat radius = sqrt(radiusSquared);
    
    return coefficient * (DIAMETER - radius);    
  }
  
} // End namespace sph
  
  
  
#endif
  
  
