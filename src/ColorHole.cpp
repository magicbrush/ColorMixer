#pragma once

#include "ColorHole.h"
#include "ColorDrop.h"
#include "ofxPopInfo.h"
#include "ColorDropDragger.h"
#include "LyuUtils.h"
#include "CurveMovingParticle.h"
#include "SoundEngine.h"


vector<float> ColorHole::MatchLevelThres;
vector<float> ColorHole::MatchLevelScoreCoefs;
vector<string> ColorHole::MatchLevelRemarks;

ColorHole::ColorHole( 
  ofVec2f pos, 
  ofFloatColor crTgt, 
  float capacity/*=1000.0f*/):
WaitingForAbsorb(false)
{  
  setGlobalPosition(pos);
  CrTgt.set("TargetColor",
    crTgt,
    ofFloatColor(0,0,0,0),
    ofFloatColor(1,1,1,1));

  Capacity.addListener(this,&ColorHole::CapacityChanged);
  Capacity.set("Capacity",capacity,10,1000000);  
  
  if(MatchLevelThres.size()==0)
  {
    MatchLevelThres.push_back(0.07f);
    MatchLevelThres.push_back(0.14f);
    MatchLevelThres.push_back(0.25f);
    MatchLevelThres.push_back(0.35f);
    MatchLevelThres.push_back(0.5f);
    MatchLevelThres.push_back(0.8f);
    MatchLevelThres.push_back(10.0f);

    MatchLevelRemarks.push_back("Perfect");
    MatchLevelRemarks.push_back("Excellent");
    MatchLevelRemarks.push_back("Good");
    MatchLevelRemarks.push_back("Average");
    MatchLevelRemarks.push_back("Fair");
    MatchLevelRemarks.push_back("Poor");
    MatchLevelRemarks.push_back("Fail");

    MatchLevelScoreCoefs.push_back(10);
    MatchLevelScoreCoefs.push_back(7);
    MatchLevelScoreCoefs.push_back(5);
    MatchLevelScoreCoefs.push_back(3);
    MatchLevelScoreCoefs.push_back(2);
    MatchLevelScoreCoefs.push_back(1);
    MatchLevelScoreCoefs.push_back(-1);
  }
}

void ColorHole::absorb( ofPtr<ColorDropDragger> dragger )
{
  Dragger = dragger;
  ofVec2f DragPos = Dragger->getPos();
  float dist = DragPos.distance(getGlobalPosition());
  bool bAbsorb(false);
  if(dist<=Radius&&Dragger->isDragging())
  {    
    WaitingForAbsorb = true;
    DraggedCrDrop = Dragger->getTgt();
  }  
  else if(dist<=Radius&&!Dragger->isDragging())
  {
    if(WaitingForAbsorb)bAbsorb=true;
    WaitingForAbsorb = false;
  }
  
  if(bAbsorb)
  {
    absorbBatch();
  }

  if(!Outputs.empty())
  {
    bool bFinished = Outputs.front().update(this);
    if(bFinished)Outputs.pop();
  }  
}

void ColorHole::customDraw()
{ 
  ofPushStyle();
  ofFill();
  ofSetColor(CrTgt.get(),60);
  ofCircle(ofVec2f(0),Radius);
  ofNoFill(); 
  ofSetColor(CrTgt.get());
  ofSetLineWidth(6.0f);
  ofCircle(ofVec2f(0),Radius);
  ofPopStyle();
}

void ColorHole::CapacityChanged( float &c )
{
  Radius = sqrt(c);
}

void ColorHole::drawTicks( ofColor Cr, ofVec2f &Offset )
{
  ofSetLineWidth(2.0f);
  //float Bright = 0.3f*sin(5.0f*ofGetElapsedTimef())+0.7f;
  ofSetColor(Cr);
  //ofCircle(Offset,Radius+1.0f);
  ofLine(ofVec3f(0),ofVec3f(0)-ofVec3f(0,Radius,0));
  for(int i=1;i<4;i++)
  {
    float Cap = Capacity*i/4.0f;
    string CapString = ofToString(Cap,0);
    float TickY = sqrt(Capacity*i/4.0f);
    ofLine(
      ofVec2f(0,-TickY)+Offset,
      ofVec2f(Radius*0.05f,-TickY)+Offset);
    ofDrawBitmapString(
      CapString,
      Radius*0.07f+Offset.x,-TickY+Offset.y);
  }
}

void ColorHole::drawInfo()
{
  ofVec2f DragPos = Dragger->getPos();
  float dist = DragPos.distance(getGlobalPosition());
  if(dist<=Radius&&Dragger->isDragging())
  {
    ofFloatColor CrBack;
    if(CrTgt.get().getBrightness()>0.5f)
    {
      CrBack = ofFloatColor::black;
    }
    else
    {
      CrBack = ofFloatColor::white;
    }
    ofPushMatrix();
    ofPushStyle();
    ofSetColor(CrBack);
    Lyu::drawTxtByFontAt(
      "Absorb?",
      &ofxAssets::font("vag",24),
      getGlobalPosition()+ofVec2f(0,0));   
    ofSetColor(CrTgt.get());
    Lyu::drawTxtByFontAt(
      "Absorb?",
      &ofxAssets::font("vag",24),
      getGlobalPosition()+ofVec2f(1.5,-1.5));
    ofPopStyle();
    ofPopMatrix();
  }
  if(!Outputs.empty())Outputs.front().draw(this);
}

void ColorHole::absorbBatch()
{
  float Amt = DraggedCrDrop.get()->getAmt();
  ColorOutputBatch B;
  if(Amt<=Capacity)
  {
    B = ColorOutputBatch(false);     
  }
  else
  {
    B = ColorOutputBatch(true);
  }  
  B.Amount = Amt;
  B.AmountRest = B.Amount;
  B.CrAbsorb = DraggedCrDrop.get()->getColor();
  ofFloatColor ColorTarget = CrTgt.get();
  float squareSum(0.0f);
  for(int i=0;i<4;i++)
  {
    squareSum += pow((ColorTarget[i]-B.CrAbsorb[i]),2.0f);
  }
  float ColorDist = sqrt(squareSum);
  // debug
  cout << "ColorDist:" << ColorDist << endl;
  for(int i=0;i<MatchLevelThres.size();i++)
  {     
    if(ColorDist<=MatchLevelThres[i])
    {
      B.MatchRank = MatchLevelScoreCoefs[i];
      B.MatchString = MatchLevelRemarks[i];
      break;
    }
  }
  B.computeScore();
 
  Outputs.push(B);    

  DraggedCrDrop.get()->setAmt(0.0f);
}

void ColorHole::drawTotalScore()
{
  string ScoreStr = ofToString(ScoreTotal,0);
  ofPushStyle();
  ofSetColor(ofColor::black);
  ofxAssets::font("vag",24).drawString(
    ScoreStr,ScoreTotalPos.x-1,ScoreTotalPos.y+1);
  ofSetColor(ofColor::white);
  ofxAssets::font("vag",24).drawString(
    ScoreStr,ScoreTotalPos.x,ScoreTotalPos.y);
  ofPopStyle();
}

void ColorHole::drawScoreParticles()
{
  for(auto &p:ScorePts)
  {
    p->draw();
  }
}

void ColorHole::updateScoreParticles()
{
  float dt = ofGetLastFrameTime();
  for(auto &p:ScorePts)
  {
    p->update(dt);
  }
  vector<ofPtr<CurveMovingParticle> >::iterator it;
  for(it=ScorePts.begin();it<ScorePts.end();it++)
  {
    if((*it)->end())
    {
      float S = (*it)->getScore();
      ColorHole::ScoreTotal += S;
      ScorePts.erase(it);      
      break;
    }
  }
}

float ColorHole::amountCoef(0.01f);

vector<ofPtr<CurveMovingParticle> > ColorHole::ScorePts;

ofVec2f ColorHole::ScoreTotalPos = ofVec2f(50,50);
float ColorHole::ScoreTotal(0);
ColorHole::ColorOutputBatch::ColorOutputBatch(bool overflow)
{
  bOverflow = overflow;
  if(!bOverflow)
  {
    AnimDur["Appear"]=1.2f;
    AnimDur["Multiply"]=0.3f;
    AnimDur["PopScore"]=0.2f;
    AnimDur["ShowScore"]=0.3f;
    AnimDur["GetScore"]=0.7f;    
  }
  else
  {
    AnimDur["PopTooMuch"]=0.75f;
    AnimDur["ShowMinusScore"]=0.3f;
    AnimDur["AddMinusScore"]=0.7f;    

    InfoPoped["PopTooMuch"]=false;
    InfoPoped["ShowMinusScore"]=false;
    InfoPoped["PopScore"]=false;
  }
  AnimDurDef = AnimDur;
  
  
}

void ColorHole::ColorOutputBatch::computeScore()
{
  if(bOverflow)
  {
    Score = -amountCoef*Amount;
  }
  else
  {
    Score = amountCoef*Amount*MatchRank;
  }  
  ScoreRest = Score;
}

bool ColorHole::ColorOutputBatch::update(ColorHole* parent)
{
  bool Finished(false);
  float dt = ofGetLastFrameTime();
  ofVec3f Pos = parent->getGlobalPosition();
  if(bOverflow)
  {
    update_Overflow(Pos, dt);
  }
  else
  {   
    update_Absorbing(parent, dt);
  }
  
  float restDur(0);
  for(auto &d:AnimDur)
  {
    restDur+=d.second;
  }
  if(restDur<=0)
  {
    Finished = true;
  }
  return Finished;
}

void ColorHole::ColorOutputBatch::update_Overflow( 
  ofVec3f Pos, float dt )
{
  if(AnimDur["PopTooMuch"]>0)
  {     
    if(!InfoPoped["PopTooMuch"])
    {
      INFORMER.pushInfoAt(
        "Too Much!",
        Pos,
        AnimDur["PopTooMuch"],
        0.0f,
        1.0f,
        "vag",
        24.0f,
        ofVec2f(1.0f,1.0f),
        ofColor::red,
        1.5f,
        ofColor::black);
      InfoPoped["PopTooMuch"] = true;
    }      
    AnimDur["PopTooMuch"]-=dt;
  }
  else if(AnimDur["ShowMinusScore"]>0)
  {
    if(!InfoPoped["ShowMinusScore"])
    {
      INFORMER.pushInfoAt(
        ofToString(ScoreRest,0),
        Pos,
        AnimDur["ShowMinusScore"],
        0.0f,
        1.0f,
        "vag",
        24.0f,
        ofVec2f(1.0f,1.0f),
        ofColor::red,
        1.5f,
        ofColor::black);
      SOUNDENGINE.playSound("Shrink",1.0f,1.0f);
      InfoPoped["ShowMinusScore"] = true;
    }      
    AnimDur["ShowMinusScore"]-=dt;
  }
  else if(AnimDur["AddMinusScore"]>0)
  { 
    gettingScore(Pos, dt, "AddMinusScore");
    AnimDur["AddMinusScore"]-=dt;
  }
}

void ColorHole::ColorOutputBatch::update_Absorbing( 
  ColorHole* parent,float dt )
{ 
  float scnSize,cellWd,cellHt;
  ofVec2f InfoPos;
  getInfoPosParams(scnSize, cellWd, cellHt, InfoPos, parent);

  if(AnimDur["Appear"]>0)
  {
    AnimDur["Appear"]-=dt;
  }
  else if(AnimDur["Multiply"]>0)
  {
    AnimDur["Multiply"]-=dt;
  }
  else if(AnimDur["PopScore"]>0)
  { 
    if(!InfoPoped["PopScore"])
    {
      string ScoreTxt = ofToString(ScoreRest,0);
      INFORMER.pushInfoWithLuaCtrl(
        ScoreTxt,
        InfoPos,
        "Lua\\ofxNodeCtrlLua.lua",
        "zoomInOut",AnimDur["PopScore"],
        0,
        1.0f,
        "vag",
        24,
        ofVec2f(1.5f,1.5f)); 
      SOUNDENGINE.playSound("GetScore",1.0f,1.0f);
      InfoPoped["PopScore"]=true;
    }
    AnimDur["PopScore"]-=dt;
  }
  else if(AnimDur["ShowScore"]>0)
  {     
    AnimDur["ShowScore"]-=dt;
  }
  else if(AnimDur["GetScore"]>0)
  {      
    gettingScore(parent->getGlobalPosition(), dt, "GetScore");
    AnimDur["GetScore"]-=dt;
  }
}

void ColorHole::ColorOutputBatch::draw( ColorHole* parent )
{
  ofVec3f Pos = parent->getGlobalPosition();
  if(bOverflow)
  {
    draw_overflow(Pos);
  }  
  else
  {   
    draw_absorbing(parent);
  }
}

void ColorHole::ColorOutputBatch::draw_overflow( ofVec3f Pos )
{
  ofPushStyle();
  ofSetColor(CrAbsorb);
  ofCircle(Pos,sqrt(AmountRest));
  ofPopStyle();

  if(AnimDur["PopTooMuch"]>0)
  { }
  else if(AnimDur["ShowMinusScore"]>0)
  { }
  else if(AnimDur["AddMinusScore"]>0)
  {
    string ScoreString = ofToString(ScoreRest,0);
    ofPushStyle();
    ofSetColor(ofColor::red);      
    Lyu::drawTxtByFontAt(
      ScoreString,
      &ofxAssets::font("vag",24),
      Pos,
      ofVec2f(1,1));   
    ofSetColor(ofColor::black);      
    Lyu::drawTxtByFontAt(
      ScoreString,
      &ofxAssets::font("vag",24),
      Pos,
      ofVec2f(1,1),1.5f);
    ofPopStyle();      
  }
}

void ColorHole::ColorOutputBatch::draw_absorbing( 
  ColorHole* parent )
{
  ofPushStyle();
  ofSetColor(CrAbsorb);
  ofCircle(parent->getGlobalPosition(),sqrt(AmountRest));
  ofPopStyle();

  float scnSize,cellWd,cellHt;
  ofVec2f InfoPos;
  getInfoPosParams(scnSize, cellWd, cellHt, InfoPos, parent);
  float moveRatio=1.0f-AnimDur["Multiply"]/AnimDurDef["Multiply"];
  if(AnimDur["Appear"]>0)
  {     
    drawRankAmount(InfoPos, cellWd, cellHt, moveRatio);
  }
  else if(AnimDur["Multiply"]>0)
  {      
    drawRankAmount(InfoPos, cellWd, cellHt, moveRatio);
  }
  else if(AnimDur["PopScore"]>0)
  {}
  else if(AnimDur["ShowScore"]>0||AnimDur["GetScore"]>0)
  {      
    string ScoreTxt = ofToString(ScoreRest,0);
    ofPushStyle();
    ofSetColor(255);
    Lyu::drawTxtByFontAt(
      ScoreTxt,
      &ofxAssets::font("vag",24),
      InfoPos,
      ofVec2f(1.5f,1.5f),
      0.0f);
    ofSetColor(0);
    Lyu::drawTxtByFontAt(
      ScoreTxt,
      &ofxAssets::font("vag",24),
      InfoPos,
      ofVec2f(1.5f,1.5f),
      1.0f);
    ofPopStyle();
  }
  /*else if(AnimDur["GetScore"]>0)
  {      
    
  }*/
}

void ColorHole::ColorOutputBatch::gettingScore(
  ofVec2f parentPos,
  float dt, 
  string Str )
{
  float dScore = dt*Score/AnimDurDef[Str];
  ScoreRest -= dScore;     

  float dAmt = -dt*Amount/AnimDurDef[Str];
  AmountRest += dAmt;

  ofPtr<CurveMovingParticle> P;
  ofVec3f start,end;
  start = parentPos + 
    Lyu::randomVectorInXYInLength(sqrt(AmountRest));
  end = ColorHole::ScoreTotalPos + 
  Lyu::randomVectorInXYInLength(
  Lyu::getWindowSize()*0.01f);
    float spd = start.distance(end)*ofRandom(0.95f,1.05f);
  float curvature = 0.1f;
  float radius = sqrt(abs(dAmt));  
  P.reset(new 
    CurveMovingParticle(
    start,end,spd,curvature,radius,ofColor(CrAbsorb),dScore));
  ColorHole::ScorePts.push_back(P);  

  SOUNDENGINE.playSound(
    "Bubble",
    ofMap(dAmt,10,100,1.5f,0.7f),
    ofMap(dAmt,10,100,0.6f,1.0f));
}

void ColorHole::ColorOutputBatch::drawRankAmount( 
  ofVec2f &InfoPos, float cellWd, float cellHt, float moveRatio )
{
  ofRectangle RRankTitle,RAmtTitle,RRank,RAmt,RX;
  RRankTitle.x = InfoPos.x-cellWd;
  RRankTitle.y = InfoPos.y;
  RRankTitle.width = cellWd;
  RRankTitle.height = cellHt;
  RRank = RRankTitle;
  RRank.translateX(cellWd);
  RAmtTitle = RRankTitle;
  RAmtTitle.translateY(cellHt);
  RAmt = RAmtTitle;
  RAmt.translateX(cellWd); 
  RRankTitle.translateY(-cellHt);
  RRank.translateY(-cellHt);
  RX = RRank;
  RX.translateY(cellHt);

  RRank.translateY(cellHt*moveRatio);
  RAmt.translateY(-cellHt*moveRatio);

  float htScale(0.96f);
  RRankTitle.scaleHeight(htScale);
  RRank.scaleHeight(htScale);
  RAmtTitle.scaleHeight(htScale);
  RAmt.scaleHeight(htScale);
  RX.scaleHeight(htScale);
  
  ofPushStyle();
  drawRandAmountText(RRankTitle, RRank, RX, RAmtTitle, RAmt);
  ofSetColor(0);
  drawRandAmountText(RRankTitle, RRank, RX, RAmtTitle, RAmt,1.0f);
  ofPopStyle();
  
}

void ColorHole::ColorOutputBatch::getInfoPosParams(
  float &scnSize, 
  float &cellWd, 
  float &cellHt, 
  ofVec2f &InfoPos, 
  ColorHole* parent )
{
  scnSize = Lyu::getWindowSize();
  cellWd = scnSize*0.2f;
  cellHt = scnSize*0.03f;  
  InfoPos = parent->getGlobalPosition()-ofVec2f(0,scnSize/50);    
}

void ColorHole::ColorOutputBatch::drawRandAmountText( 
  ofRectangle RRankTitle, 
  ofRectangle RRank, 
  ofRectangle RX,
  ofRectangle RAmtTitle, 
  ofRectangle RAmt,
  float EdgeSize)
{  
  Lyu::drawTxtByFontInRect("Rank",
    &ofxAssets::font("vag",24),RRankTitle,EdgeSize);
  Lyu::drawTxtByFontInRect(
    MatchString + " " + ofToString(MatchRank),
    &ofxAssets::font("vag",24),RRank,EdgeSize);
  Lyu::drawTxtByFontInRect(
    "X",&ofxAssets::font("vag",24),RX,EdgeSize);
  Lyu::drawTxtByFontInRect("Amount",
    &ofxAssets::font("vag",24),RAmtTitle,EdgeSize);
  Lyu::drawTxtByFontInRect(
    ofToString(AmountRest*amountCoef,0),
    &ofxAssets::font("vag",24),RAmt,EdgeSize);
}
