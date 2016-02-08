#include "CurveMovingParticle.h"

CurveMovingParticle::CurveMovingParticle( 
  ofVec3f start, 
  ofVec3f end, 
  float spd /*= 100.0f*/, 
  float curvature /*= 1.0f*/, 
  float radius /*= ofRandom(3,10)*/, 
  ofColor cr /*= ofColor::black*/,
  float score):
  Speed(spd),
  Radius(radius),
  Cr(cr),
  PassedDist(0),
  Score(score)
{
  float Dist = start.distance(end);
  ofVec3f StoE = end-start;

  ofVec3f StoEd4 =StoE * 0.25f;

  ofVec3f StoEN = StoE.getNormalized();
  ofVec3f RandDir = ofVec3f(1,0,0);
  RandDir.rotate(ofRandom(-360,360),ofVec3f(0,0,1));
  RandDir.rotate(ofRandom(-360,360),ofVec3f(0,1,0));
  RandDir.rotate(ofRandom(-360,360),ofVec3f(1,0,0));
  
  ofVec3f RandV;
  RandV = RandDir*curvature*Dist;
  
  ofVec3f P1= start + StoEd4 + RandV;
  ofVec3f P2 = start + 2*StoEd4 + ofRandom(1.0f,1.5f)*RandV;
  ofVec3f P3 = start + 3*StoEd4 + RandV;

  int Res(8);
  Path.addVertex(start);
  Path.curveTo(start,Res);
  Path.curveTo(P1,Res);
  Path.curveTo(P2,Res);
  Path.curveTo(P3,Res);
  Path.curveTo(end,Res);
  Path.curveTo(end + StoEd4,Res);
}

void CurveMovingParticle::update(float dt)
{
  PassedDist+=Speed*dt;
}

void CurveMovingParticle::draw()
{
  ofVec3f Pos = 
    Path.getPointAtLength(PassedDist);
  
  ofPushStyle();
  ofSetColor(Cr);
  ofCircle(Pos,Radius);
  ofColor CrEdge = Cr;
  float Bri = CrEdge.getBrightness();
  if(Score>0)
  {
    Bri/=0.0f;
  }
  else
  {
    Bri*=0.5f;
  }  
  CrEdge.setBrightness(Bri);
  ofNoFill();
  ofSetColor(CrEdge);
  ofSetLineWidth(ofClamp(Radius/5,1.0f,8.0f));
  ofCircle(Pos,Radius);
  ofPopStyle();  
}

bool CurveMovingParticle::end() const
{
  float TotalLen = 
    Path.getLengthAtIndexInterpolated(
    Path.size()-1);

  if(PassedDist>=TotalLen)
  {
    return true;
  }
  else
  {
    return false;
  }
}

float CurveMovingParticle::getRadius() const
{
  return Radius;
}

float CurveMovingParticle::getScore() const
{
  return Score;
}
