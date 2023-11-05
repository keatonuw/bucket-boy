#include "BBD.h"

#define UPSAMP_FACT 4
#define MAX_RATE(sr) ((sr * UPSAMP_FACT) / 2)
#define MIN_RATE(sr) (sr / 100)

BBD::BBD(float sample_rate) {
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

// BBD::BBD(float sample_rate, int num_channels) {
//   output_channels = num_channels;
//   clock.Init(sample_rate);
//   clock.SetAmp(1.f);
//   clock.SetFreq(440.0);
//   clock.SetWaveform(daisysp::Oscillator::WAVE_SQUARE);
//   length = 4096 * 4;
//   pos = 0;
//   fb = 0.f;
//   fb_amt = 0.f;
//   // memset(del_line, 0, 4096 * 4 * sizeof(float));
// }
//
// void BBD::process(juce::AudioBuffer<float> &buffer) {
//   // process buffer...
//   for (int channel = 0; channel < buffer.getNumChannels(); channel++) {
//     auto *channel_data = buffer.getWritePointer(channel);
//     juce::ignoreUnused(channel_data);
//
//     if (channel == 0) {
//       for (int i = 0; i < buffer.getNumSamples(); i++) {
//         float in = channel_data[i];
//         clock.Process();
//         if (clock.IsEOR()) {
//           this->samp(in + fb * fb_amt);
//         }
//         fb = del_line[(pos + length) % length];
//         channel_data[i] = fb;
//       }
//     }
//   }
// }
//
//
// void BBD::setFeedback(float feedback) {
//   if (feedback < 1.f && feedback > 0.f) {
//     fb_amt = feedback;
//   }
// }
//
// void BBD::setLength(int len) {
//   if (len < 4096 * 4 && len > 0) {
//     this->length = len;
//   }
// }
