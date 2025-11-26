#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_utils/juce_audio_utils.h>

//==============================================================================
// CustomMeterLAF - LookAndFeel para medidores de nivel
//==============================================================================
class CustomMeterLAF : public juce::LookAndFeel_V4
{
public:
    CustomMeterLAF() = default;
    
    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
                         float sliderPos, float minSliderPos, float maxSliderPos,
                         const juce::Slider::SliderStyle style, juce::Slider& slider) override
    {
        if (style == juce::Slider::LinearVertical)
        {
            auto bounds = juce::Rectangle<int>(x, y, width, height).toFloat();
            auto meterBounds = bounds.reduced(2.0f);
            
            // Fondo del medidor
            g.setColour(juce::Colours::black.withAlpha(0.5f));
            g.fillRoundedRectangle(meterBounds, 2.0f);
            
            // Calcular altura del nivel
            auto value = (slider.getValue() - slider.getMinimum()) / (slider.getMaximum() - slider.getMinimum());
            auto levelHeight = meterBounds.getHeight() * value;
            
            // Dibujar nivel con gradiente de color
            auto levelBounds = meterBounds.removeFromBottom(levelHeight);
            
            juce::ColourGradient gradient;
            gradient.point1 = levelBounds.getBottomLeft();
            gradient.point2 = levelBounds.getTopLeft();
            
            // Verde a rojo según nivel
            if (value < 0.5f)
                gradient = juce::ColourGradient(juce::Colours::green, levelBounds.getBottomLeft(),
                                               juce::Colours::yellow, levelBounds.getTopLeft(), false);
            else if (value < 0.8f)
                gradient = juce::ColourGradient(juce::Colours::yellow, levelBounds.getBottomLeft(),
                                               juce::Colours::orange, levelBounds.getTopLeft(), false);
            else
                gradient = juce::ColourGradient(juce::Colours::orange, levelBounds.getBottomLeft(),
                                               juce::Colours::red, levelBounds.getTopLeft(), false);
            
            g.setGradientFill(gradient);
            g.fillRoundedRectangle(levelBounds, 2.0f);
            
            // Borde
            g.setColour(juce::Colours::white.withAlpha(0.3f));
            g.drawRoundedRectangle(meterBounds, 2.0f, 1.0f);
        }
    }
};

//==============================================================================
// BasicMeter - Medidor de nivel vertical básico
//==============================================================================
class BasicMeter : public juce::Component
{
public:
    BasicMeter()
    {
        addAndMakeVisible(meter);
        meter.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
        meter.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
        meter.setRange(-60.0, 0.0, 0.1);
        meter.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::white.withAlpha(0.5f));
        meter.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colours::black.withAlpha(0.5f));
        meter.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::black.withAlpha(0.8f));
        meter.setLookAndFeel(&meterLAF);
        
        setInterceptsMouseClicks(false, false);
    }
    
    ~BasicMeter() override
    {
        meter.setLookAndFeel(nullptr);
    }
    
    void paint(juce::Graphics& g) override
    {
        // Dibujar marcas de escala
        paintScaleMarks(g);
    }
    
    void resized() override
    {
        meter.setBounds(getLocalBounds());
    }
    
    template <typename SampleType>
    void setMeterValue(SampleType newMeterValue)
    {
        auto clampedValue = juce::jlimit(SampleType(-60.0), SampleType(0.0), newMeterValue);
        meter.setValue(clampedValue);
    }
    
    juce::Slider& getSlider() { return meter; }

private:
    void paintScaleMarks(juce::Graphics& g)
    {
        auto bounds = getLocalBounds().toFloat();
        g.setColour(juce::Colours::white.withAlpha(0.3f));
        g.setFont(8.0f);
        
        // Marcas cada 10 dB
        for (int db = 0; db >= -60; db -= 10)
        {
            auto proportion = (db + 60.0f) / 60.0f;
            auto y = bounds.getBottom() - (bounds.getHeight() * proportion);
            
            g.drawLine(bounds.getRight() - 3, y, bounds.getRight(), y, 1.0f);
            g.drawText(juce::String(db), bounds.getRight() + 2, y - 6, 30, 12, 
                      juce::Justification::centredLeft);
        }
    }
    
    juce::Slider meter;
    CustomMeterLAF meterLAF;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BasicMeter)
};

//==============================================================================
// VUMeter - Medidor VU con filmstrip
//==============================================================================
class VUMeter : public juce::Component
{
public:
    VUMeter()
    {
        setInterceptsMouseClicks(false, false);
        vuMeter.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
        vuMeter.setTextBoxStyle(juce::Slider::TextBoxLeft, true, 1, 1);
        vuMeter.setRange(-20.0, 3.0, 0.1);
        vuMeter.setSkewFactor(1.85);
        vuMeter.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colours::transparentBlack);
        vuMeter.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colours::transparentBlack);
        vuMeter.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::transparentBlack);
        vuMeter.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colours::transparentBlack);
        vuMeter.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::transparentBlack);
        vuMeter.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::transparentBlack);
        vuMeter.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::transparentBlack);
        addAndMakeVisible(vuMeter);
    }
    
    ~VUMeter() override = default;
    
    void paint(juce::Graphics& g) override
    {
        if (!mainVUImage.isValid() || !vuGridImage.isValid() || !vuBGImage.isValid())
        {
            // Dibujar un VU meter simple si no hay imágenes
            auto bounds = getLocalBounds().toFloat().reduced(4.0f);
            
            // Fondo
            g.setColour(juce::Colour(0xFF1A1A1A));
            g.fillRoundedRectangle(bounds, 4.0f);
            
            // Escala
            g.setColour(juce::Colours::white.withAlpha(0.8f));
            g.setFont(10.0f);
            
            auto needleBounds = bounds.reduced(10.0f);
            auto centerX = needleBounds.getCentreX();
            auto centerY = needleBounds.getBottom();
            
            // Arco de fondo
            g.setColour(juce::Colours::darkgrey);
            juce::Path arc;
            arc.addCentredArc(centerX, centerY, needleBounds.getWidth() * 0.4f, needleBounds.getWidth() * 0.4f,
                            0.0f, -2.4f, 0.4f, true);
            g.strokePath(arc, juce::PathStrokeType(2.0f));
            
            // Aguja
            auto value = (vuMeter.getValue() - vuMeter.getMinimum()) / (vuMeter.getMaximum() - vuMeter.getMinimum());
            auto angle = -2.4f + (value * 2.8f);
            
            juce::Path needle;
            needle.startNewSubPath(centerX, centerY);
            auto needleLength = needleBounds.getWidth() * 0.35f;
            needle.lineTo(centerX + needleLength * std::sin(angle), 
                         centerY - needleLength * std::cos(angle));
            
            g.setColour(juce::Colours::red);
            g.strokePath(needle, juce::PathStrokeType(2.0f));
            
            // Centro
            g.fillEllipse(centerX - 4, centerY - 4, 8, 8);
            
            return;
        }
        
        // Renderizar filmstrip si hay imágenes
        auto vuArea = getLocalBounds().toFloat();
        auto multGlow = 0.68;
        auto backY = getHeight() * 0.093 + 10;
        g.drawImage(vuBGImage, vuArea.withSizeKeepingCentre(getWidth() * multGlow, getHeight() * multGlow).withY(backY));
        
        auto multGridWidth = 0.6;
        auto multGridHeight = multGridWidth * 0.6;
        auto scaleY = getHeight() * 0.2 + 10;
        g.drawImage(vuGridImage, vuArea.withSizeKeepingCentre(getWidth() * multGridWidth, 
                    getHeight() * multGridHeight).withY(scaleY));
    }
    
    void resized() override
    {
        vuMeter.setBounds(getLocalBounds());
    }
    
    juce::Slider& getVUMeter() { return vuMeter; }
    
    void setVUImages(const juce::Image& main, const juce::Image& grid, const juce::Image& bg)
    {
        mainVUImage = main;
        vuGridImage = grid;
        vuBGImage = bg;
        repaint();
    }

private:
    juce::Image mainVUImage;
    juce::Image vuGridImage;
    juce::Image vuBGImage;
    juce::Slider vuMeter;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VUMeter)
};

//==============================================================================
// LedMeter - Medidor tipo LED bargraph
//==============================================================================
class LedMeter : public juce::Component
{
public:
    LedMeter()
    {
        vuMeter.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
        vuMeter.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
        vuMeter.setRange(-60.0, 0.0, 0.1);
        vuMeter.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colours::transparentBlack);
        vuMeter.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::transparentBlack);
        vuMeter.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::transparentBlack);
        addAndMakeVisible(vuMeter);
        
        setInterceptsMouseClicks(false, false);
    }
    
    ~LedMeter() override = default;
    
    void paint(juce::Graphics& g) override
    {
        auto bounds = getLocalBounds().toFloat().reduced(2.0f);
        auto ledHeight = bounds.getHeight() / numLEDs;
        auto value = (vuMeter.getValue() - vuMeter.getMinimum()) / (vuMeter.getMaximum() - vuMeter.getMinimum());
        auto activeLEDs = static_cast<int>(value * numLEDs);
        
        // Dibujar LEDs
        for (int i = 0; i < numLEDs; ++i)
        {
            auto ledBounds = bounds.removeFromBottom(ledHeight).reduced(1.0f);
            
            // Color según posición
            juce::Colour ledColour;
            if (i < numLEDs * 0.6f)
                ledColour = juce::Colours::green;
            else if (i < numLEDs * 0.85f)
                ledColour = juce::Colours::yellow;
            else
                ledColour = juce::Colours::red;
            
            // LED activo o apagado
            if (i < activeLEDs)
                g.setColour(ledColour);
            else
                g.setColour(ledColour.withAlpha(0.1f));
            
            g.fillRoundedRectangle(ledBounds, 1.0f);
        }
        
        // Borde
        g.setColour(juce::Colours::darkgrey);
        g.drawRoundedRectangle(bounds, 2.0f, 1.0f);
    }
    
    void resized() override
    {
        vuMeter.setBounds(getLocalBounds());
    }
    
    juce::Slider& getVUMeter() { return vuMeter; }
    
    void setMeterValue(float newMeterValue)
    {
        vuMeter.setValue(newMeterValue);
        repaint();
    }
    
    void setNumLEDs(int newNumLEDs)
    {
        numLEDs = newNumLEDs;
        repaint();
    }

private:
    juce::Slider vuMeter;
    int numLEDs = 20;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LedMeter)
};
