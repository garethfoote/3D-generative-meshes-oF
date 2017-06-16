#include "ofApp.h"
#include "ofEvent.h"

void ofApp::setup()
{
    ofSetWindowShape(1000, 1000);

    ofLog(OF_LOG_NOTICE, "%d : %d",  ofGetWindowWidth(), ofGetWindowHeight());
    ofLog(OF_LOG_NOTICE, "%d : %d",  ofGetWidth(), ofGetHeight());

    image.load("colour-grid.jpg");
    image.resize(200, 200);
    
    gui.setup();
    gui.add(color.set("change that color", ofColor(0, 0, 0), ofColor(0, 0, 0, 255), ofColor(255, 255, 255, 255)));
    gui.add(distance.set("Max distance", 15, 10, 20));
    
    color.addListener(this, &ofApp::colorChanged);
    distance.addListener(this, &ofApp::distanceChanged);

    w = image.getWidth();
    h = image.getHeight();
    changing = false;
    
    // Starting colour
    selectedColour = color;
    selectedDistance = 0;
    createMesh(true, color);
}

void ofApp::createMesh(bool withLines)
{

}

void ofApp::createMesh(bool withLines, ofColor cColor)
{
    mesh.clear();
    
//  mesh.setMode(OF_PRIMITIVE_POINTS);
    mesh.setMode(OF_PRIMITIVE_LINES);
    mesh.enableColors();
    mesh.enableIndices();
    mesh.disableNormals();

    ofVec3f selectedV = ofVec3f(cColor.r, cColor.g, cColor.b);
    float intensityThreshold = 200.0;
    ofVec3f currentV;
    
    int numVertices = 0;
    int previousY = 0;
    // Loop all pixels adding vertices.
    for (int x=0; x<w; ++x) {
        for (int y=0; y<h; ++y) {
            ofColor c = image.getColor(x, y);
            float intensity = c.getLightness();
            // Check lightness is below threshold i.e. not much white
            if (intensity <= intensityThreshold) {
                // Get curr pixel distance from user selected color
                // using RGB cube. Use this as z value.
                currentV.set(c.r, c.g, c.b);
                float dist = currentV.distance(selectedV);
                // Add vertices.
                float z = ofMap(dist, 440, 0, 400, 0);
                ofVec3f pos(x*5, y*5, z);
                
                if(y < previousY && numVertices%2 == 1){
                    if(numVertices > 0){
                        mesh.removeVertex(mesh.getNumVertices()-1);
                        numVertices--;
                    }
//                    ofLog(OF_LOG_NOTICE, "Skipping Y pixel to avoid long line: %d %d", numVertices-1, mesh.getNumVertices());
                } else {
                    mesh.addVertex(pos);
                    mesh.addColor(c);
                    numVertices++;
                }
                previousY = y;
            }
        }
    }

    if(withLines == true){
        
        // Make connections between vertices with distance below...
        ofColor cColor = color;
//        float connectionDistance = ofMap(cColor.a, 0, 255, 10, 20);
        int numVerts = mesh.getNumVertices();
        for (int a=0; a<numVerts; ++a) {
            ofVec3f verta = mesh.getVertex(a);
            for (int b=a+1; b<numVerts; ++b) {
                ofVec3f vertb = mesh.getVertex(b);
                float vertexDistance = verta.distance(vertb);
                if (vertexDistance <= selectedDistance) {
                    // In OF_PRIMITIVE_LINES, every pair of vertices or indices will be
                    // connected to form a line
                                    ofLog(OF_LOG_NOTICE, "distance %f", vertexDistance);

                    mesh.addIndex(a);
                    mesh.addIndex(b);
                }
            }
        }
    }
}

void ofApp::colorChanged(ofColor & value)
{
    ofLog(OF_LOG_NOTICE, "colourChanging");

    changing = true;
    ofAddListener(ofEvents().mouseReleased, this, &ofApp::customMouseReleased, -1);
    
    ofColor cColor = value;
    createMesh(false, cColor);
}


void ofApp::distanceChanged(int & value)
{
    ofLog(OF_LOG_NOTICE, "distanceChanging");

    changing = true;
    ofAddListener(ofEvents().mouseReleased, this, &ofApp::customMouseReleased, -1);
    
    ofColor cColor = selectedColour;
    createMesh(false, cColor);
}


void ofApp::customMouseReleased(ofMouseEventArgs & mouse){
    ofLog(OF_LOG_NOTICE, "changing = false");
    changing = false;
    ofRemoveListener(ofEvents().mouseReleased, this, &ofApp::customMouseReleased, -1);
}


void ofApp::update()
{
    if(changing == false){
        ofColor cColor = color;
        
        if((ofColor)color != selectedColour || selectedDistance != distance){
            ofLog(OF_LOG_NOTICE, "Changed summit: %d, %d, %d", cColor.r, cColor.g, cColor.b);
            selectedColour = cColor;
            selectedDistance = distance;
            createMesh(true, selectedColour);
        }
    }
}

void ofApp::draw()
{
    
    ofColor centerColor = ofColor(231, 231, 231);
    ofColor edgeColor(255, 255, 255);
    ofBackgroundGradient(edgeColor, centerColor, OF_GRADIENT_CIRCULAR);
//    ofBackground(edgeColor);
    
//    image.draw(0, 0);
    
    easyCam.begin();
        ofPushMatrix();
            ofTranslate(-ofGetWindowWidth()/2, -ofGetWindowHeight()/2);
            mesh.drawFaces();
        ofPopMatrix();
    easyCam.end();
    
    gui.draw();
    
}

void ofApp::processOpenFileSelection(ofFileDialogResult openFileResult){
	
	ofLogVerbose("getName(): "  + openFileResult.getName());
	ofLogVerbose("getPath(): "  + openFileResult.getPath());
	
	ofFile file (openFileResult.getPath()); 
	
	if (file.exists()){
		
		ofLogVerbose("The file exists - now checking the type via file extension");
		string fileExtension = ofToUpper(file.getExtension());
		
		//We only want images
		if (fileExtension == "JPG" || fileExtension == "PNG") {
            
			//Load the selected image
			image.load(openFileResult.getPath());
            
            int maxW = 200;
            int maxH = 200;
            float ratio = max(maxW/image.getWidth(), maxH/image.getHeight());
            
            int newW = round(image.getWidth()*ratio);
            int newH = round(image.getHeight()*ratio);
            
            image.resize(newW, newH);
            image.crop((newW-maxW)/2, (newH-maxH)/2, maxW, maxH);
            
            ofLog(OF_LOG_NOTICE, "%d / %f = %f,  %d / %f = %f", ofGetWidth(), image.getWidth(), (float)maxW/image.getWidth(),ofGetHeight(), image.getHeight(), maxH/image.getHeight());
            ofLog(OF_LOG_NOTICE, "%d %d, %d %d", (newW-maxW)/2, (newH-maxH)/2, newW, newH);
            
			// Update width + height
            w = image.getWidth();
            h = image.getHeight();
            
//            selected = cColor;
            createMesh(true, selectedColour);
		}
	}
	
}



void ofApp::exit()
{
}

void ofApp::keyPressed(ofKeyEventArgs& key)
{
    // ofLog(OF_LOG_NOTICE, "NUM NORMALS: %d", size);
    if (key.key == 's'){
        mesh.save("mesh.ply");
    }
    
    if (key.key == ' '){
		
		//Open the Open File Dialog
		ofFileDialogResult openFileResult= ofSystemLoadDialog("Select a jpg or png"); 
		
		//Check if the user opened a file
		if (openFileResult.bSuccess){
			
			ofLogVerbose("User selected a file");
			
			//We have a file, check it and process it
			processOpenFileSelection(openFileResult);
			
		}else {
			ofLogVerbose("User hit cancel");
		}
	}
}

void ofApp::keyReleased(ofKeyEventArgs& key)
{
}

void ofApp::mouseMoved(ofMouseEventArgs& mouse)
{
}

void ofApp::mouseDragged(ofMouseEventArgs& mouse)
{
}

void ofApp::mousePressed(ofMouseEventArgs& mouse)
{
}

void ofApp::mouseReleased(ofMouseEventArgs& mouse)
{
}

void ofApp::mouseScrolled(ofMouseEventArgs& mouse)
{
}

void ofApp::mouseEntered(ofMouseEventArgs& mouse)
{
}

void ofApp::mouseExited(ofMouseEventArgs& mouse)
{
}

void ofApp::touchDown(ofTouchEventArgs& touch)
{
}

void ofApp::touchMoved(ofTouchEventArgs& touch)
{
}

void ofApp::touchUp(ofTouchEventArgs& touch)
{
}

void ofApp::touchDoubleTap(ofTouchEventArgs& touch)
{
}

void ofApp::touchCancelled(ofTouchEventArgs& touch)
{
}

void ofApp::windowResized(ofResizeEventArgs& window)
{
}

void ofApp::dragged(ofDragInfo& dragged)
{
}

void ofApp::messageReceived(ofMessage& message)
{
}
