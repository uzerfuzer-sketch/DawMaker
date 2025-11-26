#include "MainWindow.h"
#include <juce_gui_basics/juce_gui_basics.h>

/**
 * @class CustomDAWApplication
 * @brief Clase principal de la aplicación JUCE
 * 
 * Esta clase gestiona el ciclo de vida de la aplicación:
 * - Inicialización
 * - Creación de la ventana principal
 * - Manejo de eventos del sistema
 * - Cierre limpio
 */
class CustomDAWApplication : public juce::JUCEApplication
{
public:
    CustomDAWApplication() = default;

    const juce::String getApplicationName() override 
    { 
        return "DawMaker"; 
    }
    
    const juce::String getApplicationVersion() override 
    { 
        return "1.0.1"; 
    }
    
    bool moreThanOneInstanceAllowed() override 
    { 
        return true; 
    }

    /**
     * Inicializa la aplicación y crea la ventana principal
     */
    void initialise(const juce::String& commandLine) override
    {
        // Ignorar advertencias no utilizadas
        juce::ignoreUnused(commandLine);

        // Crear la ventana principal
        mainWindow = std::make_unique<MainWindow>(getApplicationName());
    }

    /**
     * Limpia y cierra la aplicación
     */
    void shutdown() override
    {
        // Cerrar la ventana principal antes de salir
        mainWindow = nullptr;
    }

    /**
     * Maneja la solicitud de cierre del sistema
     */
    void systemRequestedQuit() override
    {
        // Llamar a quit() para cerrar la aplicación de forma limpia
        quit();
    }

    /**
     * Maneja la suspensión de la aplicación (ej: en iOS/Android)
     */
    void anotherInstanceStarted(const juce::String& commandLine) override
    {
        // Esta aplicación permite múltiples instancias, pero puedes
        // modificar este comportamiento aquí si lo necesitas
        juce::ignoreUnused(commandLine);
    }

private:
    std::unique_ptr<MainWindow> mainWindow;
};

//==============================================================================
// Macro que crea el punto de entrada de la aplicación
// JUCE maneja las diferencias entre plataformas automáticamente
START_JUCE_APPLICATION(CustomDAWApplication)
