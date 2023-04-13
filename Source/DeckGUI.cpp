/*
  ==============================================================================

    DeckGUI.cpp
    Created: 14 Feb 2023 1:33:30am
    Author:  aditya

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player, 
                juce::AudioFormatManager& formatManagerToUse,
                juce::AudioThumbnailCache& cacheToUse,
                juce::String deckName) : player(_player),
                  waveformDisplay(formatManagerToUse, cacheToUse)
                                         
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    
    //set components to style elements
    lightBlueSliderStyle.setColour(juce::Slider::thumbColourId, lightBlue);
    lightBlueSliderStyle.setColour(juce::Slider::trackColourId, lightBlue);

    //set images to button elements
    playPauseButton.setImages(false, true, true, playImage, 0.5f, juce::Colours::grey, playImage, 1.0f, lightBlue, playImage, 0.5f, juce::Colours::transparentBlack, 0.0f);
    muteButton.setImages(false, true, true, muteImage, 0.5f, juce::Colours::grey, muteImage, 1.0f, lightBlue, muteImage, 0.5f, juce::Colours::transparentBlack, 0.0f);

    //setstyles to deckTitle and trackPlaying
    addAndMakeVisible(deckTitle);
    deckTitle.setText(deckName, juce::NotificationType::dontSendNotification);
    deckTitle.setFont(deckTitle.getFont().withStyle(juce::Font::bold));
    deckTitle.setFont(juce::Font(20.0f));
    addAndMakeVisible(trackCurrentlyPlaying);
    trackCurrentlyPlaying.setText("No track Selected", juce::NotificationType::dontSendNotification);
    trackCurrentlyPlaying.setFont(juce::Font(20.0f));
    
    //make buttons visble
    addAndMakeVisible(playPauseButton);
    addAndMakeVisible(muteButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(loopButton);

    //make sliders visible
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);
    addAndMakeVisible(roomSizeSlider);
    addAndMakeVisible(DampingSlider);
    addAndMakeVisible(wetLevelSlider);
    addAndMakeVisible(dryLevelSlider);

    //make labels visble
    addAndMakeVisible(timeElapsedLabel);

    //make waveform visble
    addAndMakeVisible(waveformDisplay);

    //add button listeners
    playPauseButton.addListener(this);
    loadButton.addListener(this);
    loopButton.addListener(this);
    muteButton.addListener(this);

    //add listeners to sliders
    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);
    roomSizeSlider.addListener(this);
    DampingSlider.addListener(this);
    wetLevelSlider.addListener(this);
    dryLevelSlider.addListener(this);

    //set toggle state of buttons
    muteButton.setToggleState(isMuted, juce::NotificationType::dontSendNotification);
    playPauseButton.setToggleState(isPlaying, juce::NotificationType::dontSendNotification);

    //set slider styles
    speedSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    roomSizeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    DampingSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    wetLevelSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    dryLevelSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    posSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    volSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    wetLevelSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    dryLevelSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    //set slider ranges and values and start positions
    roomSizeSlider.setRange(0.0f, 1.0f);
    roomSizeSlider.setValue(0.0f);
    
    wetLevelSlider.setRange(0.0f, 1.0f);
    wetLevelSlider.setValue(0.0f);

    dryLevelSlider.setRange(0.0f, 1.0f);
    dryLevelSlider.setValue(1.0f);
    
    DampingSlider.setRange(0.0f, 1.0f);
    DampingSlider.setValue(0.0f);

    volSlider.setRange(0.0, 1.0);
    volSlider.setValue(0.5f);
    
    speedSlider.setValue(1.0f);
    speedSlider.setRange(0.0, 10);
    

    //set decimal places for each slider
    speedSlider.setNumDecimalPlacesToDisplay(2);
    roomSizeSlider.setNumDecimalPlacesToDisplay(2);
    DampingSlider.setNumDecimalPlacesToDisplay(2);
    wetLevelSlider.setNumDecimalPlacesToDisplay(2);
    dryLevelSlider.setNumDecimalPlacesToDisplay(2);

    speedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false ,100,25);
    roomSizeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 25);
    DampingSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 25);
    wetLevelSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 25);
    dryLevelSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 25);

    //set labels to sliders and label
    timeElapsedLabel.setText("0:00", juce::NotificationType::dontSendNotification); //initialise timer to 0s
    timeElapsedLabel.attachToComponent(&posSlider, true);

    //attach slider labels to sliders 
    speedSliderLabel.setText("Speed", juce::NotificationType::dontSendNotification);
    speedSliderLabel.attachToComponent(&speedSlider, false);
    speedSliderLabel.setJustificationType(juce::Justification::centredBottom);

    dampingLabel.setText("Damping", juce::NotificationType::dontSendNotification);
    dampingLabel.attachToComponent(&DampingSlider, false);
    dampingLabel.setJustificationType(juce::Justification::centredBottom);

    roomSizeSliderLabel.setText("Room Size", juce::NotificationType::dontSendNotification);
    roomSizeSliderLabel.attachToComponent(&roomSizeSlider, false);
    roomSizeSliderLabel.setJustificationType(juce::Justification::centredBottom);

    wetLevelSliderLabel.setText("Wet Level", juce::NotificationType::dontSendNotification);
    wetLevelSliderLabel.attachToComponent(&wetLevelSlider, false);
    wetLevelSliderLabel.setJustificationType(juce::Justification::centredBottom);

    dryLevelSliderLabel.setText("Dry Level", juce::NotificationType::dontSendNotification);
    dryLevelSliderLabel.attachToComponent(&dryLevelSlider, false);
    dryLevelSliderLabel.setJustificationType(juce::Justification::centredBottom);

    startTimer(100);
}

DeckGUI::~DeckGUI()
{
  stopTimer();
}

void DeckGUI::paint (juce::Graphics& g)
{
    g.fillAll(darkBlue);

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);

    loadButton.setColour(juce::TextButton::buttonColourId, darkerGrey);
   
    //set all textBoxes to darkerGrey
    speedSlider.setColour(juce::Slider::textBoxBackgroundColourId, darkerGrey);
    roomSizeSlider.setColour(juce::Slider::textBoxBackgroundColourId, darkerGrey);
    DampingSlider.setColour(juce::Slider::textBoxBackgroundColourId, darkerGrey);
    wetLevelSlider.setColour(juce::Slider::textBoxBackgroundColourId, darkerGrey);
    dryLevelSlider.setColour(juce::Slider::textBoxBackgroundColourId, darkerGrey);

    //setlook for sliders
    volSlider.setLookAndFeel(&lightBlueSliderStyle);
    speedSlider.setLookAndFeel(&lightBlueSliderStyle);
    posSlider.setLookAndFeel(&lightBlueSliderStyle);
    roomSizeSlider.setLookAndFeel(&lightBlueSliderStyle);
    DampingSlider.setLookAndFeel(&lightBlueSliderStyle);
    dryLevelSlider.setLookAndFeel(&lightBlueSliderStyle);
    wetLevelSlider.setLookAndFeel(&lightBlueSliderStyle);
}

void DeckGUI::resized()
{
   
    double rowH = getHeight() / 8;
    double buttonWidth =     getWidth() / 3;
    auto offsett = getWidth() / 16;

    //deck title and track both take dimensions based on the width of their text string
    deckTitle.setBounds(0, 0, deckTitle.getFont().getStringWidth(deckTitle.getText()), deckTitle.getFont().getHeight());
    trackCurrentlyPlaying.setBounds(deckTitle.getBounds().getRight()+offsett/4, 0, trackCurrentlyPlaying.getFont().getStringWidth(trackCurrentlyPlaying.getText()) * 2, trackCurrentlyPlaying.getFont().getHeight());
    
    loadButton.setBounds(getWidth()-buttonWidth/3, 0, buttonWidth / 3, rowH * 0.50);

    auto sliderWidth = getWidth() / 5;

    
    speedSlider.setBounds(0, rowH, sliderWidth, rowH*2.75);
    roomSizeSlider.setBounds(sliderWidth, rowH, sliderWidth, rowH * 2.75);
    DampingSlider.setBounds(sliderWidth * 2, rowH, sliderWidth, rowH * 2.75);
    wetLevelSlider.setBounds(sliderWidth * 3, rowH, sliderWidth, rowH * 2.75);
    dryLevelSlider.setBounds(sliderWidth * 4, rowH, sliderWidth, rowH * 2.75);

    auto iconButtonWidth = rowH * 0.65;
    auto startX = offsett/2;

    playPauseButton.setBounds(startX, rowH*4, iconButtonWidth/1.5, rowH * 0.65);

    startX += iconButtonWidth/2;

    auto posSliderWidth = getWidth()/2;

    posSlider.setBounds(startX+= offsett*1.5, rowH * 4, posSliderWidth, rowH * 0.65);

    startX += posSliderWidth;

    muteButton.setBounds(startX+=offsett/2, rowH*4, iconButtonWidth/2, rowH*0.65);

    volSlider.setBounds(startX +=offsett/2, rowH * 4, posSliderWidth/3, rowH*0.65);

    startX += posSliderWidth / 3;

    loopButton.setBounds(startX += offsett / 3.5, rowH * 4, posSliderWidth / 3, rowH * 0.65);

    waveformDisplay.setBounds(0, rowH*5, getWidth(), rowH*3);



}


void DeckGUI::buttonClicked(juce::Button* button){

    if (button == &playPauseButton)
    {
      changePlayingState();
    }

    if (button == &loadButton)
    {
        DBG("Load button was clicked ");
        juce::FileChooser chooser{"Select a file"};
        if (chooser.browseForFileToOpen())
        {
            loadFile(juce::URL{ chooser.getResult() });
            fileLoaded = true;
            changePlayingState();
        }
    }

    if (button == &muteButton)
    {
        toggleMuteUnMute();
    }
}



void DeckGUI::sliderValueChanged(juce::Slider* slider)
{
  if(slider == &volSlider)
  {
    player->setGain( slider->getValue() );
  }

  if(slider == &speedSlider)
  {
    player->setSpeed( slider->getValue() );
  }

  if(slider == &posSlider && fileLoaded)// prevent user from changing track position if no track is selected
  {
      player->setPosition(slider->getValue());
      timeElapsedLabel.setText(timeElapsedString(slider->getValue()), juce::dontSendNotification);
  }
  if (slider == &roomSizeSlider)
  {
      player->setRoomSize(slider->getValue());
  }
  if (slider == &DampingSlider)
  {
      player->setDamping(slider->getValue());
  }
  if (slider == &wetLevelSlider)
  {
      player->setWetLevel(slider->getValue());
  }
  if (slider == &dryLevelSlider)
  {
      player->setDryLevel(slider->getValue());
  }
}


bool DeckGUI::isInterestedInFileDrag ( const juce::StringArray &files )
{
  std::cout << "file dragged" << std::endl;
  return true;
}

void DeckGUI::filesDropped(const juce::StringArray& files, int x, int y)
{

    DBG("DeckGUI::filesDropped at " + std::to_string(x) 
        + "x and " + std::to_string(y) + "y" );
    if (files.size() == 1)
    {
        loadFile(juce::URL{ juce::File{files[0]} });
        fileLoaded = true;
        changePlayingState();
    }
}

void DeckGUI::timerCallback()
{   
   
  if (std::to_string(loopButton.getToggleState()) == "1") { //If loop button
        if (player->getPositionRelative() >= 1) {// and the position more than 1 meaning it is the end of the audio file
            player->setPositionRelative(0);
            player->start();
        }
    }

    if (std::to_string(loopButton.getToggleState()) == "0") { //If noloop button
        if (player->getPositionRelative() >= 1) { //pos > 1 = end of file
            player->setPositionRelative(0);
            changePlayingState();//stop playing the track
            timeElapsedLabel.setText("0.00", juce::NotificationType::dontSendNotification); //reset timer to 0
        }
    }

    if (player != nullptr && player->isAudioPlaying())
    {
        juce::String time = timeElapsedString(player->getCurrentPosition());
        timeElapsedLabel.setText(time, juce::NotificationType::dontSendNotification);
    }


    waveformDisplay.setPositionRelative(player->getPositionRelative()); //Every 100ms, updates the waveform display to show current timestamp for the audio file
    updateSliderPos();
    repaint();


}

void DeckGUI::changePlayingState()
{
    if (fileLoaded == false)
    {
        return; //return because no file has been loaded
    }


    if (isPlaying)
    {
        playPauseButton.setImages(false, true, true, playImage, 0.5f, juce::Colours::grey, playImage, 1.0f, lightBlue, playImage, 0.5f, juce::Colours::transparentBlack, 0.0f);
        player->stop();
    }
    else
    {
        playPauseButton.setImages(false, true, true, pauseImage, 0.5f, juce::Colours::grey, pauseImage, 1.0f, lightBlue, pauseImage, 0.5f, juce::Colours::transparentBlack, 0.0f);
        player->start();
    }

    isPlaying = !isPlaying;
}

void DeckGUI::toggleMuteUnMute()
{

    if (isMuted)
    {
        volSlider.setValue(volume);
        muteButton.setImages(false, true, true, muteImage, 0.5f, juce::Colours::grey, muteImage, 1.0f, lightBlue, muteImage, 0.5f, juce::Colours::transparentBlack, 0.0f);
    }
    else
    {
        volume = volSlider.getValue();
        volSlider.setValue(0.0f);
        muteButton.setImages(false, true, true, unMuteImage, 0.5f, juce::Colours::grey, unMuteImage, 1.0f, lightBlue, unMuteImage, 0.5f, juce::Colours::transparentBlack, 0.0f);
    }

    isMuted = !isMuted;
}
void DeckGUI::loadFile(juce::URL audioURL)
{
    //get fileName for trackCurrentlyPlaying label
    juce::String fileName = audioURL.getLocalFile().getFileNameWithoutExtension();


    DBG("DeckGUI::loadFile called");
    player->loadURL(audioURL);
    waveformDisplay.loadURL(audioURL);
    trackCurrentlyPlaying.setText("Track selected : " +
        fileName,
        juce::NotificationType::dontSendNotification); //update trackPlaying label to show curr track
    posSlider.setRange(0.0, player->getLengthInSeconds() ); //set slider length = track length
    posSlider.setValue(0.0); //set track pos to 0
}


juce::String DeckGUI::timeElapsedString(double positionInSeconds)
{   
    int positionInMinutes = (int)(positionInSeconds / 60);
    int positionInSecondsRemaining = (int)positionInSeconds % 60;
    juce::String secondsString = juce::String(positionInSecondsRemaining).paddedLeft('0', 2); //ensure that 2 digits always shown for seconds
    return juce::String(positionInMinutes) + ":" + secondsString;

}

void DeckGUI::updateSliderPos()
{   
    auto pos = player->getCurrentPosition();
    posSlider.setValue(pos, juce::dontSendNotification );
}




