/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
LudEQAudioProcessor::LudEQAudioProcessor()
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

LudEQAudioProcessor::~LudEQAudioProcessor()
{
}

//==============================================================================
const juce::String LudEQAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool LudEQAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool LudEQAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool LudEQAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double LudEQAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int LudEQAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int LudEQAudioProcessor::getCurrentProgram()
{
    return 0;
}

void LudEQAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String LudEQAudioProcessor::getProgramName (int index)
{
    return {};
}

void LudEQAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void LudEQAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void LudEQAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool LudEQAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void LudEQAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
}

//==============================================================================
bool LudEQAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* LudEQAudioProcessor::createEditor()
{
    /*
     -----  Changes to see how it looks -----
     */
    //return new LudEQAudioProcessorEditor (*this);
    return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void LudEQAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void LudEQAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new LudEQAudioProcessor();
}

/*
 ----- New implemented Code -----
 */

// Extremely long name of return type.
juce::AudioProcessorValueTreeState::ParameterLayout
    LudEQAudioProcessor::createParameters()
{
    // Create empty layout
    juce::AudioProcessorValueTreeState::ParameterLayout ctrl_layout;
    
    // Add Low Cut Parameter
    ctrl_layout.add
    (
        std::make_unique<juce::AudioParameterFloat>
        (
        "Low Cut",
        "Low Cut",
        juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 1.f),
        20.f
        )
    );
    
    // Add High Cut Parameter
    ctrl_layout.add
    (
        std::make_unique<juce::AudioParameterFloat>
        (
        "High Cut",
        "High Cut",
        juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 1.f),
        20000.f
        )
    );
    
    // Add Peak Freq Parameter
    ctrl_layout.add
    (
        std::make_unique<juce::AudioParameterFloat>
        (
        "Peak Freq",
        "Peak Freq",
        juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 1.f),
        1000.f
        )
    );
    
    // Add Peak Gain Parameter
    ctrl_layout.add
    (
        std::make_unique<juce::AudioParameterFloat>
        (
        "Peak Gain",
        "Peak Gain",
        juce::NormalisableRange<float>(-12.f, 12.f, 0.1f, 1.f),
        0.f
        )
    );
    
    // Add Peak Q Parameter
    ctrl_layout.add
    (
        std::make_unique<juce::AudioParameterFloat>
        (
        "Peak Q",
        "Peak Q",
        juce::NormalisableRange<float>(0.1f, 10.f, 0.05f, 1.f),
        1.f
        )
    );
    
    // The steepness of the Cutoff Frequencies shoould be selectable
    // Therefore we need a String Array with the options.
    juce::StringArray dB_per_oct;
    for (int i = 0; i < 4; ++i)
    {
        juce::String optn;
        optn << (12 + 12*i) << "dB/Oct";
        dB_per_oct.add(optn);
    }
    
    // Add those Options for High and Lowcut Slope.
    ctrl_layout.add
    (
        std::make_unique<juce::AudioParameterChoice>
        ("Low Cut Slope", "Low Cut Slope", dB_per_oct, 0)
     );
    
    ctrl_layout.add
    (
        std::make_unique<juce::AudioParameterChoice>
        ("High Cut Slope", "High Cut Slope", dB_per_oct, 0)
     );
    
    return ctrl_layout;
}
