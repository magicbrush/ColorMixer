#pragma once

#include "ofMain.h"

class ColorDropDragger;
class ColorDrop;
class CurveMovingParticle;

class ColorHole:
  public ofNode
{
  friend struct ColorOutputBatch;
private:
  struct ColorOutputBatch
  {    
    bool bOverflow;
    float Amount;
    float AmountRest;
    ofFloatColor CrAbsorb;
    int MatchRank;
    string MatchString;
    float Score;
    float ScoreRest;

    ColorOutputBatch(bool overflow=false);
    void computeScore();
    bool update(ColorHole* parent);
    void update_Absorbing( ColorHole* parent,float dt );
    void gettingScore( ofVec2f parentPos, float dt, string Str );
    void update_Overflow( ofVec3f Pos, float dt );
    void draw(ColorHole* parent);
    void draw_absorbing( ColorHole* parent );
    void getInfoPosParams( 
      float &scnSize, 
      float &cellWd, float &cellHt, 
      ofVec2f &InfoPos, 
      ColorHole* parent );
    void drawRankAmount( 
      ofVec2f &InfoPos, 
      float cellWd,
      float cellHt, 
      float moveRatio );
    void drawRandAmountText( 
      ofRectangle RRankTitle, 
      ofRectangle RRank, 
      ofRectangle RX, 
      ofRectangle RAmtTitle, 
      ofRectangle RAmt,
      float EdgeSize = 0.0f);

    void draw_overflow( ofVec3f Pos );
    map<string, float> AnimDur; 
    map<string, float> AnimDurDef;
    map<string, bool> InfoPoped;
  };
public:  
  ColorHole(
    ofVec2f pos,
    ofFloatColor crTgt, 
    float capacity=1000.0f);

  void absorb(ofPtr<ColorDropDragger> dragger); 

  void absorbBatch();

  virtual void customDraw();
  void drawInfo();

private:
  void drawTicks( ofColor Cr, ofVec2f &Offset );

private:
  static vector<float> MatchLevelThres;
  static vector<string> MatchLevelRemarks;
  static vector<float> MatchLevelScoreCoefs;
private:
  ofParameter<ofFloatColor> CrTgt;
  ofParameter<float> Capacity;
  float Radius;
  void CapacityChanged(float &c);

  ofPtr<ColorDropDragger> Dragger;
  ofPtr<ColorDrop> DraggedCrDrop;
  bool WaitingForAbsorb;
  
  queue<ColorOutputBatch> Outputs;

public:
  static float amountCoef;
  static float ScoreTotal;
  static ofVec2f ScoreTotalPos;
  static void drawTotalScore();
  static void drawScoreParticles();
  static void updateScoreParticles();
  static vector<ofPtr<CurveMovingParticle> > ScorePts;

};