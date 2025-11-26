#pragma once

#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include "DraggableWidget.h"

/**
 * @class PluginExporter
 * @brief Exportador de código para múltiples frameworks de audio
 * 
 * Soporta:
 * - iPlug2 (VST3, AU, AAX, Web Audio)
 * - DPF (DISTRHO Plugin Framework)
 * - JUCE Plugin
 * - Elements (cycfi)
 */
class PluginExporter
{
public:
    enum class FrameworkType
    {
        JUCE,
        IPlug2,
        DPF,
        Elements
    };
    
    enum class PluginFormat
    {
        VST3,
        AU,
        AAX,
        LV2,
        Standalone
    };
    
    PluginExporter() = default;
    ~PluginExporter() = default;
    
    // Generar código para el framework seleccionado
    static juce::String generatePluginCode(
        FrameworkType framework,
        const juce::OwnedArray<DraggableWidget>& widgets,
        const juce::String& pluginName,
        const juce::String& manufacturer
    );
    
    // Generar archivos de proyecto
    static bool exportProject(
        FrameworkType framework,
        const juce::File& outputDirectory,
        const juce::OwnedArray<DraggableWidget>& widgets,
        const juce::String& pluginName,
        const juce::String& manufacturer
    );
    
private:
    // Generadores específicos por framework
    static juce::String generateJUCECode(const juce::OwnedArray<DraggableWidget>& widgets, const juce::String& pluginName);
    static juce::String generateIPlug2Code(const juce::OwnedArray<DraggableWidget>& widgets, const juce::String& pluginName);
    static juce::String generateDPFCode(const juce::OwnedArray<DraggableWidget>& widgets, const juce::String& pluginName);
    static juce::String generateElementsCode(const juce::OwnedArray<DraggableWidget>& widgets, const juce::String& pluginName);
    
    // Helpers
    static juce::String widgetToJUCECode(const DraggableWidget* widget);
    static juce::String widgetToIPlug2Code(const DraggableWidget* widget, int index);
    static juce::String widgetToDPFCode(const DraggableWidget* widget, int index);
    static juce::String widgetToElementsCode(const DraggableWidget* widget, int index);
    
    // Generadores de archivos de configuración
    static juce::String generateCMakeLists(FrameworkType framework, const juce::String& pluginName);
    static juce::String generateIPlug2Config(const juce::String& pluginName, const juce::String& manufacturer);
    static juce::String generateDPFMakefile(const juce::String& pluginName);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginExporter)
};
