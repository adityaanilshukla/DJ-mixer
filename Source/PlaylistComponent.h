/*
  ==============================================================================

    PlaylistComponent.h
    Created: 17 Feb 2023 3:20:42am
    Author:  aditya

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>
#include <fstream>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "AudioTrack.h"

//==============================================================================
/*
*/
class PlaylistComponent  : public juce::Component,
                           public juce::TableListBoxModel,
                           public juce::Button::Listener,
                           public juce::TextEditor::Listener
{
public:
    PlaylistComponent( DeckGUI* _deckGUI1, DeckGUI* _deckGUI2, DJAudioPlayer* _trackMetaData );
    ~PlaylistComponent() override;

    int getNumRows() override;

    void paintRowBackground (juce::Graphics&,
                            int rowNumber,
                            int width,
                            int height,
                            bool rowIsSelected) override;
    
    void paintCell (juce::Graphics&, 
                    int rowNumber, 
                    int columnId, 
                    int width, 
                    int height, 
                    bool rowIsSelected) override;

    void paint (juce::Graphics&) override;
    void resized() override;

    Component * refreshComponentForCell ( int rowNumber,
                                               int columnId,
                                               bool rowIsSelected,
                                               juce::Component *existingComponentToUpdate) override;

    void buttonClicked(juce::Button* button) override;

    //colors used in playlistComponent
    const juce::Colour lightBlue = juce::Colour::fromRGB(56, 132, 217);
    const juce::Colour darkerGrey = juce::Colour::fromRGB(50, 50, 50);

private:
    
    std::vector<AudioTrack> audioTracks;

    juce::TextButton addTrack { "Add Track to Playlist" };
    juce::TextEditor searchBar;
    juce::TableListBox library;
    juce::TextButton playOnDeck1 { "Play on Deck 1" } ;
    juce::TextButton playOnDeck2 { "Play on Deck 2" } ;

    DeckGUI* deckGUI1;
    DeckGUI* deckGUI2;
    DJAudioPlayer* trackMetaData;

    juce::String getTrackLength(juce::URL audioURL);
    juce::String convertSecToMin(double seconds);

    //library components
    void importToLibrary();
    void searchLibrary(juce::String searchText);
    void saveLibrary();
    void loadLibrary();


    void deleteFromTracks(int trackID);
    bool isFileInPlaylist(juce::String trackName);
    int  getPlaylistLocation(juce::String searchText);
    void loadTrackIntoDeck(DeckGUI* deckGUI);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
