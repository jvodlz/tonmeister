#include "ButtonFactory.h"

std::unique_ptr<CustomButton> ButtonFactory::createMidiButton(int midiKey, const juce::String& buttonText, juce::MidiOutput* midiOutput, bool& loopEnabled)
{
	return std::make_unique<CustomButton>(midiKey, buttonText, midiOutput, loopEnabled);
}
