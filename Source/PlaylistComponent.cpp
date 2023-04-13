/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 17 Feb 2023 3:20:42am
    Author:  aditya

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent( DeckGUI* _deckGUI1, DeckGUI* _deckGUI2, DJAudioPlayer* _trackMetaData )
  
      : deckGUI1( _deckGUI1 ), deckGUI2( _deckGUI2 ), trackMetaData( _trackMetaData )

{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    
    //display library GUI items
    addAndMakeVisible( library );
    addAndMakeVisible( addTrack );
    addAndMakeVisible( searchBar );
    addAndMakeVisible( playOnDeck1 );
    addAndMakeVisible( playOnDeck2 );  

    //add listensers to library GUI items
    addTrack.addListener( this );
    searchBar.addListener( this );
    playOnDeck1.addListener( this );
    playOnDeck2.addListener( this );


    searchBar.setTextToShowWhenEmpty("Search for track in playlist", lightBlue);
    searchBar.setFont(juce::Font(18.0f));
    searchBar.onReturnKey = [this] { searchLibrary (searchBar.getText()); };
    

    library.getHeader().addColumn("Name", 1, 1);
    library.getHeader().addColumn("Length", 2, 1);
    library.getHeader().addColumn("", 3, 1);
    library.setModel(this);
    loadLibrary();
}

PlaylistComponent::~PlaylistComponent()
{
  saveLibrary();
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background
    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);

    addTrack.setColour(juce::TextButton::buttonColourId, darkerGrey);
    playOnDeck1.setColour(juce::TextButton::buttonColourId, darkerGrey);
    playOnDeck2.setColour(juce::TextButton::buttonColourId, darkerGrey);

    library.setColour(juce::ListBox::backgroundColourId, darkerGrey);


}

void PlaylistComponent::resized()
{
    auto playOnDeckButtonWidth = getWidth() / 2;
    auto columnWidth = getWidth() / 20;
    auto segmentHeight = getHeight() / 16;

    library.setBounds(0, 1 * getHeight() / 16, getWidth(), 13 * segmentHeight);

    playOnDeck1.setBounds(0, 0, playOnDeckButtonWidth, segmentHeight);
    playOnDeck2.setBounds(playOnDeckButtonWidth, 0, playOnDeckButtonWidth, segmentHeight);

    searchBar.setBounds(0, 14 * segmentHeight, getWidth(), segmentHeight);

    addTrack.setBounds(0, 15 * segmentHeight, getWidth(), segmentHeight);

    //set columns
    library.getHeader().setColumnWidth(1, 12.8 * columnWidth);
    library.getHeader().setColumnWidth(2, 5 * columnWidth);
    library.getHeader().setColumnWidth(3, 2 * columnWidth);
}



int PlaylistComponent::getNumRows()
{
  return audioTracks.size();
}

void PlaylistComponent::paintRowBackground ( juce::Graphics& g,
                                             int rowNumber, 
                                             int width, 
                                             int height, 
                                             bool rowIsSelected )
{
  if(rowIsSelected)
  {
    g.fillAll(lightBlue);
  }
  else
  {
    g.fillAll(juce::Colours::darkgrey);
  }
}


void PlaylistComponent::paintCell (juce::Graphics & g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) 

{
    if (rowNumber < getNumRows())
    {
        if (columnId == 1)
        {
            g.drawText(audioTracks[rowNumber].title,
                2,
                0,
                width - 4,
                height,
                juce::Justification::centredLeft,
                true
            );
        }
        if (columnId == 2)
        {
            g.drawText(audioTracks[rowNumber].length,
                2,
                0,
                width - 4,
                height,
                juce::Justification::centred,
                true
            );
        }
    }

}


PlaylistComponent::Component* PlaylistComponent::refreshComponentForCell(  int rowNumber, int columnId,
                                                        bool rowIsSelected, juce::Component *existingComponentToUpdate)
{

    if (columnId == 3)
    {
        if (existingComponentToUpdate == nullptr)
        {
            juce::TextButton* btn = new juce::TextButton{ "DEL" };
            juce::String id{ std::to_string(rowNumber) };
            btn->setComponentID(id);
            
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(juce::Button* button)
{
    if (button == &addTrack)
    {
        importToLibrary();
        library.updateContent();
    }
    else if (button == &playOnDeck1)
    {
        loadTrackIntoDeck(deckGUI1);
    }
    else if (button == &playOnDeck2)
    {
        loadTrackIntoDeck(deckGUI2);
    }
    else //button clicked is delete button
    {
        int id = std::stoi(button->getComponentID().toStdString());
        DBG(audioTracks[id].title + " removed from Library");
        deleteFromTracks(id);
        library.updateContent();
    }

}


void PlaylistComponent::importToLibrary()
{
    DBG("PlaylistComponent::importToLibrary called");

    //initialize file chooser
    juce::FileChooser chooser{ "Select files" };
    if (chooser.browseForMultipleFilesToOpen())
    {
        for (const juce::File& file : chooser.getResults())
        {
            juce::String fileNameWithoutExtension{ file.getFileNameWithoutExtension() };
            if (!isFileInPlaylist(fileNameWithoutExtension)) // if not already loaded
            {
                AudioTrack newTrack{ file };
                juce::URL audioURL{ file };
                newTrack.length = getTrackLength(audioURL) ;
                audioTracks.push_back(newTrack);
            }
            else // display info message
            {
                juce::AlertWindow::showMessageBox(juce::AlertWindow::AlertIconType::WarningIcon,
                    "Oops!:",
                    fileNameWithoutExtension + " is already in your playlist",
                    "close",
                    nullptr
                );
            }
        }
    }
}

bool PlaylistComponent::isFileInPlaylist(juce::String trackName)
{
    return (std::find(audioTracks.begin(), audioTracks.end(), trackName) != audioTracks.end());
}

juce::String PlaylistComponent::convertSecToMin(double seconds)
{
    //find seconds and minutes and make into string
    int secondsRounded{ int(std::round(seconds)) };
    juce::String min{ std::to_string(secondsRounded / 60) };
    juce::String sec{ std::to_string(secondsRounded % 60) };
    
    //ensure that seconds is always 2 digits
    if (sec.length() < 2)
    {
        sec = sec.paddedLeft('0', 2);
    }
    return juce::String{ min + ":" + sec };
}

void PlaylistComponent::searchLibrary(juce::String searchText)
{
    if (searchText != "")
    {
        int rowNumber = getPlaylistLocation(searchText);
        library.selectRow(rowNumber);
    }
    else
    {
        library.deselectAllRows();
    }
}

int PlaylistComponent::getPlaylistLocation(juce::String searchText)
{
    // finds index where track title contains searchText
    auto it = find_if(audioTracks.begin(), audioTracks.end(),
        [&searchText](const AudioTrack& obj) {return obj.title.contains(searchText); });
    int i = -1;

    if (it != audioTracks.end())
    {
        i = std::distance(audioTracks.begin(), it);
    }

    return i;
}

void PlaylistComponent::saveLibrary()
{
    // create .csv to save library
    std::ofstream Library("playlistLibrary.csv");

    // save library to file
    for (AudioTrack& trackS : audioTracks)
    {
        Library << trackS.file.getFullPathName() << "," << trackS.length << "\n";
    }
}

void PlaylistComponent::loadLibrary()
{
    // create input stream from saved library
    std::ifstream Library("playlistLibrary.csv");
    std::string filePath;
    std::string length;

    // Read data, line by line
    if (Library.is_open())
    {
        while (getline(Library, filePath, ',')) {
            juce::File file{ filePath };
            AudioTrack newTrack{ file };

            getline(Library, length);
            newTrack.length = length;
            audioTracks.push_back(newTrack);
        }
    }
    Library.close();
}

void PlaylistComponent::deleteFromTracks(int trackId)
{
    audioTracks.erase(audioTracks.begin() + trackId);
}

juce::String PlaylistComponent::getTrackLength(juce::URL audioURL)
{
    trackMetaData->loadURL( audioURL );
    double seconds{ trackMetaData->getLengthInSeconds() };
    juce::String minutes{ convertSecToMin(seconds) };
    return minutes;
}


void PlaylistComponent::loadTrackIntoDeck(DeckGUI* deckGUI)
{
    int row{ library.getrow() };
    if (row != -1)
    {
        deckGUI->loadFile(audioTracks[row].URL);
        deckGUI->fileLoaded = true;
        deckGUI->changePlayingState();
    }
    else
    {
        juce::AlertWindow::showMessageBox(juce::AlertWindow::AlertIconType::WarningIcon,
            "Oops!",
            "please select a track to add to either deck 1 or 2",
            "close",
            nullptr
        );
    }
}
