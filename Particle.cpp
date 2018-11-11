#include "Particle.h"

//Particle was modified to have different draw methods for different particle types
//miscellaneous game values were also added.
Particle::Particle() {
	type = Ball;
	init();
}

Particle::Particle(ParticleType t)
{
	init();
	type = t;
	switch (type) {
	case ShipParticle:
		position.set(ofGetWindowWidth()/2, ofGetWindowHeight()/2, 0);
		lifespan = -1;
		radius = 2;
		color = ofColor(ofColor::black);
		break;
	case Bullet:
		width = 3;
		height = 15;
		break;
	case Enemy:
		position.set(500, 500);
		radius = 25;
		lifespan = 5000;
		color = ofColor::blue;
		break;
	default:
		break;
	}
	maxLife = life;
}

void Particle::init()
{
	// initialize particle with some reasonable values first;
	velocity.set(0, 0, 0);
	acceleration.set(0, 0, 0);
	position.set(0, 0, 0);
	forces.set(0, 0, 0);
	lifespan = 5;
	birthtime = 0;
	radius = 1;
	damping = .99;
	mass = 1;
	life = 1;
	points = 10;
	color = ofColor(255, 255, 255);

}



void Particle::draw() {
	drawAt(position);
//	ofSetColor(ofMap(age(), 0, lifespan, 255, 10), 0, 0);
	//ofDrawSphere(position, radius);
}

void Particle::drawAt(ofVec3f pos)
{
	switch (type) {
	case Bullet: {
		ofPushMatrix();
		ofFill();
		ofSetColor(color);
		ofTranslate(position);
		ofRotateZ(atan(velocity.x / velocity.y) * -180 / 3.1415926535);
		//terniary statement makes sure position is the tip of the bullet
		ofDrawRectangle(-width / 2, velocity.y < 0 ? 0 : -height, width, height);
		ofPopMatrix();
		break;
	}
	case Enemy: {
		ofPushMatrix();
		ofFill();
		ofSetColor(color);
		ofDrawSphere(pos, radius);
		ofPopMatrix();
		break;
	}
	case Hunter:
		ofNoFill();
		ofSetColor(color);
		ofDrawSphere(pos, radius);
		break;
	default:
		ofPushMatrix();
		ofFill();
		ofSetColor(color);
		ofDrawSphere(pos, radius);
		ofPopMatrix();
		break;
	}
}

void Particle::takeDamage() {
	life--;
}

// write your own integrator here.. (hint: it's only 3 lines of code)
//
void Particle::integrate() {

	// check for 0 framerate to avoid divide errors
	//
	float framerate = ofGetFrameRate();
	if (framerate < 1.0) return;

	// interval for this step
	//
	float dt = 1.0 / framerate;

	// update position based on velocity
	//
	position += (velocity * dt);

	// update acceleration with accumulated paritcles forces
	// remember :  (f = ma) OR (a = 1/m * f)
	//
	ofVec3f accel = acceleration;    // start with any acceleration already on the particle
	accel += (forces * (1.0 / mass));
	velocity += accel * dt;

	// add a little damping for good measure
	//
	velocity *= damping;

	if (type == Bullet)
	{
		if (position.x < 0 || position.x > ofGetWindowWidth()) velocity = ofVec3f(-velocity.x, velocity.y);
		if (position.y < 0 || position.y > ofGetWindowHeight()) velocity = ofVec3f(velocity.x, -velocity.y);
	}

	// clear forces on particle (they get re-added each step)
	//
	forces.set(0, 0, 0);
}

//  return age in seconds
//
float Particle::age() {
	return (ofGetElapsedTimeMillis() - birthtime)/1000.0;
}

bool Particle::inside(int x, int y) {
	bool result = false;
	switch (type)
	{
	case Bullet:
		break;
	default: 
		result = ofVec3f(x, y).distance(position) <= radius;
		break;
	}
	return result;
}

bool Particle::inside(ofVec3f p)
{
	return inside(p.x, p.y);
}


