#pragma once

#include "ofMain.h"

class ColorDrop;

typedef
  pair<ofPtr<ColorDrop>, ofPtr<ColorDrop> > 
  ColorDropPtrPair;

class ColorDropMerger
{
public:
  ColorDropMerger(
    float tipDist = 3.5f,
    float thresDist = 1.0f, 
    float spd = 1.0f);

  void merge(
    ofPtr<ColorDrop> a, 
    ofPtr<ColorDrop> b, 
    float dt = ofGetLastFrameTime());

  void draw();
  
private:
  void updateSmallDropMesh(
    ofPtr<ColorDrop> small, 
    ofPtr<ColorDrop> big);

private:
  ofParameter<float> TipDist;
  ofParameter<float> ThresDist;
  ofParameter<float> Spd;
  
  vector<ColorDropPtrPair> toShowTip; 

  ofMesh SmallDrop;
};









