/*
  ==============================================================================

    Track.h
    Created: 2 Mar 2023 3:02:23pm
    Author:  adity

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class AudioTrack
{
    public:
        AudioTrack(juce::File _file);
        juce::File file;
        juce::URL URL;
        juce::String title;
        juce::String length;
        /**objects are compared by title*/
        bool operator==(const juce::String& other) const;
};
