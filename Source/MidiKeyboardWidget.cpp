#include "MidiKeyboardWidget.h"

DraggableMidiKeyboard::DraggableMidiKeyboard(const juce::String& name, juce::MidiKeyboardState& state)
    : DraggableWidget(WidgetType::MidiKeyboard, name),
      keyboardState(state),
      keyboardComponent(state, juce::MidiKeyboardComponent::horizontalKeyboard)
{
    addAndMakeVisible(keyboardComponent);
    keyboardComponent.setColour(juce::MidiKeyboardComponent::whiteNoteColourId, juce::Colours::white);
    keyboardComponent.setColour(juce::MidiKeyboardComponent::blackNoteColourId, juce::Colours::black);
    keyboardComponent.setColour(juce::MidiKeyboardComponent::keySeparatorLineColourId, juce::Colours::grey);
    keyboardComponent.setColour(juce::MidiKeyboardComponent::mouseOverKeyOverlayColourId, juce::Colours::lightyellow.withAlpha(0.5f));
}

void DraggableMidiKeyboard::resized()
{
    DraggableWidget::resized();
    keyboardComponent.setBounds(getLocalBounds().reduced(10));
}

juce::ValueTree DraggableMidiKeyboard::toValueTree() const
{
    auto tree = DraggableWidget::toValueTree();
    // Aquí se podrían añadir propiedades específicas del teclado si las hubiera
    return tree;
}

void DraggableMidiKeyboard::paintWidget(juce::Graphics& g)
{
    // El componente de teclado se pinta a sí mismo, no necesitamos hacer nada aquí
}
