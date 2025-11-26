#include "CustomButton.h"

CustomButton::CustomButton(const juce::String& buttonText)
    : juce::TextButton(buttonText)
{
    // Configuración inicial del botón
    setClickingTogglesState(false);
}

void CustomButton::setActiveState(bool shouldBeActive)
{
    if (isActiveState != shouldBeActive)
    {
        isActiveState = shouldBeActive;
        repaint();
    }
}
