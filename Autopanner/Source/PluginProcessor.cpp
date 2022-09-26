/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AutopannerAudioProcessor::AutopannerAudioProcessor()
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
    // Adds an Audio Parameter Float to gain with a default value of 0.5f
    addParameter(gain = new juce::AudioParameterFloat("GAIN", "Gain", 0.0f, 1.0f, 0.5f));
    addParameter(ms = new juce::AudioParameterFloat("MS", "ms", 10.0f, 5000.0f, 250.0f));
}

AutopannerAudioProcessor::~AutopannerAudioProcessor()
{
}

//==============================================================================
const juce::String AutopannerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AutopannerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AutopannerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AutopannerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AutopannerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AutopannerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AutopannerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AutopannerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String AutopannerAudioProcessor::getProgramName (int index)
{
    return {};
}

void AutopannerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void AutopannerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void AutopannerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AutopannerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void AutopannerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    // Get a write pointer which can be accessed like an array to both channels.
    auto* channelDataL = buffer.getWritePointer(0);
    auto* channelDataR = buffer.getWritePointer(1);

    // Get the value of the ms Float Param
    float mSeconds = ms->get();

    // Gets the value of the gain parameter.
    float gSlider = gain->get();

    // Gets the total amount of samples per second to go through.
    int numberSamples = getSampleRate() * mSeconds;

    // Gets the total sampleRate
    //const int numberSamples = getSampleRate();

    // How much to increment per sample in rads
    const float radsPerSample = (2 * juce::double_Pi) / numberSamples;

    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        // Get a sample from both channels
        auto inputL = channelDataL[sample];
        auto inputR = channelDataR[sample];

        // Create a sine wave of height 0.5 but then add 0.5 so it goes between 0 and 1
        //float sinValue = 0.5f * std::sin(nextRad) + 0.5f;

        // Multiply the sample by the current sine value and its opposite.
        //inputL = inputL * (1.0f - sinValue);
        //inputR = inputR * sinValue;

        // Get a sine wave between 0 and 1
        float sinValue = std::sin(nextRad) + 1.0f;

        // Get the sinValue
        sinValue = (sinValue * juce::double_Pi) / 4.0f;

        // The left channel follows the laws of Cos whereas the right channel follows the laws of sine.
        inputL = inputL * std::cos(sinValue);
        inputR = inputR * std::sin(sinValue);

        // Mutiply the sample by the total amount of panning.
        //inputL = inputL * (1.0f - gSlider);
        //inputR = inputR * gSlider;

        // Output the modified samples
        channelDataL[sample] = inputL;
        channelDataR[sample] = inputR;

        // Increment the nextRad by the amount of rads we need per sample.
        nextRad += radsPerSample;
    }
    // When nextRad gets to be bigger than the total amount of samples per second...
    if (nextRad > numberSamples)
    {
        // Reset nextRad to 0.0f;
        nextRad = 0.0f;
    }
}

//==============================================================================
bool AutopannerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AutopannerAudioProcessor::createEditor()
{
    //return new AutopannerAudioProcessorEditor (*this);

    // Adds a default JUCE style interface.
    return new juce::GenericAudioProcessorEditor(this);
}

//==============================================================================
void AutopannerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void AutopannerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AutopannerAudioProcessor();
}
