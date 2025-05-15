#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() : synthAudioSource (keyboardState)
{
    midiOutput = juce::MidiOutput::openDevice(0);
    loopToggleButton.setButtonText("Loop");
    loopToggleButton.onClick = [this] { 
        toggleLoop(); 
        toggleLoopEnabled();
    };
    addAndMakeVisible(loopToggleButton);

    createMidiButtons(midiOutput.get());

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
        // 0 in, 2 out
        setAudioChannels (0, 2);
    }

    // Make sure you set the size of the component after
    // you add any child components.
    setSize (300, 360);

    //int midiChannel = 10; 
    startTimer (400);    

}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    juce::String message;
    message << "Preparing to play audio\n";
    message << " samplesPerBlockExpected = " << samplesPerBlockExpected << "\n";
    message << " sampleRate = " << sampleRate;
    juce::Logger::getCurrentLogger()->writeToLog(message);

    synthAudioSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    synthAudioSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.
    synthAudioSource.releaseResources();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    int xOffset = getWidth() * 0.2;
    int yOffset = getHeight() * 0.1;

    // Tuner 
    //g.fillRoundedRectangle(xOffset * factor, yOffset * factor, 160, 250, 10);
    g.fillRoundedRectangle(xOffset, yOffset, 160, 250, 10);
}

void MainComponent::resized() 
{
    auto buttonSize = 50;
    int yMargin = 20; // Space between buttons
    int xOffset = getWidth() * 0.2;
    int yOffset = getHeight() * 0.1;

    for (int i = 0; i < buttons.size(); ++i)
    {
        int row = i % 3;
        int column = i < 3 ? 0 : 1; // First three buttons in the left column, last three in the right column
        int spacing = i < 3 ? 20 : 40; // Space between buttons

        int x = column * buttonSize + xOffset + spacing; // Adjust x based on the column
        int y = row * (buttonSize + yMargin) + yOffset + yMargin + 10; // Adjust y based on the row

        buttons[i]->setBounds(x, y, buttonSize, buttonSize);
    }

    // Toggle Button
    loopToggleButton.setBounds(xOffset + 170, yOffset, 100, 30);
}

void MainComponent::createMidiButtons(juce::MidiOutput* midiOutput)
{
    const int midiKeys[6] = { 40, 45, 50, 55, 59, 64 };
    const juce::String buttonLabels[6] = { "E1", "A1", "D2", "G2", "B2", "E3" };

    for (int i = 0; i < 6; ++i)
    {
        auto button = ButtonFactory::createMidiButton(midiKeys[i], buttonLabels[i], midiOutput, loopEnabled);
        button->setBounds(10, 10 + i * 50, 100, 40);
        addAndMakeVisible (button.get());
        buttons.push_back(std::move(button));
    }
}

void MainComponent::toggleLoop()
{
    loopEnabled = loopToggleButton.getToggleState();
}

void MainComponent::timerCallback()
{
    stopTimer();
}

void MainComponent::toggleLoopEnabled()
{
    
    for (auto& button : buttons) {
        bool loopEnabledState = !loopEnabled; // Calculate the new state

        if (loopEnabledState != button->isLoopEnabled())
        {
            loopEnabled = loopEnabledState;
        }
        button->setLoopEnabled(loopEnabled);
    }
}