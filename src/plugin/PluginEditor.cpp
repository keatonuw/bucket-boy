// Nathan Blair January 2023

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

  // add slider BEFORE setting size
  gain_slider = std::make_unique<ParameterSlider>(state, PARAM::GAIN);
  gain_slider->setColour(1, juce::Colour(0xffffffff));
  addAndMakeVisible(*gain_slider);

  fb_slider = std::make_unique<ParameterSlider>(state, PARAM::FB);
  fb_slider->setColour(1, juce::Colour(0xffffffff));
  addAndMakeVisible(*fb_slider);

  length_slider = std::make_unique<ParameterSlider>(state, PARAM::LEN);
  length_slider->setColour(1, juce::Colour(0xffffffff));
  addAndMakeVisible(*length_slider);

  stages_slider = std::make_unique<ParameterSlider>(state, PARAM::STAGES);
  stages_slider->setColour(1, juce::Colour(0xffffffff));
  addAndMakeVisible(*stages_slider);

  noise_slider = std::make_unique<ParameterSlider>(state, PARAM::NOISE);
  noise_slider->setColour(1, juce::Colour(0xffffffff));
  addAndMakeVisible(*noise_slider);

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
  // set the position of your components here
  int slider_size = proportionOfWidth(0.1f);
  int slider_x = proportionOfWidth(0.25f) - (slider_size / 2);
  int slider_y = proportionOfHeight(0.25f) - (slider_size / 2);
  gain_slider->setBounds(slider_x, slider_y, slider_size, slider_size);

  slider_x = proportionOfWidth(0.5f) - (slider_size / 2);
  fb_slider->setBounds(slider_x, slider_y, slider_size, slider_size);

  slider_x = proportionOfWidth(0.75f) - (slider_size / 2);
  length_slider->setBounds(slider_x, slider_y, slider_size, slider_size);

  slider_y = proportionOfHeight(0.5f) - (slider_size / 2);

  slider_x = proportionOfWidth(0.25f) - (slider_size / 2);
  stages_slider->setBounds(slider_x, slider_y, slider_size, slider_size);
  slider_x = proportionOfWidth(0.75f) - (slider_size / 2);
  noise_slider->setBounds(slider_x, slider_y, slider_size, slider_size);

  for (int i = 0; i < 4; i++) {
    slider_y = proportionOfHeight(0.75f) - (slider_size / 2);
    slider_x =
        proportionOfWidth((0.75f / 4) * (float)(1 + i)) - (slider_size / 2);
    gain_sliders[i]->setBounds(slider_x, slider_y, slider_size, slider_size);
  }
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
