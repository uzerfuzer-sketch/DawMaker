#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

/**
 * @class CustomRotarySlider
 * @brief Slider rotatorio completamente personalizable
 * 
 * Este widget demuestra cómo crear controles personalizados en JUCE.
 * El estilo visual se define en CustomLookAndFeel.
 */
class CustomRotarySlider : public juce::Slider
{
public:
    explicit CustomRotarySlider(const juce::String& componentName = {});
    ~CustomRotarySlider() override = default;

    // Puedes añadir métodos personalizados aquí
    void setCustomColour(juce::Colour newColour);

private:
    juce::Colour customColour{juce::Colours::cyan};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomRotarySlider)
};
