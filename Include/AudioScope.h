#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_utils/juce_audio_utils.h>

//==============================================================================
// AudioScope - Osciloscopio de audio con AudioVisualiserComponent
//==============================================================================
class AudioScope : public juce::Component
{
public:
    AudioScope(int numChannels = 2)
    {
        scopeModule = std::make_unique<juce::AudioVisualiserComponent>(numChannels);
        
        scopeModule->setColours(juce::Colours::black, juce::Colours::green.brighter());
        scopeModule->setRepaintRate(30);
        scopeModule->setSamplesPerBlock(128);
        scopeModule->setBufferSize(512);
        addAndMakeVisible(*scopeModule);
        
        // Controles
        setupSlider(sampleDial, "Samples", 1.0, 1024.0, 128.0);
        setupSlider(bufferDial, "Buffer", 128.0, 2048.0, 512.0);
        setupSlider(repaintDial, "Refresh", 1.0, 120.0, 30.0);
        
        sampleDial.onValueChange = [this]()
        {
            scopeModule->setSamplesPerBlock(static_cast<int>(sampleDial.getValue()));
        };
        
        bufferDial.onValueChange = [this]()
        {
            scopeModule->setBufferSize(static_cast<int>(bufferDial.getValue()));
        };
        
        repaintDial.onValueChange = [this]()
        {
            scopeModule->setRepaintRate(static_cast<int>(repaintDial.getValue()));
        };
    }
    
    ~AudioScope() override = default;
    
    void paint(juce::Graphics& g) override
    {
        // Fondo
        g.fillAll(juce::Colour(0xFF1A1A1A));
        
        // Grid
        g.setColour(juce::Colours::darkgrey.withAlpha(0.3f));
        auto bounds = scopeModule->getBounds().toFloat();
        
        // Líneas horizontales
        for (int i = 1; i < 4; ++i)
        {
            auto y = bounds.getY() + (bounds.getHeight() * i / 4.0f);
            g.drawLine(bounds.getX(), y, bounds.getRight(), y, 0.5f);
        }
        
        // Líneas verticales
        for (int i = 1; i < 8; ++i)
        {
            auto x = bounds.getX() + (bounds.getWidth() * i / 8.0f);
            g.drawLine(x, bounds.getY(), x, bounds.getBottom(), 0.5f);
        }
        
        // Borde
        g.setColour(juce::Colours::grey);
        g.drawRect(bounds, 1.0f);
    }
    
    void resized() override
    {
        auto bounds = getLocalBounds();
        
        // Área de controles (arriba)
        auto controlArea = bounds.removeFromTop(40);
        auto dialWidth = controlArea.getWidth() / 3;
        
        sampleDial.setBounds(controlArea.removeFromLeft(dialWidth).reduced(4));
        bufferDial.setBounds(controlArea.removeFromLeft(dialWidth).reduced(4));
        repaintDial.setBounds(controlArea.reduced(4));
        
        // Scope ocupa el resto
        scopeModule->setBounds(bounds.reduced(4));
    }
    
    void fillBuffer(juce::AudioBuffer<float>& buffer)
    {
        scopeModule->pushBuffer(buffer);
    }
    
    void setSamplesPerBlock(int samplesPerBlock)
    {
        scopeModule->setSamplesPerBlock(samplesPerBlock);
        sampleDial.setValue(samplesPerBlock, juce::dontSendNotification);
    }
    
    void setBufferSize(int bufferSize)
    {
        scopeModule->setBufferSize(bufferSize);
        bufferDial.setValue(bufferSize, juce::dontSendNotification);
    }
    
    void setRepaintRate(int repaintRate)
    {
        scopeModule->setRepaintRate(repaintRate);
        repaintDial.setValue(repaintRate, juce::dontSendNotification);
    }
    
    void setColors(juce::Colour background, juce::Colour waveform)
    {
        scopeModule->setColours(background, waveform);
        repaint();
    }

private:
    void setupSlider(juce::Slider& slider, const juce::String& name, 
                    double min, double max, double defaultValue)
    {
        slider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 16);
        slider.setRange(min, max, 1.0);
        slider.setValue(defaultValue);
        slider.setName(name);
        addAndMakeVisible(slider);
    }
    
    std::unique_ptr<juce::AudioVisualiserComponent> scopeModule;
    juce::Slider sampleDial;
    juce::Slider bufferDial;
    juce::Slider repaintDial;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioScope)
};
