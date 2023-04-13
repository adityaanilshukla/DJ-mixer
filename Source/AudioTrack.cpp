/*
  ==============================================================================

    Track.cpp
    Created: 2 Mar 2023 3:02:23pm
    Author:  adity

  ==============================================================================
*/

#include "AudioTrack.h"
#include <filesystem>

AudioTrack::AudioTrack(juce::File _file) : file(_file), 
                                 title(_file.getFileNameWithoutExtension()),
                                 URL(juce::URL{ _file })
{
    
}

bool AudioTrack::operator==(const juce::String& other) const 
{
    return title == other;
}

