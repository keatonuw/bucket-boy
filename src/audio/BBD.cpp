// Keaton Kowal Nov 2023

#include "BBD.h"

#define UPSAMP_FACT 4
#define MAX_RATE(sr) ((sr * UPSAMP_FACT) / 2)
#define MIN_RATE(sr) (sr / 100)

BBD::BBD(float sample_rate)
    : lo_shelf(sample_rate, false), hi_shelf(sample_rate, true) {
  clock.Init(sample_rate);
  clock.SetAmp(1.f);
  clock.SetFreq(440.0);
  clock.SetWaveform(daisysp::Oscillator::WAVE_SQUARE);
  noise.Init();

  sr = sample_rate;
  freq = 440.0;
  length = DBUF_SIZE;
  pos = 0;
  fb = 0.f;
  fb_amt = 0.f;
  noise_amt = 0.f;
  memset(del_line, 0, DBUF_SIZE * sizeof(float));
  memset(tap_lvls, 0, 4 * sizeof(float));
}

float BBD::process(float in) {
  fb = fb * fb_amt;
  float leak = 0;
  for (int i = 0; i < UPSAMP_FACT; i++) {
    leak = clock.Process();
    clock.SetFreq(freq + noise.Process() * noise_amt * 10);
    if (clock.IsEOC()) {
      sample(in + fb + (noise.Process() * noise_amt * 0.25f));
    }
  }
  fb = 0;
  for (int i = 0; i < 4; i++) {
    int l = length / (i + 1);
    fb += del_line[(pos + l) % length] * tap_lvls[i];
  }
  fb += leak * noise_amt;
  fb = hi_shelf.process(lo_shelf.process(fb));
  fb = daisysp::SoftLimit(fb);
  return fb;
}

void BBD::sample(float s) {
  del_line[pos] = s;
  pos = (pos + 1) % length;
}

void BBD::setRate(float rate) {
  if (rate > 0 && rate < 1) {
    freq = rate * MAX_RATE(sr) + MIN_RATE(sr);
    clock.SetFreq(freq);
  }
}

void BBD::setLength(int len) {
  if (len > 0 && len < DBUF_SIZE) {
    length = len;
  }
}

void BBD::setFeedback(float feedback) {
  if (feedback > 0 && feedback < 1) {
    fb_amt = feedback;
  }
}

void BBD::setNoiseAmount(float n_amt) {
  if (n_amt > 0 && n_amt < 0.5) {
    noise_amt = n_amt;
  }
}

void BBD::setTapLevel(int tap_num, float tap_lvl) {
  if (tap_num >= 0 && tap_num < 4) {
    tap_lvls[tap_num] = tap_lvl;
  }
}

void BBD::setLowGain(float lo_gain) { lo_shelf.setGain(lo_gain); }

void BBD::setLowFreq(float lo_freq) { lo_shelf.setFreq(lo_freq); }

void BBD::setHighGain(float hi_gain) { hi_shelf.setGain(hi_gain); }

void BBD::setHighFreq(float hi_freq) { hi_shelf.setFreq(hi_freq); }
