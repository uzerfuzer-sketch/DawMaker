#include "CustomLookAndFeel.h"

CustomLookAndFeel::CustomLookAndFeel()
{
    initializeDefaultTheme();
    
    // Configurar fuente personalizada
    customFont = juce::Font("Arial", 14.0f, juce::Font::plain);
    setDefaultSansSerifTypefaceName("Arial");
}

void CustomLookAndFeel::initializeDefaultTheme()
{
    // Definir paleta de colores oscura profesional tipo DAW
    themeColours["background"] = juce::Colour(0xff1a1a2e);
    themeColours["backgroundDark"] = juce::Colour(0xff16213e);
    themeColours["primary"] = juce::Colour(0xff0f3460);
    themeColours["accent"] = juce::Colour(0xff00adb5);
    themeColours["accentBright"] = juce::Colour(0xff00d9e1);
    themeColours["highlight"] = juce::Colour(0xffe94560);
    themeColours["text"] = juce::Colour(0xffeeeeee);
    themeColours["textDark"] = juce::Colour(0xff999999);
    themeColours["border"] = juce::Colour(0xff2d2d44);
    themeColours["knobFill"] = juce::Colour(0xff00adb5);
    themeColours["knobOutline"] = juce::Colour(0xff00d9e1);

    // Configurar colores predeterminados de JUCE
    setColour(juce::ResizableWindow::backgroundColourId, themeColours["background"]);
    setColour(juce::DocumentWindow::textColourId, themeColours["text"]);
    setColour(juce::TextButton::buttonColourId, themeColours["primary"]);
    setColour(juce::TextButton::textColourOffId, themeColours["text"]);
    setColour(juce::TextButton::textColourOnId, themeColours["text"]);
    setColour(juce::Label::textColourId, themeColours["text"]);
}

void CustomLookAndFeel::drawRotarySlider(juce::Graphics& g,
                                        int x, int y, int width, int height,
                                        float sliderPosProportional,
                                        float rotaryStartAngle,
                                        float rotaryEndAngle,
                                        juce::Slider& slider)
{
    auto bounds = juce::Rectangle<int>(x, y, width, height).toFloat().reduced(10.0f);
    auto radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
    auto centreX = bounds.getCentreX();
    auto centreY = bounds.getCentreY();
    auto angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);

    // Dibujar fondo del knob (círculo exterior)
    g.setColour(themeColours["backgroundDark"]);
    g.fillEllipse(bounds);

    // Dibujar borde del knob
    g.setColour(themeColours["border"]);
    g.drawEllipse(bounds, 2.0f);

    // Dibujar el arco de progreso
    juce::Path valueArc;
    valueArc.addCentredArc(centreX, centreY,
                          radius * 0.8f, radius * 0.8f,
                          0.0f,
                          rotaryStartAngle, angle,
                          true);

    // Gradiente para el arco
    juce::ColourGradient gradient(
        themeColours["knobFill"], centreX, centreY - radius,
        themeColours["knobOutline"], centreX, centreY + radius,
        false
    );
    g.setGradientFill(gradient);
    g.strokePath(valueArc, juce::PathStrokeType(4.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    // Dibujar el indicador (línea desde el centro)
    juce::Path pointer;
    auto pointerLength = radius * 0.6f;
    auto pointerThickness = 3.0f;
    pointer.addRectangle(-pointerThickness * 0.5f, -radius * 0.6f, pointerThickness, pointerLength);
    pointer.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));

    g.setColour(themeColours["accentBright"]);
    g.fillPath(pointer);

    // Dibujar punto central
    auto dotSize = 8.0f;
    g.setColour(themeColours["background"]);
    g.fillEllipse(centreX - dotSize / 2.0f, centreY - dotSize / 2.0f, dotSize, dotSize);
    g.setColour(themeColours["accent"]);
    g.fillEllipse(centreX - dotSize / 2.0f + 1.0f, centreY - dotSize / 2.0f + 1.0f, dotSize - 2.0f, dotSize - 2.0f);
}

void CustomLookAndFeel::drawButtonBackground(juce::Graphics& g,
                                            juce::Button& button,
                                            const juce::Colour& backgroundColour,
                                            bool shouldDrawButtonAsHighlighted,
                                            bool shouldDrawButtonAsDown)
{
    auto bounds = button.getLocalBounds().toFloat().reduced(2.0f);
    auto cornerSize = 6.0f;

    // Determinar color basado en estado
    juce::Colour baseColour = themeColours["primary"];
    
    if (button.getToggleState())
    {
        baseColour = themeColours["accent"];
    }
    else if (shouldDrawButtonAsDown)
    {
        baseColour = themeColours["primary"].darker(0.3f);
    }
    else if (shouldDrawButtonAsHighlighted)
    {
        baseColour = themeColours["primary"].brighter(0.1f);
    }

    // Dibujar fondo con gradiente
    juce::ColourGradient gradient(
        baseColour.brighter(0.1f), bounds.getCentreX(), bounds.getY(),
        baseColour.darker(0.2f), bounds.getCentreX(), bounds.getBottom(),
        false
    );
    g.setGradientFill(gradient);
    g.fillRoundedRectangle(bounds, cornerSize);

    // Dibujar borde
    g.setColour(shouldDrawButtonAsHighlighted ? themeColours["accentBright"] : themeColours["border"]);
    g.drawRoundedRectangle(bounds, cornerSize, 2.0f);

    // Efecto de brillo en la parte superior
    if (shouldDrawButtonAsHighlighted || button.getToggleState())
    {
        auto highlight = bounds.removeFromTop(bounds.getHeight() * 0.3f);
        g.setColour(juce::Colours::white.withAlpha(0.1f));
        g.fillRoundedRectangle(highlight, cornerSize);
    }
}

void CustomLookAndFeel::drawButtonText(juce::Graphics& g,
                                       juce::TextButton& button,
                                       bool shouldDrawButtonAsHighlighted,
                                       bool shouldDrawButtonAsDown)
{
    auto font = customFont.withHeight(15.0f);
    g.setFont(font);

    // Color del texto
    juce::Colour textColour = themeColours["text"];
    if (!button.isEnabled())
        textColour = themeColours["textDark"];
    else if (button.getToggleState())
        textColour = juce::Colours::white;
    else if (shouldDrawButtonAsHighlighted)
        textColour = themeColours["accentBright"];

    g.setColour(textColour);

    auto textArea = button.getLocalBounds().reduced(4);
    g.drawText(button.getButtonText(), textArea, juce::Justification::centred, true);
}

void CustomLookAndFeel::drawLabel(juce::Graphics& g, juce::Label& label)
{
    g.fillAll(label.findColour(juce::Label::backgroundColourId));

    if (!label.isBeingEdited())
    {
        auto alpha = label.isEnabled() ? 1.0f : 0.5f;
        auto font = getLabelFont(label);

        g.setColour(label.findColour(juce::Label::textColourId).withMultipliedAlpha(alpha));
        g.setFont(font);

        auto textArea = getLabelBorderSize(label).subtractedFrom(label.getLocalBounds());

        g.drawFittedText(label.getText(), textArea, label.getJustificationType(),
                        juce::jmax(1, (int)((float)textArea.getHeight() / font.getHeight())),
                        label.getMinimumHorizontalScale());

        g.setColour(label.findColour(juce::Label::outlineColourId).withMultipliedAlpha(alpha));
    }
    else if (label.isEnabled())
    {
        g.setColour(label.findColour(juce::Label::outlineColourId));
    }

    g.drawRect(label.getLocalBounds());
}

void CustomLookAndFeel::setThemeColour(const juce::String& name, juce::Colour colour)
{
    themeColours[name] = colour;
}

juce::Colour CustomLookAndFeel::getThemeColour(const juce::String& name) const
{
    auto it = themeColours.find(name);
    if (it != themeColours.end())
        return it->second;
    
    return juce::Colours::white; // Color por defecto
}
