#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

//==============================================================================
// CustomImageButton - Botón con imágenes separadas para on/off
//==============================================================================
class CustomImageButton : public juce::Component
{
public:
    enum class ButtonType
    {
        kLED,
        kDarkFlat,
        kLightFlat
    };
    
    CustomImageButton(const juce::Image& offImage,
                     const juce::Image& onImage,
                     const juce::String& offText,
                     const juce::String& onText)
        : btnOffText(offText), btnOnText(onText)
    {
        setInterceptsMouseClicks(true, true);
        
        // Botón
        button.setClickingTogglesState(true);
        button.setImages(false, true, true, 
                        offImage, 1.0f, juce::Colours::transparentBlack,
                        offImage, 1.0f, juce::Colours::transparentBlack,
                        onImage, 1.0f, juce::Colours::transparentBlack);
        addAndMakeVisible(button);
        
        // Label
        addAndMakeVisible(btnLabel);
        btnLabel.setJustificationType(juce::Justification::centred);
        btnLabel.setFont(juce::Font("Arial", 10.0f, juce::Font::FontStyleFlags::bold));
        btnLabel.setColour(juce::Label::textColourId, juce::Colours::whitesmoke);
        
        // Callback para cambiar texto
        button.onClick = [this]()
        {
            updateLabelText();
        };
        
        updateLabelText();
    }
    
    ~CustomImageButton() override = default;
    
    void paint(juce::Graphics& g) override
    {
        // Dibujar fondo según tipo de botón
        auto bounds = getLocalBounds().toFloat();
        
        switch (buttonType)
        {
            case ButtonType::kLED:
            {
                // Efecto LED brillante
                if (button.getToggleState())
                {
                    g.setColour(juce::Colours::green.withAlpha(0.3f));
                    g.fillEllipse(bounds.reduced(2.0f));
                }
                break;
            }
            
            case ButtonType::kDarkFlat:
            {
                g.setColour(juce::Colour(0xFF2A2A2A));
                g.fillRoundedRectangle(bounds.reduced(2.0f), 4.0f);
                
                if (button.getToggleState())
                {
                    g.setColour(juce::Colour(0xFF4A90E2));
                    g.drawRoundedRectangle(bounds.reduced(2.0f), 4.0f, 2.0f);
                }
                break;
            }
            
            case ButtonType::kLightFlat:
            {
                if (button.getToggleState())
                {
                    g.setColour(juce::Colours::lightgrey);
                    g.fillRoundedRectangle(bounds.reduced(2.0f), 4.0f);
                }
                else
                {
                    g.setColour(juce::Colours::darkgrey);
                    g.fillRoundedRectangle(bounds.reduced(2.0f), 4.0f);
                }
                break;
            }
        }
    }
    
    void resized() override
    {
        auto bounds = getLocalBounds();
        
        // Botón arriba, label abajo
        auto labelHeight = 16;
        btnLabel.setBounds(bounds.removeFromBottom(labelHeight));
        button.setBounds(bounds);
    }
    
    juce::ImageButton& getButton() { return button; }
    juce::Label& getLabel() { return btnLabel; }
    
    void setButtonType(ButtonType newBtnType)
    {
        buttonType = newBtnType;
        repaint();
    }
    
    void setButtonText(const juce::String& offText, const juce::String& onText)
    {
        btnOffText = offText;
        btnOnText = onText;
        updateLabelText();
    }

private:
    void updateLabelText()
    {
        if (button.getToggleState())
        {
            btnLabel.setText(btnOnText, juce::dontSendNotification);
            btnLabel.setColour(juce::Label::textColourId, textOn);
        }
        else
        {
            btnLabel.setText(btnOffText, juce::dontSendNotification);
            btnLabel.setColour(juce::Label::textColourId, textOff);
        }
    }
    
    juce::ImageButton button;
    juce::String btnOffText;
    juce::String btnOnText;
    juce::Label btnLabel;
    
    ButtonType buttonType = ButtonType::kLED;
    juce::Colour textOff = juce::Colours::whitesmoke.withAlpha(0.5f);
    juce::Colour textOn = juce::Colours::white;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomImageButton)
};

//==============================================================================
// SimpleToggleButton - Botón toggle simple sin imágenes
//==============================================================================
class SimpleToggleButton : public juce::ToggleButton
{
public:
    SimpleToggleButton(const juce::String& buttonText = "")
        : juce::ToggleButton(buttonText)
    {
        setColour(juce::ToggleButton::textColourId, juce::Colours::white);
        setColour(juce::ToggleButton::tickColourId, juce::Colour(0xFF4A90E2));
        setColour(juce::ToggleButton::tickDisabledColourId, juce::Colours::grey);
    }
    
    ~SimpleToggleButton() override = default;
    
    void paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, 
                    bool shouldDrawButtonAsDown) override
    {
        auto bounds = getLocalBounds().toFloat();
        auto tickBounds = bounds.removeFromLeft(bounds.getHeight()).reduced(4.0f);
        
        // Fondo del checkbox
        g.setColour(juce::Colour(0xFF2A2A2A));
        g.fillRoundedRectangle(tickBounds, 3.0f);
        
        // Borde
        if (shouldDrawButtonAsHighlighted)
            g.setColour(juce::Colour(0xFF4A90E2).brighter());
        else
            g.setColour(juce::Colours::grey);
        
        g.drawRoundedRectangle(tickBounds, 3.0f, 1.5f);
        
        // Checkmark si está activado
        if (getToggleState())
        {
            g.setColour(findColour(juce::ToggleButton::tickColourId));
            
            juce::Path tick;
            tick.startNewSubPath(tickBounds.getX() + tickBounds.getWidth() * 0.25f,
                               tickBounds.getCentreY());
            tick.lineTo(tickBounds.getCentreX(), 
                       tickBounds.getY() + tickBounds.getHeight() * 0.75f);
            tick.lineTo(tickBounds.getRight() - tickBounds.getWidth() * 0.25f,
                       tickBounds.getY() + tickBounds.getHeight() * 0.25f);
            
            g.strokePath(tick, juce::PathStrokeType(2.5f));
        }
        
        // Texto
        if (getButtonText().isNotEmpty())
        {
            g.setColour(findColour(juce::ToggleButton::textColourId));
            g.setFont(14.0f);
            
            auto textBounds = bounds.withTrimmedLeft(tickBounds.getWidth() + 8);
            g.drawText(getButtonText(), textBounds.toNearestInt(), 
                      juce::Justification::centredLeft);
        }
    }
};
