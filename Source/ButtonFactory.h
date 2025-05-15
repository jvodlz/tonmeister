#pragma once
#ifndef ButtonFactory_H
#define ButtonFactory_H
#include <memory>
#include <JuceHeader.h>
#include "CustomButton.h"

class ButtonFactory
{
public:
	static std::unique_ptr<CustomButton> createMidiButton(int midiKey, const juce::String& buttonText, juce::MidiOutput* midiOutput, bool& loopEnabled);
};

#endif // !ButtonFactory_H