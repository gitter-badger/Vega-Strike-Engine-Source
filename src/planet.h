/* planet.h
ok * 
 * Unit type that is a gravity source, and follows a set orbit pattern
 */
#ifndef _PLANET_H_
#define _PLANET_H_

#include <stdio.h>

#include "cmd_unit.h"
#include "cmd_ai.h"
#include "physics.h"
#include "UnitCollection.h"
#include "iterator.h"

/* Orbits in the xy plane with the given radius. Depends on a reorientation of coordinate bases */

struct Texture;
class Planet;
typedef float Matrix[16];

class PlanetaryOrbit : public AI {
 private:
  Unit *parent;
  double velocity;
  double theta;

  Vector x_size;
  Vector y_size;
  Vector focus;

 public:
  PlanetaryOrbit(Unit *p, double velocity, double initpos, const Vector &x_axis, const Vector &y_axis) : parent(p), velocity(velocity), theta(initpos), x_size(x_axis), y_size(y_axis) { 
    double delta = x_size.Magnitude() - y_size.Magnitude();
    if(delta == 0) {
      focus = Vector(0,0,0);
    }
    else if(delta>0) {
      focus = x_size*(delta/x_size.Magnitude());
    } else {
      focus = y_size*(-delta/y_size.Magnitude());
    }
  }
  AI *Execute();

  friend class Unit;
};

class Planet : public Unit {
 private:
  float radius;
  float gravity;

  Unit **satellites;
  int numSatellites;

 public:
  Planet();
  void endElement();
  void beginElement(Vector x,Vector y,float vely,float velx,float gravity,float radius,char * filename,int level,bool isunit=false);
  Planet(Vector x,Vector y,float vely,float velx,float gravity,float radius,char * filename);
  ~Planet();
  virtual enum clsptr isUnit() {return PLANETPTR;}

//  void InitPlanet(FILE *fp);

  void gravitate(UnitCollection *units);

  class PlanetIterator : public Iterator {
    UnitCollection planetStack;
    Iterator *pos;
  public:
    PlanetIterator(Planet *p) : planetStack(true) { 
      planetStack.append(p);
      pos = planetStack.createIterator();
    }
    virtual ~PlanetIterator() {
      delete pos;
    }
    virtual void preinsert(Unit *unit) {
      abort();
    }
    virtual void postinsert(Unit *unit) {
      abort();
    }
    virtual void remove() {
      abort();
    }
    virtual Unit *current() {
      return pos->current();
    }
    virtual Unit *advance() {
      if(pos->current()==NULL)
	return NULL;
	
      Unit *currentPlanet = pos->current();
	  if (currentPlanet->isUnit()==PLANETPTR) {
	    for(int a=0; a<((Planet*)currentPlanet)->numSatellites; a++) {
			planetStack.append(((Planet *)currentPlanet)->satellites[a]);
		}
	  }
      return pos->advance();
    }
  };
  Iterator *createIterator() { return new PlanetIterator(this);}

  friend class Planet::PlanetIterator;
  friend class PlanetaryOrbit;
};

#endif

