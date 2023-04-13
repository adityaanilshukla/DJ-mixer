/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 10 Feb 2023 2:59:03pm
    Author:  aditya

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <juce_dsp/juce_dsp.h>


class DJAudioPlayer : public juce::AudioSource
{

  public:

    DJAudioPlayer(juce::AudioFormatManager& _formatManager);
    ~DJAudioPlayer();

    //============================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void loadURL(juce::URL audioURL);
    void setGain(double gain);
    void setSpeed(double ratio);
    void setPosition(double posInSecs);
    void setPositionRelative(double pos);

    void start();
    void stop();

    /** return the length of transportSource */
    double getLengthInSeconds();
    
    /** get the relative position of the playhead*/
    double getPositionRelative();

    double getCurrentPosition();

    void setRoomSize(float roomSize);

    void setWetLevel(float wetLevel);
    void setDryLevel(float dryLevel);
    void setDamping(float damping);

    bool isAudioPlaying() const;

  private:

    juce::AudioFormatManager& formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    juce::ResamplingAudioSource resampleSource {&transportSource, false, 2};

    juce::ReverbAudioSource reverbSource{ &resampleSource, false };
    juce::Reverb::Parameters reverbParameters;

    //juce::AudioSource* source;
};

