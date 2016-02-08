#pragma once

#include "ofMain.h"

class ColorDrop;

class ColorDropDragger
{
public:
  ColorDropDragger();
  void update();
  void clicked(ofVec2f pos, ofPtr<ColorDrop> cd);
  void dragged(ofVec2f pos);
  void released(ofVec2f pos);
  ofPtr<ColorDrop> getTgt() const;

  bool isDragging() const;

  ofVec2f getPos() const;

private:
  ofPtr<ColorDrop> cdTgt;
 
  ofVec2f Offset;
  ofVec2f Pos;

};