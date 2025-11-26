#include "../Include/SimpleAudioPlayer.h"

SimpleAudioPlayer::SimpleAudioPlayer()
    : thumbnailCache(5),
      thumbnail(512, formatManager, thumbnailCache)
{
    // Registrar formatos de audio (WAV, MP3, FLAC, etc.)
    formatManager.registerBasicFormats();
    
    // Configurar thumbnail para recibir cambios
    thumbnail.addChangeListener(this);
    
    // Configurar botones
    loadButton.onClick = [this] { loadAudioFile(); };
    addAndMakeVisible(loadButton);
    
    playButton.onClick = [this] { playAudio(); };
    playButton.setEnabled(false);
    addAndMakeVisible(playButton);
    
    stopButton.onClick = [this] { stopAudio(); };
    stopButton.setEnabled(false);
    addAndMakeVisible(stopButton);
    
    // Configurar slider de volumen
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.7);
    volumeSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    volumeSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 60, 20);
    volumeSlider.onValueChange = [this]
    {
        transportSource.setGain((float)volumeSlider.getValue());
    };
    addAndMakeVisible(volumeSlider);
    addAndMakeVisible(volumeLabel);
    
    // Configurar label de estado
    statusLabel.setText("No hay audio cargado", juce::dontSendNotification);
    statusLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    statusLabel.setJustificationType(juce::Justification::centredLeft);
    addAndMakeVisible(statusLabel);
    
    // Iniciar timer para actualizar posición
    startTimer(40); // 25 fps
}

SimpleAudioPlayer::~SimpleAudioPlayer()
{
    stopAudio();
    transportSource.setSource(nullptr);
}

void SimpleAudioPlayer::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff1e1e1e));
    
    // Área del waveform
    auto waveformArea = getLocalBounds().reduced(10);
    waveformArea.removeFromTop(80); // Espacio para controles
    waveformArea.removeFromBottom(60); // Espacio para volumen
    
    // Dibujar borde del waveform
    g.setColour(juce::Colours::grey);
    g.drawRect(waveformArea, 1);
    
    // Dibujar waveform
    drawWaveform(g, waveformArea.reduced(2));
}

void SimpleAudioPlayer::resized()
{
    auto bounds = getLocalBounds().reduced(10);
    
    // Área de controles superior
    auto topArea = bounds.removeFromTop(80);
    auto buttonArea = topArea.removeFromTop(40);
    
    loadButton.setBounds(buttonArea.removeFromLeft(120).reduced(2));
    buttonArea.removeFromLeft(10);
    playButton.setBounds(buttonArea.removeFromLeft(80).reduced(2));
    buttonArea.removeFromLeft(10);
    stopButton.setBounds(buttonArea.removeFromLeft(80).reduced(2));
    
    topArea.removeFromTop(10);
    statusLabel.setBounds(topArea);
    
    // Área de volumen inferior
    auto bottomArea = bounds.removeFromBottom(60);
    bottomArea.removeFromTop(10);
    volumeLabel.setBounds(bottomArea.removeFromLeft(80));
    volumeSlider.setBounds(bottomArea);
}

void SimpleAudioPlayer::changeListenerCallback(juce::ChangeBroadcaster*)
{
    // El thumbnail ha cambiado, repintar
    repaint();
}

void SimpleAudioPlayer::timerCallback()
{
    if (isPlaying && transportSource.isPlaying())
    {
        updatePlaybackPosition();
        repaint();
    }
}

void SimpleAudioPlayer::loadAudioFile()
{
    // Crear file chooser
    auto chooser = std::make_shared<juce::FileChooser>(
        "Seleccionar archivo de audio",
        juce::File(),
        "*.wav;*.mp3;*.aiff;*.flac;*.ogg"
    );
    
    auto chooserFlags = juce::FileBrowserComponent::openMode |
                       juce::FileBrowserComponent::canSelectFiles;
    
    chooser->launchAsync(chooserFlags, [this, chooser](const juce::FileChooser& fc)
    {
        auto file = fc.getResult();
        
        if (file != juce::File())
        {
            // Detener reproducción actual
            stopAudio();
            
            // Crear reader para el archivo
            auto* reader = formatManager.createReaderFor(file);
            
            if (reader != nullptr)
            {
                // Crear nueva source
                auto newSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
                
                // Configurar transport source
                transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
                
                // Guardar la source
                readerSource.reset(newSource.release());
                transportSource.setGain((float)volumeSlider.getValue());
                
                // Cargar thumbnail
                thumbnail.setSource(new juce::FileInputSource(file));
                
                // Actualizar UI
                currentAudioFile = file;
                statusLabel.setText("Cargado: " + file.getFileName(), juce::dontSendNotification);
                playButton.setEnabled(true);
                stopButton.setEnabled(true);
                
                repaint();
            }
            else
            {
                juce::AlertWindow::showMessageBoxAsync(
                    juce::AlertWindow::WarningIcon,
                    "Error",
                    "No se pudo cargar el archivo de audio.\nFormato no soportado.",
                    "OK"
                );
            }
        }
    });
}

void SimpleAudioPlayer::playAudio()
{
    if (readerSource != nullptr)
    {
        if (!isPlaying)
        {
            transportSource.start();
            isPlaying = true;
            playButton.setButtonText("Pause");
        }
        else
        {
            transportSource.stop();
            isPlaying = false;
            playButton.setButtonText("Play");
        }
    }
}

void SimpleAudioPlayer::stopAudio()
{
    transportSource.stop();
    transportSource.setPosition(0.0);
    isPlaying = false;
    playButton.setButtonText("Play");
    currentPosition = 0.0;
    repaint();
}

void SimpleAudioPlayer::drawWaveform(juce::Graphics& g, const juce::Rectangle<int>& bounds)
{
    if (thumbnail.getTotalLength() > 0.0)
    {
        // Dibujar waveform
        g.setColour(juce::Colour(0xff3a3a3a));
        g.fillRect(bounds);
        
        g.setColour(juce::Colour(0xff00d4ff)); // Color cyan para el waveform
        thumbnail.drawChannels(g, bounds, 0.0, thumbnail.getTotalLength(), 1.0f);
        
        // Dibujar línea de posición actual
        if (transportSource.getLengthInSeconds() > 0.0)
        {
            double position = transportSource.getCurrentPosition();
            double length = transportSource.getLengthInSeconds();
            float xPos = bounds.getX() + (float)(position / length * bounds.getWidth());
            
            g.setColour(juce::Colours::yellow);
            g.drawLine(xPos, (float)bounds.getY(), xPos, (float)bounds.getBottom(), 2.0f);
            
            // Mostrar tiempo actual
            int minutes = (int)position / 60;
            int seconds = (int)position % 60;
            int totalMinutes = (int)length / 60;
            int totalSeconds = (int)length % 60;
            
            juce::String timeText = juce::String::formatted("%d:%02d / %d:%02d", 
                                                            minutes, seconds, 
                                                            totalMinutes, totalSeconds);
            
            g.setColour(juce::Colours::white);
            g.setFont(14.0f);
            
            auto textArea = bounds;
            g.drawText(timeText, textArea.getX(), textArea.getBottom() - 20, 
                      textArea.getWidth(), 20, juce::Justification::centred);
        }
    }
    else
    {
        // Mensaje cuando no hay audio
        g.setColour(juce::Colours::grey);
        g.setFont(16.0f);
        g.drawText("Haz clic en 'Cargar Audio' para comenzar", 
                   bounds, juce::Justification::centred);
    }
}

void SimpleAudioPlayer::updatePlaybackPosition()
{
    currentPosition = transportSource.getCurrentPosition();
    
    // Detener cuando llega al final
    if (currentPosition >= transportSource.getLengthInSeconds())
    {
        stopAudio();
    }
}

void SimpleAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void SimpleAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    if (readerSource == nullptr)
    {
        bufferToFill.clearActiveBufferRegion();
        return;
    }
    
    transportSource.getNextAudioBlock(bufferToFill);
}

void SimpleAudioPlayer::releaseResources()
{
    transportSource.releaseResources();
}
