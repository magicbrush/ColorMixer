#pragma once

#include "ofMain.h"

class ColorDrop;

class ColorDropAttractor
{
public:
  ColorDropAttractor(
    float thres = 1.0f, 
    float spd = 100.0f);
  void attract(
    ColorDrop* src, 
    ColorDrop* tgt, 
    float dt = ofGetLastFrameTime());

private:
  ofParameter<float> Thres;
  ofParameter<float> Spd;
};