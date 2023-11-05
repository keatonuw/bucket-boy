#pragma once

#include "Noise/whitenoise.h"
#include <daisysp.h>
#include <juce_audio_basics/juce_audio_basics.h>

#define DBUF_SIZE (4096 * 4)

// class BBD {
// public:
//   BBD(float sample_rate, int num_channels);
//   void process(juce::AudioBuffer<float> &buffer);
//   void setFeedback(float fb);
//   void setLength(int length);
//
// private:
//   int output_channels;
//   int length;
//   int pos;
//   float del_line[4096 * 4];
//   float fb;
//   float fb_amt;
//   daisysp::Oscillator clock;
//
//   void samp(float s);
// };

class BBD {
public:
  BBD(float sample_rate);
  float process(float in);
  void setFeedback(float fb);
  void setRate(float rate);
  void setLength(int len);
  void setNoiseAmount(float n_amt);
  void setTapLevel(int tap_num, float tap_lvl);

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

  void sample(float s);
};
