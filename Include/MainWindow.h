#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "MainComponent.h"

/**
 * @class MainWindow
 * @brief Ventana principal de la aplicación DAW
 * 
 * Esta clase gestiona la ventana principal y contiene el componente principal
 * de la aplicación. Configura el tamaño, título y comportamiento de cierre.
 */
class MainWindow : public juce::DocumentWindow
{
public:
    MainWindow(juce::String name);
    ~MainWindow() override;

    void closeButtonPressed() override;

private:
    std::unique_ptr<MainComponent> mainComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
};
