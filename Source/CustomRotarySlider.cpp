#include "CustomRotarySlider.h"

CustomRotarySlider::CustomRotarySlider(const juce::String& componentName)
    : juce::Slider(componentName)
{
    // Configurar como slider rotatorio
    setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    setRotaryParameters(juce::MathConstants<float>::pi * 1.2f,
                       juce::MathConstants<float>::pi * 2.8f,
                       true);
    
    // Configurar cuadro de texto
    setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    
    // Color personalizado (se puede cambiar dinámicamente)
    setColour(juce::Slider::rotarySliderFillColourId, customColour);
}

void CustomRotarySlider::setCustomColour(juce::Colour newColour)
{
    customColour = newColour;
    setColour(juce::Slider::rotarySliderFillColourId, customColour);
    repaint();
}
