/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class MidiKeyDetectorAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    MidiKeyDetectorAudioProcessor();
    ~MidiKeyDetectorAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    
    //Attributen
    int numTracks;
    int currentTrack;
    bool trackHasChanged;

    //getters & setters
    void setNumTracks(int value) {
        numTracks = value;
    }
    int getNumTracks() {
        return numTracks;
    }
    void setCurrentTrack(int value) {
        currentTrack = value;
    }
    int getCurrentTrack() {
        return currentTrack;
    }


    juce::StringArray SearchKey(int Track);

    void loadMIDIFile(juce::File input);

private:
    //==============================================================================
    double nextStartTime = -1.0;
    
    juce::MidiFile MIDIFile;              
    

    juce::StringArray MidiArr;
    juce::StringArray Output;

    //SCALES


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiKeyDetectorAudioProcessor)
};
