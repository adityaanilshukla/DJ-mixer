/*
  ==============================================================================

    DeckGUI.h
    Created: 14 Feb 2023 1:33:30am
    Author:  aditya

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "BinaryData.h"

//==============================================================================
/*
*/
class DeckGUI  : public juce::Component,
                 public juce::Button::Listener,
                 public juce::Slider::Listener,
                 public juce::FileDragAndDropTarget,
                 public juce::Timer
{
public:

    DeckGUI(DJAudioPlayer* player,
        juce::AudioFormatManager& formatManagerToUse,
        juce::AudioThumbnailCache& cacheToUse,
        juce::String deckName);


    DeckGUI( DJAudioPlayer* player );
    ~DeckGUI() override;

    void paint (juce::Graphics&) override;
    void resized() override;


    /** implement Button::Listener */
    void buttonClicked(juce::Button *button) override;

    /** implement Slider::Listener */
    void sliderValueChanged(juce::Slider *slider) override;

    bool FileDragAndDropTarget::isInterestedInFileDrag ( const juce::StringArray &files ) override;

    void FileDragAndDropTarget::filesDropped ( const juce::StringArray &files, int x, int y )	override;

    void timerCallback() override;

    juce::Image playImage = juce::ImageCache::getFromMemory(BinaryData::play_png, BinaryData::play_pngSize);
    juce::Image pauseImage = juce::ImageCache::getFromMemory(BinaryData::pause_png, BinaryData::pause_pngSize);
    juce::Image muteImage = juce::ImageCache::getFromMemory(BinaryData::unmute_png, BinaryData::unmute_pngSize);
    juce::Image unMuteImage = juce::ImageCache::getFromMemory(BinaryData::mute_png, BinaryData::mute_pngSize);
   
    bool fileLoaded = false;

    const juce::Colour lightBlue = juce::Colour::fromRGB(56, 132, 217);
    const juce::Colour darkBlue  = juce::Colour::fromRGB(7, 35, 64);
    const juce::Colour darkerGrey = juce::Colour::fromRGB(50, 50, 50);

    
private:

    void changePlayingState();
    void toggleMuteUnMute();

    juce::LookAndFeel_V4 lightBlueSliderStyle;
    juce::FileChooser fChooser{"Select a file..."};

    juce::ImageButton playPauseButton;
    juce::ImageButton muteButton;
    juce::ToggleButton loopButton{ "Loop" };
    juce::TextButton loadButton{ "Load" };
    juce::Slider volSlider;
    juce::Slider speedSlider;
    juce::Slider posSlider;
    juce::Slider roomSizeSlider;
    juce::Slider DampingSlider;
    juce::Slider wetLevelSlider;
    juce::Slider dryLevelSlider;
       
    juce::Label volSliderLabel;
    juce::Label speedSliderLabel;
    juce::Label timeElapsedLabel;
    juce::Label roomSizeSliderLabel;
    juce::Label dampingLabel;
    juce::Label wetLevelSliderLabel;
    juce::Label dryLevelSliderLabel;
    juce::Label deckTitle; //deck title placed in each deck
    juce::Label trackCurrentlyPlaying;

    juce::String timeElapsedString(double positionInSeconds);

    double volume;
    bool isMuted = false;
    bool isPlaying = false;
    bool isLoopEnabled = false;

    void loadFile ( juce::URL audioURL );

    DJAudioPlayer* player;
    WaveformDisplay waveformDisplay;

    void updateSliderPos();    
    friend class PlaylistComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
