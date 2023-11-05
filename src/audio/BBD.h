#pragma once

#include "Noise/whitenoise.h"
#include "ShelfFilter.h"
#include <daisysp.h>
#include <juce_audio_basics/juce_audio_basics.h>

#define DBUF_SIZE (4096 * 4)

class BBD {
public:
  BBD(float sample_rate);
  float process(float in);
  void setFeedback(float fb);
  void setRate(float rate);
  void setLength(int len);
  void setNoiseAmount(float n_amt);
  void setTapLevel(int tap_num, float tap_lvl);

  void setLowGain(float lo_gain);
  void setLowFreq(float lo_freq);
  void setHighGain(float hi_gain);
  void setHighFreq(float hi_freq);

private:
  int length;
  float tap_lvls[4];
  int pos;
  float sr;
  float freq;
  float del_line[DBUF_SIZE];
  float fb;
  float fb_amt;
  float noise_amt;
  daisysp::Oscillator clock;
  daisysp::WhiteNoise noise;
  ShelfFilter lo_shelf;
  ShelfFilter hi_shelf;

  void sample(float s);
};
