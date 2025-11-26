#pragma once

#include <juce_core/juce_core.h>
#include <juce_data_structures/juce_data_structures.h>

/**
 * @class ProjectTemplates
 * @brief Gestor de plantillas de proyectos predefinidas
 * 
 * Proporciona plantillas listas para usar:
 * - Sintetizador básico
 * - Plugin de efectos de audio
 * - Utilidad MIDI
 * - DAW básico
 * - Analizador de audio
 */
class ProjectTemplates
{
public:
    enum class TemplateType
    {
        Synthesizer,
        AudioEffect,
        MidiUtility,
        BasicDAW,
        AudioAnalyzer
    };
    
    struct TemplateInfo
    {
        juce::String name;
        juce::String description;
        juce::StringArray requiredModules;
        juce::String defaultCode;
        TemplateType type;
    };
    
    static juce::Array<TemplateInfo> getAllTemplates();
    static TemplateInfo getTemplate(TemplateType type);
    static juce::ValueTree createTemplateProject(TemplateType type, const juce::String& projectName);
    
private:
    static TemplateInfo createSynthesizerTemplate();
    static TemplateInfo createAudioEffectTemplate();
    static TemplateInfo createMidiUtilityTemplate();
    static TemplateInfo createBasicDAWTemplate();
    static TemplateInfo createAudioAnalyzerTemplate();
};
