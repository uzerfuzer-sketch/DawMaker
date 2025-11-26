#pragma once

#include "DraggableWidget.h"
#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_gui_extra/juce_gui_extra.h>

class DraggableMidiKeyboard : public DraggableWidget
{
public:
    DraggableMidiKeyboard(const juce::String& name, juce::MidiKeyboardState& state);
    
    void resized() override;
    juce::ValueTree toValueTree() const;

protected:
    void paintWidget(juce::Graphics& g) override;

private:
    juce::MidiKeyboardState& keyboardState;
    juce::MidiKeyboardComponent keyboardComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DraggableMidiKeyboard)
};
