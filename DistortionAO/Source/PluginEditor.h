/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class DistortionAOAudioProcessorEditor  : public juce::AudioProcessorEditor,
    // Both of these are used to listen to changes on their respective selector.
    private juce::ComboBox::Listener,
    private juce::Slider::Listener
{
public:
    DistortionAOAudioProcessorEditor (DistortionAOAudioProcessor&);
    ~DistortionAOAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // Called whenever the Combo Box changes
    void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;

    // Called whenever one of the sliders value changes.
    void sliderValueChanged(juce::Slider* sliderThatHasChanged) override;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DistortionAOAudioProcessor& audioProcessor;

    // A ComboBox to choose the type of distortion.
    juce::ComboBox disChoice;

    // Two sliders, the first the only realy DSP knob, and the second the wet versus try knob.
    juce::Slider thresholdSlider;
    juce::Slider mixSlider;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistortionAOAudioProcessorEditor)
};
