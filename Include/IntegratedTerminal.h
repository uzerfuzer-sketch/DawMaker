#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

/**
 * @class IntegratedTerminal
 * @brief Terminal integrada en el DAW para ejecutar comandos
 */
class IntegratedTerminal : public juce::Component
{
public:
    IntegratedTerminal();
    ~IntegratedTerminal() override = default;
    
    void paint(juce::Graphics& g) override;
    void resized() override;
    
    void executeCommand(const juce::String& command);
    void clearTerminal();
    void appendOutput(const juce::String& text, bool isError = false);

private:
    juce::Label titleLabel{"", "Terminal"};
    juce::TextEditor outputEditor;
    juce::TextEditor inputEditor;
    juce::TextButton executeButton{"Ejecutar"};
    juce::TextButton clearButton{"Limpiar"};
    
    juce::String currentDirectory;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IntegratedTerminal)
};
