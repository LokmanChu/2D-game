#pragma once

#include "ofMain.h"
#include "ofxGui/src/ofxGui.h"
#include "ParticleSystem.h"
#include "ParticleEmitter.h"
#include "Ship.h"
#include "EnemyHandler.h"


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		void drawLives(int x, int y);
		void drawScore(int x, int y);
		void drawMessageCentered(const char* str);
		void drawStartMessage();
		void drawRestartMessage();
		void drawFrameRate();
		void removeOffScreenBullets();
		void reset();

		Ship ship;
		EnemyHandler enemyHandler;

		ofImage heart;
		ofImage heart2;
		ofImage heart3;
		ofImage livesText;

		ofTrueTypeFont myFont;
		int flashCount;

		bool frameRate;

		ofxPanel gui;

		//Key press Booleans 
		bool bUpKeyDown;
		bool bDownKeyDown;
		bool bLeftKeyDown;
		bool bRightKeyDown;
		bool bShiftKeyDown;
};
