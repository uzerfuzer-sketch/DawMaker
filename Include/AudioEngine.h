#pragma once

#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_audio_formats/juce_audio_formats.h>

/**
 * @class AudioEngine
 * @brief Motor de audio de la DAW
 * 
 * Esta clase gestiona el procesamiento de audio y MIDI de baja latencia.
 * Proporciona métodos para configurar dispositivos y procesar streams de audio.
 */
class AudioEngine
{
public:
    AudioEngine();
    ~AudioEngine();

    // Preparar el motor para reproducción
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    
    // Procesar el siguiente bloque de audio
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    
    // Liberar recursos
    void releaseResources();

    // Obtener información del estado actual
    double getCurrentSampleRate() const { return currentSampleRate; }
    int getCurrentBufferSize() const { return currentBufferSize; }

private:
    double currentSampleRate = 0.0;
    int currentBufferSize = 0;

    // Aquí puedes añadir procesadores de audio, sintetizadores, etc.
    // Por ejemplo: juce::Synthesiser synth;
    // juce::MixerAudioSource mixer;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioEngine)
};
