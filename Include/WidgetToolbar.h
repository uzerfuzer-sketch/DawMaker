#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "DraggableWidget.h"

/**
 * @class WidgetToolbar
 * @brief Barra de herramientas para añadir nuevos widgets
 */
class WidgetToolbar : public juce::Component
{
public:
    WidgetToolbar();
    ~WidgetToolbar() override = default;

    void paint(juce::Graphics& g) override;
    void resized() override;

    // Callback cuando se solicita añadir un widget
    std::function<void(DraggableWidget::WidgetType)> onAddWidget;
    std::function<void()> onSave;
    std::function<void()> onLoad;
    std::function<void()> onClear;

private:
    juce::TextButton addKnobButton;
    juce::TextButton addSliderButton;
    juce::TextButton addButtonButton;
    juce::TextButton addLabelButton;
    juce::TextButton addWaveformButton;
    juce::TextButton addMeterButton;
    juce::TextButton saveButton;
    juce::TextButton loadButton;
    juce::TextButton clearButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WidgetToolbar)
};
