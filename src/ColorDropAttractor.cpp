#include "ColorDropAttractor.h"

#include "ColorDrop.h"

ColorDropAttractor::ColorDropAttractor( 
  float thres /*= 0.66f*/, 
  float spd /*= 1.0f*/ )
{
  Thres.set("Thres",ofClamp(thres,0,100.0f),0.0f,100.0f);
  float spdMax = sqrt(ofGetWidth()*ofGetHeight());
  Spd.set("Speed",ofClamp(spd,0.1f,spdMax),0.1f,spdMax);
}

void ColorDropAttractor::attract( 
  ColorDrop* src, 
  ColorDrop* tgt,
  float dt)
{
  float rsrc = src->getRadius();
  float rtgt = tgt->getRadius();

  ofVec3f psrc = src->getGlobalPosition();
  ofVec3f ptgt = tgt->getGlobalPosition();

  float dist = psrc.distance(ptgt);

  float distN = dist/(rsrc+rtgt);

  if(distN<=Thres)
  {    
    ofVec3f Dir = (psrc-ptgt).getNormalized(); 
    float speed = Spd*rsrc/(rsrc+rtgt);
    ofVec3f Movement = Dir*Spd*dt;
    tgt->move(Movement.x,Movement.y,Movement.z);
  }
}
