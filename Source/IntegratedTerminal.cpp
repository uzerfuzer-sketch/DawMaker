#include "../Include/IntegratedTerminal.h"

IntegratedTerminal::IntegratedTerminal()
{
    // Configurar título
    titleLabel.setFont(juce::FontOptions(16.0f, juce::Font::bold));
    titleLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(titleLabel);
    
    // Configurar editor de salida
    outputEditor.setMultiLine(true);
    outputEditor.setReadOnly(false);
    outputEditor.setScrollbarsShown(true);
    outputEditor.setCaretVisible(false);
    outputEditor.setPopupMenuEnabled(true);
    outputEditor.setColour(juce::TextEditor::backgroundColourId, juce::Colour(0xff1a1a1a));
    outputEditor.setColour(juce::TextEditor::textColourId, juce::Colours::lightgreen);
    outputEditor.setFont(juce::FontOptions("Consolas", 12.0f, juce::Font::plain));
    addAndMakeVisible(outputEditor);
    
    // Configurar editor de entrada
    inputEditor.setMultiLine(false);
    inputEditor.setReturnKeyStartsNewLine(false);
    inputEditor.setPopupMenuEnabled(true);
    inputEditor.setColour(juce::TextEditor::backgroundColourId, juce::Colour(0xff2a2a2a));
    inputEditor.setColour(juce::TextEditor::textColourId, juce::Colours::white);
    inputEditor.setFont(juce::FontOptions("Consolas", 12.0f, juce::Font::plain));
    inputEditor.onReturnKey = [this]() { 
        executeCommand(inputEditor.getText());
    };
    addAndMakeVisible(inputEditor);
    
    // Configurar botones
    executeButton.onClick = [this]() {
        executeCommand(inputEditor.getText());
    };
    addAndMakeVisible(executeButton);
    
    clearButton.onClick = [this]() { clearTerminal(); };
    addAndMakeVisible(clearButton);
    
    // Mensaje de bienvenida
    appendOutput("Terminal Integrada - DawMaker 1.0.1\n");
    appendOutput("Escribe comandos PowerShell y presiona Enter o 'Ejecutar'\n\n");
    
    currentDirectory = juce::File::getCurrentWorkingDirectory().getFullPathName();
    appendOutput("Directorio actual: " + currentDirectory + "\n\n");
}

void IntegratedTerminal::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff2a2a2a));
}

void IntegratedTerminal::resized()
{
    auto bounds = getLocalBounds().reduced(5);
    
    // Título
    titleLabel.setBounds(bounds.removeFromTop(25));
    bounds.removeFromTop(5);
    
    // Área de botones
    auto buttonArea = bounds.removeFromBottom(35);
    clearButton.setBounds(buttonArea.removeFromRight(80).reduced(2));
    buttonArea.removeFromRight(5);
    executeButton.setBounds(buttonArea.removeFromRight(80).reduced(2));
    
    bounds.removeFromBottom(5);
    
    // Input
    auto inputArea = bounds.removeFromBottom(30);
    inputEditor.setBounds(inputArea);
    
    bounds.removeFromBottom(5);
    
    // Output
    outputEditor.setBounds(bounds);
}

void IntegratedTerminal::executeCommand(const juce::String& command)
{
    if (command.trim().isEmpty())
        return;
    
    // Mostrar comando
    appendOutput("> " + command + "\n", false);
    
    // Ejecutar comando en PowerShell
    juce::String fullCommand = "powershell.exe -NoProfile -Command \"cd '" + currentDirectory + "'; " + command + "\"";
    
    juce::ChildProcess process;
    if (process.start(fullCommand))
    {
        auto output = process.readAllProcessOutput();
        if (output.isEmpty())
            output = "(Sin salida)\n";
        
        appendOutput(output + "\n", false);
    }
    else
    {
        appendOutput("Error: No se pudo ejecutar el comando\n", true);
    }
    
    // Limpiar input
    inputEditor.clear();
}

void IntegratedTerminal::clearTerminal()
{
    outputEditor.clear();
    appendOutput("Terminal limpiada.\n\n");
}

void IntegratedTerminal::appendOutput(const juce::String& text, bool isError)
{
    outputEditor.moveCaretToEnd();
    outputEditor.insertTextAtCaret(text);
    outputEditor.moveCaretToEnd();
}
