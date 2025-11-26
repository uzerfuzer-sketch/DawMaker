#include "../Include/ProjectTemplates.h"

// ============================================================================
// Plantillas de Proyectos Predefinidas
// ============================================================================

juce::Array<ProjectTemplates::TemplateInfo> ProjectTemplates::getAllTemplates()
{
    return {
        createSynthesizerTemplate(),
        createAudioEffectTemplate(),
        createMidiUtilityTemplate(),
        createBasicDAWTemplate(),
        createAudioAnalyzerTemplate()
    };
}

ProjectTemplates::TemplateInfo ProjectTemplates::getTemplate(TemplateType type)
{
    switch (type)
    {
        case TemplateType::Synthesizer:    return createSynthesizerTemplate();
        case TemplateType::AudioEffect:    return createAudioEffectTemplate();
        case TemplateType::MidiUtility:    return createMidiUtilityTemplate();
        case TemplateType::BasicDAW:       return createBasicDAWTemplate();
        case TemplateType::AudioAnalyzer:  return createAudioAnalyzerTemplate();
        default:                           return createBasicDAWTemplate();
    }
}

juce::ValueTree ProjectTemplates::createTemplateProject(TemplateType type, const juce::String& projectName)
{
    auto templateInfo = getTemplate(type);
    
    juce::ValueTree project("DAWProject");
    project.setProperty("name", projectName, nullptr);
    project.setProperty("version", "1.0.0", nullptr);
    project.setProperty("template", templateInfo.name, nullptr);
    
    // Configuración básica
    juce::ValueTree config("Configuration");
    config.setProperty("windows", true, nullptr);
    config.setProperty("macos", false, nullptr);
    config.setProperty("linux", false, nullptr);
    config.setProperty("cppStandard", "17", nullptr);
    config.setProperty("enableVST3", true, nullptr);
    config.setProperty("enableStandalone", true, nullptr);
    config.setProperty("projectName", projectName, nullptr);
    project.appendChild(config, nullptr);
    
    // Módulos requeridos
    juce::ValueTree modules("Modules");
    for (const auto& moduleName : templateInfo.requiredModules)
    {
        juce::ValueTree module("Module");
        module.setProperty("name", moduleName, nullptr);
        modules.appendChild(module, nullptr);
    }
    project.appendChild(modules, nullptr);
    
    // Layout vacío (se llenará según el template)
    juce::ValueTree layout("Layout");
    project.appendChild(layout, nullptr);
    
    // Código personalizado del template
    juce::ValueTree code("Code");
    code.setProperty("customCode", templateInfo.defaultCode, nullptr);
    project.appendChild(code, nullptr);
    
    return project;
}

// ============================================================================
// Template: Sintetizador
// ============================================================================
ProjectTemplates::TemplateInfo ProjectTemplates::createSynthesizerTemplate()
{
    TemplateInfo info;
    info.name = "Sintetizador Polifonico";
    info.description = "Sintetizador virtual con osciladores, filtros y envolventes ADSR";
    info.type = TemplateType::Synthesizer;
    
    info.requiredModules = {
        "juce_core",
        "juce_events",
        "juce_audio_basics",
        "juce_audio_devices",
        "juce_audio_processors",
        "juce_audio_utils",
        "juce_dsp",
        "juce_gui_basics",
        "juce_gui_extra"
    };
    
    info.defaultCode = R"(
// Codigo de Sintetizador
class SynthVoice : public juce::SynthesiserVoice
{
public:
    bool canPlaySound(juce::SynthesiserSound*) override { return true; }
    
    void startNote(int midiNoteNumber, float velocity,
                   juce::SynthesiserSound*, int) override
    {
        frequency = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        level = velocity * 0.5;
        phase = 0.0;
    }
    
    void stopNote(float, bool) override
    {
        clearCurrentNote();
    }
    
    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer,
                        int startSample, int numSamples) override
    {
        if (!isVoiceActive())
            return;
        
        auto* leftBuffer = outputBuffer.getWritePointer(0, startSample);
        auto* rightBuffer = outputBuffer.getWritePointer(1, startSample);
        
        for (int sample = 0; sample < numSamples; ++sample)
        {
            auto sineValue = std::sin(phase * juce::MathConstants<double>::twoPi);
            auto output = static_cast<float>(sineValue * level);
            
            leftBuffer[sample] += output;
            rightBuffer[sample] += output;
            
            phase += frequency / getSampleRate();
            if (phase >= 1.0)
                phase -= 1.0;
        }
    }
    
private:
    double frequency = 440.0;
    double phase = 0.0;
    double level = 0.0;
};
)";
    
    return info;
}

// ============================================================================
// Template: Plugin de Efectos
// ============================================================================
ProjectTemplates::TemplateInfo ProjectTemplates::createAudioEffectTemplate()
{
    TemplateInfo info;
    info.name = "Plugin de Efectos";
    info.description = "Procesador de efectos de audio (reverb, delay, chorus, etc.)";
    info.type = TemplateType::AudioEffect;
    
    info.requiredModules = {
        "juce_core",
        "juce_audio_basics",
        "juce_audio_processors",
        "juce_audio_utils",
        "juce_dsp",
        "juce_gui_basics"
    };
    
    info.defaultCode = R"(
// Codigo de Plugin de Efectos
class SimpleReverb
{
public:
    void prepare(double sampleRate, int samplesPerBlock)
    {
        juce::dsp::ProcessSpec spec;
        spec.sampleRate = sampleRate;
        spec.maximumBlockSize = samplesPerBlock;
        spec.numChannels = 2;
        
        reverb.prepare(spec);
        
        juce::dsp::Reverb::Parameters params;
        params.roomSize = 0.5f;
        params.damping = 0.5f;
        params.wetLevel = 0.3f;
        params.dryLevel = 0.7f;
        reverb.setParameters(params);
    }
    
    void process(juce::AudioBuffer<float>& buffer)
    {
        juce::dsp::AudioBlock<float> block(buffer);
        juce::dsp::ProcessContextReplacing<float> context(block);
        reverb.process(context);
    }
    
private:
    juce::dsp::Reverb reverb;
};
)";
    
    return info;
}

// ============================================================================
// Template: Utilidad MIDI
// ============================================================================
ProjectTemplates::TemplateInfo ProjectTemplates::createMidiUtilityTemplate()
{
    TemplateInfo info;
    info.name = "Utilidad MIDI";
    info.description = "Herramienta para procesamiento MIDI (arpeggiator, chord generator, etc.)";
    info.type = TemplateType::MidiUtility;
    
    info.requiredModules = {
        "juce_core",
        "juce_events",
        "juce_audio_basics",
        "juce_audio_devices",
        "juce_audio_processors",
        "juce_gui_basics"
    };
    
    info.defaultCode = R"(
// Codigo de Utilidad MIDI
class MidiArpeggiator
{
public:
    void processBlock(juce::MidiBuffer& midiMessages, int numSamples)
    {
        juce::MidiBuffer processedMidi;
        
        for (const auto metadata : midiMessages)
        {
            auto message = metadata.getMessage();
            
            if (message.isNoteOn())
            {
                heldNotes.add(message.getNoteNumber());
            }
            else if (message.isNoteOff())
            {
                heldNotes.removeAllInstancesOf(message.getNoteNumber());
            }
        }
        
        // Agregar notas arpegiadas
        if (!heldNotes.isEmpty())
        {
            int currentNote = heldNotes[arpIndex % heldNotes.size()];
            auto noteOn = juce::MidiMessage::noteOn(1, currentNote, (juce::uint8)100);
            processedMidi.addEvent(noteOn, 0);
        }
        
        midiMessages.swapWith(processedMidi);
    }
    
private:
    juce::Array<int> heldNotes;
    int arpIndex = 0;
};
)";
    
    return info;
}

// ============================================================================
// Template: DAW Básico
// ============================================================================
ProjectTemplates::TemplateInfo ProjectTemplates::createBasicDAWTemplate()
{
    TemplateInfo info;
    info.name = "DAW Basico";
    info.description = "Estacion de trabajo digital con multiples tracks y mixer";
    info.type = TemplateType::BasicDAW;
    
    info.requiredModules = {
        "juce_core",
        "juce_events",
        "juce_data_structures",
        "juce_audio_basics",
        "juce_audio_devices",
        "juce_audio_formats",
        "juce_audio_processors",
        "juce_audio_utils",
        "juce_gui_basics",
        "juce_gui_extra"
    };
    
    info.defaultCode = R"(
// Codigo de DAW Basico
class AudioTrack
{
public:
    AudioTrack(const juce::String& trackName) : name(trackName) {}
    
    void prepareToPlay(double sampleRate, int samplesPerBlock)
    {
        gainProcessor.setGainLinear(volume);
    }
    
    void processBlock(juce::AudioBuffer<float>& buffer)
    {
        if (muted)
        {
            buffer.clear();
            return;
        }
        
        // Aplicar ganancia
        for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        {
            auto* channelData = buffer.getWritePointer(channel);
            for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
                channelData[sample] *= volume;
        }
    }
    
    void setVolume(float newVolume) { volume = newVolume; }
    void setMuted(bool shouldBeMuted) { muted = shouldBeMuted; }
    void setSolo(bool shouldBeSolo) { solo = shouldBeSolo; }
    
private:
    juce::String name;
    float volume = 1.0f;
    bool muted = false;
    bool solo = false;
    juce::dsp::Gain<float> gainProcessor;
};
)";
    
    return info;
}

// ============================================================================
// Template: Analizador de Audio
// ============================================================================
ProjectTemplates::TemplateInfo ProjectTemplates::createAudioAnalyzerTemplate()
{
    TemplateInfo info;
    info.name = "Analizador de Audio";
    info.description = "Herramientas de analisis: espectro FFT, medidores RMS/Peak, oscil oscopio";
    info.type = TemplateType::AudioAnalyzer;
    
    info.requiredModules = {
        "juce_core",
        "juce_audio_basics",
        "juce_audio_devices",
        "juce_audio_utils",
        "juce_dsp",
        "juce_gui_basics"
    };
    
    info.defaultCode = R"(
// Codigo de Analizador de Audio
class FFTAnalyzer
{
public:
    FFTAnalyzer() : forwardFFT(fftOrder)
    {
        window.resize(fftSize, juce::dsp::WindowingFunction<float>::hann);
    }
    
    void processBlock(const juce::AudioBuffer<float>& buffer)
    {
        for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        {
            auto* channelData = buffer.getReadPointer(channel);
            
            for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
            {
                fifo[fifoIndex++] = channelData[sample];
                
                if (fifoIndex == fftSize)
                {
                    performFFT();
                    fifoIndex = 0;
                }
            }
        }
    }
    
    void performFFT()
    {
        window.multiplyWithWindowingTable(fifo.data(), fftSize);
        forwardFFT.performFrequencyOnlyForwardTransform(fifo.data());
        
        // Calcular magnitudes en dB
        for (int i = 0; i < fftSize / 2; ++i)
        {
            float magnitude = fifo[i];
            float db = juce::Decibels::gainToDecibels(magnitude, -100.0f);
            spectrum[i] = db;
        }
    }
    
    const std::array<float, 2048>& getSpectrum() const { return spectrum; }
    
private:
    static constexpr int fftOrder = 11;
    static constexpr int fftSize = 1 << fftOrder;
    
    juce::dsp::FFT forwardFFT;
    juce::dsp::WindowingFunction<float> window;
    std::array<float, fftSize * 2> fifo;
    std::array<float, fftSize / 2> spectrum;
    int fifoIndex = 0;
};
)";
    
    return info;
}
