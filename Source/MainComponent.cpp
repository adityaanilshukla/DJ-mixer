#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);
    }

    addAndMakeVisible(deckGUI1);
    addAndMakeVisible(deckGUI2);

    addAndMakeVisible(playlistComponent);
    
    formatManager.registerBasicFormats();
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.D
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    player1.prepareToPlay( samplesPerBlockExpected, sampleRate );
    player2.prepareToPlay( samplesPerBlockExpected, sampleRate );
    mixerSource.prepareToPlay( samplesPerBlockExpected, sampleRate );
    mixerSource.addInputSource( &player1, false );
    mixerSource.addInputSource( &player2, false );
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
  mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
    
    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();
}
//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{

    const juce::Colour darkBlue = juce::Colour::fromRGB(7, 35, 64);

    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{

  auto libraryWidth = getWidth()/3;
  auto deckWidth = getWidth() - libraryWidth;

  deckGUI1.setBounds( 0, 0, deckWidth, getHeight() / 2 );

  deckGUI2.setBounds( 0, getHeight()/2 , deckWidth, getHeight() / 2 );

  playlistComponent.setBounds( deckWidth, 0, libraryWidth, getHeight() );
}










