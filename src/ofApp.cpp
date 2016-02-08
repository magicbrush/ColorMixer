#include "ofApp.h"
#include "ColorDrop.h"
#include "ColorDropDragger.h"
#include "ColorDropAttractor.h"
#include "ColorDropMerger.h"
#include "ColorHole.h"
#include "ofxPopInfo.h"
#include "SoundEngine.h"

//--------------------------------------------------------------
void ofApp::setup(){  
  ofSetFrameRate(60.0f);

  for(int i=0;i<12;i++)
  {
    randColorDrop();
  }
  Attractor.reset(new ColorDropAttractor());
  Merger.reset(new ColorDropMerger());
  Dragger.reset(new ColorDropDragger());
  
  ofPtr<ColorHole> H;
  ofFloatColor C0(ofFloatColor::yellow);
  C0.setBrightness(0.7f);
  H.reset(new ColorHole(
    ofVec2f(200,200),
    C0,2600));
  Holes.push_back(H);
  ofFloatColor C1(ofFloatColor::cyan);
  C1.setBrightness(0.7f);
  H.reset(new ColorHole(
    ofVec2f(750,240),
    ofFloatColor(0.8f,0.0f,0.8f,1),3000));
  Holes.push_back(H);
  ofFloatColor C2(ofFloatColor::violet);
  C2.setBrightness(0.7f);
  H.reset(new ColorHole(
    ofVec2f(450,500),
    ofFloatColor(0.0f,0.9f,0.9f,1),3800));
  Holes.push_back(H);
}

//--------------------------------------------------------------
void ofApp::update(){

  if(ofRandom(0,1)<0.008f&&CrDrops.size()<60)
  {
    randColorDrop();
  }

  float dt = ofGetLastFrameTime();

  for(auto &c:CrDrops)
  {
    c->update();
  }

  for(int i=0;i<CrDrops.size();i++)
  {
    for(int j=i+1;j<CrDrops.size();j++)
    {      
      Attractor->attract(
        CrDrops[i].get(),CrDrops[j].get(),dt);           
      Attractor->attract(
        CrDrops[j].get(),CrDrops[i].get(),dt);  

      Merger->merge(CrDrops[i],CrDrops[j],dt);
    }
  }

  vector<ofPtr<ColorDrop> >::iterator it;
  for(it=CrDrops.begin();it<CrDrops.end();it++)
  {
    if((*it)->getAmt()<0.01f)
    {
      ofVec2f Pos = (*it)->getPosition();
      INFORMER.pushInfoAt("eat",Pos);
      SOUNDENGINE.playSound("Step",1.0f,1.0f);
      CrDrops.erase(it);
      break;
    }
  } 

  Dragger->update();

  for(auto &h:Holes)
  {
    h->absorb(Dragger);
  }

  ColorHole::updateScoreParticles();

  INFORMER.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
  
  for(auto &c:CrDrops)
  {
    c->draw();
  }
  Merger->draw(); 

  ColorHole::drawScoreParticles();
  for(auto &h:Holes)
  {
    h->draw();
    h->drawInfo();
  }  
  ColorHole::drawTotalScore();
  
  float fps = ofGetFrameRate();
  string FPS("FPS: ");
  FPS += ofToString(fps,1);
  ofDrawBitmapString(FPS,15,ofGetHeight()-30,0);
  INFORMER.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
  if(button==0)
  {
    Dragger->dragged(ofVec2f(x,y));
  }
  
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
  if(button==0)
  {
    for(auto &cd:CrDrops)
    {
      Dragger->clicked(ofVec2f(x,y),cd);
    }    
  }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
  if(button==0)
  {
    Dragger->released(ofVec2f(x,y));
  }
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::randColorDrop()
{
  ofFloatColor C;
  float randVal = ofRandom(0,100);
  if(randVal<22)
  {
    C = ofFloatColor::red;
  }
  else if(randVal<44)
  {
    C = ofFloatColor::green;
  }
  else if(randVal<66)
  {
    C = ofFloatColor::blue;
  }
  else if(randVal<74)
  {
    C = ofFloatColor::yellow;
  }
  else if(randVal<82)
  {
    C = ofFloatColor::cyan;
  }
  else if(randVal<90)
  {
    C = ofFloatColor::violet;
  }
  else
  {
    C = ofFloatColor::black;
  }

  float Amt = ofRandom(50,400);  

  ofPtr<ColorDrop> CD;
  CD.reset(new ColorDrop(C,Amt));
  CD->setGlobalPosition(
    ofVec2f(ofRandomWidth(),ofRandomHeight()));
  CrDrops.push_back(CD);
}
