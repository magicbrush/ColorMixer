#include "ColorDropMerger.h"
#include "ColorDrop.h"

ColorDropMerger::ColorDropMerger( 
  float tipDist,
  float thresDist /*= 0.66*/, 
  float spd )
{
  TipDist.set("TipDist",ofClamp(tipDist,0,100.0f),0,100.0f);
  ThresDist.set("ThresDist",ofClamp(thresDist,0,1),0,1);
  Spd.set("Speed",ofClamp(spd,0,100.0f),0,100.0f);
}

void ColorDropMerger::merge( 
  ofPtr<ColorDrop> a, 
  ofPtr<ColorDrop> b,
  float dt )
{ 
  float ra = a->getRadius();
  float rb = b->getRadius();

  ofVec3f pa = a->getGlobalPosition();
  ofVec3f pb = b->getGlobalPosition();

  float dist = pa.distance(pb);

  float distN = dist/(ra+rb);

  ofPtr<ColorDrop> big,small;
  if(a->getAmt()>b->getAmt())
  {
    big = a;
    small = b;
  }
  else
  {
    big = b;
    small = a;
  }

  

  if(distN<TipDist)
  {
    ColorDropPtrPair P;
    P.first = big;
    P.second = small;
    toShowTip.push_back(P);
  }

  if(distN<=ThresDist)
  {
    float amtBig = big->getAmt();
    float amtSmall = small->getAmt();

    float dAmt = 1000.0f*Spd*dt;
    
    amtBig += dAmt;
    amtSmall -= dAmt;

    big->setAmt(amtBig);
    small->setAmt(amtSmall);

    ofFloatColor CrBig = big->getColor();
    ofFloatColor CrSmall = small->getColor();
   
    ofFloatColor CrBig2;
    for(int i=0;i<3;i++)
    {
      CrBig2[i] = 
        (amtBig*CrBig[i] + dAmt*CrSmall[i])/
        (amtBig+dAmt);
    }
    big->setColor(CrBig2);
  }
}

void ColorDropMerger::draw()
{
  for(auto &p:toShowTip)
  {
    p.second->draw();    
    updateSmallDropMesh(p.second,p.first);    
    SmallDrop.draw();

    ofVec3f posBig = p.first->getGlobalPosition();
    ofVec3f posSmall = p.second->getGlobalPosition();
    float rBig = p.first->getRadius();
    float rSmall = p.second->getRadius();
    float distN = posBig.distance(posSmall)/(rBig+rSmall);
    float distNC = ofClamp(distN,1,TipDist);
    float TipLineWd = ofMap(distNC,1,TipDist+0.01f,10.0f,1.0f);
   
    ofFloatColor cBig = p.first->getColor();
    ofFloatColor cSmall = p.second->getColor();
    ofVec3f Ctr = 0.5f*(posBig+posSmall);
    float amtBig = p.first->getAmt();
    float amtSmall = p.second->getAmt();
    ofFloatColor CrMerge;
    for (int i=0;i<3;i++)
    {
      CrMerge[i] = 
        (cBig[i]*amtBig + cSmall[i]*amtSmall)/
        (amtBig+amtSmall);
    }
    CrMerge.a = 1;
    
    ofPushStyle();
    ofSetColor(CrMerge);
    ofSetLineWidth(TipLineWd);
    ofLine(posBig,posSmall);
    ofPopStyle();  
  }

  toShowTip.clear();
}

void ColorDropMerger::updateSmallDropMesh( 
  ofPtr<ColorDrop> small, 
  ofPtr<ColorDrop> big )
{
  SmallDrop.clear();
  ofVec3f posSmall = small->getGlobalPosition();
  ofVec3f posBig = big->getGlobalPosition();

  float rSmall = small->getRadius();
  float rBig = big->getRadius();

  float dist = posSmall.distance(posBig);
  if(dist>rSmall+rBig)
  {
    return;
  }

  SmallDrop.addVertex(posSmall);
  SmallDrop.addColor(small->getColor());

  for(float theta=0;theta<370;theta+=10)
  {
    float x = (1+rSmall)*cos(ofDegToRad(theta))+posSmall.x;
    float y = (1+rSmall)*sin(ofDegToRad(theta))+posSmall.y;
    ofVec2f Vt(x,y);
    SmallDrop.addVertex(Vt);

    ofFloatColor CrVt;
    float dist = Vt.distance(posBig);
    float alpha = ofMap(
      dist,rBig,rBig*1.08f,1,0,true);

    ofFloatColor CrBig = big->getColor();
    ofFloatColor CrSmall = small->getColor();
    for(int i=0;i<4;i++)
    {
      CrVt[i] = alpha*CrBig[i]+(1-alpha)*CrSmall[i];
    }    
    SmallDrop.addColor(CrVt);

    if(SmallDrop.getNumVertices()>2)
    {
      int idEnd = SmallDrop.getNumVertices()-1;
      SmallDrop.addTriangle(idEnd,idEnd-1,0);
    }
  }
  ofPushStyle();
  ofSetColor(ofColor::red);
  ofNoFill();
  ofCircle(posSmall,20);
  ofPopStyle();
}
