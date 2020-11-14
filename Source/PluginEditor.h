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
class MidiKeyDetectorAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Button::Listener, public juce::ComboBox::Listener, public juce::FileDragAndDropTarget, public juce::ComponentBoundsConstrainer
{
public:
    MidiKeyDetectorAudioProcessorEditor (MidiKeyDetectorAudioProcessor&);
    ~MidiKeyDetectorAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void buttonClicked(juce::Button* button) override;

    void updateTrackComboBox();

    void comboBoxChanged(juce::ComboBox* combo);

    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;
    
    void setKey(juce::String text) {
        LblOutput->setText(TRANS(text));
    }

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MidiKeyDetectorAudioProcessor& audioProcessor;

    juce::ScopedPointer<juce::TextButton> btnLoadMIDIFile;
    juce::ScopedPointer<juce::ComboBox> comboTrack;
    juce::ScopedPointer<juce::TextEditor> LblOutput;
    juce::ScopedPointer<juce::ComponentBoundsConstrainer> BC;
    juce::Label lblKey;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiKeyDetectorAudioProcessorEditor)
};
