

#include "Ship.h"

/*
	Contains everything to do with the player. 
	From the ship itself, to the projectiles it fires, and the thruster exhaust
	particles. 
*/
//Lok Man Chu - CS 134 2018
Ship::Ship()
{
	shipParticle = Particle(ShipParticle);
	shipSys = new ParticleSystem();
	tForce = new ThrusterForce(ofVec3f(0, 0, 0));
	shipSys->add(shipParticle);
	shipSys->addForce(tForce);
	setupBulletEmitter();
	setupThrusterEmitter();

	
	//Ship Values
	p1 = ofVec3f(shipSys->particles.at(0).position + ofVec3f(0, -height / 2, 0));
	p2 = p1 + ofVec3f(-width / 2, height);
	p3 = p1 + ofVec3f(width / 2, height);

	width = 20;
	height = 40;
	shipSpeed = 100;
	boost = false;
	rotationSpeed = 1;
	bulletSpeed = bulletEmitter.velocity.distance(position);
	rotation = 0;
	targetRotationAngle = 0;

	life = 3;
	tookDamage = false;

	powerup.load("sound/powerup.mp3");
	powerup.setMultiPlay(true);
	pew.load("sound/pew.mp3");
	pew.setMultiPlay(true);

	color = ofColor(255, 255, 255);
}

//resets ship to game start values
void Ship::reset()
{
	shipSys->particles.at(0).position.set(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2, 0);
	shipSys->particles.at(0).velocity.set(0, 0, 0);
	shipSpeed = 100;
	rotationSpeed = 1;
	rotation = 0;
	targetRotationAngle = 0;
	life = 3;
	bulletEmitter.sys->removeAll();
	thrusterEmitter.sys->removeAll();
	setupBulletEmitter();
}


//wasd control scheme with thruster/boost logic
void Ship::setThrustDir(ofVec3f direction)
{
	ofVec3f dir = direction.getNormalized().rotate(rotation, ofVec3f(0, 0, 1));
	//Go half as fast not going forward
	if (boost)
	{
		tForce->set(ofVec3f(0, -1, 0).rotate(rotation, ofVec3f(0, 0, 1)) * shipSpeed * 2);
	}
	else
	{
		tForce->set(dir * shipSpeed / (direction.y == -1 ? 1 : 2));
	}
	//cout << direction << endl;
	thrusterEmitter.rate = boost ? 40 : 20;
	(direction.y == -1 || boost) ? thrusterEmitter.start() : thrusterEmitter.stop();
	rotationSpeed = direction.y == -1 ? 1 : 2;
}

//sets rotation target of ship to where mouse pointer is
void Ship::setTargetRotAngle(ofVec3f pos)
{
	ofVec3f v1 = ofVec3f(0, -1, 0);
	ofVec3f v2 = (pos - shipSys->particles.at(0).position).normalize();
	float angle = atan2(v2.y, v2.x) - atan2(v1.y, v1.x);
	angle *= 180 / 3.1415926535;
	if (angle < 0) {
		angle += 360;
	}
	targetRotationAngle = angle;
}


//turns the ship towards mouse
void Ship::updateRotationAngle()
{
	float angleDiff = targetRotationAngle - rotation;
	rotation += (angleDiff > 0 ? 1:-1) * (abs(angleDiff) > 180 ? -1 : 1) * rotationSpeed;
	rotation += (rotation < 0 ? 360 : 0);
	rotation = fmod(rotation, 360);
}

void Ship::takeDamage()
{
	if (tookDamage){
		return;
	}
	else {
		life--;
		flashCount = 0;
		tookDamage = true;
	}
}


void Ship::draw()
{
	if (life >= 1)
	{
		drawShip();
		thrusterEmitter.draw();
	}
	bulletEmitter.draw();
}

void Ship::drawShip() {
	if (tookDamage)
	{
		flashCount++;
		if (flashCount % 20 < 10)
		{
			return;
		}
	}
	ofSetColor(color);
	ofPushMatrix();
	ofTranslate(shipSys->particles.at(0).position.x, shipSys->particles.at(0).position.y);
	ofRotateZ(rotation);
	ofVec3f position = ofVec3f(0, -height / 2, 0);

	ofSetColor(0, 0, 0, 255);
	ofNoFill();
	ofDrawTriangle(position.x, position.y + 30, position.x - width / 4 - 1, position.y + height + 1, position.x + width / 4 + 1, position.y + height + 1);

	ofFill();
	ofSetColor(255, 255, 255, 255);
	ofDrawTriangle(position.x, position.y + 30, position.x - width / 4, position.y + height, position.x + width / 4, position.y + height);
	ofDrawTriangle(position.x, position.y, position.x - width / 2, position.y + height - 5, position.x + width / 2, position.y + height - 5);

	ofSetColor(0, 0, 0, 255);
	ofNoFill();
	ofDrawTriangle(position.x, position.y, position.x - width / 2, position.y + height - 5, position.x + width / 2, position.y + height - 5);

	shipSys->particles.at(0).drawAt(ofVec3f(0, 0, 0));
	ofPopMatrix();
}

void Ship::update()
{
	if (flashCount >= 120)
	{
		tookDamage = false;
	}

	//Rotates ship towards mouse
	setTargetRotAngle(lastMousePos);
	updateRotationAngle();
	ofVec3f pos = shipSys->particles.at(0).position;
	p1 = (pos + ofVec3f(0, -height / 2, 0)).rotate(rotation, pos, ofVec3f(0, 0, 1));
	p2 = (pos - p1) * 2 + p1;
	ofVec2f p = ofVec2f(pos - p1).perpendicular().normalize() * width / 2;
	p3 = p2 - p;
	p4 = p2 + p;
	//Moves shipParticle

	ofVec3f v = shipSys->particles.at(0).velocity;
	if (pos.x < 0 || pos.x > ofGetWindowWidth()) v = ofVec3f(-v.x, v.y);
	if (pos.y < 0 || pos.y > ofGetWindowHeight()) v = ofVec3f(v.x, -v.y);
	shipSys->particles.at(0).velocity = v;
	shipSys->update();

	//Sets bulletEmitter's position relative to ship
	//(15-5) is bulletHeight - value to make it seem like ship is shooting the bullet
	bulletEmitter.setPosition((p1 - pos).normalize()*(15 - 5) + p1);

	//Sets bullet direction
	bulletEmitter.velocity = ofVec3f(0, -bulletSpeed, 0).rotate(rotation, ofVec3f(0, 0, 1));
	if ((ofGetElapsedTimeMillis() - bulletEmitter.lastSpawned) > (1000.0 / bulletEmitter.rate) && bulletEmitter.started)
	{
		pew.play();
	}
	bulletEmitter.update();

	thrusterEmitter.velocity = ofVec3f(0, 1, 0).rotate(rotation, ofVec3f(0, 0, 1));
	thrusterEmitter.setPosition(p2);
	thrusterEmitter.update();
}

void Ship::setupBulletEmitter() {
	bulletEmitter.init();
	bulletEmitter.visible = false;
	bulletEmitter.oneShot = false;
	bulletEmitter.setEmitterType(DirectionalEmitter);
	bulletEmitter.setPosition(position);
	bulletEmitter.particleType = Bullet;
	bulletEmitter.particleRadius = 1.5;
	bulletEmitter.particleColor = ofColor::red;
	bulletEmitter.velocity = ofVec3f(0, -400, 0);
	bulletEmitter.lifespan = 5000;
	bulletEmitter.damping = 1;
	bulletEmitter.rate = 1;
	bulletEmitter.groupSize = 1;
}

void Ship::setupThrusterEmitter() {
	thrusterEmitter.init();
	thrusterEmitter.visible = false;
	thrusterEmitter.oneShot = false;
	thrusterEmitter.setEmitterType(DiscEmitter);
	thrusterEmitter.setPosition(position);
	thrusterEmitter.particleType = Ball;
	thrusterEmitter.particleColor = ofColor::fireBrick;
	thrusterEmitter.velocity = ofVec3f(0, 50, 0);
	thrusterEmitter.lifespan = 2;
	thrusterEmitter.damping = .99;
	thrusterEmitter.particleRadius = 1.5;
	thrusterEmitter.groupSize = 3;
	thrusterEmitter.randomLife = true;
	thrusterEmitter.lifeMinMax = ofVec3f(.25, .5);
	thrusterEmitter.rate = 20;

	thrusterEmitter.sys->addForce(new TurbulenceForce(ofVec3f(-5,-5,0), ofVec3f(5,5,0)));
}