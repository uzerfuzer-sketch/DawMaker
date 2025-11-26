#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <map>

/**
 * @class CustomLookAndFeel
 * @brief Clase de apariencia personalizada que controla el estilo visual completo
 * 
 * Esta clase demuestra el poder de LookAndFeel en JUCE, permitiendo
 * personalizar completamente la apariencia de todos los widgets:
 * - Colores personalizados
 * - Formas y geometrías
 * - Grosor de líneas
 * - Fuentes y tipografías
 * - Sombras y efectos
 * - Animaciones
 * 
 * Cada método draw* puede ser sobrescrito para control total.
 */
class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeel();
    ~CustomLookAndFeel() override = default;

    // Personalización de sliders rotatorios
    void drawRotarySlider(juce::Graphics& g,
                         int x, int y, int width, int height,
                         float sliderPosProportional,
                         float rotaryStartAngle,
                         float rotaryEndAngle,
                         juce::Slider& slider) override;

    // Personalización de botones
    void drawButtonBackground(juce::Graphics& g,
                             juce::Button& button,
                             const juce::Colour& backgroundColour,
                             bool shouldDrawButtonAsHighlighted,
                             bool shouldDrawButtonAsDown) override;

    void drawButtonText(juce::Graphics& g,
                       juce::TextButton& button,
                       bool shouldDrawButtonAsHighlighted,
                       bool shouldDrawButtonAsDown) override;

    // Personalización de labels
    void drawLabel(juce::Graphics& g, juce::Label& label) override;

    // Métodos para configurar el tema
    void setThemeColour(const juce::String& name, juce::Colour colour);
    juce::Colour getThemeColour(const juce::String& name) const;

private:
    // Paleta de colores personalizada
    std::map<juce::String, juce::Colour> themeColours;

    // Configuración de fuentes
    juce::Font customFont;

    void initializeDefaultTheme();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomLookAndFeel)
};
