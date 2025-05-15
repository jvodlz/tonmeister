//#pragma once
//#include <JuceHeader.h>
#include "CustomButton.h"

int CustomButton::activeNote = -1;

CustomButton::CustomButton(int midiKey, const juce::String& buttonText, juce::MidiOutput* midiOutput, bool& loopEnabled)
	: TextButton(buttonText), key(midiKey), midiOutput(midiOutput), loopEnabled(loopEnabled)
{
	onClick = [this]() { playMidiNote(); };
}

void CustomButton::playMidiNote()
{
	if (midiOutput)
	{

		juce::MidiMessage noteMessage = juce::MidiMessage::noteOn(1, key, (juce::uint8)127); // Channel 1, velocity 127

		if (activeNote != -1 && activeNote != key && !loopEnabled)
		{
			if (isTimerRunning()) {
				stopTimer();
			}
			juce::MidiMessage noteMessage = juce::MidiMessage::noteOff(1, activeNote); // Channel 1
		}
		else if (loopEnabled) {
			// send noteMessage every 4s
			stopTimer();
			startTimer (4000);
		}

		midiOutput->sendMessageNow(noteMessage);
		activeNote = key;
	}
}

void CustomButton::timerCallback() {
	if (!loopEnabled) {
		stopTimer();
		juce::MidiMessage noteMessage = juce::MidiMessage::noteOff(1, activeNote); // Channel 1
		midiOutput->sendMessageNow(noteMessage);
	}

	else if (activeNote == key && midiOutput && activeNote != -1) {
		juce::MidiMessage noteMessage = juce::MidiMessage::noteOn(1, activeNote, (juce::uint8)127); // Channel 1
		midiOutput->sendMessageNow(noteMessage);
	}
}

void CustomButton::setLoopEnabled(bool enabled) {
	loopEnabled = enabled;
}

bool CustomButton::isLoopEnabled() {
	return loopEnabled;
}