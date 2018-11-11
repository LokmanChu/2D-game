#include "EnemyHandler.h"

/*
	EnemyHandler contains everything to do with enemies. From enemy emitters,
	enemy death explosions, score, and etc. Also checks for collisions between 
	the player and enemies and also player projectiles with enemies.

	Currently supports 3 enemy types. 

*/
//Lok Man Chu - CS 134 2018

EnemyHandler::EnemyHandler()
{
	setupExplosionEmitter();
	setupStraightEmitter();
	setupArcEmitter();
	setupHunterEmitter();
	started = false;
	hunterSent = true;
	lastHunterSent = 0;
	myFont.load("ARCADECLASSIC.TTF", 16);
	hunterSpawn.load("sound/found.wav");
	hunterSpawn.setMultiPlay(true);
	coin.load("sound/coin.mp3");
	coin.setMultiPlay(true);
	gameover.load("sound/gameover.mp3");
	gameover.setMultiPlay(true);
	theme.load("sound/themesong.mp3");
	theme.setLoop(true);
	score = 0;
}

//resets all values to start of game values
void EnemyHandler::reset()
{
	started = false;
	score = 0;
	hunterSent = true;
	lastHunterSent = 0;
	explosionEmitter.sys->removeAll();
	scores.clear();
	for (int i = 0; i < systems.size(); i++)
	{
		systems[i]->removeAll();
		systems[i]->removeAllForces();
	}
	setupStraightEmitter();
	setupArcEmitter();
	setupHunterEmitter();
}

void EnemyHandler::start()
{
	if (started) return;
	started = true;
}

//checks all particle systems for collision between enemies and ship
void EnemyHandler::checkShipHit(Ship * ship){
	for (int i = 0; i < systems.size(); i++)
	{
		isShipHit(ship, systems[i]);
	}
}

bool EnemyHandler::isShipHit(Ship * ship, ParticleSystem * enemySys) {
	vector<Particle>::iterator e = enemySys->particles.begin();
	vector<Particle>::iterator temp;

	while (e != enemySys->particles.end()) {
		if (e->inside(ship->p1) || e->inside(ship->p2) || e->inside(ship->p3)){
			temp = enemySys->particles.erase(e);
			e = temp;
			ship->takeDamage();
			return true;
		}
		else e++;
	}
	return false;
}

//checks all particle systems for collision between enemies and ship projectiles, 
//including explosion particles
void EnemyHandler::checkEnemiesHit(ParticleSystem * shipSys) {
	for (int i = 0; i < systems.size(); i++)
	{
		areEnemiesHit(shipSys, systems[i]);
		areEnemiesHit(explosionEmitter.sys, systems[i]);
	}
}

bool EnemyHandler::areEnemiesHit(ParticleSystem * bSys, ParticleSystem * enemySys) {
	vector<Particle>::iterator b = bSys->particles.begin();
	vector<Particle>::iterator btmp;

	bool destroy = false;
	bool hit = false;

	while (b != bSys->particles.end()) {
		vector<Particle>::iterator e = enemySys->particles.begin();
		vector<Particle>::iterator etmp;
		int i = 0;
		while (e != enemySys->particles.end()) {
			if ((e->inside(b->position))) {
				e->takeDamage();
				if (e->life <= 0)
				{
					explode(&enemySys->particles.at(i));
					score += e->points;
					etmp = enemySys->particles.erase(e);
					e = etmp;
					destroy = true;
				}
				btmp = bSys->particles.erase(b);
				hit = true;
				break;
			}
			else
			{
				e++;
				i++;
			}
		}
		if (hit)
		{
			b = btmp;
			hit = false;
		}
		else b++;

	}
	return destroy;
}


void EnemyHandler::drawEnemyEmitters() {
	straightEmitter.draw();
	arcEmitter.draw();
	arcEmitter2.draw();
	hunterEmitter.draw();
}

//logic for increasing difficulty
void EnemyHandler::updateEnemyEmitters() {
	if (score >= 0)
	{
		straightEmitter.start();
	}
	if (score >= 200)
	{
		arcEmitter.start();
		arcEmitter2.start();
	}
	if (score - lastHunterSent >= 500)
	{
		hunterSent = false;
		lastHunterSent = score / 500 * 500;
	}
	updateHunterEmitter();
	updateStraightEmitter();
	updateArcEmitter();
}


void EnemyHandler::updateStraightEmitter()
{
	int from = score < 50 ? 1 : 2;
	if (straightEmitter.fired)
	{
		straightEmitter.position = randomPositionOutside(0,from,straightEmitter.particleRadius);
		ofVec2f pos = straightEmitter.position;
		int x = pos.x < 0 ? 100 : 0 + pos.x > ofGetWindowWidth() ? -100 : 0;
		int y = pos.y < 0 ? 100 : 0 + pos.y > ofGetWindowHeight() ? -100 : 0;
		straightEmitter.velocity = ofVec3f(x, y, 0);
		straightEmitter.fired = false;
		if (score > 30)
		{
			straightEmitter.particleColor = randomColor();
		}
	}

	if (score > 0)
	{
		straightEmitter.rate = (score / 10 <= 10) ? .2 * score / 10 : 2 ;
	}
	
	straightEmitter.update();
}
void EnemyHandler::updateArcEmitter() {
	
	if (arcEmitter.fired)
	{
		arcEmitter.position = ofVec3f(ofRandomf() > 0 ? -30 : ofGetWidth() + 30, 0, 0);
		ofVec3f pos = arcEmitter.position;
		int x = pos.x <= 0 ? ofRandom(200,1000) : 0 + pos.x >= ofGetWindowWidth() ? -ofRandom(200, 1000) : 0;
		int y = ofRandom(-100,100);
		arcEmitter.velocity = ofVec3f(x, y, 0);
		arcEmitter.fired = false;
		arcEmitter.particleColor = randomColor();
	}	

	if (arcEmitter2.fired)
	{
		arcEmitter2.position = ofVec3f(ofRandomf() > 0 ? -30 : ofGetWidth() + 30, ofGetHeight(), 0);
		ofVec3f pos = arcEmitter2.position;
		int x = pos.x <= 0 ? ofRandom(200, 1000) : 0 + pos.x >= ofGetWindowWidth() ? -ofRandom(200, 1000) : 0;
		int y = ofRandom(-100, 100);
		arcEmitter2.velocity = ofVec3f(x, y, 0);
		arcEmitter2.fired = false;
		arcEmitter2.particleColor = randomColor();
	}

	arcEmitter.update();
	arcEmitter2.update();
}
void EnemyHandler::updateHunterEmitter() {
	for (int i = 0; i < hunterEmitter.sys->particles.size(); i++)
	{
		ofVec3f v = (shipPos - hunterEmitter.sys->particles[i].position).normalize() * 100;
		hunterEmitter.sys->particles[i].velocity = v;
	}

	hunterEmitter.update();

	if (hunterSent) return;

	float time = ofGetElapsedTimeMillis();
	int x1 = 0 - 30; //left
	int x2 = ofGetWindowWidth() + 30; //right
	int y1 = 0 - 30; // up
	int y2 = ofGetWindowHeight() + 30; //down

	hunterEmitter.position = ofVec3f(x1, y1);
	hunterEmitter.spawn(time);

	if (score >= 1000)
	{
		hunterEmitter.position = ofVec3f(x2, y1);
		hunterEmitter.spawn(time);
	}
	if (score >= 1500)
	{
		hunterEmitter.position = ofVec3f(x2, y2);
		hunterEmitter.spawn(time);
	}
	if (score >= 2000)
	{
		hunterEmitter.position = ofVec3f(x1, y2);
		hunterEmitter.spawn(time);
	}
	hunterSpawn.play();
	hunterSent = true;
}

//draws the score popups after enemy death
void EnemyHandler::drawScoreInfo() {
	for (int i = 0; i < scores.size(); i++) {
		ScoreInfo s = scores[i];
		ofSetColor((s.flashCount % 6 > 2 ? ofColor::black : ofColor::white));
		myFont.drawString(std::to_string(s.points) + " points", s.x - (myFont.getStringBoundingBox(std::to_string(s.points) + " points", 0, 0).width * .5f), s.y);
	}
}

//handles the removal of ScoreInfo objects
void EnemyHandler::updateScoreInfo() {
	if (scores.size() == 0) return;
	vector<ScoreInfo>::iterator p = scores.begin();
	vector<ScoreInfo>::iterator tmp;

	while (p != scores.end()) {
		p->flashCount++;
		if (p->lifespan != -1 && p->age() > p->lifespan) {
			tmp = scores.erase(p);
			p = tmp;
		}
		else p++;
	}

	for (int i = 0; i < scores.size(); i++)
		scores[i].y--;
}

void EnemyHandler::draw() {
	drawEnemyEmitters();
	explosionEmitter.draw();
	drawScoreInfo();
}

void EnemyHandler::update() {
	if (!started) return;
	checkEnemiesHit(shipBulletSys);
	updateEnemyEmitters();
	explosionEmitter.update();
	updateScoreInfo();
}

//explodes enemies
void EnemyHandler::explode(Particle * p) {
	explosionEmitter.groupSize = p->maxLife * 10;
	explosionEmitter.position = p->position;
	explosionEmitter.particleRadius = p->radius/5;
	explosionEmitter.particleColor = p->color;
	explosionEmitter.velocity = ofVec3f(0, p->velocity.distance(ofVec3f(0,0)) * 2, 0);
	explosionEmitter.start();

	coin.play();

	scores.push_back(ScoreInfo(p));
}

//player explosion
void EnemyHandler::explodeShip(Particle * p)
{
	explosionEmitter.groupSize = score / 20;
	explosionEmitter.position = p->position;
	explosionEmitter.particleRadius = p->radius*3;
	explosionEmitter.particleColor = p->color;
	explosionEmitter.velocity = ofVec3f(0, score/2, 0);
	explosionEmitter.start();
	theme.stop();
	gameover.play();
}

//helper method for color
ofColor EnemyHandler::randomColor()
{
	return ofColor(ofRandom(250), ofRandom(250), ofRandom(250));
}

//helper method for random position to place emitters
// 1-up 2- down 3-left 4-right
ofVec3f EnemyHandler::randomPositionOutside(int min, int max, int offset)
{
	int x1 = 0 - offset; //left
	int x2 = ofGetWindowWidth() + offset; //right
	int y1 = 0 - offset; // up
	int y2 = ofGetWindowHeight() + offset; //down

	float direction = ofRandom(min, max);

	if (direction < 1) { 
		return ofVec3f(ofRandom(offset,ofGetWidth()-offset), y1);
	}
	else if (direction < 2) {
		return ofVec3f(ofRandom(offset, ofGetWidth() - offset), y2);
	}
	else if (direction < 3) { 
		return ofVec3f(x1, ofRandom(offset, ofGetHeight() - offset));
	}
	else { 
		return ofVec3f(x2, ofRandom(offset, ofGetHeight() - offset));
	}
}

void EnemyHandler::setupExplosionEmitter() {
	explosionEmitter.init();
	explosionEmitter.visible = false;
	explosionEmitter.oneShot = true;
	explosionEmitter.setEmitterType(RadialEmitter);
	explosionEmitter.particleType = Ball;
	explosionEmitter.lifespan = 2;
	explosionEmitter.damping = .99;
	explosionEmitter.randomLife = true;
	explosionEmitter.lifeMinMax = ofVec3f(.5, 1);

	explosionEmitter.sys->addForce(new TurbulenceForce(ofVec3f(-500, -500, 0), ofVec3f(500, 500, 0)));
}

void EnemyHandler::setupStraightEmitter() {
	straightEmitter.init();
	straightEmitter.visible = false;
	straightEmitter.oneShot = false;
	straightEmitter.setEmitterType(DirectionalEmitter);
	straightEmitter.particleType = Enemy;
	straightEmitter.lifespan = 10;
	straightEmitter.damping = 1;
	straightEmitter.particleRadius = 35;
	straightEmitter.particleColor = ofColor::blue;
	straightEmitter.groupSize = 1;
	straightEmitter.velocity = ofVec3f(0, 100, 0);
	straightEmitter.position = ofVec3f(ofGetWindowWidth()/2, -10, 0);
	straightEmitter.rate = .2;
	straightEmitter.points = 10;
	systems.push_back(straightEmitter.sys);
}

void EnemyHandler::setupArcEmitter() {
	arcEmitter.init();
	arcEmitter.visible = false;
	arcEmitter.oneShot = false;
	arcEmitter.setEmitterType(DirectionalEmitter);
	arcEmitter.particleType = Enemy;
	arcEmitter.lifespan = 10;
	arcEmitter.damping = .99;
	arcEmitter.particleRadius = 25;
	arcEmitter.particleColor = ofColor::blue;
	arcEmitter.groupSize = 1;
	arcEmitter.velocity = ofVec3f(0, 200, 0);
	arcEmitter.position = randomPositionOutside(2, 4,30);
	arcEmitter.rate = .5;
	arcEmitter.points = 20;
	arcEmitter.life = 2;
	arcEmitter.sys->addForce(new GravityForce(ofVec3f(0,100,0)));
	arcEmitter.sys->addForce(new TurbulenceForce(ofVec3f(-10,-10, 0), ofVec3f(10, 10, 0)));
	systems.push_back(arcEmitter.sys);

	arcEmitter2.init();
	arcEmitter2.visible = false;
	arcEmitter2.oneShot = false;
	arcEmitter2.setEmitterType(DirectionalEmitter);
	arcEmitter2.particleType = Enemy;
	arcEmitter2.lifespan = 10;
	arcEmitter2.damping = .99;
	arcEmitter2.particleRadius = 20;
	arcEmitter2.particleColor = ofColor::blue;
	arcEmitter2.groupSize = 1;
	arcEmitter2.velocity = ofVec3f(0, 200, 0);
	arcEmitter2.position = randomPositionOutside(2, 4, 30);
	arcEmitter2.rate = .5;
	arcEmitter2.points = 20;
	arcEmitter2.life = 2;
	arcEmitter2.sys->addForce(new GravityForce(ofVec3f(0, -100, 0)));
	arcEmitter2.sys->addForce(new TurbulenceForce(ofVec3f(-10, -10, 0), ofVec3f(10, 10, 0)));
	systems.push_back(arcEmitter2.sys);
}

void EnemyHandler::setupHunterEmitter() {
	hunterEmitter.init();
	hunterEmitter.visible = false;
	hunterEmitter.oneShot = true;
	hunterEmitter.setEmitterType(DirectionalEmitter);
	hunterEmitter.particleType = Hunter;
	hunterEmitter.lifespan = 20;
	hunterEmitter.damping = 1;
	hunterEmitter.particleRadius = 30;
	hunterEmitter.particleColor = ofColor::black;
	hunterEmitter.groupSize = 1;
	hunterEmitter.velocity = ofVec3f(0, 100, 0);
	hunterEmitter.position = ofVec3f(ofGetWindowWidth() / 2, -10, 0);
	hunterEmitter.rate = .2;
	hunterEmitter.points = 50;
	hunterEmitter.life = 5;
	systems.push_back(hunterEmitter.sys);
}



ScoreInfo::ScoreInfo(Particle * p)
{
	this->points = p->points;
	this->x = p->position.x;
	this->y = p->position.y;
	flashCount = 0;
	lifespan = 1;
	birthtime = ofGetElapsedTimeMillis();
}

float ScoreInfo::age() {
	return (ofGetElapsedTimeMillis() - birthtime) / 1000.0;
}
