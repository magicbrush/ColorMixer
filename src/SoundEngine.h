#pragma once

#include "ofMain.h"
#include "SoundEngine.h"
#include "SingletonTemplate.h"

#define SOUNDENGINE (Lyu::singleton<DrawGame::SoundEngine>::instance())

namespace DrawGame
{
  class SoundEngine
  {
  public:
    struct SoundSetting
    {
      SoundSetting(string Name="");
      ofParameter<string> File;
      ofParameter<bool> Loop;
      ofParameter<bool> Multiplay;
      ofParameter<float> Pan;
      ofParameter<float> Position;
      ofParameter<float> Speed;
      ofParameter<float> Volume;
      ofParameterGroup S;
    };

  public:
    SoundEngine(string XmlFile = "sounds/sounds.xml");
    ~SoundEngine();  
    void playSound(string Name);
    void playSound(
      string Name,      
      float spd,
      float vol,
      float pan=0.0f,
      bool bStop=false);
    void stopSound(string Name);
    void resetSoundPlayerSetting( string Name );

    map<string,SoundSetting> Settings;
    map<string,ofSoundPlayer> Sounds;       
  };

}
