#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "AudioMeters.h"
#include "AudioScope.h"
#include "VerticalFader.h"
#include "CustomButtons.h"

/**
 * @brief LookAndFeel personalizado para renderizar knobs con diferentes formas
 */
class CustomKnobLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CustomKnobLookAndFeel() = default;
    
    void setKnobStyle(int style, bool filled) 
    { 
        knobStyle = style; 
        fillKnob = filled;
    }
    
    void setFilmstripImage(const juce::Image& img)
    {
        filmstripImage = img;
    }
    
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                         float sliderPosProportional, float rotaryStartAngle,
                         float rotaryEndAngle, juce::Slider& slider) override;

private:
    int knobStyle = 0; // 0-9 diferentes estilos
    bool fillKnob = true;
    juce::Image filmstripImage;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomKnobLookAndFeel)
};

/**
 * @brief LookAndFeel personalizado para sliders lineales con 10 estilos diferentes
 */
class CustomSliderLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CustomSliderLookAndFeel() = default;
    
    void setSliderStyle(int style) { sliderStyle = style; }
    
    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
                         float sliderPos, float minSliderPos, float maxSliderPos,
                         const juce::Slider::SliderStyle style, juce::Slider& slider) override;

private:
    int sliderStyle = 0; // 0-9 diferentes estilos
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomSliderLookAndFeel)
};

/**
 * @brief LookAndFeel personalizado para botones con 10 estilos diferentes
 */
class CustomButtonLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CustomButtonLookAndFeel() = default;
    
    void setButtonStyle(int style) { buttonStyle = style; }
    
    void drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour,
                             bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
    
    void drawButtonText(juce::Graphics& g, juce::TextButton& button, bool shouldDrawButtonAsHighlighted,
                       bool shouldDrawButtonAsDown) override;

private:
    int buttonStyle = 0; // 0-9 diferentes estilos
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomButtonLookAndFeel)
};

/**
 * @class DraggableWidget
 * @brief Widget base que puede ser arrastrado, redimensionado y personalizado
 * 
 * Cada widget tiene:
 * - Posición y tamaño personalizables
 * - Capacidad de arrastre
 * - Redimensionamiento desde las esquinas
 * - Botón de eliminación
 * - Serialización para guardar/cargar
 */
class DraggableWidget : public juce::Component
{
public:
    enum WidgetType
    {
        Knob,
        Slider,
        Button,
        Waveform,
        Meter,
        Label,
        XYPad,
        Joystick,
        PitchWheel,
        IndexedSlider,
        BasicMeter,      // Nuevo: Medidor vertical básico
        VUMeter,         // Nuevo: Medidor VU con filmstrip
        LedMeter,        // Nuevo: Medidor tipo LED bargraph
        AudioScope,      // Nuevo: Osciloscopio de audio
        VerticalFader,   // Nuevo: Fader vertical
        ImageButton,     // Nuevo: Botón con imágenes on/off
        ToggleButton,    // Nuevo: Toggle button simple
        TransportPlay,   // Nuevo: Botón Play
        TransportPause,  // Nuevo: Botón Pause
        TransportStop,   // Nuevo: Botón Stop
        TransportRecord, // Nuevo: Botón Record
        TransportRewind, // Nuevo: Botón Rewind
        TransportFastForward, // Nuevo: Botón Fast Forward
        TransportLoop,   // Nuevo: Botón Loop
        TransportMetronome, // Nuevo: Botón Metronome
        TransportTap,    // Nuevo: Botón Tap Tempo
        TransportSync,   // Nuevo: Botón Sync
        TransportReturnToZero, // Nuevo: Return to Zero
        TransportMarkerNext,   // Nuevo: Next Marker
        TransportMarkerPrevious, // Nuevo: Previous Marker
        TransportNudgeForward, // Nuevo: Nudge Forward
        TransportNudgeBackward, // Nuevo: Nudge Backward
        TransportDrop,         // Nuevo: Drop in/out
        TransportReplace,      // Nuevo: Replace mode
        TransportAutomationMode, // Nuevo: Automation
        TransportJogWheel,     // Nuevo: Jog Wheel
        TransportScrub,        // Nuevo: Scrub
        TransportPreRoll,      // Nuevo: Pre-roll
        TransportPostRoll,      // Nuevo: Post-roll
        MidiKeyboard     // Nuevo: Teclado MIDI
    };

    DraggableWidget(WidgetType type, const juce::String& name);
    ~DraggableWidget() override = default;

    // Métodos de Component
    void paint(juce::Graphics& g) override;
    void resized() override;
    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void mouseUp(const juce::MouseEvent& e) override;
    void mouseEnter(const juce::MouseEvent& e) override;
    void mouseExit(const juce::MouseEvent& e) override;

    // Serialización
    juce::ValueTree toValueTree() const;
    static std::unique_ptr<DraggableWidget> fromValueTree(const juce::ValueTree& tree);

    // Getters
    WidgetType getWidgetType() const { return widgetType; }
    juce::String getWidgetName() const { return widgetName; }
    
    // Setters
    virtual void setWidgetName(const juce::String& newName) { widgetName = newName; }
    
    // Propiedades de estilo personalizables
    struct WidgetStyle
    {
        juce::Colour bgColor = juce::Colours::transparentBlack;
        juce::Colour borderColor = juce::Colours::transparentBlack;
        juce::Colour knobColor = juce::Colour(0xff00d4ff);
        juce::Colour textColor = juce::Colours::white;
        float borderWidth = 2.0f;
        float cornerRadius = 5.0f;
        bool fillKnob = true;
        int knobStyle = 0; // 0-9 estilos de knob
        juce::String filmstripPath = "";
        int sliderStyle = 0; // 0-9 estilos de slider
        int buttonStyle = 0; // 0-9 estilos de button
        int labelStyle = 0; // 0-9 estilos de label
    };
    
    WidgetStyle style;
    void setStyle(const WidgetStyle& newStyle) { style = newStyle; repaint(); }
    WidgetStyle getStyle() const { return style; }
    
    // MIDI Control
    void setMidiCC(int cc, int channel) 
    { 
        midiCC = cc; 
        midiChannel = channel; 
        hasMidiAssignment = true;
    }
    void clearMidiCC() { hasMidiAssignment = false; }
    bool hasMidiCC() const { return hasMidiAssignment; }
    int getMidiCC() const { return midiCC; }
    int getMidiChannel() const { return midiChannel; }
    
    // Método virtual para actualizar valor desde MIDI (0-127)
    virtual void updateFromMidiValue(int value) {}
    
    // Callbacks
    std::function<void(DraggableWidget*)> onDeleteRequested;
    std::function<void(DraggableWidget*)> onConfigRequested;
    std::function<void(DraggableWidget*)> onMidiLearnRequested;

protected:
    virtual void paintWidget(juce::Graphics& g) = 0;
    
    bool isResizing = false;
    bool isDragging = false;

private:
    WidgetType widgetType;
    juce::String widgetName;
    
    // MIDI assignment
    bool hasMidiAssignment = false;
    int midiCC = -1;
    int midiChannel = 1;
    
    juce::ComponentDragger dragger;
    juce::ComponentBoundsConstrainer constrainer;
    juce::ResizableBorderComponent resizer;
    
    juce::TextButton deleteButton;
    bool isHovered = false;

    void deleteButtonClicked();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DraggableWidget)
};

/**
 * @class DraggableKnob
 * @brief Knob rotatorio arrastrable
 */
class DraggableKnob : public DraggableWidget
{
public:
    DraggableKnob(const juce::String& name);
    
    void resized() override;
    void setWidgetName(const juce::String& newName) override;
    juce::ValueTree toValueTree() const;
    
    juce::Slider& getKnob() { return knob; }
    juce::Label& getLabel() { return label; }
    
    // Método para cambiar el estilo visual del knob
    void setKnobStyle(int style, bool filled = true) { knobLookAndFeel.setKnobStyle(style, filled); knob.repaint(); }
    
    void updateFromMidiValue(int value) override;
    
    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void mouseUp(const juce::MouseEvent& e) override;

protected:
    void paintWidget(juce::Graphics& g) override;

private:
    juce::Slider knob;
    juce::Label label;
    CustomKnobLookAndFeel knobLookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DraggableKnob)
};

/**
 * @class DraggableSlider
 * @brief Slider vertical/horizontal arrastrable
 */
class DraggableSlider : public DraggableWidget
{
public:
    DraggableSlider(const juce::String& name, bool isVertical = true);
    
    void resized() override;
    void setWidgetName(const juce::String& newName) override;
    juce::ValueTree toValueTree() const;
    
    // Método para cambiar el estilo visual del slider
    void setSliderStyle(int style) { sliderLookAndFeel.setSliderStyle(style); slider.repaint(); }
    
    void updateFromMidiValue(int value) override;
    
    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void mouseUp(const juce::MouseEvent& e) override;

protected:
    void paintWidget(juce::Graphics& g) override;

private:
    juce::Slider slider;
    juce::Label label;
    bool vertical;
    CustomSliderLookAndFeel sliderLookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DraggableSlider)
};

/**
 * @class DraggableButton
 * @brief Botón arrastrable personalizable
 */
class DraggableButton : public DraggableWidget
{
public:
    DraggableButton(const juce::String& name);
    
    void resized() override;
    void setWidgetName(const juce::String& newName) override;
    juce::ValueTree toValueTree() const;
    
    // Método para cambiar el estilo visual del botón
    void setButtonStyle(int style) { buttonLookAndFeel.setButtonStyle(style); button.repaint(); }
    
    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void mouseUp(const juce::MouseEvent& e) override;

protected:
    void paintWidget(juce::Graphics& g) override;

private:
    juce::TextButton button;
    CustomButtonLookAndFeel buttonLookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DraggableButton)
};

/**
 * @class DraggableLabel
 * @brief Etiqueta de texto arrastrable
 */
class DraggableLabel : public DraggableWidget
{
public:
    DraggableLabel(const juce::String& name, const juce::String& text);
    
    void resized() override;
    void setWidgetName(const juce::String& newName) override;
    juce::ValueTree toValueTree() const;
    
    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void mouseUp(const juce::MouseEvent& e) override;

protected:
    void paintWidget(juce::Graphics& g) override;

private:
    juce::Label label;
    bool draggingWidget = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DraggableLabel)
};

/**
 * @class DraggableMeter
 * @brief Medidor de nivel de audio con 10 estilos diferentes
 */
class DraggableMeter : public DraggableWidget
{
public:
    enum MeterStyle
    {
        Vertical = 0,       // Barra vertical clasica
        Horizontal,         // Barra horizontal
        LED,                // LEDs discretos
        VU,                 // VU meter analogico
        Peak,               // Peak meter con hold
        RMS,                // RMS con suavizado
        Gradient,           // Gradiente continuo
        Segmented,          // Segmentos con gaps
        Circular,           // Meter circular
        Radial              // Radial desde centro
    };
    
    DraggableMeter(const juce::String& name);
    
    void resized() override;
    juce::ValueTree toValueTree() const;
    
    void setMeterValue(float value);
    float getMeterValue() const { return currentValue; }
    void setPeakValue(float value) { peakValue = value; }
    void setStyle(int style) { meterStyle = style % 10; repaint(); }
    void setMeterColor(juce::Colour color) { meterColour = color; repaint(); }
    int getStyle() const { return meterStyle; }
    
protected:
    void paintWidget(juce::Graphics& g) override;

private:
    float currentValue = 0.0f;
    float peakValue = 0.0f;
    float peakHoldTime = 0.0f;
    int meterStyle = 0;
    juce::Colour meterColour{0xff00ff00};
    
    void drawVerticalMeter(juce::Graphics& g, const juce::Rectangle<float>& bounds);
    void drawHorizontalMeter(juce::Graphics& g, const juce::Rectangle<float>& bounds);
    void drawLEDMeter(juce::Graphics& g, const juce::Rectangle<float>& bounds);
    void drawVUMeter(juce::Graphics& g, const juce::Rectangle<float>& bounds);
    void drawPeakMeter(juce::Graphics& g, const juce::Rectangle<float>& bounds);
    void drawRMSMeter(juce::Graphics& g, const juce::Rectangle<float>& bounds);
    void drawGradientMeter(juce::Graphics& g, const juce::Rectangle<float>& bounds);
    void drawSegmentedMeter(juce::Graphics& g, const juce::Rectangle<float>& bounds);
    void drawCircularMeter(juce::Graphics& g, const juce::Rectangle<float>& bounds);
    void drawRadialMeter(juce::Graphics& g, const juce::Rectangle<float>& bounds);
    
    juce::Colour getColorForLevel(float level);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DraggableMeter)
};

/**
 * @class DraggableWaveform
 * @brief Visualizador de forma de onda con buffer de audio
 */
class DraggableWaveform : public DraggableWidget
{
public:
    enum WaveformStyle
    {
        Line = 0,           // Linea simple
        LineGlow,           // Linea con glow
        Filled,             // Area rellena
        Mirror,             // Espejo vertical
        Bars,               // Barras verticales
        Dots,               // Puntos
        Gradient,           // Gradiente de color
        Stepped,            // Escalonado
        Frequency,          // Espectro de frecuencias
        Vectorscope         // Vectorscopio (Lissajous)
    };
    
    DraggableWaveform(const juce::String& name);
    
    void resized() override;
    juce::ValueTree toValueTree() const;
    
    void pushSample(float sample);
    void pushBuffer(const float* samples, int numSamples);
    void clear();
    void setZoom(float zoom) { zoomLevel = juce::jlimit(0.1f, 10.0f, zoom); repaint(); }
    void setDisplayChannels(int channels) { numChannels = juce::jlimit(1, 2, channels); }
    void setStyle(int style) { waveformStyle = style % 10; repaint(); }
    void setWaveformColor(juce::Colour color) { waveformColour = color; repaint(); }
    int getStyle() const { return waveformStyle; }

protected:
    void paintWidget(juce::Graphics& g) override;

private:
    static constexpr int bufferSize = 2048;
    juce::AudioBuffer<float> audioBuffer;
    int writePosition = 0;
    float zoomLevel = 1.0f;
    int numChannels = 1;
    int waveformStyle = 0;
    juce::Colour waveformColour{0xff00ffff};
    
    void drawLineWaveform(juce::Graphics& g, const juce::Rectangle<float>& bounds);
    void drawLineGlowWaveform(juce::Graphics& g, const juce::Rectangle<float>& bounds);
    void drawFilledWaveform(juce::Graphics& g, const juce::Rectangle<float>& bounds);
    void drawMirrorWaveform(juce::Graphics& g, const juce::Rectangle<float>& bounds);
    void drawBarsWaveform(juce::Graphics& g, const juce::Rectangle<float>& bounds);
    void drawDotsWaveform(juce::Graphics& g, const juce::Rectangle<float>& bounds);
    void drawGradientWaveform(juce::Graphics& g, const juce::Rectangle<float>& bounds);
    void drawSteppedWaveform(juce::Graphics& g, const juce::Rectangle<float>& bounds);
    void drawFrequencyWaveform(juce::Graphics& g, const juce::Rectangle<float>& bounds);
    void drawVectorscopeWaveform(juce::Graphics& g, const juce::Rectangle<float>& bounds);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DraggableWaveform)
};

/**
 * @class DraggableXYPad
 * @brief Pad XY 2D con indicador visual (inspirado en AudioKit Controls)
 * Control bidimensional para controlar dos parametros simultaneamente
 */
class DraggableXYPad : public DraggableWidget
{
public:
    enum XYPadStyle {
        Circle,        // Círculo con crosshair
        Square,        // Cuadrado con grid
        Orbital,       // Orbital con trail
        Crosshair,     // Crosshair grande
        Grid,          // Grid con punto
        Glow,          // Punto con glow
        Vector,        // Vector desde centro
        Radar,         // Radar circular
        Diamond,       // Diamante con punto
        Nebula         // Nebula con partículas
    };
    
    DraggableXYPad(const juce::String& name);
    
    void resized() override;
    juce::ValueTree toValueTree() const;
    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    
    float getXValue() const { return xValue; }
    float getYValue() const { return yValue; }
    void setXValue(float x) { xValue = juce::jlimit(0.0f, 1.0f, x); repaint(); }
    void setYValue(float y) { yValue = juce::jlimit(0.0f, 1.0f, y); repaint(); }
    
    void updateFromMidiValue(int value) override;
    
    void setStyle(int style) { xyPadStyle = style % 10; repaint(); }
    int getStyle() const { return xyPadStyle; }
    void setXYColor(juce::Colour color) { xyColour = color; repaint(); }

protected:
    void paintWidget(juce::Graphics& g) override;

private:
    float xValue = 0.5f; // 0-1
    float yValue = 0.5f; // 0-1
    juce::Point<float> indicatorPos;
    float indicatorSize = 20.0f;
    int xyPadStyle = 0;
    juce::Colour xyColour{0xff00ffff};
    
    void updateIndicatorFromMouse(const juce::Point<int>& mousePos);
    
    // Drawing methods for different styles
    void drawCircleStyle(juce::Graphics& g, const juce::Rectangle<float>& bounds);
    void drawSquareStyle(juce::Graphics& g, const juce::Rectangle<float>& bounds);
    void drawOrbitalStyle(juce::Graphics& g, const juce::Rectangle<float>& bounds);
    void drawCrosshairStyle(juce::Graphics& g, const juce::Rectangle<float>& bounds);
    void drawGridStyle(juce::Graphics& g, const juce::Rectangle<float>& bounds);
    void drawGlowStyle(juce::Graphics& g, const juce::Rectangle<float>& bounds);
    void drawVectorStyle(juce::Graphics& g, const juce::Rectangle<float>& bounds);
    void drawRadarStyle(juce::Graphics& g, const juce::Rectangle<float>& bounds);
    void drawDiamondStyle(juce::Graphics& g, const juce::Rectangle<float>& bounds);
    void drawNebulaStyle(juce::Graphics& g, const juce::Rectangle<float>& bounds);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DraggableXYPad)
};

/**
 * @class DraggableJoystick
 * @brief Joystick con coordenadas polares (radio + angulo) que vuelve al centro
 * Inspirado en AudioKit Joystick control
 */
class DraggableJoystick : public DraggableWidget
{
public:
    enum JoystickStyle {
        Classic,       // Joystick clásico con círculo
        Arcade,        // Estilo arcade con base
        Flight,        // Stick de vuelo
        Minimal,       // Minimalista
        Neon,          // Neon con glow
        Mechanical,    // Mecánico con detalles
        Orb,           // Orbe flotante
        Target,        // Target con mira
        Compass,       // Brújula
        Galaxy         // Galaxia con estrellas
    };
    
    DraggableJoystick(const juce::String& name);
    
    void resized() override;
    juce::ValueTree toValueTree() const;
    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void mouseUp(const juce::MouseEvent& e) override;
    
    float getRadius() const { return radius; }
    float getAngle() const { return angle; }
    
    void updateFromMidiValue(int value) override;
    
    void setStyle(int style) { joystickStyle = style % 10; repaint(); }
    int getStyle() const { return joystickStyle; }
    void setJoystickColor(juce::Colour color) { joystickColour = color; repaint(); }

protected:
    void paintWidget(juce::Graphics& g) override;

private:
    float radius = 0.0f; // 0-1 (distancia desde el centro)
    float angle = 0.0f; // 0-1 (angulo normalizado)
    juce::Point<float> stickPos;
    int joystickStyle = 0;
    juce::Colour joystickColour{0xff00ff00};
    
    void updateFromMouse(const juce::Point<int>& mousePos);
    void returnToCenter();
    
    // Drawing methods for different styles
    void drawClassicStyle(juce::Graphics& g, const juce::Rectangle<float>& bounds);
    void drawArcadeStyle(juce::Graphics& g, const juce::Rectangle<float>& bounds);
    void drawFlightStyle(juce::Graphics& g, const juce::Rectangle<float>& bounds);
    void drawMinimalStyle(juce::Graphics& g, const juce::Rectangle<float>& bounds);
    void drawNeonStyle(juce::Graphics& g, const juce::Rectangle<float>& bounds);
    void drawMechanicalStyle(juce::Graphics& g, const juce::Rectangle<float>& bounds);
    void drawOrbStyle(juce::Graphics& g, const juce::Rectangle<float>& bounds);
    void drawTargetStyle(juce::Graphics& g, const juce::Rectangle<float>& bounds);
    void drawCompassStyle(juce::Graphics& g, const juce::Rectangle<float>& bounds);
    void drawGalaxyStyle(juce::Graphics& g, const juce::Rectangle<float>& bounds);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DraggableJoystick)
};

/**
 * @class DraggablePitchWheel
 * @brief Slider vertical con retorno automatico al centro (pitch bend)
 * Similar a las ruedas de modulacion MIDI
 */
class DraggablePitchWheel : public DraggableWidget
{
public:
    DraggablePitchWheel(const juce::String& name);
    
    void resized() override;
    juce::ValueTree toValueTree() const;
    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void mouseUp(const juce::MouseEvent& e) override;
    
    float getValue() const { return value; }
    void updateFromMidiValue(int value) override;

protected:
    void paintWidget(juce::Graphics& g) override;

private:
    float value = 0.5f; // 0-1 (0.5 = centro)
    float indicatorHeight = 40.0f;
    
    void updateFromMouse(const juce::Point<int>& mousePos);
    void returnToCenter();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DraggablePitchWheel)
};

/**
 * @class DraggableIndexedSlider
 * @brief Slider horizontal con posiciones discretas y etiquetas
 * Para seleccion de modos, escalas, u opciones predefinidas
 */
class DraggableIndexedSlider : public DraggableWidget
{
public:
    DraggableIndexedSlider(const juce::String& name, const juce::StringArray& labels);
    
    void resized() override;
    juce::ValueTree toValueTree() const;
    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    
    int getIndex() const { return currentIndex; }
    void setIndex(int idx);
    void setLabels(const juce::StringArray& newLabels);
    juce::StringArray getLabels() const { return labels; }
    
    void updateFromMidiValue(int value) override;

protected:
    void paintWidget(juce::Graphics& g) override;

private:
    juce::StringArray labels;
    int currentIndex = 0;
    
    void updateFromMouse(const juce::Point<int>& mousePos);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DraggableIndexedSlider)
};

/**
 * @class DraggableBasicMeter
 * @brief Medidor de nivel vertical básico con gradiente de color
 */
class DraggableBasicMeter : public DraggableWidget
{
public:
    enum BasicMeterStyle
    {
        Gradient = 0,
        Segmented,
        Neon,
        Vintage,
        Digital,
        Plasma,
        Rainbow,
        Thermal,
        Crystal,
        Wave
    };
    
    DraggableBasicMeter(const juce::String& name);
    
    void resized() override;
    juce::ValueTree toValueTree() const;
    
    void setMeterValue(float value);
    float getMeterValue() const;
    
    void updateFromMidiValue(int value) override;
    
    void setStyle(int style) { basicMeterStyle = style; repaint(); }
    int getStyle() const { return basicMeterStyle; }
    void setMeterColor(const juce::Colour& color) { meterColour = color; repaint(); }

protected:
    void paintWidget(juce::Graphics& g) override;

private:
    std::unique_ptr<class BasicMeter> meter;
    int basicMeterStyle = 0;
    juce::Colour meterColour = juce::Colours::green;
    
    void drawGradientStyle(juce::Graphics& g, juce::Rectangle<int> area, float level);
    void drawSegmentedStyle(juce::Graphics& g, juce::Rectangle<int> area, float level);
    void drawNeonStyle(juce::Graphics& g, juce::Rectangle<int> area, float level);
    void drawVintageStyle(juce::Graphics& g, juce::Rectangle<int> area, float level);
    void drawDigitalStyle(juce::Graphics& g, juce::Rectangle<int> area, float level);
    void drawPlasmaStyle(juce::Graphics& g, juce::Rectangle<int> area, float level);
    void drawRainbowStyle(juce::Graphics& g, juce::Rectangle<int> area, float level);
    void drawThermalStyle(juce::Graphics& g, juce::Rectangle<int> area, float level);
    void drawCrystalStyle(juce::Graphics& g, juce::Rectangle<int> area, float level);
    void drawWaveStyle(juce::Graphics& g, juce::Rectangle<int> area, float level);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DraggableBasicMeter)
};

/**
 * @class DraggableVUMeter
 * @brief Medidor VU estilo analógico con aguja
 */
class DraggableVUMeter : public DraggableWidget
{
public:
    enum VUMeterStyle
    {
        Analog = 0,
        Modern,
        Retro,
        Chrome,
        Wood,
        Steampunk,
        Neon,
        Digital,
        Minimal,
        Luxury
    };
    
    DraggableVUMeter(const juce::String& name);
    
    void resized() override;
    juce::ValueTree toValueTree() const;
    
    void setMeterValue(float value);
    float getMeterValue() const;
    
    void updateFromMidiValue(int value) override;
    
    void setStyle(int style) { vuMeterStyle = style; repaint(); }
    int getStyle() const { return vuMeterStyle; }
    void setVUColor(const juce::Colour& color) { vuColour = color; repaint(); }

protected:
    void paintWidget(juce::Graphics& g) override;

private:
    std::unique_ptr<class VUMeter> vuMeter;
    int vuMeterStyle = 0;
    juce::Colour vuColour = juce::Colours::orange;
    
    void drawAnalogStyle(juce::Graphics& g, juce::Rectangle<int> area, float level);
    void drawModernStyle(juce::Graphics& g, juce::Rectangle<int> area, float level);
    void drawRetroStyle(juce::Graphics& g, juce::Rectangle<int> area, float level);
    void drawChromeStyle(juce::Graphics& g, juce::Rectangle<int> area, float level);
    void drawWoodStyle(juce::Graphics& g, juce::Rectangle<int> area, float level);
    void drawSteampunkStyle(juce::Graphics& g, juce::Rectangle<int> area, float level);
    void drawNeonStyle(juce::Graphics& g, juce::Rectangle<int> area, float level);
    void drawDigitalStyle(juce::Graphics& g, juce::Rectangle<int> area, float level);
    void drawMinimalStyle(juce::Graphics& g, juce::Rectangle<int> area, float level);
    void drawLuxuryStyle(juce::Graphics& g, juce::Rectangle<int> area, float level);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DraggableVUMeter)
};

/**
 * @class DraggableLedMeter
 * @brief Medidor tipo LED bargraph
 */
class DraggableLedMeter : public DraggableWidget
{
public:
    enum LedMeterStyle
    {
        Classic = 0,
        Round,
        Square,
        Diamond,
        Bar,
        Dot,
        Glow,
        Neon,
        Matrix,
        Spectrum
    };
    
    DraggableLedMeter(const juce::String& name);
    
    void resized() override;
    juce::ValueTree toValueTree() const;
    
    void setMeterValue(float value);
    void setNumLEDs(int numLEDs);
    
    void setStyle(int style) { ledMeterStyle = style; repaint(); }
    int getStyle() const { return ledMeterStyle; }
    void setLedColor(const juce::Colour& color) { ledColour = color; repaint(); }

protected:
    void paintWidget(juce::Graphics& g) override;

private:
    std::unique_ptr<class LedMeter> ledMeter;
    int ledMeterStyle = 0;
    juce::Colour ledColour = juce::Colours::red;
    
    void drawClassicStyle(juce::Graphics& g, juce::Rectangle<int> area, float level);
    void drawRoundStyle(juce::Graphics& g, juce::Rectangle<int> area, float level);
    void drawSquareStyle(juce::Graphics& g, juce::Rectangle<int> area, float level);
    void drawDiamondStyle(juce::Graphics& g, juce::Rectangle<int> area, float level);
    void drawBarStyle(juce::Graphics& g, juce::Rectangle<int> area, float level);
    void drawDotStyle(juce::Graphics& g, juce::Rectangle<int> area, float level);
    void drawGlowStyle(juce::Graphics& g, juce::Rectangle<int> area, float level);
    void drawNeonStyle(juce::Graphics& g, juce::Rectangle<int> area, float level);
    void drawMatrixStyle(juce::Graphics& g, juce::Rectangle<int> area, float level);
    void drawSpectrumStyle(juce::Graphics& g, juce::Rectangle<int> area, float level);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DraggableLedMeter)
};

/**
 * @class DraggableAudioScope
 * @brief Osciloscopio de audio con controles
 */
class DraggableAudioScope : public DraggableWidget
{
public:
    enum AudioScopeStyle
    {
        Waveform = 0,
        Spectrum,
        Vectorscope,
        Lissajous,
        Waterfall,
        Spectrogram,
        Phase,
        Goniometer,
        Oscilloscope,
        FFT
    };
    
    DraggableAudioScope(const juce::String& name);
    
    void resized() override;
    juce::ValueTree toValueTree() const;
    
    void fillBuffer(juce::AudioBuffer<float>& buffer);
    
    void setStyle(int style) { scopeStyle = style; repaint(); }
    int getStyle() const { return scopeStyle; }
    void setScopeColor(const juce::Colour& color) { scopeColour = color; repaint(); }

protected:
    void paintWidget(juce::Graphics& g) override;

private:
    std::unique_ptr<class AudioScope> scope;
    int scopeStyle = 0;
    juce::Colour scopeColour = juce::Colours::cyan;
    
    void drawWaveformStyle(juce::Graphics& g, juce::Rectangle<int> area);
    void drawSpectrumStyle(juce::Graphics& g, juce::Rectangle<int> area);
    void drawVectorscopeStyle(juce::Graphics& g, juce::Rectangle<int> area);
    void drawLissajousStyle(juce::Graphics& g, juce::Rectangle<int> area);
    void drawWaterfallStyle(juce::Graphics& g, juce::Rectangle<int> area);
    void drawSpectrogramStyle(juce::Graphics& g, juce::Rectangle<int> area);
    void drawPhaseStyle(juce::Graphics& g, juce::Rectangle<int> area);
    void drawGoniometerStyle(juce::Graphics& g, juce::Rectangle<int> area);
    void drawOscilloscopeStyle(juce::Graphics& g, juce::Rectangle<int> area);
    void drawFFTStyle(juce::Graphics& g, juce::Rectangle<int> area);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DraggableAudioScope)
};

/**
 * @class DraggableVerticalFader
 * @brief Fader vertical con soporte para filmstrip
 */
class DraggableVerticalFader : public DraggableWidget
{
public:
    enum VerticalFaderStyle
    {
        Modern = 0,
        Classic,
        Minimal,
        Studio,
        Vintage,
        Neon,
        Glass,
        Metal,
        Wood,
        Digital
    };
    
    DraggableVerticalFader(const juce::String& name);
    
    void resized() override;
    juce::ValueTree toValueTree() const;
    
    void updateFromMidiValue(int value) override;
    
    void setStyle(int style) { faderStyle = style; repaint(); }
    int getStyle() const { return faderStyle; }
    void setFaderColor(const juce::Colour& color) { faderColour = color; repaint(); }

protected:
    void paintWidget(juce::Graphics& g) override;

private:
    std::unique_ptr<class VerticalFader> fader;
    juce::Label label;
    int faderStyle = 0;
    juce::Colour faderColour = juce::Colours::blue;
    
    void drawModernStyle(juce::Graphics& g, juce::Rectangle<int> area, float value);
    void drawClassicStyle(juce::Graphics& g, juce::Rectangle<int> area, float value);
    void drawMinimalStyle(juce::Graphics& g, juce::Rectangle<int> area, float value);
    void drawStudioStyle(juce::Graphics& g, juce::Rectangle<int> area, float value);
    void drawVintageStyle(juce::Graphics& g, juce::Rectangle<int> area, float value);
    void drawNeonStyle(juce::Graphics& g, juce::Rectangle<int> area, float value);
    void drawGlassStyle(juce::Graphics& g, juce::Rectangle<int> area, float value);
    void drawMetalStyle(juce::Graphics& g, juce::Rectangle<int> area, float value);
    void drawWoodStyle(juce::Graphics& g, juce::Rectangle<int> area, float value);
    void drawDigitalStyle(juce::Graphics& g, juce::Rectangle<int> area, float value);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DraggableVerticalFader)
};

/**
 * @class DraggableImageButton
 * @brief Botón con imágenes on/off y label
 */
class DraggableImageButton : public DraggableWidget
{
public:
    enum ImageButtonStyle
    {
        Flat = 0,
        Raised,
        Rounded,
        Circle,
        Neon,
        Glass,
        Metal,
        Glow,
        Shadow,
        Gradient
    };
    
    DraggableImageButton(const juce::String& name);
    
    void resized() override;
    juce::ValueTree toValueTree() const;
    
    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void mouseUp(const juce::MouseEvent& e) override;
    
    void setStyle(int style) { buttonStyle = style; repaint(); }
    int getStyle() const { return buttonStyle; }
    void setButtonColor(const juce::Colour& color) { buttonColour = color; repaint(); }

protected:
    void paintWidget(juce::Graphics& g) override;

private:
    std::unique_ptr<class CustomImageButton> imageButton;
    int buttonStyle = 0;
    juce::Colour buttonColour = juce::Colours::grey;
    
    void drawFlatStyle(juce::Graphics& g, juce::Rectangle<int> area, bool isDown);
    void drawRaisedStyle(juce::Graphics& g, juce::Rectangle<int> area, bool isDown);
    void drawRoundedStyle(juce::Graphics& g, juce::Rectangle<int> area, bool isDown);
    void drawCircleStyle(juce::Graphics& g, juce::Rectangle<int> area, bool isDown);
    void drawNeonStyle(juce::Graphics& g, juce::Rectangle<int> area, bool isDown);
    void drawGlassStyle(juce::Graphics& g, juce::Rectangle<int> area, bool isDown);
    void drawMetalStyle(juce::Graphics& g, juce::Rectangle<int> area, bool isDown);
    void drawGlowStyle(juce::Graphics& g, juce::Rectangle<int> area, bool isDown);
    void drawShadowStyle(juce::Graphics& g, juce::Rectangle<int> area, bool isDown);
    void drawGradientStyle(juce::Graphics& g, juce::Rectangle<int> area, bool isDown);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DraggableImageButton)
};

/**
 * @class DraggableToggleButton
 * @brief Toggle button simple con checkbox
 */
class DraggableToggleButton : public DraggableWidget
{
public:
    enum ToggleButtonStyle
    {
        Checkbox = 0,
        Switch,
        Radio,
        LED,
        Button,
        Slider,
        Rocker,
        Flip,
        Power,
        Custom
    };
    
    DraggableToggleButton(const juce::String& name);
    
    void resized() override;
    juce::ValueTree toValueTree() const;
    
    void setStyle(int style) { toggleStyle = style; repaint(); }
    int getStyle() const { return toggleStyle; }
    void setToggleColor(const juce::Colour& color) { toggleColour = color; repaint(); }

protected:
    void paintWidget(juce::Graphics& g) override;

private:
    std::unique_ptr<class SimpleToggleButton> toggleButton;
    int toggleStyle = 0;
    juce::Colour toggleColour = juce::Colours::green;
    
    void drawCheckboxStyle(juce::Graphics& g, juce::Rectangle<int> area, bool isOn);
    void drawSwitchStyle(juce::Graphics& g, juce::Rectangle<int> area, bool isOn);
    void drawRadioStyle(juce::Graphics& g, juce::Rectangle<int> area, bool isOn);
    void drawLEDStyle(juce::Graphics& g, juce::Rectangle<int> area, bool isOn);
    void drawButtonStyle(juce::Graphics& g, juce::Rectangle<int> area, bool isOn);
    void drawSliderStyle(juce::Graphics& g, juce::Rectangle<int> area, bool isOn);
    void drawRockerStyle(juce::Graphics& g, juce::Rectangle<int> area, bool isOn);
    void drawFlipStyle(juce::Graphics& g, juce::Rectangle<int> area, bool isOn);
    void drawPowerStyle(juce::Graphics& g, juce::Rectangle<int> area, bool isOn);
    void drawCustomStyle(juce::Graphics& g, juce::Rectangle<int> area, bool isOn);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DraggableToggleButton)
};

/**
 * @class DraggableTransportButton
 * @brief Botones de transporte (Play, Pause, Stop, Record, etc.)
 */
class DraggableTransportButton : public DraggableWidget
{
public:
    enum TransportType
    {
        Play = 0,
        Pause,
        Stop,
        Record,
        Rewind,
        FastForward,
        Loop,
        Metronome,
        Tap,
        Sync,
        ReturnToZero,      // Go to Start/Beginning
        MarkerNext,        // Next Marker
        MarkerPrevious,    // Previous Marker
        NudgeForward,      // Nudge playhead forward
        NudgeBackward,     // Nudge playhead backward
        Drop,              // Drop in/out of record
        Replace,           // Replace recording mode
        AutomationMode,    // Automation (Read/Write/Latch/Touch)
        JogWheel,          // Jog wheel for scrubbing
        Scrub,             // Audio scrubbing
        PreRoll,           // Pre-roll setting
        PostRoll           // Post-roll setting
    };
    
    enum ButtonStyle
    {
        Flat = 0,
        Raised,
        Circular,
        Rounded,
        Neon,
        Retro,
        Modern,
        Minimal,
        Studio,
        Vintage
    };
    
    DraggableTransportButton(const juce::String& name, TransportType type);
    
    void resized() override;
    juce::ValueTree toValueTree() const;
    
    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void mouseUp(const juce::MouseEvent& e) override;
    
    void setStyle(int style) { buttonStyle = style; repaint(); }
    int getStyle() const { return buttonStyle; }
    void setButtonColor(const juce::Colour& color) { buttonColour = color; repaint(); }
    void setActiveColor(const juce::Colour& color) { activeColour = color; repaint(); }
    
    TransportType getTransportType() const { return transportType; }
    bool isActive() const { return active; }
    void setActive(bool shouldBeActive) { active = shouldBeActive; repaint(); }

protected:
    void paintWidget(juce::Graphics& g) override;

private:
    TransportType transportType;
    int buttonStyle = 0;
    bool active = false;
    juce::Colour buttonColour = juce::Colours::darkgrey;
    juce::Colour activeColour = juce::Colours::green;
    
    void drawTransportIcon(juce::Graphics& g, juce::Rectangle<float> area);
    void drawPlayIcon(juce::Graphics& g, juce::Rectangle<float> area);
    void drawPauseIcon(juce::Graphics& g, juce::Rectangle<float> area);
    void drawStopIcon(juce::Graphics& g, juce::Rectangle<float> area);
    void drawRecordIcon(juce::Graphics& g, juce::Rectangle<float> area);
    void drawRewindIcon(juce::Graphics& g, juce::Rectangle<float> area);
    void drawFastForwardIcon(juce::Graphics& g, juce::Rectangle<float> area);
    void drawLoopIcon(juce::Graphics& g, juce::Rectangle<float> area);
    void drawMetronomeIcon(juce::Graphics& g, juce::Rectangle<float> area);
    void drawTapIcon(juce::Graphics& g, juce::Rectangle<float> area);
    void drawSyncIcon(juce::Graphics& g, juce::Rectangle<float> area);
    void drawReturnToZeroIcon(juce::Graphics& g, juce::Rectangle<float> area);
    void drawMarkerNextIcon(juce::Graphics& g, juce::Rectangle<float> area);
    void drawMarkerPreviousIcon(juce::Graphics& g, juce::Rectangle<float> area);
    void drawNudgeForwardIcon(juce::Graphics& g, juce::Rectangle<float> area);
    void drawNudgeBackwardIcon(juce::Graphics& g, juce::Rectangle<float> area);
    void drawDropIcon(juce::Graphics& g, juce::Rectangle<float> area);
    void drawReplaceIcon(juce::Graphics& g, juce::Rectangle<float> area);
    void drawAutomationModeIcon(juce::Graphics& g, juce::Rectangle<float> area);
    void drawJogWheelIcon(juce::Graphics& g, juce::Rectangle<float> area);
    void drawScrubIcon(juce::Graphics& g, juce::Rectangle<float> area);
    void drawPreRollIcon(juce::Graphics& g, juce::Rectangle<float> area);
    void drawPostRollIcon(juce::Graphics& g, juce::Rectangle<float> area);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DraggableTransportButton)
};
