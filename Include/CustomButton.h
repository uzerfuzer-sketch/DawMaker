#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

/**
 * @class CustomButton
 * @brief Botón completamente personalizable
 * 
 * Este widget demuestra cómo crear botones personalizados con estado visual.
 * El estilo se define en CustomLookAndFeel.
 */
class CustomButton : public juce::TextButton
{
public:
    explicit CustomButton(const juce::String& buttonText = {});
    ~CustomButton() override = default;

    // Métodos personalizados
    void setActiveState(bool shouldBeActive);
    bool isActive() const { return isActiveState; }

private:
    bool isActiveState = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomButton)
};
