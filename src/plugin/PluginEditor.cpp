// Nathan Blair January 2023
// Keaton Kowal Nov 2023

#include "PluginEditor.h"
#include "../interface/ParameterSlider.h"
#include "../parameters/StateManager.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(
    PluginProcessor &p)
    : AudioProcessorEditor(&p), processorRef(p) {
  state = processorRef.state.get();
  startTimerHz(int(TIMER_HZ));

  // INIT UNDO/REDO
  undo_manager = state->get_undo_manager();

  auto fg_color = juce::Colour(0xffffffff);

  // add slider BEFORE setting size
  gain_slider = std::make_unique<ParameterSlider>(state, PARAM::GAIN);
  gain_slider->setColour(1, fg_color);
  addAndMakeVisible(*gain_slider);

  fb_slider = std::make_unique<ParameterSlider>(state, PARAM::FB);
  fb_slider->setColour(1, fg_color);
  addAndMakeVisible(*fb_slider);

  length_slider = std::make_unique<ParameterSlider>(state, PARAM::LEN);
  length_slider->setColour(1, fg_color);
  addAndMakeVisible(*length_slider);

  stages_slider = std::make_unique<ParameterSlider>(state, PARAM::STAGES);
  stages_slider->setColour(1, fg_color);
  addAndMakeVisible(*stages_slider);

  noise_slider = std::make_unique<ParameterSlider>(state, PARAM::NOISE);
  noise_slider->setColour(1, fg_color);
  addAndMakeVisible(*noise_slider);

  lo_freq_slider = std::make_unique<ParameterSlider>(state, PARAM::LOW_FREQ);
  lo_freq_slider->setColour(1, fg_color);
  addAndMakeVisible(*lo_freq_slider);

  lo_gain_slider = std::make_unique<ParameterSlider>(state, PARAM::LOW_GAIN);
  lo_gain_slider->setColour(1, fg_color);
  addAndMakeVisible(*lo_gain_slider);

  hi_freq_slider = std::make_unique<ParameterSlider>(state, PARAM::HIGH_FREQ);
  hi_freq_slider->setColour(1, fg_color);
  addAndMakeVisible(*hi_freq_slider);

  hi_gain_slider = std::make_unique<ParameterSlider>(state, PARAM::HIGH_GAIN);
  hi_gain_slider->setColour(1, fg_color);
  addAndMakeVisible(*hi_gain_slider);

  lpf_slider = std::make_unique<ParameterSlider>(state, PARAM::LP_FREQ);
  lpf_slider->setColour(1, fg_color);
  addAndMakeVisible(*lpf_slider);

  for (int i = 0; i < 4; i++) {
    gain_sliders[i] =
        std::make_unique<ParameterSlider>(state, PARAM::TAP_ONE + i);
    gain_sliders[i]->setColour(1, juce::Colour(0xffffffff));
    addAndMakeVisible(*gain_sliders[i]);
  }

  // some settings about UI
  setOpaque(true);
  setSize(W, H);
  setColour(0, juce::Colour(0x36u, 0x44u, 0x38u)); // background color

  // resizable window
  setResizable(true, true);
  setResizeLimits((W * 4) / 5, (H * 4) / 5, (W * 3) / 2, (H * 3) / 2);
  getConstrainer()->setFixedAspectRatio(float(W) / float(H));
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor() {
  // remove any listeners here

  // also, if we have a lookAndFeel object we should call:
  // setLookAndFeel(nullptr);
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint(juce::Graphics &g) {
  // Our component is opaque, so we must completely fill the background with a
  // solid colour
  g.fillAll(findColour(0));
}

void AudioPluginAudioProcessorEditor::resized() {
  // create the layout. desired layout looks like:
  // noise, gain 1, gain 2, gain 3, gain 4
  // lo freq, lo gain, hi gain, hi freq
  // clock rate, stages, fb, lpf, mix

  int size = proportionOfWidth(0.1f);
  int row_1 = proportionOfHeight(0.10f) - (size / 2);
  int row_2 = proportionOfHeight(0.30f) - (size / 2);
  int row_3 = proportionOfHeight(0.50f) - (size / 2);

  int inc_5 = proportionOfWidth(1.f / 7.f);
  int inc_4 = proportionOfWidth(1.f / 6.f);

  noise_slider->setBounds(inc_5, row_1, size, size);
  for (int i = 0; i < 4; i++) {
    gain_sliders[i]->setBounds(inc_5 * (i + 2), row_1, size, size);
  }

  lo_freq_slider->setBounds(inc_4 * 1, row_2, size, size);
  lo_gain_slider->setBounds(inc_4 * 2, row_2, size, size);
  hi_freq_slider->setBounds(inc_4 * 3, row_2, size, size);
  hi_gain_slider->setBounds(inc_4 * 4, row_2, size, size);

  length_slider->setBounds(inc_5 * 1, row_3, size, size);
  stages_slider->setBounds(inc_5 * 2, row_3, size, size);
  fb_slider->setBounds(inc_5 * 3, row_3, size, size);
  lpf_slider->setBounds(inc_5 * 4, row_3, size, size);
  gain_slider->setBounds(inc_5 * 5, row_3, size, size);
}

void AudioPluginAudioProcessorEditor::timerCallback() {
  // repaint UI and note that we have updated ui, if parameter values have
  // changed
  if (state->any_parameter_changed.exchange(false)) {
    if (state->get_parameter_modified(PARAM::GAIN)) {
      gain_slider->repaint();
    }
    if (state->get_parameter_modified(PARAM::FB)) {
      fb_slider->repaint();
    }
    if (state->get_parameter_modified(PARAM::LEN)) {
      length_slider->repaint();
    }
    if (state->get_parameter_modified(PARAM::STAGES)) {
      stages_slider->repaint();
    }
    if (state->get_parameter_modified(PARAM::NOISE)) {
      noise_slider->repaint();
    }
    if (state->get_parameter_modified(PARAM::LOW_FREQ)) {
      lo_freq_slider->repaint();
    }
    if (state->get_parameter_modified(PARAM::LOW_GAIN)) {
      lo_gain_slider->repaint();
    }
    if (state->get_parameter_modified(PARAM::HIGH_FREQ)) {
      hi_freq_slider->repaint();
    }
    if (state->get_parameter_modified(PARAM::HIGH_GAIN)) {
      hi_gain_slider->repaint();
    }
    if (state->get_parameter_modified(PARAM::LP_FREQ)) {
      lpf_slider->repaint();
    }
    if (state->get_parameter_modified(PARAM::TAP_ONE) ||
        state->get_parameter_modified(PARAM::TAP_TWO) ||
        state->get_parameter_modified(PARAM::TAP_THREE) ||
        state->get_parameter_modified(PARAM::TAP_FOUR)) {
      for (int i = 0; i < 4; i++) {
        gain_sliders[i]->repaint();
      }
    }
  }
  state->update_preset_modified();

  if (timer_counter % (TIMER_HZ / UNDO_HZ) == 0) {
    if (!isMouseButtonDownAnywhere()) {
      processorRef.state->get_undo_manager()->beginNewTransaction();
    }
  }

  timer_counter++;
}
