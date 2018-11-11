#pragma once

#include "ofMain.h"
#include "ParticleSystem.h"
#include "ParticleEmitter.h"

class Ship : public TransformObject {
public:
	Ship();

	Particle shipParticle;
	ParticleSystem * shipSys;
	ThrusterForce * tForce;
	ParticleEmitter bulletEmitter;
	ParticleEmitter thrusterEmitter;
	ofVec3f position;
	ofVec3f lastMousePos;
	ofVec3f p1;
	ofVec3f p2;
	ofVec3f p3;
	ofVec3f p4;

	float width;
	float height;
	float shipSpeed;
	bool boost;
	float rotationSpeed; //degrees per frame
	float targetRotationAngle;
	float bulletSpeed;

	int life;
	int flashCount;
	bool tookDamage;

	ofSoundPlayer powerup;
	ofSoundPlayer pew;

	ofColor color;
	
	void reset();

	void setThrustDir(ofVec3f direction);
	void setTargetRotAngle(ofVec3f);

	void updateRotationAngle();

	void takeDamage();

	void draw();
	void drawShip();
	void update();
	void setupBulletEmitter();
	void setupThrusterEmitter();
};
