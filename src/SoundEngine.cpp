#include "SoundEngine.h"

namespace DrawGame
{
  SoundEngine::SoundSetting::SoundSetting(string Name)
  {
    S.setName(Name);
    S.add(File.set("File","..\\"));
    S.add(Loop.set("Loop",false));
    S.add(Multiplay.set("MultiPlay",true));
    S.add(Pan.set("Pan",0.0f,0.0f,1.0f));
    S.add(Position.set("Position",0.0f,0.0f,1.0f));
    S.add(Speed.set("Speed",1.0f,1/5.0f,5.0f));
    S.add(Volume.set("Volume",1.0f,0.01f,2.0f));
  }

  SoundEngine::SoundEngine( string XmlFile /*= "sounds/sounds.xml"*/ )
  {
    ofXml X;
    X.load(XmlFile);
    X.setTo("Sounds");
    int numChd = X.getNumChildren();
    for(int i=0;i<numChd;i++)
    {
      X.setToChild(i);
      string Name = X.getName();
      SoundSetting Setting(Name);     
      X.setToParent(); 

      X.deserialize(Setting.S);
      Settings[Name]= Setting;

      ofXml X2;
      X2.serialize(Setting.S);
      X2.save("test.xml");
    }
    
    for(auto s:Settings)
    {
      string Name = s.first;      
      string FilePath = s.second.File;
      ofSoundPlayer SP;
      SP.loadSound(FilePath);
      Sounds[Name]=SP;
      resetSoundPlayerSetting(Name);
    } 
  }

  SoundEngine::~SoundEngine()
  {

  }

  void SoundEngine::playSound( string Name )
  {
    resetSoundPlayerSetting(Name);
    Sounds[Name].play();
  }

  void SoundEngine::playSound( 
    string Name, float spd, float vol, float pan, bool bStop )
  {
    resetSoundPlayerSetting(Name);
    if(bStop)Sounds[Name].stop();
    Sounds[Name].setSpeed(spd);
    Sounds[Name].setVolume(vol);
    Sounds[Name].setPan(pan);
    Sounds[Name].play();
  }

  void SoundEngine::stopSound( string Name )
  {
    Sounds[Name].stop();
  }

  void SoundEngine::resetSoundPlayerSetting( string Name )
  {
    ofSoundPlayer SP = Sounds[Name];
    SoundSetting Setting = Settings[Name];
    SP.setLoop(Setting.Loop);
    SP.setMultiPlay(Setting.Multiplay);
    SP.setPan(Setting.Pan);
    SP.setPosition(Setting.Position);
    SP.setSpeed(Setting.Speed);
    SP.setVolume(Setting.Volume);
  }

  

}