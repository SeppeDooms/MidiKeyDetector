/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
const juce::MidiMessageSequence* theSequence;
juce::StringArray NamenScales[8] = { "Natural Minor","Harmonic Minor","Melodic Minor","Byzantine","Arabian","Hungarian Major","Diminished lydian","Persian" };

juce::StringArray ScalesSeven[8][12][7] = {
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
    {{"C","C#","E","F","F#","G#","B"},{"C#","D","F","F#","G","A","C"},{"D","D#","F#","G","G#","A#","C#"},{"D#","E","G","G#","A","B","D"},{"E","F","G#","A","Bb","C","D#"},{"F","F#","A","A#","B","C#","E"},{"F#","G","A#","B","C","D","F"},{"G","G#","B","C","C#","D#","F#"},{"G#","A","C","C#","D","E","G"},{"A","A#","C#","D","D#","F","G#"},{"A#","B","D","D#","E","F#","A"},{"B","C","D#","E","F","G","A#"}}
};


MidiKeyDetectorAudioProcessor::MidiKeyDetectorAudioProcessor()
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

MidiKeyDetectorAudioProcessor::~MidiKeyDetectorAudioProcessor()
{
}

//==============================================================================
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

void MidiKeyDetectorAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused(index);
}

const juce::String MidiKeyDetectorAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused(index);
    return {};
}

void MidiKeyDetectorAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused(index);
    juce::ignoreUnused(newName);
}

//==============================================================================
void MidiKeyDetectorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
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
bool MidiKeyDetectorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
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

void MidiKeyDetectorAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{


}

//==============================================================================
bool MidiKeyDetectorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MidiKeyDetectorAudioProcessor::createEditor()
{
    return new MidiKeyDetectorAudioProcessorEditor (*this);
}

//==============================================================================
void MidiKeyDetectorAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void MidiKeyDetectorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
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

juce::StringArray MidiKeyDetectorAudioProcessor::SearchKey(int Track)
{
    MidiArr.clear();
    Output.clear();
  
    //omzetten naar array
    if (MIDIFile.getTrack(Track)->getNumEvents() > 5) {
       
        for (int i = 0; i < MIDIFile.getTrack(Track)->getNumEvents(); i++)
        {
            if (MIDIFile.getTrack(Track)->getEventPointer(i)->message.getDescription().contains("Note on")) {
                
                MidiArr.add(MIDIFile.getTrack(Track)->getEventPointer(i)->message.getDescription().substring(8, 11).trim().substring(0, MIDIFile.getTrack(Track)->getEventPointer(i)->message.getDescription().substring(8, 11).trim().length() - 1));
                //DBG("Komt hier");
            }
        }
    }
    //duplicaten uithalen
    MidiArr.removeDuplicates(true);
    
    
    //uitlezen array
    for (size_t i = 0; i < MidiArr.size(); i++)
    {
        DBG(MidiArr[i]);
    }
    
    //omzetten naar method
    int counter = 0;
    if (MidiArr.size() > 0) {
        //Scale
        for (int  s = 0; s < 8; s++)
        {
            //kolom
            for (int o = 0; o < 12; o++)
            {
                //rij
                for (int p = 0; p < 7; p++)
                {
                    //midiArr
                    for (int i = 0; i < MidiArr.size(); i++)
                    {

                        if (ScalesSeven[s][o][p][0] == MidiArr[i]) {
                            counter++;
                        }

                    }
                }
                // scale klopt
                if (counter == MidiArr.size()) {
                    Output.add(ScalesSeven[s][o][0][0]+" "+NamenScales[s][0]);
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












