#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp
{
  public:
    ofMesh mesh;
    ofImage image;
    ofEasyCam easyCam;
    
    int w;
    int h;
    
    bool changing;
    
	ofxPanel gui;
    ofParameter<ofColor> color;
    ofColor selectedColour;

    ofParameter<int> distance;
    int selectedDistance;
    
    void customMouseReleased(ofMouseEventArgs & mouse);
    void colorChanged(ofColor & value);
    void distanceChanged(int & value);
    
    void createMesh (bool withLines);
    void createMesh (bool withLines, ofColor overrideColor);

    void processOpenFileSelection(ofFileDialogResult openFileResult);

    void setup  ();
    void update ();
    void draw   ();
    void exit   ();
    
    void keyPressed      (ofKeyEventArgs&);
    void keyReleased     (ofKeyEventArgs&);

    void mouseMoved      (ofMouseEventArgs&);
    void mouseDragged    (ofMouseEventArgs&);
    void mousePressed    (ofMouseEventArgs&);
    void mouseReleased   (ofMouseEventArgs&);
    void mouseScrolled   (ofMouseEventArgs&);
    void mouseEntered    (ofMouseEventArgs&);
    void mouseExited     (ofMouseEventArgs&);

    void touchDown       (ofTouchEventArgs&);
    void touchMoved      (ofTouchEventArgs&);
    void touchUp         (ofTouchEventArgs&);
    void touchDoubleTap  (ofTouchEventArgs&);
    void touchCancelled  (ofTouchEventArgs&);

    void windowResized   (ofResizeEventArgs&);
    void dragged         (ofDragInfo&);
    void messageReceived (ofMessage&);
};
