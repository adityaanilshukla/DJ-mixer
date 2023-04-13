/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 10 Feb 2023 2:59:03pm
    Author:  aditya

  ==============================================================================
*/

#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(juce::AudioFormatManager& _formatManager)
: formatManager( _formatManager )
{
    reverbParameters.roomSize = 0.0f;

    reverbParameters.damping = 0.0f;
    reverbParameters.wetLevel = 0.0f;
    reverbParameters.dryLevel = 1.0f;
    reverbSource.setParameters(reverbParameters);
}

DJAudioPlayer::~DJAudioPlayer()
{}

//========================================
void DJAudioPlayer::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
  transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
  resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
  reverbSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}


void DJAudioPlayer::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
  reverbSource.getNextAudioBlock(bufferToFill);
}


void DJAudioPlayer::releaseResources()
{
  transportSource.releaseResources();
  resampleSource.releaseResources();
  reverbSource.releaseResources();
}


void DJAudioPlayer::loadURL(juce::URL audioURL)
{
    auto* reader = formatManager.createReaderFor (audioURL.createInputStream (false));

    if (reader != nullptr) //good file! 
    { 
      std::unique_ptr<juce::AudioFormatReaderSource> newSource (new juce::AudioFormatReaderSource (reader, true));

      transportSource.setSource (newSource.get(), 0, nullptr, reader->sampleRate);
      readerSource.reset (newSource.release());
      transportSource.start();
    }
}

void DJAudioPlayer::setGain(double gain)
{
  if(gain < 0 || gain > 1.0)
  {
    std::cout << "gain should be between 0 and 1" << std::endl;
  }
  else
  {
    transportSource.setGain(gain);
  }
}

void DJAudioPlayer::setSpeed(double ratio)
{
  
  if(ratio < 0 || ratio > 100)
  {
    std::cout << "gain should be between 0 and 1" << std::endl;
  }
  else
  {
    resampleSource.setResamplingRatio(ratio);
  }
}

void DJAudioPlayer::setPosition(double posInSecs)
{
  transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setPositionRelative(double pos)
{

  if(pos < 0 || pos > 1.0)
  {
    std::cout << "positon should be between 0 and 1" << std::endl;
  }
  else
  { 
    double posInSecs = pos * transportSource.getLengthInSeconds();
    setPosition(posInSecs);
  }

}


void DJAudioPlayer::start()
{
  transportSource.start();
}  

void DJAudioPlayer::stop()
{
  transportSource.stop();
}



double DJAudioPlayer::getPositionRelative()
{
  return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}

double DJAudioPlayer::getCurrentPosition()
{
    return transportSource.getCurrentPosition();
}

double DJAudioPlayer::getLengthInSeconds()
{
    return transportSource.getLengthInSeconds();
}

void DJAudioPlayer::setRoomSize(float roomSize)
{
    reverbParameters.roomSize = roomSize;
    reverbSource.setParameters(reverbParameters);
}


void DJAudioPlayer::setWetLevel(float wetLevel)
{
    reverbParameters.wetLevel = wetLevel;
    reverbSource.setParameters(reverbParameters);
}


void DJAudioPlayer::setDryLevel(float dryLevel)
{
    reverbParameters.dryLevel = dryLevel;
    reverbSource.setParameters(reverbParameters);
}

void DJAudioPlayer::setDamping(float damping)
{
    reverbParameters.damping = damping;
    reverbSource.setParameters(reverbParameters);
}



bool DJAudioPlayer::isAudioPlaying() const
{
    return transportSource.isPlaying();
}




