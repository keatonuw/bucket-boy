#pragma once

#include <daisysp.h>
#include <juce_audio_basics/juce_audio_basics.h>

class ShelfFilter {
public:
  ShelfFilter(float sample_rate, bool hi_shelf);
  float process(float in);
  void setGain(float gain);
  void setFreq(float freq);

private:
  float wc; /* normalized center freq */
  float v0;
  float h0;
  float c;
  bool hp;
  float xh;
  float sr;
  float f;
  float g; /* gain */

  void setGainAndFreq(float gain, float freq);
};
