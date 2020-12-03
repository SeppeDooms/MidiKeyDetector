/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"



//constructor
MidiKeyDetectorAudioProcessorEditor::MidiKeyDetectorAudioProcessorEditor (MidiKeyDetectorAudioProcessor& p): AudioProcessorEditor (&p), audioProcessor (p)
{
    btnLoadMIDIFile = new juce::TextButton("Drop/load a MIDI file");
    btnLoadMIDIFile->addListener(this);
   
    addAndMakeVisible(btnLoadMIDIFile);
    
    //Combobox
    addAndMakeVisible(comboTrack = new juce::ComboBox());
    comboTrack->addListener(this);
    updateTrackComboBox();

    //ComboScale
    addAndMakeVisible(comboScale = new juce::ComboBox());
    comboScale->addListener(this);
    MakeScaleComboBox();

    //Lbl Output
    LblOutput.reset(new juce::TextEditor("LblOutput"));
    LblOutput->setMultiLine(true);
    LblOutput->setReturnKeyStartsNewLine(true);
    LblOutput->setReadOnly(true);
    LblOutput->setScrollbarsShown(true);
    LblOutput->setCaretVisible(false);
    LblOutput->setPopupMenuEnabled(true);
    LblOutput->setColour(juce::TextEditor::textColourId, juce::Colours::lime);
    LblOutput->setColour(juce::TextEditor::highlightColourId, juce::Colour(0xbf000000));
    LblOutput->setColour(juce::TextEditor::shadowColourId, juce::Colour(0x00000000));
    LblOutput->setColour(juce::CaretComponent::caretColourId, juce::Colours::black);
    LblOutput->setText(TRANS("No key detected"));
    
    addAndMakeVisible(LblOutput.get());
    
    //class voor de min size van de window
    BC = new juce::ComponentBoundsConstrainer();
    BC->setMinimumSize(300,200);
    setConstrainer(BC);
    
    setResizable(true, true);
    
    setSize(400, 300);

    
    
}
//Deconstructor
MidiKeyDetectorAudioProcessorEditor::~MidiKeyDetectorAudioProcessorEditor()
{
}

//==============================================================================
void MidiKeyDetectorAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("MidiKeyDetector v2.1.0", getLocalBounds(), juce::Justification::centredBottom, 1);
}

void MidiKeyDetectorAudioProcessorEditor::resized()
{
    btnLoadMIDIFile->setBounds(getLocalBounds().getWidth() / 4, 0 + getLocalBounds().getHeight()/10, getLocalBounds().getWidth() / 2, getLocalBounds().getHeight() / 12);
    comboTrack->setBounds(getLocalBounds().getWidth() / 4, getLocalBounds().getHeight() - getLocalBounds().getHeight() / 4, getLocalBounds().getWidth() / 2, getLocalBounds().getHeight() / 12);
    LblOutput->setBounds(getLocalBounds().getWidth() / 4, getLocalBounds().getHeight() / 3, getLocalBounds().getWidth() / 2, getLocalBounds().getHeight() / 3);
    LblOutput->applyFontToAllText(juce::Font((float) getLocalBounds().getHeight() / 14,0));
    comboScale->setBounds(getLocalBounds().getWidth() / 4, getLocalBounds().getHeight() / 8.5 + getLocalBounds().getHeight() / 10, getLocalBounds().getWidth() / 2, getLocalBounds().getHeight() / 12);
}

void MidiKeyDetectorAudioProcessorEditor::buttonClicked(juce::Button* button)
{
    if (button == btnLoadMIDIFile)
    {
        juce::FileChooser theFileChooser("Find a MIDI file", juce::File(), "*.mid*");

        if (theFileChooser.browseForFileToOpen())
        {
            audioProcessor.loadMIDIFile(theFileChooser.getResult());
            updateTrackComboBox();   
        }
    }
}

void MidiKeyDetectorAudioProcessorEditor::updateTrackComboBox()
{
    comboTrack->clear();
    for (auto i = 0; i < audioProcessor.getNumTracks(); i++) {
        comboTrack->addItem("Track " + juce::String(i + 1), i + 1);
    }

    comboTrack->setSelectedId(audioProcessor.getCurrentTrack() + 1, juce::dontSendNotification);

}

void MidiKeyDetectorAudioProcessorEditor::MakeScaleComboBox()
{
    comboScale->clear();
    comboScale->addItem("Minor Scale Only", 1);
    comboScale->addItem("5 Scales", 2);
    comboScale->addItem("6 Scales", 3);
    comboScale->addItem("7 Scales", 4);
    comboScale->addItem("8 Scales", 5);
    comboScale->setSelectedId(1);
}

void MidiKeyDetectorAudioProcessorEditor::comboBoxChanged(juce::ComboBox* combo)
{

    if (combo == comboTrack)
    {
        audioProcessor.setCurrentTrack(combo->getSelectedId() - 1);
    }

    if (combo == comboScale)
    {
        audioProcessor.setselectedScale(combo->getSelectedId());   
    }

    if (audioProcessor.getNumTracks() > 0) {
        juce::StringArray output = audioProcessor.SearchKey2(audioProcessor.getCurrentTrack(), audioProcessor.getselectedScale());
        juce::StringArray temp;
        if (!output.isEmpty())
        {
            for (int i = 0; i < output.size(); i++)
            {
                temp.add(output[i] + "\n");
            }
            setKey(temp.joinIntoString("", 0, -1));
        }
        else
        {
            setKey("No key detected");
        }
    }

}

//controle of het .mid(i) files zin
bool MidiKeyDetectorAudioProcessorEditor::isInterestedInFileDrag(const juce::StringArray& files)
{
    for (auto file : files)
    {
        if (file.contains(".mid") || file.contains(".midi")) {
            return true;
        }
    }
    return false;
}

//files dropped
void MidiKeyDetectorAudioProcessorEditor::filesDropped(const juce::StringArray& files, int x, int y)
{
    for (auto file : files) {
        if (isInterestedInFileDrag(files)) {
            audioProcessor.loadMIDIFile(file);
            updateTrackComboBox();
        }
    }
}