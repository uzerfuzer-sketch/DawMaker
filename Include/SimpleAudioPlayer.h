#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_audio_formats/juce_audio_formats.h>
#include <juce_audio_utils/juce_audio_utils.h>

/**
 * @class SimpleAudioPlayer
 * @brief Reproductor de audio simple con play, stop y visualización de waveform
 */
class SimpleAudioPlayer : public juce::Component,
                         public juce::ChangeListener,
                         private juce::Timer,
                         public juce::AudioSource
{
public:
    SimpleAudioPlayer();
    ~SimpleAudioPlayer() override;
    
    void paint(juce::Graphics& g) override;
    void resized() override;
    
    // ChangeListener para actualizar el waveform
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    
    // Timer para actualizar la posición de reproducción
    void timerCallback() override;
    
    // AudioSource implementation
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

private:
    // Audio components (sin deviceManager, usamos el del MainComponent)
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    
    // UI Components
    juce::TextButton loadButton{"Cargar Audio"};
    juce::TextButton playButton{"Play"};
    juce::TextButton stopButton{"Stop"};
    juce::Label statusLabel;
    juce::Slider volumeSlider;
    juce::Label volumeLabel{"", "Volumen:"};
    
    // Waveform display
    juce::AudioThumbnailCache thumbnailCache;
    juce::AudioThumbnail thumbnail;
    
    // Estado
    bool isPlaying = false;
    juce::File currentAudioFile;
    double currentPosition = 0.0;
    
    // Métodos privados
    void loadAudioFile();
    void playAudio();
    void stopAudio();
    void drawWaveform(juce::Graphics& g, const juce::Rectangle<int>& bounds);
    void updatePlaybackPosition();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SimpleAudioPlayer)
};
