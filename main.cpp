
#include <cstdlib>
#include <sys/time.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include "./glvu/glvu.h"

#include "Wall.h"
#include "Particle.h"
#include "ParticleDomain.h"
#include <vector>

#include "snapshot.h"

using std::cout;
using std::endl;

// GUI
GLVU glvu;

sph::ParticleDomain<double> *particleSystem;

double dt = 1.0 / 100.0;
bool animate = false;


void keyboardCallback(unsigned char key, int x, int y)
{
  switch (key)
  {
    case 'q':{
      exit(0);
      break;
    }
    case 'a':{
      animate = !animate;
      break;
    }
    case ' ':{
      particleSystem->stepVerlet(dt);
      glutPostRedisplay();
      // SnapShot(); // Optional save snapshot to snap*.ppm
      break;
    }
    case 'g':{
      particleSystem->switchGridVisble();
      break;
    }
      
    case '1':{
      particleSystem->loadScenario(SCENARIO_DAM);
      break;
    }
    case '2':{
      particleSystem->loadScenario(SCENARIO_CUBE);
      break;
    }
    case '3':{
      particleSystem->loadScenario(SCENARIO_FAUCET);
      break;
    }

    default:{
      printf("input is not recognized ! \n" );
    }
  }
  
  glvu.Keyboard(key, x, y);
  
  glutPostRedisplay();
  
}





void glutMouseClick(int button, int state, int x, int y){
  glvu.Mouse(button,state,x,y);
}

void glutMouseMotion(int x, int y){
  glvu.Motion(x,y);
  glvuVec3f viewVector = glvu.GetCurrentCam()->Y;
}

void displayCallback(){
  glvu.BeginFrame();
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
    
  particleSystem->draw();

  glutSwapBuffers();
  
  glvu.EndFrame();
}

void idleCallback(){
  if (!animate) return;
  particleSystem->stepVerlet(dt);
  glutPostRedisplay();
}











int main(int argc, char** argv)
{
  char title[] = "Smoothed Particle Hydrodynamics Demo ";

  glutInit(&argc, argv);
  glvu.Init(title, GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH, 0, 0, 800, 800);
  glShadeModel(GL_SMOOTH);
  
  glvu.SetInertiaEnabled(0);

  // point GLUT to our callback functions
  glutDisplayFunc(displayCallback); 
  glutIdleFunc(idleCallback); 
  glutKeyboardFunc(keyboardCallback);
  glutMouseFunc(glutMouseClick);
  glutMotionFunc(glutMouseMotion);

  // set background to black
  glClearColor(1.0, 1.0, 1.0, 1.0);

  // enable lights
  GLfloat ambient[] = {0.7,0.7,0.7};
  GLfloat diffuse[] = {1.0,1.0,1.0};
  GLfloat specular[] = {0.0, 0.0, 0.0};

  glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  
  glvuVec3f ModelMin(-10,-10,-10), ModelMax(10,10,10), 
  Eye(0, 0, 1.5),  LookAtCntr(0, 0, 0),  Up(0, 1, 0);
  
  float Yfov = 45;
  float Aspect = 1;
  float Near = 0.001f;
  float Far = 10.0f;
  glvu.SetAllCams(ModelMin, ModelMax, Eye, LookAtCntr, Up, Yfov, Aspect, Near, Far);
  
  glvuVec3f center(0.0, 0.0, 0.0);
  glvu.SetWorldCenter(center);
  
  particleSystem = new sph::ParticleDomain<double>();

  glutMainLoop();
  
  return 0;
}
