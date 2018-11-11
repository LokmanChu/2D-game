#pragma once

#include "Ship.h"
#include "ParticleSystem.h"
#include "ParticleEmitter.h"

class ScoreInfo {
public:
	ScoreInfo(Particle * p);
	int points;
	int x;
	int y;
	int birthtime;
	int lifespan;
	int flashCount;
	float age();
};

class EnemyHandler {
public:
	EnemyHandler();

	vector<ParticleSystem *> systems;
	vector<ScoreInfo> scores;
	ParticleSystem * shipBulletSys;
	ParticleEmitter explosionEmitter;

	ParticleEmitter straightEmitter;
	ParticleEmitter arcEmitter;
	ParticleEmitter arcEmitter2;
	ParticleEmitter hunterEmitter;

	ofTrueTypeFont myFont;
	int score;
	bool started;
	bool hunterSent;
	int lastHunterSent;
	ofVec3f shipPos;

	ofSoundPlayer hunterSpawn;
	ofSoundPlayer coin;
	ofSoundPlayer gameover;
	ofSoundPlayer theme;

	void start();
	void reset();

	void checkShipHit(Ship * ship);
	bool isShipHit(Ship * ship, ParticleSystem * enemySys);
	void checkEnemiesHit(ParticleSystem * shipSys);
	bool areEnemiesHit(ParticleSystem * shipSys, ParticleSystem * enemySys);

	void drawEnemyEmitters();
	void updateEnemyEmitters();

	void updateStraightEmitter();
	void updateArcEmitter();
	void updateHunterEmitter();

	void drawScoreInfo();
	void updateScoreInfo();

	void draw();
	void update();

	void explode(Particle * p);
	void explodeShip(Particle * p);

	ofColor randomColor();
	ofVec3f randomPositionOutside(int min, int max, int offset);

	void setupExplosionEmitter();
	void setupStraightEmitter();
	void setupArcEmitter();
	void setupHunterEmitter();
};
