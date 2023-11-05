// Nathan Blair June 2023
// Keaton Kowal Oct 2023

#include "PluginProcessor.h"
#include "../audio/BBD.h"
#include "../audio/Gain.h"
#include "../parameters/StateManager.h"
#include "PluginEditor.h"

//==============================================================================
PluginProcessor::PluginProcessor() {
  state = std::make_unique<StateManager>(this);
}

PluginProcessor::~PluginProcessor() {
  // stop any threads, delete any raw pointers, remove any listeners, etc
}

//==============================================================================
void PluginProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
  // Called after the constructor, but before playback starts
  // Use this to allocate up any resources you need, and to reset any
  // variables that depend on sample rate or block size

  gain = std::make_unique<Gain>(float(sampleRate), samplesPerBlock,
                                getTotalNumOutputChannels(),
                                PARAMETER_DEFAULTS[PARAM::GAIN] / 100.0f);
  bbd_l = std::make_unique<BBD>(float(sampleRate));
  bbd_r = std::make_unique<BBD>(float(sampleRate));
}

void PluginProcessor::processBlock(juce::AudioBuffer<float> &buffer,
                                   juce::MidiBuffer &midiMessages) {
  juce::ScopedNoDenormals noDenormals;

  //--------------------------------------------------------------------------------
  // read in some parameter values here, if you want
  // in this case, gain goes from 0 to 100 (see: ../parameters/parameters.csv)
  // so we normalize it to 0 to 1
  //--------------------------------------------------------------------------------
  auto requested_gain = state->param_value(PARAM::GAIN) / 100.0f;
  auto requested_feedback = state->param_value(PARAM::FB) / 100.0f;
  auto requested_rate = state->param_value(PARAM::LEN) / 100.0f;
  auto requested_length = (int)state->param_value(PARAM::STAGES);
  auto requested_noise = state->param_value(PARAM::NOISE) / 1000.0f;

  //--------------------------------------------------------------------------------
  // process samples below. use the buffer argument that is passed in.
  // for an audio effect, buffer is filled with input samples, and you should
  // fill it with output samples for a synth, buffer is filled with zeros, and
  // you should fill it with output samples see:
  // https://docs.juce.com/master/classAudioBuffer.html
  //--------------------------------------------------------------------------------

  gain->setGain(requested_gain);
  bbd_l->setFeedback(requested_feedback);
  bbd_r->setFeedback(requested_feedback);
  bbd_l->setRate(requested_rate);
  bbd_r->setRate(requested_rate);
  bbd_l->setLength(requested_length);
  bbd_r->setLength(requested_length);
  bbd_l->setNoiseAmount(requested_noise);
  bbd_r->setNoiseAmount(requested_noise);

  for (size_t i = 0; i < 4; i++) {
    bbd_l->setTapLevel((int)i, state->param_value(PARAM::TAP_ONE + i) / 100.f);
    bbd_r->setTapLevel((int)i, state->param_value(PARAM::TAP_ONE + i) / 100.f);
  }

  // gain->process(buffer);
  auto *in_buffer = buffer.getReadPointer(0);
  auto *out_buffer = buffer.getWritePointer(0);

  for (auto sample = 0; sample < buffer.getNumSamples(); ++sample) {
    out_buffer[sample] = bbd_l->process(in_buffer[sample]);
  }

  in_buffer = buffer.getReadPointer(1);
  out_buffer = buffer.getWritePointer(1);

  for (auto sample = 0; sample < buffer.getNumSamples(); ++sample) {
    out_buffer[sample] = bbd_r->process(in_buffer[sample]);
  }

  // bbd->process(buffer);
  //--------------------------------------------------------------------------------
  // you can use midiMessages to read midi if you need.
  // since we are not using midi yet, we clear the buffer.
  //--------------------------------------------------------------------------------
  midiMessages.clear();
}

//==============================================================================
void PluginProcessor::getStateInformation(juce::MemoryBlock &destData) {
  // You should use this method to store your parameters in the memory block.
  // You could do that either as raw data, or use the XML or ValueTree classes
  // as intermediaries to make it easy to save and load complex data.

  // We will just store our parameter state, for now
  auto plugin_state = state->get_state();
  std::unique_ptr<juce::XmlElement> xml(plugin_state.createXml());
  copyXmlToBinary(*xml, destData);
}

void PluginProcessor::setStateInformation(const void *data, int sizeInBytes) {
  // You should use this method to restore your parameters from this memory
  // block, whose contents will have been created by the getStateInformation()
  // call.

  // Restore our parameters from file
  std::unique_ptr<juce::XmlElement> xmlState(
      getXmlFromBinary(data, sizeInBytes));
  state->load_from(xmlState.get());
}

juce::AudioProcessorEditor *PluginProcessor::createEditor() {
  return new AudioPluginAudioProcessorEditor(*this);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
  return new PluginProcessor();
}
