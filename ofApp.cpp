
#include "ofApp.h"
#include "ofxGui/src/ofxGui.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);
	
	frameRate = false;

	ofBackground(ofColor(220,220,220));
	heart.load("images/heart.png");
	heart2 = heart;
	heart3 = heart;
	livesText.load("images/lives.png");
	myFont.load("ARCADECLASSIC.TTF",32);
	enemyHandler.shipBulletSys = ship.bulletEmitter.sys;
	gui.setup();
	enemyHandler.theme.play();
}

//--------------------------------------------------------------
void ofApp::update() {
	ofVec3f dir = ofVec3f(0, 0, 0);
	ship.boost = bShiftKeyDown ? true : false;
	if (bUpKeyDown) {
		dir.y--;
	}
	if (bDownKeyDown) {
		dir.y++;
		bUpKeyDown = false;
		bLeftKeyDown = false;
		bRightKeyDown = false;
	} 
	if (bLeftKeyDown) {
		dir.x--;
	}  
	if (bRightKeyDown) {
		dir.x++;
	}
	ship.setThrustDir(dir);

	enemyHandler.checkShipHit(&ship);
	enemyHandler.update();

	if (ship.life == 0)
	{
		enemyHandler.explodeShip(&ship.shipSys->particles.at(0));
		ship.shipSys->particles.at(0).position = ofVec3f(-10000, -10000);
		ship.life--;
	}
	else {
		ship.update();
		enemyHandler.shipPos = ship.shipSys->particles.at(0).position;
	}

	if (enemyHandler.score >= 750 && ship.bulletEmitter.groupSize == 1)
	{
		ship.bulletEmitter.groupSize = 2;
		ship.powerup.play();
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	drawStartMessage();
	ship.draw();
	enemyHandler.draw();
	drawLives(ofGetWindowWidth() - 205, 15);
	drawScore(10, 30);
	drawRestartMessage();
	drawFrameRate();
}


void ofApp::keyPressed(int key) {
	switch (key) {
	case 'w':
		bUpKeyDown = true;
		break;
	case 's':
		bDownKeyDown = true;
		break;
	case 'a':
		bLeftKeyDown = true;
		break;
	case 'd':
		bRightKeyDown = true;
		break;
	case 'r':
		reset();
		break;
	case 'f':
		frameRate = frameRate ? false : true;
		break;
	case 32:
		enemyHandler.start();
		ship.bulletEmitter.sys->removeAll();
		break;
	case OF_KEY_LEFT_SHIFT:
		bShiftKeyDown = true;
	default:
		break;
	}
}



void ofApp::keyReleased(int key) {
	switch (key) {
	case 'w':
		bUpKeyDown = false;
		break;
	case 's':
		bDownKeyDown = false;
		break;
	case 'a':
		bLeftKeyDown = false;
		break;
	case 'd':
		bRightKeyDown = false;
		break;
	case OF_KEY_LEFT_SHIFT:
		bShiftKeyDown = false;
		break;
	default:
		break;

	}
}


//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	ship.lastMousePos = ofVec3f(x, y);
}


//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	if (ship.life >= 1) {
		ship.bulletEmitter.start();
	}
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
	ship.lastMousePos = ofVec3f(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	ship.bulletEmitter.stop();
}


//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

void ofApp::drawLives(int x, int y) {
	int lives = ship.life;
	ofPushMatrix();
	ofTranslate(x, y);
	ofSetColor(0, 0, 0, 255);
	ofNoFill();
	ofSetLineWidth(4);
	ofDrawRectangle(-5,-5,ofGetWindowWidth() - x - 3, 37);
	ofSetLineWidth(1);
	ofSetColor(ofColor::white);
	livesText.draw(0, 0, 100, 30);
	
	lives >= 1 ? heart.draw(60 + 40, -3, 36, 36) : 0;
	lives >= 2 ? heart2.draw(60 + 70, -3, 36, 36) : 0;
	lives >= 3 ? heart3.draw(60 + 100, -3, 36, 36) : 0;
	ofPopMatrix();
}

void ofApp::drawScore(int x, int y) {
	ofPushMatrix();
	ofTranslate(x, y);
	ofSetColor(ofColor::black);
	myFont.drawString("Score " + std::to_string(enemyHandler.score), 0, 0);
	ofPopMatrix();
}

void ofApp::drawMessageCentered(const char* str)
{
	myFont.drawString(str, (ofGetWidth() * 0.5f) - (myFont.getStringBoundingBox(str, 0, 0).width * .5f), 300);
}

void ofApp::drawStartMessage() {
	if (enemyHandler.started) return;
	ofSetColor(ofColor::black);
	drawMessageCentered("Press Space To Start");
}

void ofApp::drawRestartMessage() {
	if (ship.life > 0) return;
	ofSetColor(ofColor::black);
	drawMessageCentered("Press R To Restart");
}

void ofApp::drawFrameRate()
{
	if (!frameRate) return;
	string str;
	str += "Frame Rate: " + std::to_string(ofGetFrameRate());
	ofSetColor(ofColor::black);
	ofDrawBitmapString(str, ofGetWindowWidth() - 170, ofGetWindowHeight() - 10);
}

void ofApp::removeOffScreenBullets()
{
	vector<Particle>::iterator s = ship.bulletEmitter.sys->particles.begin();
	vector<Particle>::iterator tmp;

	while (s != ship.bulletEmitter.sys->particles.end()) {
		if (s->position.x < -30 || s->position.x > ofGetWindowWidth() + 30 || 
			s->position.y < -30 || s->position.y > ofGetWindowHeight() + 30) {
			tmp = ship.bulletEmitter.sys->particles.erase(s);
			s = tmp;
		}
		else s++;
	}
}

void ofApp::reset() {
	enemyHandler.reset();
	ship.reset();
	enemyHandler.theme.play();
}
