#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

//==============================================================================
// CustomFaderLAF - LookAndFeel para faders verticales
//==============================================================================
class CustomFaderLAF : public juce::LookAndFeel_V4
{
public:
    enum class ValueType
    {
        kInt,
        kFloat
    };
    
    CustomFaderLAF() = default;
    
    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
                         float sliderPos, float minSliderPos, float maxSliderPos,
                         const juce::Slider::SliderStyle style, juce::Slider& slider) override
    {
        if (style == juce::Slider::LinearVertical)
        {
            auto bounds = juce::Rectangle<int>(x, y, width, height).toFloat();
            auto trackBounds = bounds.reduced(width * 0.2f, 0);
            
            // Track
            g.setColour(slider.findColour(juce::Slider::backgroundColourId));
            g.fillRoundedRectangle(trackBounds, 3.0f);
            
            // Fill (nivel actual)
            auto fillHeight = sliderPos - trackBounds.getY();
            auto fillBounds = trackBounds.withHeight(fillHeight);
            
            juce::ColourGradient gradient(
                juce::Colour(0xFF4A90E2), fillBounds.getBottomLeft(),
                juce::Colour(0xFF357ABD), fillBounds.getTopLeft(), false);
            
            g.setGradientFill(gradient);
            g.fillRoundedRectangle(fillBounds, 3.0f);
            
            // Thumb (perilla)
            auto thumbWidth = width * 0.8f;
            auto thumbHeight = 8.0f;
            auto thumbBounds = juce::Rectangle<float>(thumbWidth, thumbHeight)
                .withCentre(juce::Point<float>(bounds.getCentreX(), sliderPos));
            
            g.setColour(juce::Colours::white);
            g.fillRoundedRectangle(thumbBounds, 2.0f);
            
            // Borde del thumb
            g.setColour(juce::Colours::grey);
            g.drawRoundedRectangle(thumbBounds, 2.0f, 1.0f);
        }
    }
    
    void drawLabel(juce::Graphics& g, juce::Label& label) override
    {
        g.setColour(label.findColour(juce::Label::textColourId));
        g.setFont(12.0f);
        
        auto text = label.getText();
        if (dialValueType == ValueType::kInt)
        {
            auto value = text.getFloatValue();
            text = juce::String(static_cast<int>(value));
        }
        
        g.drawText(text, label.getLocalBounds(), juce::Justification::centred);
    }
    
    void setDialValueType(ValueType newValueType)
    {
        dialValueType = newValueType;
    }

private:
    ValueType dialValueType = ValueType::kFloat;
};

//==============================================================================
// VerticalFader - Fader vertical con soporte para filmstrip
//==============================================================================
class VerticalFader : public juce::Slider
{
public:
    VerticalFader()
    {
        setRange(-15.0, 15.0, 0.1);
        setTextBoxStyle(juce::Slider::TextBoxBelow, false, 64, 20);
        setTextValueSuffix(" dB");
        setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
        setDoubleClickReturnValue(true, 0.0);
        setColour(juce::Slider::backgroundColourId, juce::Colour(0xFF2A2A2A));
        setColour(juce::Slider::trackColourId, juce::Colour(0xFF4A90E2));
        setColour(juce::Slider::thumbColourId, juce::Colours::white);
        setLookAndFeel(&customFaderLAF);
        
        // Shadow
        shadowProperties.radius = 5;
        shadowProperties.offset = juce::Point<int>(0, 0);
        shadowProperties.colour = juce::Colours::black.withAlpha(0.6f);
        dialShadow.setShadowProperties(shadowProperties);
    }
    
    VerticalFader(int numFrames, const juce::Image& filmStrip)
        : numFilmstripFrames(numFrames), filmstripImage(filmStrip)
    {
        setRange(-15.0, 15.0, 0.1);
        setTextBoxStyle(juce::Slider::TextBoxBelow, false, 64, 20);
        setTextValueSuffix(" dB");
        setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
        setDoubleClickReturnValue(true, 0.0);
        useFilmstrip = true;
        
        if (filmstripImage.isValid() && numFilmstripFrames > 0)
        {
            frameWidth = filmstripImage.getWidth();
            frameHeight = filmstripImage.getHeight() / numFilmstripFrames;
        }
    }
    
    ~VerticalFader() override
    {
        setLookAndFeel(nullptr);
    }
    
    void paint(juce::Graphics& g) override
    {
        if (useFilmstrip && filmstripImage.isValid() && numFilmstripFrames > 0)
        {
            // Renderizar filmstrip
            auto value = (getValue() - getMinimum()) / (getMaximum() - getMinimum());
            auto frameIndex = static_cast<int>(value * (numFilmstripFrames - 1));
            frameIndex = juce::jlimit(0, numFilmstripFrames - 1, frameIndex);
            
            auto sourceY = frameIndex * frameHeight;
            auto bounds = getLocalBounds();
            
            g.drawImage(filmstripImage, 
                       bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight(),
                       0, sourceY, frameWidth, frameHeight,
                       false);
        }
        else
        {
            juce::Slider::paint(g);
        }
    }
    
    void resized() override
    {
        juce::Slider::resized();
    }
    
    void setDialValueType(CustomFaderLAF::ValueType newValueType)
    {
        customFaderLAF.setDialValueType(newValueType);
    }
    
    void setFaderColors(juce::Colour background, juce::Colour track, juce::Colour thumb)
    {
        setColour(juce::Slider::backgroundColourId, background);
        setColour(juce::Slider::trackColourId, track);
        setColour(juce::Slider::thumbColourId, thumb);
        repaint();
    }

private:
    CustomFaderLAF customFaderLAF;
    
    // Shadow
    juce::DropShadow shadowProperties;
    juce::DropShadowEffect dialShadow;
    
    // Filmstrip
    bool useFilmstrip = false;
    int numFilmstripFrames = 0;
    int frameWidth = 0;
    int frameHeight = 0;
    juce::Image filmstripImage;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VerticalFader)
};
