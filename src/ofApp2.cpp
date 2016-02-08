#include "ofApp2.h"
#include "ColorDrop.h"
#include "ColorDropDragger.h"
#include "ColorDropAttractor.h"
#include "ColorDropMerger.h"
#include "ColorHole.h"
#include "ofxPopInfo.h"
#include "SoundEngine.h"

//--------------------------------------------------------------
void ofApp2::setup(){  
  ofSetFrameRate(60.0f);

  for(int i=0;i<12;i++)
  {
    randColorDrop();
  }
  Attractor.reset(new ColorDropAttractor());
  Merger.reset(new ColorDropMerger());
  Dragger.reset(new ColorDropDragger());
  
  ofPtr<ColorHole> H;  
  H.reset(new ColorHole(
    ofVec2f(400,200),
    ofFloatColor::white,1800));
  Holes.push_back(H);  
  H.reset(new ColorHole(
    ofVec2f(800,440),
    ofFloatColor(0.66f,0.66f,0.66f,1),4000));
  Holes.push_back(H);  
  H.reset(new ColorHole(
    ofVec2f(300,600),
    ofFloatColor::black,2000));
  Holes.push_back(H);
}

//--------------------------------------------------------------
void ofApp2::update(){

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
void ofApp2::draw(){
  
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
void ofApp2::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp2::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp2::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp2::mouseDragged(int x, int y, int button){
  if(button==0)
  {
    Dragger->dragged(ofVec2f(x,y));
  }
  
}

//--------------------------------------------------------------
void ofApp2::mousePressed(int x, int y, int button){
  if(button==0)
  {
    for(auto &cd:CrDrops)
    {
      Dragger->clicked(ofVec2f(x,y),cd);
    }    
  }
}

//--------------------------------------------------------------
void ofApp2::mouseReleased(int x, int y, int button){
  if(button==0)
  {
    Dragger->released(ofVec2f(x,y));
  }
}

//--------------------------------------------------------------
void ofApp2::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp2::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp2::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp2::randColorDrop()
{
  ofFloatColor C;
  float randVal = ofRandom(0,100);
  float Amt = ofRandom(50,200);  
  if(randVal<36)
  {
    C = ofFloatColor::white;
  }
  else if(randVal<60)
  {
    C = ofFloatColor::black;
  }
  else if(randVal<70)
  {
    C = ofFloatColor(0.8,0.8,0.8);
  }
  else if(randVal<80)
  {
    C = ofFloatColor(0.3,0.3,0.3);
  }
  else if(randVal<90)
  {
    C = ofFloatColor::cyan;
    Amt = ofRandom(250,400);
  }
  else if(randVal<100)
  {
    C = ofFloatColor::red;
    Amt = ofRandom(250,400);
  }  

  ofPtr<ColorDrop> CD;
  CD.reset(new ColorDrop(C,Amt));
  CD->setGlobalPosition(
    ofVec2f(ofRandomWidth(),ofRandomHeight()));
  CrDrops.push_back(CD);
}
