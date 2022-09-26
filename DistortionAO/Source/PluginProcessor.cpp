/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DistortionAOAudioProcessor::DistortionAOAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

DistortionAOAudioProcessor::~DistortionAOAudioProcessor()
{
}

//==============================================================================
const juce::String DistortionAOAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DistortionAOAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DistortionAOAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DistortionAOAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DistortionAOAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DistortionAOAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DistortionAOAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DistortionAOAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DistortionAOAudioProcessor::getProgramName (int index)
{
    return {};
}

void DistortionAOAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DistortionAOAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void DistortionAOAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DistortionAOAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void DistortionAOAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    // Loop through each of the channels
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        // Each time create a Write Pointer to that channel's buffe.
        auto* channelData = buffer.getWritePointer(channel);

        // Loop through all the samples.
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            // For use in the distortion
            auto input = channelData[sample];

            // A clean sample that is not touched by the distortion algorithm.
            auto cleanOut = channelData[sample];

            // The Distortion Algorithm depends on the one choosen.
            switch (menuChoice)
            {
            // Hard Clipping
            // Hard Clipping clips the sample's value to the threshold when its absolute value is greater than the threshold.
            case 1:
                if (input > threshold)
                {
                    input = threshold;
                }
                else if (input < -threshold)
                {
                    input = -threshold;
                }
                else
                {
                    input = input;
                }
                break;
            // Soft Clipping Exp
            // Similar to Hard Clipping except its input is slightly pushed away with the expf function.
            case 2:
                if (input > threshold)
                {
                    input = 1.0f - expf(-input);
                }
                else if (input < -threshold)
                {
                    input = -1.0f + expf(input);
                }
                else
                {
                    input = input;
                }
                break;
            // Half-Wave Rectifier
            // Only keeps half the waveform, or in other words anything above threshold is kept, but nothing below it.
            case 3:
                if (input > threshold)
                {
                    input = input;
                }
                else
                    input = 0;
                break;
            default:
                // If not a valid choice, we have some kind of error.
                jassertfalse;
                    
            }
            // Finally return the sample to channelData with the correct dry / wet ratio
            channelData[sample] = ((1 - mix) * cleanOut) + (mix * input);
        }
    }
}

//==============================================================================
bool DistortionAOAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DistortionAOAudioProcessor::createEditor()
{
    return new DistortionAOAudioProcessorEditor (*this);
}

//==============================================================================
void DistortionAOAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DistortionAOAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DistortionAOAudioProcessor();
}
