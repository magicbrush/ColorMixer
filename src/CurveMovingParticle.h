#pragma once

#include "ofMain.h"

class CurveMovingParticle
{
public:
  CurveMovingParticle(
    ofVec3f start,
    ofVec3f end,
    float spd = 100.0f,
    float Curvature = 0.1f,
    float radius = ofRandom(3,10),
    ofColor cr = ofColor::black,
    float score = 0.0);
  void update(float dt);
  void draw();
  bool end() const;

  float getRadius() const;
  float getScore() const;
private:
  ofPolyline Path;
  float PassedDist;
  
  float Speed;
  float Radius;
  ofColor Cr;

  float Score;
};

