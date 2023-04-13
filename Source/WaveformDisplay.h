/*
  ==============================================================================

    WaveformDisplay.h
    Created: 16 Feb 2023 4:37:19pm
    Author:  adity

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class WaveformDisplay  : public juce::Component,
                         public juce::ChangeListener

{
public:
    WaveformDisplay(
        juce::AudioFormatManager& formatManagerToUse,
        juce::AudioThumbnailCache& cacheToUse);
    ~WaveformDisplay() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void changeListenerCallback ( juce::ChangeBroadcaster *source) override;


    void loadURL(juce::URL audioURL);

    void setPositionRelative(double pos);

    //colors used in waveform display
    const juce::Colour darkBlue = juce::Colour::fromRGB(7, 35, 64);
    const juce::Colour darkerGrey = juce::Colour::fromRGB(50, 50, 50);
    const juce::Colour lightBlue = juce::Colour::fromRGB(56, 132, 217);

private:
    juce::AudioThumbnail audioThumb;

    bool fileLoaded;

    double position;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
