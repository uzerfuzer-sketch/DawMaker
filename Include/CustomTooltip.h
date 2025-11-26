#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

//==============================================================================
// CustomTooltipWindow - Sistema de tooltips personalizado
//==============================================================================
class CustomTooltipWindow : public juce::TooltipWindow
{
public:
    CustomTooltipWindow(juce::Component* parentComponent = nullptr, 
                       int millisecondsBeforeTipAppears = 700)
        : juce::TooltipWindow(parentComponent, millisecondsBeforeTipAppears)
        , millisecondsDelay(millisecondsBeforeTipAppears)
    {
        setMillisecondsBeforeTipAppears(millisecondsDelay);
    }
    
    ~CustomTooltipWindow() override = default;
    
    juce::String getTipFor(juce::Component& c) override
    {
        if (!shouldShowTooltips)
            return {};
        
        return juce::TooltipWindow::getTipFor(c);
    }
    
    void enableTooltips(bool shouldShow)
    {
        shouldShowTooltips = shouldShow;
        
        if (!shouldShow)
            hideTip();
    }
    
    void setDelay(int milliseconds)
    {
        millisecondsDelay = milliseconds;
        setMillisecondsBeforeTipAppears(millisecondsDelay);
    }
    
    int getDelay() const { return millisecondsDelay; }
    bool areTooltipsEnabled() const { return shouldShowTooltips; }

private:
    int millisecondsDelay = 700;
    bool shouldShowTooltips = true;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomTooltipWindow)
};
