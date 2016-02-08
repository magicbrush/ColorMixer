#pragma once

#include "ofMain.h"

class ColorDrop;
class ColorDropDragger;
class ColorDropAttractor;
class ColorDropMerger;
class ColorHole;

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
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

    void randColorDrop();

    vector<ofPtr<ColorDrop> > CrDrops;
    ofPtr<ColorDropAttractor> Attractor;
    ofPtr<ColorDropMerger> Merger;
    ofPtr<ColorDropDragger> Dragger;
    vector<ofPtr<ColorHole> > Holes;
    
    
		
};
