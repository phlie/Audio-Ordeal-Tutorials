/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DistortionAOAudioProcessorEditor::DistortionAOAudioProcessorEditor (DistortionAOAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Adds the possible items to choose from for the type of distortion.
    disChoice.addItem("Hard Clip", 1);
    disChoice.addItem("Soft Clip", 2);
    disChoice.addItem("Half-Wave Rect", 3);
    
    // Set the default choice as 1, Hard Clip.
    disChoice.setSelectedId(1);
    addAndMakeVisible(disChoice);

    // Set up the threshold slider
    thresholdSlider.setRange(0.0f, 1.0f, 0.001f);
    thresholdSlider.addListener(this);
    addAndMakeVisible(thresholdSlider);

    // Setup the mix slider
    mixSlider.setRange(0.0f, 1.0f, 0.001f);
    mixSlider.addListener(this);
    addAndMakeVisible(mixSlider);

    // Define the size of the plugin.
    setSize (300, 250);
}

DistortionAOAudioProcessorEditor::~DistortionAOAudioProcessorEditor()
{
}

//==============================================================================
void DistortionAOAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::black);

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
}

void DistortionAOAudioProcessorEditor::resized()
{
    // Set the bounds for all three selectors.
    disChoice.setBounds(50, 50, 200, 50);
    thresholdSlider.setBounds(50, 100, 200, 50);
    mixSlider.setBounds(50, 150, 200, 50);
}

void DistortionAOAudioProcessorEditor::comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged)
{
    // Set the menu choice to be what was selected in the combo box.
    audioProcessor.menuChoice = comboBoxThatHasChanged->getSelectedId();
}

void DistortionAOAudioProcessorEditor::sliderValueChanged(juce::Slider* sliderThatHasChanged)
{
    // If it is the mix slider, pass the value back to the backend.
    if (&mixSlider == sliderThatHasChanged)
    {
        audioProcessor.mix = sliderThatHasChanged->getValue();
    }
    // Else if it is the threshold slider, update the AudioProcessor's threshold value.
    else if (&thresholdSlider == sliderThatHasChanged)
    {
        audioProcessor.threshold = sliderThatHasChanged->getValue();
    }
}
