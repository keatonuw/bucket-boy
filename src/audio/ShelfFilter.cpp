// Keaton Kowal Nov 2023
// Based on code from Zolzer's DAFX

#include "ShelfFilter.h"

ShelfFilter::ShelfFilter(float sample_rate, bool hi_shelf) {
  hp = hi_shelf;
  sr = sample_rate;
  setGainAndFreq(0.0f, 4000.0f);
}

float ShelfFilter::process(float in) {
  float xhl = in - c * this->xh;
  float ap_y = c * xhl + this->xh;
  this->xh = xhl;
  if (hp)
    return 0.5f * h0 * (in - ap_y) + in;
  return 0.5f * h0 * (in - ap_y) + in;
}

void ShelfFilter::setGain(float gain) { setGainAndFreq(gain, f); }

void ShelfFilter::setFreq(float freq) { setGainAndFreq(g, freq); }

void ShelfFilter::setGainAndFreq(float gain, float freq) {
  wc = 2 * freq / sr;
  v0 = std::pow(10.f, gain / 20.f);
  h0 = v0 - 1;
  if (g >= 0) {
    c = (std::tan(PI_F * wc / 2) - 1) / (std::tan(PI_F * wc / 2) + 1); // boost
  } else {
    c = (std::tan(PI_F * wc / 2) - v0) / (std::tan(PI_F * wc / 2) + v0); // cut
  }
  g = gain;
  f = freq;
}
