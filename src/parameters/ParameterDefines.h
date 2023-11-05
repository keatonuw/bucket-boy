#pragma once
#include <juce_core/juce_core.h>
enum PARAM {
  GAIN,
  FB,
  LEN,
  STAGES,
  NOISE,
  TAP_ONE,
  TAP_TWO,
  TAP_THREE,
  TAP_FOUR,
  TOTAL_NUMBER_PARAMETERS
};
static const std::array<juce::Identifier, PARAM::TOTAL_NUMBER_PARAMETERS>
    PARAMETER_IDS{
        "GAIN",    "FB",      "LEN",       "STAGES",   "NOISE",
        "TAP_ONE", "TAP_TWO", "TAP_THREE", "TAP_FOUR",
    };
static const std::array<juce::String, PARAM::TOTAL_NUMBER_PARAMETERS>
    PARAMETER_NAMES{
        "GAIN",    "FB",      "LEN",       "STAGES",   "NOISE",
        "TAP_ONE", "TAP_TWO", "TAP_THREE", "TAP_FOUR",
    };
static const std::array<juce::NormalisableRange<float>,
                        PARAM::TOTAL_NUMBER_PARAMETERS>
    PARAMETER_RANGES{
        juce::NormalisableRange<float>(0.0f, 100.0f, 0.01f, 1.0f),
        juce::NormalisableRange<float>(0.0f, 100.0f, 0.01f, 0.4f),
        juce::NormalisableRange<float>(0.0f, 100.0f, 0.01f, 0.4f),
        juce::NormalisableRange<float>(128.0f, 16384.0f, 1.0f, 1.0f),
        juce::NormalisableRange<float>(0.0f, 100.0f, 0.01f, 1.0f),
        juce::NormalisableRange<float>(0.0f, 100.0f, 0.01f, 1.0f),
        juce::NormalisableRange<float>(0.0f, 100.0f, 0.01f, 1.0f),
        juce::NormalisableRange<float>(0.0f, 100.0f, 0.01f, 1.0f),
        juce::NormalisableRange<float>(0.0f, 100.0f, 0.01f, 1.0f),
    };
static const std::array<float, PARAM::TOTAL_NUMBER_PARAMETERS>
    PARAMETER_DEFAULTS{
        50.0f, 0.0f, 25.0f, 1024.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    };
static const std::array<bool, PARAM::TOTAL_NUMBER_PARAMETERS>
    PARAMETER_AUTOMATABLE{
        true, true, true, true, true, true, true, true, true,
    };
static const std::array<juce::String, PARAM::TOTAL_NUMBER_PARAMETERS>
    PARAMETER_NICKNAMES{
        "Mix",   "Feedback", "Clock Rate", "Delay Stages", "Noise",
        "Tap 1", "Tap 2",    "Tap 3",      "Tap 4",
    };
static const std::array<juce::String, PARAM::TOTAL_NUMBER_PARAMETERS>
    PARAMETER_SUFFIXES{
        "%", "%", "%", "st", "%", "%", "%", "%", "%",
    };
static const std::array<juce::String, PARAM::TOTAL_NUMBER_PARAMETERS>
    PARAMETER_TOOLTIPS{
        "Mix Parameter",      "Feedback Amount",    "Delay Length",
        "BBD Stages",         "Noise Amount",       "Short Tap Level",
        "Medium 1 Tap Level", "Medium 2 Tap Level", "Long Tap Level",
    };
static const std::array<std::vector<juce::String>,
                        PARAM::TOTAL_NUMBER_PARAMETERS>
    PARAMETER_TO_STRING_ARRS{
        std::vector<juce::String>{}, std::vector<juce::String>{},
        std::vector<juce::String>{}, std::vector<juce::String>{},
        std::vector<juce::String>{}, std::vector<juce::String>{},
        std::vector<juce::String>{}, std::vector<juce::String>{},
        std::vector<juce::String>{},
    };
