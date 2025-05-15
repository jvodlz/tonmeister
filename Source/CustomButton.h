#pragma once
#ifndef CustomButton_H
#define CustomButton_H
#include <JuceHeader.h>

class CustomButton : public juce::TextButton, public juce::Timer
{
public:
	CustomButton(int midiKey, const juce::String& buttonText, juce::MidiOutput* midiOutput, bool& loopEnabled);
	//~CustomButton() override;
	void playMidiNote();
	void setLoopEnabled(bool enabled);
	bool isLoopEnabled();

private:
	int key;
	juce::MidiOutput* midiOutput; // Ptr to MIDI output
	static int activeNote;
	bool loopEnabled;

	void timerCallback() override;
};

#endif // !CustomButton_H