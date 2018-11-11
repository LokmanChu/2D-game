
//  Kevin M. Smith - CS 134 SJSU

#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter() {
	sys = new ParticleSystem();
	createdSys = true;
	init();
}

ParticleEmitter::ParticleEmitter(ParticleSystem *s) {
	if (s == NULL)
	{
		cout << "fatal error: null particle system passed to ParticleEmitter()" << endl;
		ofExit();
	}
	sys = s;
	createdSys = false;
	init();
}

ParticleEmitter::~ParticleEmitter() {

	// deallocate particle system if emitter created one internally
	//
	if (createdSys) delete sys;
}

void ParticleEmitter::init() {
	rate = 1;
	velocity = ofVec3f(0, 10, 0);
	lifespan = 1;
	mass = 1;
	randomLife = false;
	lifeMinMax = ofVec3f(2, 4);
	started = false;
	oneShot = true;
	fired = false;
	lastSpawned = 0;
	radius = 5;
	particleRadius = 5;
	visible = true;
	type = DiscEmitter;
	groupSize = 15;
	damping = .99;
	particleColor = ofColor(255,255,0);
	life = 1;
	position = ofVec3f(0, 0, 0);
	particleType = Ball;
}



void ParticleEmitter::draw() {
	if (visible) {
		switch (type) {
		case DirectionalEmitter:
			ofDrawSphere(position, radius);  // just draw a small sphere for point emitters 
			break;
		case SphereEmitter:
		case RadialEmitter:
			ofDrawSphere(position, radius);  // just draw a small sphere as a placeholder
			break;
		case DiscEmitter:
			ofDrawSphere(position, radius);
			break;
		default:
			break;
		}
	}
	sys->draw();  
}
void ParticleEmitter::start() {
	if (started) return;
	started = true;
	if (oneShot){
		lastSpawned = ofGetElapsedTimeMillis();
	}
}

void ParticleEmitter::stop() {
	started = false;
	fired = false;
}

void ParticleEmitter::update() {

	float time = ofGetElapsedTimeMillis();

	if (oneShot && started) {
		if (!fired) {

			// spawn a new particle(s)
			//
			for (int i = 0; i < groupSize; i++) {
				spawn(time);
			}

			lastSpawned = time;
		}
		fired = true;
		stop();
	}

	else if (((time - lastSpawned) > (1000.0 / rate)) && started) {

		// spawn a new particle(s)
		//
		for (int i= 0; i < groupSize; i++)
			spawn(time);
	
		lastSpawned = time;
		fired = true;
	}

	sys->update();
}

// spawn a single particle.  time is current time of birth
//
void ParticleEmitter::spawn(float time) {

	Particle particle = Particle(particleType);

	// set initial velocity and position
	// based on emitter type
	//
	switch (type) {
	case RadialEmitter:
	{
		ofVec3f dir = ofVec3f(ofRandom(-1, 1), ofRandom(-1, 1));
		float speed = velocity.length();
		particle.velocity = dir.getNormalized() * ofRandom(.5 * speed, speed);
		particle.position.set(position); 
	}
	break;
	case SphereEmitter:
		break;
	case DirectionalEmitter:
		particle.velocity = velocity;
		particle.position.set(position);
		break;
	case DiscEmitter:
	{
		ofVec3f dir = velocity;
		float speed = 50;
		ofVec2f p = ofVec2f(velocity).perpendicular().normalize();
		int r = ofRandom(-4, 4);
		ofVec3f pos = p * r + dir.getNormalized() * ofRandom(0,2);
		particle.position.set(position + pos);
		particle.velocity = (dir.getNormalized() + p *(r > 0 ? ofRandom(-.25, .75): ofRandom(-.75, .25)))* speed;
	}
	break;
	}

	// other particle attributes
	//
	if (randomLife) {
		particle.lifespan = ofRandom(lifeMinMax.x, lifeMinMax.y);
	}
	else particle.lifespan = lifespan;
	particle.birthtime = time;
	particle.radius = particleRadius;
	particle.mass = mass;
	particle.damping = damping;
	particle.color = particleColor;
	particle.points = points;
	particle.maxLife = life;
	particle.life = life;


	// add to system
	//
	sys->add(particle);
}
