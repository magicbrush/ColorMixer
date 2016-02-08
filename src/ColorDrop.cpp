#include "ColorDrop.h"

ColorDrop::ColorDrop( ofFloatColor C, float amt /*= 100.0f*/ ):
  Cr(C),
  Amt(amt)
{

}

void ColorDrop::update()
{

}

bool ColorDrop::inside( ofVec3f P )
{
  ofVec3f PThis = getGlobalPosition();
  float Dist = PThis.distance(P);
  float Radius = getRadius();
  if(Dist<=Radius)
  {
    return true;
  }
  else
  {
    return false;
  }
}

float ColorDrop::getRadius() const
{
  float R = sqrt(Amt);
  return R;
}

void ColorDrop::customDraw()
{
  float Radius = getRadius();
  ofPushStyle();
  ofSetCircleResolution(12+Radius/5);
  ofSetColor(Cr);
  ofCircle(ofPoint(0),Radius);
  ofPopStyle();
}

ofPtr<ColorDrop> ColorDrop::randColorDrop(ofVec2f AmtBound)
{
  ofPtr<ColorDrop> CD;
  ofFloatColor Cr(ofRandom(0,1),ofRandom(0,1),ofRandom(0,1));
  CD.reset(new ColorDrop(Cr,ofRandom(AmtBound.x,AmtBound.y)));
  CD->setPosition(ofRandomWidth(),ofRandomHeight(),0);
  return CD;
}

bool ColorDrop::contact( ColorDrop* another )
{
  ofPoint PThis = getGlobalPosition();
  float Radius = getRadius();

  ofPoint PAno = another->getGlobalPosition();
  float RadiusAno = another->getRadius();

  float Dist = PThis.distance(PAno);
  if(Dist<=(Radius+RadiusAno))
  {
    return true;
  }
  else
  {
    return false;
  }
}

float ColorDrop::getAmt() const
{
  return Amt;
}

void ColorDrop::setAmt( float amt )
{
  Amt = ofClamp(amt,0.0f,10000000.0f);
}

ofFloatColor ColorDrop::getColor() const
{
  return Cr;
}

void ColorDrop::setColor( ofFloatColor cr )
{
  Cr = cr;
}
