/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
using namespace std;
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
    int selectedScale;
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
    void setselectedScale(int value) {
        selectedScale = value;
    }
    int getselectedScale() {
        return selectedScale;
    }
    juce::MidiFile getMIDIFile() {
        return MIDIFile;
    }
    void loadMIDIFile(juce::File input);
    
    vector<vector<vector<juce::String>>> vect
    {
        //Egyptian 5
        {{ "C","D","F","G","A#" },{ "C#","D#","F#","G#","B" },{ "D","E","G","A","C" },{ "D#","F","G#","A#","C#" },{ "E","F#","A","B","D" },{ "F","G","A#","C","D#" },{ "F#","G#","B","C#","E" },{ "G","A","C","D","F" },{ "G#","A#","C#","D#","F#" },{ "A","B","D","E","G" },{ "A#","C","D#","F","G#" },{ "B","C#","E","F#","A" }},
        //Six Tone Symmetrical 6
        { { "C","C#","E","F","G#","A" },{ "C#","D","F","F#","A","A#" },{ "D","D#","F#","G","A#","B" },{ "D#","E","G","G#","B","C" },{ "E","F","G#","A","C","C#" },{ "F","F#","A","A#","C#","D" },{ "F#","G","A#","B","D","D#" },{ "G","G#","B","C","D#","E" },{ "G#","A","C","C#","E","F" },{ "A","A#","C#","D","F","F#" },{ "A#","B","D","D#","F#","G" },{ "B","C","D#","E","G","G#" }},
        //Augmented 6
        {{ "C","D#","E","G","G#","B" },{ "C#","E","F","G#","A","C" },{ "D","F","F#","A","A#","C#" },{ "D#","F#","G","A#","B","D" },{ "E","G","G#","B","C","D#" },{ "F","G#","A","C","C#","E" },{ "F#","A","A#","C#","D","F" },{ "G","A#","B","D","D#","F#" },{ "G#","B","C","D#","E","G" },{ "A","C","C#","E","F","G#" },{ "A#","C#","D","F","F#","A" },{ "B","D","D#","F#","G","A#" }},
        //Prometheus Neopolitan 6
        {{ "C","C#","E","F#","A","A#" },{ "C#","D","F","G","A#","B" },{ "D","D#","F#","G#","B","C" },{ "D#","E","G","A","C","C#" },{ "E","F","G#","A#","C#","D" },{ "F","F#","A","B","D","D#" },{ "F#","G","A#","C","D#","E" },{ "G","G#","B","C#","E","F" },{ "G#","A","C","D","F","F#" },{ "A","A#","C#","D#","F#","G" },{ "A#","B","D","E","G","G#" },{ "B","C","D#","F","G#","A" }},
        //Natural_Minor 7
        {{"C","D","D#","F","G","G#","A#"},{"C#","D#","E","F#","G#","A","B"},{"D","E","F","G","A","A#","C"},{"D#","F","F#","G#","A#","B","C#"},{"E","F#","G","A","B","C","D"},{"F","G","G#","A#","C","C#","D#"},{"F#","G#","A","B","C#","D","E"},{"G","A","A#","C","D","D#","F"},{"G#","A#","B","C#","D#","E","F#"},{"A","B","C","D","E","F","G"},{"A#","C","C#","D#","F","F#","G#"},{"B","C#","D","E","F#","G","A"}},
        //HarmonicMinor 7
        {{"C","D","D#","F","G","G#","B"},{"C#","D#","E","F#","G#","A","C"},{"D","E","F","G","A","A#","C#"},{"D#","F","F#","G#","A#","B","D"},{"E","F#","G","A","B","C","D#"},{"F","G","G#","A#","C","C#","E"},{"F#","G#","A","B","C#","D","F"},{"G","A","A#","C","D","D#","F#"},{"G#","A#","B","C#","D#","E","G"},{"A","B","C","D","E","F","G#"},{"A#","C","C#","D#","F","F#","A"},{"B","C#","D","E","F#","G","A#"}},
        //MelodicMinor 7
        {{"C","D","D#","F","G","A","B"},{"C#","D#","E","F#","G#","A#","C"},{"D","E","F","G","A","B","C#"},{"D#","F","F#","G#","A#","C","D"},{"E","F#","G","A","B","C#","D#"},{"F","G","G#","A#","C","D","E"},{"F#","G#","A","B","C#","D#","F"},{"G","A","A#","C","D","E","F#"},{"G#","A#","B","C#","D#","F","G"},{"A","B","C","D","E","F#","G#"},{"A#","C","C#","D#","F","G","A"},{"B","C#","D","E","F#","G#","A#"}},
        //byzantine 7
        {{"C","C#","E","F","G","G#","B"},{"C#","D","F","F#","G#","A","C"},{"D","D#","F#","G","A","A#","C#"},{"D#","E","G","G#","A#","B","D"},{"E","F","G#","A","B","C","D#"},{"F","F#","A","A#","C","C#","E"},{"F#","G","A#","B","C#","D","F"},{"G","G#","B","C","D","D#","F#"},{"G#","A","C","C#","D#","E","G"},{"A","A#","C#","D","E","F","G#"},{"A#","B","D","D#","F","F#","A"},{"B","C","D#","E","F#","G","A#"}},
        //Arabian 7
        {{"C","D","E","F","F#","G#","A#"},{"C#","D#","F","F#","G","A","B"},{"D","E","F#","G","G#","A#","C"},{"D#","F","G","G#","A","B","C#"},{"E","F#","G#","A","A#","C","D"},{"F","G","A","A#","B","C#","D#"},{"F#","G#","A#","B","C","D","E"},{"G","A","B","C","C#","D#","F"},{"G#","A#","C","C#","D","E","F#"},{"A","B","C#","D","D#","F","G"},{"A#","C","D","D#","E","F#","G#"},{"B","C#","D#","E","F","G","A"}},
        //hungarianMajor 7
        {{"C","D#","E","F#","G","A","A#"},{"C#","E","F","G","G#","A#","B"},{"D","F","F#","G#","A","B","C"},{"D#","F#","G","A","A#","C","C#"},{"E","G","G#","A#","B","C#","D"},{"F","G#","A","B","C","D","D#"},{"F#","A","A#","C","C#","D#","E"},{"G","A#","B","C#","D","E","F"},{"G#","B","C","D","D#","F","F#"},{"A","C","C#","D#","E","F#","G"},{"A#","C#","D","E","F","G","G#"},{"B","D","D#","F","F#","G#","A"}},
        //diminishedlydian 7
        {{"C","D","D#","F#","G","A","B"},{"C#","D#","E","G","G#","A#","C"},{"D","E","F","G#","A","B","C#"},{"D#","F","F#","A","A#","C","D"},{"E","F#","G","A#","B","C#","D#"},{"F","G","G#","B","C","D","E"},{"F#","G#","A","C","C#","D#","F"},{"G","A","A#","C#","D","E","F#"},{"G#","A#","B","D","D#","F","G"},{"A","B","C","D#","E","F#","G#"},{"A#","C","C#","E","F","G","A"},{"B","C#","D","F","F#","G#","A#"}},
        //persian 7 
        {{"C","C#","E","F","F#","G#","B"},{"C#","D","F","F#","G","A","C"},{"D","D#","F#","G","G#","A#","C#"},{"D#","E","G","G#","A","B","D"},{"E","F","G#","A","A#","C","D#"},{"F","F#","A","A#","B","C#","E"},{"F#","G","A#","B","C","D","F"},{"G","G#","B","C","C#","D#","F#"},{"G#","A","C","C#","D","E","G"},{"A","A#","C#","D","D#","F","G#"},{"A#","B","D","D#","E","F#","A"},{"B","C","D#","E","F","G","A#"}},
        //Eight Tone Spanish 8
        {{ "C","C#","D#","E","F","F#","G#","A#" },{ "C#","D","E","F","F#","G","A","B" },{ "D","D#","F","F#","G","G#","A#","C" },{ "D#","E","F#","G","G#","A","B","C#" },{ "E","F","G","G#","A","A#","C","D" },{ "F","F#","G#","A","A#","B","C#","D#" },{ "F#","G","A","A#","B","C","D","E" },{ "G","G#","A#","B","C","C#","D#","F" },{ "G#","A","B","C","C#","D","E","F#" },{ "A","A#","C","C#","D","D#","F","G" },{ "A#","B","C#","D","D#","E","F#","G#" },{ "B","C","D","D#","E","F","G","A" }}
    };

    vector<juce::String> vectnamen{ "Egyptian","Six Tone Symmetrical","Augmented","Prometheus Neopolitan","Natural Minor","Harmonic Minor","Melodic Minor","byzantine","Arabian","hungarian Major","diminished lydian","persian","Eight Tone Spanish" };
  
    juce::StringArray SearchKey2(int Track, int scale);

private:
    //==============================================================================
    double nextStartTime = -1.0;
    
    juce::MidiFile MIDIFile;              
    vector<juce::String> MidiArr;
    juce::StringArray Output;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiKeyDetectorAudioProcessor)
};


