/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
const juce::MidiMessageSequence* theSequence;

//constructor
MidiKeyDetectorAudioProcessor::MidiKeyDetectorAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    )
#endif
{


}
//DeContructor
MidiKeyDetectorAudioProcessor::~MidiKeyDetectorAudioProcessor()
{
}

//Methods
const juce::String MidiKeyDetectorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MidiKeyDetectorAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool MidiKeyDetectorAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool MidiKeyDetectorAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double MidiKeyDetectorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MidiKeyDetectorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MidiKeyDetectorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MidiKeyDetectorAudioProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String MidiKeyDetectorAudioProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return {};
}

void MidiKeyDetectorAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
    juce::ignoreUnused(index);
    juce::ignoreUnused(newName);
}

//==============================================================================
void MidiKeyDetectorAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::ignoreUnused(sampleRate);
    juce::ignoreUnused(samplesPerBlock);

    nextStartTime = -1.0;
}

void MidiKeyDetectorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MidiKeyDetectorAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
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

void MidiKeyDetectorAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    //MIDI 
}

//==============================================================================
bool MidiKeyDetectorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MidiKeyDetectorAudioProcessor::createEditor()
{
    return new MidiKeyDetectorAudioProcessorEditor(*this);
}

//==============================================================================
void MidiKeyDetectorAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void MidiKeyDetectorAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void MidiKeyDetectorAudioProcessor::loadMIDIFile(juce::File input)
{
    MIDIFile.clear();
    numTracks = 0;
    juce::FileInputStream theStream(input);
    MIDIFile.readFrom(theStream);
    setNumTracks(MIDIFile.getNumTracks());
    setCurrentTrack(0);
    trackHasChanged = false;
}

juce::StringArray MidiKeyDetectorAudioProcessor::SearchKey2(int Track, int value)
{
    int scalebegin;
    int scaleEinde;
    int kolom;
    int rij;

    switch (value) {
    case 1:
        // minor
        scalebegin = 4;
        scaleEinde = 5;
        kolom = 12;
        rij = 7;
        break;
    case 2:
        // 5scales
        scalebegin = 0;
        scaleEinde = 1;
        kolom = 12;
        rij = 5;
        break;
    case 3:
        // 6scales
        scalebegin = 1;
        scaleEinde = 4;
        kolom = 12;
        rij = 6;
        break;
    case 4:
        // 7scales
        scalebegin = 4;
        scaleEinde = 11;
        kolom = 12;
        rij = 7;
        break;
    case 5:
        // 8scales
        scalebegin = 12;
        scaleEinde = 13;
        kolom = 12;
        rij = 8;
        break;
    }
    MidiArr.clear();
    Output.clear();

    //omzetten naar array
    if (MIDIFile.getTrack(Track)->getNumEvents() > 5) {

        for (int i = 0; i < MIDIFile.getTrack(Track)->getNumEvents(); i++)
        {
            if (MIDIFile.getTrack(Track)->getEventPointer(i)->message.getDescription().contains("Note on")) {
                MidiArr.push_back(MIDIFile.getTrack(Track)->getEventPointer(i)->message.getDescription().substring(8, 11).trim().substring(0, MIDIFile.getTrack(Track)->getEventPointer(i)->message.getDescription().substring(8, 11).trim().length() - 1));
            }
        }
    }
    //duplicaten uithalen
    sort(MidiArr.begin(), MidiArr.end());
    MidiArr.erase(unique(MidiArr.begin(), MidiArr.end()), MidiArr.end());

    //omzetten naar method
    int counter = 0;
    if (MidiArr.size() > 0) {
        //Scale
        for (int s = scalebegin; s < scaleEinde; s++)
        {
            //kolom
            for (int o = 0; o < 12; o++)
            {
                //rij
                for (int p = 0; p < rij; p++)
                {
                    //midiArr
                    for (int i = 0; i < MidiArr.size(); i++)
                    {  
                        if (vect[s][o][p] == MidiArr[i]) {
                            counter++;
                        }
                    }
                }
                // scale klopt
                if (counter == MidiArr.size()) {
                    Output.add(vect[s][o][0] + " " + vectnamen[s]);
                }
                counter = 0;
            }
        }
    }
    return Output;
}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MidiKeyDetectorAudioProcessor();
}

