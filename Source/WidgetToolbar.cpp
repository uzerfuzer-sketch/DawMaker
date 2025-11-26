#include "WidgetToolbar.h"

WidgetToolbar::WidgetToolbar()
{
    // Configurar botones de añadir widgets
    addKnobButton.setButtonText("+ Knob");
    addKnobButton.onClick = [this] {
        if (onAddWidget)
            onAddWidget(DraggableWidget::WidgetType::Knob);
    };
    addAndMakeVisible(addKnobButton);
    
    addSliderButton.setButtonText("+ Slider");
    addSliderButton.onClick = [this] {
        if (onAddWidget)
            onAddWidget(DraggableWidget::WidgetType::Slider);
    };
    addAndMakeVisible(addSliderButton);
    
    addButtonButton.setButtonText("+ Button");
    addButtonButton.onClick = [this] {
        if (onAddWidget)
            onAddWidget(DraggableWidget::WidgetType::Button);
    };
    addAndMakeVisible(addButtonButton);
    
    addLabelButton.setButtonText("+ Label");
    addLabelButton.onClick = [this] {
        if (onAddWidget)
            onAddWidget(DraggableWidget::WidgetType::Label);
    };
    addAndMakeVisible(addLabelButton);
    
    addWaveformButton.setButtonText("+ Waveform");
    addWaveformButton.onClick = [this] {
        if (onAddWidget)
            onAddWidget(DraggableWidget::WidgetType::Waveform);
    };
    addAndMakeVisible(addWaveformButton);
    
    addMeterButton.setButtonText("+ Meter");
    addMeterButton.onClick = [this] {
        if (onAddWidget)
            onAddWidget(DraggableWidget::WidgetType::Meter);
    };
    addAndMakeVisible(addMeterButton);
    
    saveButton.setButtonText("💾 Save");
    saveButton.onClick = [this] {
        if (onSave)
            onSave();
    };
    addAndMakeVisible(saveButton);
    
    loadButton.setButtonText("📁 Load");
    loadButton.onClick = [this] {
        if (onLoad)
            onLoad();
    };
    addAndMakeVisible(loadButton);
    
    clearButton.setButtonText("🗑️ Clear");
    clearButton.onClick = [this] {
        if (onClear)
            onClear();
    };
    addAndMakeVisible(clearButton);
}

void WidgetToolbar::paint(juce::Graphics& g)
{
    g.setColour(juce::Colour(0xff16213e));
    g.fillAll();
    
    g.setColour(juce::Colour(0xff00adb5));
    g.drawLine(0, getHeight() - 1.0f, static_cast<float>(getWidth()), getHeight() - 1.0f, 2.0f);
}

void WidgetToolbar::resized()
{
    auto bounds = getLocalBounds().reduced(5);
    auto buttonWidth = 80;
    auto spacing = 5;
    
    addKnobButton.setBounds(bounds.removeFromLeft(buttonWidth));
    bounds.removeFromLeft(spacing);
    
    addSliderButton.setBounds(bounds.removeFromLeft(buttonWidth));
    bounds.removeFromLeft(spacing);
    
    addButtonButton.setBounds(bounds.removeFromLeft(buttonWidth));
    bounds.removeFromLeft(spacing);
    
    addLabelButton.setBounds(bounds.removeFromLeft(buttonWidth));
    bounds.removeFromLeft(spacing);
    
    addWaveformButton.setBounds(bounds.removeFromLeft(buttonWidth));
    bounds.removeFromLeft(spacing);
    
    addMeterButton.setBounds(bounds.removeFromLeft(buttonWidth));
    bounds.removeFromLeft(spacing * 3);
    
    saveButton.setBounds(bounds.removeFromLeft(buttonWidth));
    bounds.removeFromLeft(spacing);
    
    loadButton.setBounds(bounds.removeFromLeft(buttonWidth));
    bounds.removeFromLeft(spacing);
    
    clearButton.setBounds(bounds.removeFromLeft(buttonWidth));
}
