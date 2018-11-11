#pragma once

#include "ofMain.h"

class ParticleForceField;

typedef enum {Ball, Bullet, Enemy, Hunter, ShipParticle} ParticleType;

class Particle {
public:
	Particle();
	Particle(ParticleType t);

	ofVec3f position;
	ofVec3f velocity;
	ofVec3f acceleration;
	ofVec3f forces;
	float	damping;
	float   mass;
	float   lifespan;
	float   radius;
	float   birthtime;
	float	width;
	float	height;
	ParticleType type;

	int life;
	int maxLife;
	int points;

	void	init();
	void	takeDamage();
	void    integrate();
	void    draw();
	void	drawAt(ofVec3f pos);
	float   age();        // sec
	bool	inside(int x, int y);
	bool	inside(ofVec3f p);
	ofColor color;
};

