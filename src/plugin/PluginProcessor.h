// Nathan Blair June 2023
// Keaton Kowal Oct 2023

#pragma once

class StateManager;
class Gain;
class BBD;

#include <daisysp.h>
#include <juce_audio_basics/juce_audio_basics.h>

#include "PluginProcessorBase.h"

//==============================================================================
class PluginProcessor : public PluginProcessorBase {
public:
  //==============================================================================
  PluginProcessor();
  ~PluginProcessor() override;
  //==============================================================================
  void prepareToPlay(double sampleRate, int samplesPerBlock) override;
  void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;
  //==============================================================================
  void getStateInformation(juce::MemoryBlock &destData) override;
  void setStateInformation(const void *data, int sizeInBytes) override;
  //==============================================================================
  juce::AudioProcessorEditor *createEditor() override;
  //==============================================================================
  // state
  //==============================================================================
  std::unique_ptr<StateManager> state;

private:
  std::unique_ptr<Gain> gain;
  std::unique_ptr<BBD> bbd_l;
  std::unique_ptr<BBD> bbd_r;
  std::unique_ptr<daisysp::CrossFade> mix_l;
  std::unique_ptr<daisysp::CrossFade> mix_r;
  std::unique_ptr<daisysp::Svf> filter_l;
  std::unique_ptr<daisysp::Svf> filter_r;

  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginProcessor)
};
