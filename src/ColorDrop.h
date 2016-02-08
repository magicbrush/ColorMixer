#pragma once

#include "ofMain.h"


class ColorDrop:
  public ofNode
{
public:
  ColorDrop(ofFloatColor C, float amt = 100.0f);
  
  void update(); 
  bool inside(ofVec3f P);
  
  bool contact(ColorDrop* another);
  float getRadius() const;

  float getAmt() const;
  void setAmt(float amt);  

  ofFloatColor getColor() const;
  void setColor(ofFloatColor cr);

protected:
  virtual void customDraw() override;

private:
  ofFloatColor Cr;
  float Amt;

public:
  static ofPtr<ColorDrop> randColorDrop(
    ofVec2f AmtBound=ofVec2f(10,1000));

};