#include "ColorDropDragger.h"
#include "ColorDrop.h"

ColorDropDragger::ColorDropDragger()
{
  
}

void ColorDropDragger::clicked( ofVec2f pos, ofPtr<ColorDrop> cd )
{
  if(cd->inside(pos)&&cdTgt==NULL)
  {
    cdTgt = cd;
    Offset = cd->getGlobalPosition()-pos;
    Pos = pos;     
  }
}

void ColorDropDragger::dragged( ofVec2f pos )
{
  Pos = pos;  
}

void ColorDropDragger::released( ofVec2f pos )
{
  dragged(pos);
  cdTgt.reset(); 
}

ofPtr<ColorDrop> ColorDropDragger::getTgt() const
{
  return cdTgt;
}

void ColorDropDragger::update()
{
  if(cdTgt.use_count()>0)
  {    
    cdTgt->setGlobalPosition(Pos+Offset);   
  }
}

ofVec2f ColorDropDragger::getPos() const
{
  return Pos;
}

bool ColorDropDragger::isDragging() const
{
  return cdTgt.use_count()>0;
}
