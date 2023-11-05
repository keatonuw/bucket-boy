#include "ParameterSlider.h"
#include "../parameters/StateManager.h"

ParameterSlider::ParameterSlider(StateManager *s, size_t p_id)
    : juce::SettableTooltipClient(), juce::Component(), param_id(p_id + 1),
      state(s) {
  // param_id is initialized to p_id + 1 so that update_param_id runs on the
  // first call
  update_param_id(p_id);
  setOpaque(true);
  setColour(
      ColourIds::sliderColourId,
      juce::Colour(
          0xff000000)); // to change the colour of the slider, set colour id 1
}

ParameterSlider::~ParameterSlider() {}

void ParameterSlider::paint(juce::Graphics &g) {
  // paint background
  g.fillAll(findColour(ColourIds::backgroundColourId, true));

  // keep up to date with the parameter via polling
  auto cur_val = state->param_value(param_id);
  auto normed_val = PARAMETER_RANGES[param_id].convertTo0to1(cur_val);
  jassert(normed_val >= 0 && normed_val <= 1.0f);

  draw_rotary_slider(g, normed_val);

  // draw text
  g.setColour(findColour(ColourIds::sliderColourId, true));
  auto param_name = PARAMETER_NICKNAMES[param_id];
  auto text = state->get_parameter_text(param_id);
  g.drawText(param_name, 0, 0, getWidth(), proportionOfHeight(0.25f),
             juce::Justification::centred, true);
  g.drawText(text, 0, proportionOfHeight(0.75f), getWidth(),
             proportionOfHeight(0.25f), juce::Justification::centred, true);

  // draw bounding box
  g.drawRect(getLocalBounds());
}

void ParameterSlider::update_param_id(size_t p_id) {
  if (param_id == p_id) {
    return;
  }

  param_id = p_id;
  setTooltip(PARAMETER_TOOLTIPS[param_id]);
  setName(PARAMETER_NICKNAMES[param_id]);
}

void ParameterSlider::update_slider_sensitivity(float pixels_per_percent_) {
  pixels_per_percent = pixels_per_percent_;
}

void ParameterSlider::mouseDown(const juce::MouseEvent &e) {
  if (e.mods.isRightButtonDown()) {
    // right click to reset
    state->reset_parameter(param_id);
  } else {
    // left click to start dragging
    // last_mouse_position = e.getPosition();
    cur_normed_value =
        PARAMETER_RANGES[param_id].convertTo0to1(state->param_value(param_id));
    last_mouse_position = e.getPosition();
  }
}

void ParameterSlider::mouseDoubleClick(const juce::MouseEvent &e) {
  // double click to reset
  state->reset_parameter(param_id);
  juce::ignoreUnused(e);
}

// void ParameterSlider::mouseUp(const juce::MouseEvent& e) {
// }

void ParameterSlider::mouseDrag(const juce::MouseEvent &e) {
  // change parameter value
  auto change = e.getPosition() - last_mouse_position;
  last_mouse_position = e.getPosition();
  auto speed = (e.mods.isShiftDown() ? 20.0f : 1.0f) * pixels_per_percent;
  auto slider_change = float(change.getX() - change.getY()) / speed;
  cur_normed_value += slider_change;
  state->set_parameter_normalized(param_id, cur_normed_value);
}

void ParameterSlider::draw_rotary_slider(juce::Graphics &g, float slider_pos,
                                         float x, float y, float w, float h) {
  // draw the rotary slider
  const float width = w * getWidth();
  const float height = h * getHeight();
  const float x_ = x * getWidth();
  const float y_ = y * getWidth();
  const float rotaryStartAngle = -3.0f * juce::MathConstants<float>::pi / 4.0f;
  const float rotaryEndAngle = 3.0f * juce::MathConstants<float>::pi / 4.0f;
  ;
  const float radius = std::min(width / 2, height / 2) - 2.0f;
  const float centreX = x_ + width * 0.5f;
  const float centreY = y_ + height * 0.5f;
  const float rw = radius * 2.0f;
  const float angle =
      rotaryStartAngle + slider_pos * (rotaryEndAngle - rotaryStartAngle);

  juce::Path p;
  p.addEllipse(-0.5f * rw, -0.5f * rw, rw, rw);
  juce::PathStrokeType(rw * 0.05f).createStrokedPath(p, p);
  p.addLineSegment(juce::Line<float>(0.0f, 0.0f, 0.0f, -radius), rw * 0.05f);
  g.setColour(findColour(ColourIds::sliderColourId));
  g.fillPath(
      p, juce::AffineTransform::rotation(angle).translated(centreX, centreY));
}
