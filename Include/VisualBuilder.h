#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "DraggableWidget.h"

/**
 * @class WidgetPalette
 * @brief Paleta de widgets predefinidos listos para drag & drop
 * 
 * Biblioteca completa de componentes:
 * - Widgets basicos (botones, labels, sliders)
 * - Widgets de audio específicos (waveform, meters, spectrum)
 */
class WidgetPalette : public juce::Component
{
public:
    WidgetPalette();
    ~WidgetPalette() override = default;

    void paint(juce::Graphics& g) override;
    void resized() override;

    // Callback cuando se arrastra un widget desde la paleta
    std::function<void(DraggableWidget::WidgetType, const juce::Point<int>&)> onWidgetDraggedFromPalette;
    
    // Callbacks para gestión de proyecto
    std::function<void()> onSaveProject;
    std::function<void()> onLoadProject;
    std::function<void()> onClearProject;
    
    // Callbacks para configuración del grid
    std::function<void(bool)> onToggleGrid;
    std::function<void(bool)> onToggleSnap;
    std::function<void(int)> onGridSizeChanged;
    
    // Callback para configuración de audio/MIDI
    std::function<void()> onAudioSettings;

private:
    // Botones de la paleta - Widgets basicos
    juce::TextButton knobButton{"Knob"};
    juce::TextButton sliderButton{"Slider"};
    juce::TextButton buttonButton{"Button"};
    juce::TextButton labelButton{"Label"};
    juce::TextButton waveformButton{"Waveform"};
    juce::TextButton meterButton{"Meter"};
    
    // Botones de la paleta - Controles 2D (inspirados en AudioKit)
    juce::TextButton xyPadButton{"XY Pad"};
    juce::TextButton joystickButton{"Joystick"};
    juce::TextButton pitchWheelButton{"Pitch Wheel"};
    juce::TextButton indexedSliderButton{"Indexed Slider"};
    
    // Botones de la paleta - Widgets profesionales (inspirados en ViatorDSP)
    juce::TextButton basicMeterButton{"Basic Meter"};
    juce::TextButton vuMeterButton{"VU Meter"};
    juce::TextButton ledMeterButton{"LED Meter"};
    juce::TextButton audioScopeButton{"Scope"};
    juce::TextButton verticalFaderButton{"Fader"};
    juce::TextButton imageButtonButton{"Image Button"};
    juce::TextButton toggleButtonButton{"Toggle"};
    
    // Botones de transporte
    juce::TextButton transportPlayButton{"Play"};
    juce::TextButton transportPauseButton{"Pause"};
    juce::TextButton transportStopButton{"Stop"};
    juce::TextButton transportRecordButton{"Record"};
    juce::TextButton transportRewindButton{"Rewind"};
    juce::TextButton transportFFButton{"FF"};
    juce::TextButton transportLoopButton{"Loop"};
    juce::TextButton transportMetronomeButton{"Metro"};
    juce::TextButton transportTapButton{"Tap"};
    juce::TextButton transportSyncButton{"Sync"};
    juce::TextButton transportReturnToZeroButton{"Return 0"};
    juce::TextButton transportMarkerNextButton{"Marker >"};
    juce::TextButton transportMarkerPrevButton{"Marker <"};
    juce::TextButton transportNudgeFwdButton{"Nudge >"};
    juce::TextButton transportNudgeBackButton{"Nudge <"};
    juce::TextButton transportDropButton{"Drop"};
    juce::TextButton transportReplaceButton{"Replace"};
    juce::TextButton transportAutomationButton{"Auto"};
    juce::TextButton transportJogWheelButton{"Jog"};
    juce::TextButton transportScrubButton{"Scrub"};
    juce::TextButton transportPreRollButton{"Pre-Roll"};
    juce::TextButton transportPostRollButton{"Post-Roll"};
    
    // Botones de gestión de proyecto
    juce::TextButton saveButton{"Guardar"};
    juce::TextButton loadButton{"Cargar"};
    juce::TextButton clearButton{"Limpiar"};
    
    // Controles de grid
    juce::ToggleButton gridToggle;
    juce::ToggleButton snapToggle;
    juce::Slider gridSizeSlider;
    juce::Label gridSizeLabel;
    
    // Botón de configuración de audio
    juce::TextButton audioSettingsButton{"Audio/MIDI"};
    
    // Categorias
    juce::Label basicWidgetsLabel{"", "Widgets Basicos"};
    juce::Label audioWidgetsLabel{"", "Widgets de Audio"};
    juce::Label controlsLabel{"", "Controles 2D"};
    juce::Label proWidgetsLabel{"", "Widgets Pro"};
    juce::Label transportLabel{"", "Transporte"};
    juce::Label projectLabel{"", "Proyecto"};
    juce::Label gridLabel{"", "Grid"};
    
    // Viewport for scrolling
    juce::Viewport viewport;
    juce::Component contentComponent;

    void setupButton(juce::TextButton& button, DraggableWidget::WidgetType type);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WidgetPalette)
};

/**
 * @class PropertyInspector
 * @brief Inspector de propiedades para widgets seleccionados
 * 
 * Permite ajustar visualmente:
 * - Posición y tamaño (X, Y, Width, Height)
 * - Apariencia (colores, fuentes, bordes)
 * - Propiedades específicas del widget
 * - Conexiones de código (variable names, callbacks)
 */
class PropertyInspector : public juce::Component
{
public:
    PropertyInspector();
    ~PropertyInspector() override = default;

    void paint(juce::Graphics& g) override;
    void resized() override;

    // Establecer el widget a inspeccionar
    void setSelectedWidget(DraggableWidget* widget);
    void clearSelection();

private:
    enum class ColorTarget
    {
        Background,
        Border,
        Knob,
        Text
    };
    
    DraggableWidget* selectedWidget = nullptr;

    // Secciones del inspector
    juce::Label titleLabel{"", "Inspector de Propiedades"};
    juce::Label noSelectionLabel{"", "Ningun widget seleccionado"};
    
    // Propiedades de posicion y tamano
    juce::Label positionLabel{"", "Posicion y Tamano"};
    juce::Label xLabel{"", "X:"};
    juce::Label yLabel{"", "Y:"};
    juce::Label widthLabel{"", "Width:"};
    juce::Label heightLabel{"", "Height:"};
    
    juce::Slider xSlider;
    juce::Slider ySlider;
    juce::Slider widthSlider;
    juce::Slider heightSlider;
    
    // Propiedades de apariencia
    juce::Label appearanceLabel{"", "Apariencia"};
    juce::Label nameLabel{"", "Nombre:"};
    juce::TextEditor nameEditor;
    
    juce::Label bgColorLabel{"", "Color de Fondo:"};
    juce::TextButton bgColorButton{"Seleccionar Color"};
    
    juce::Label borderColorLabel{"", "Color de Borde:"};
    juce::TextButton borderColorButton{"Seleccionar Color"};
    
    juce::Label knobColorLabel{"", "Color de Knob:"};
    juce::TextButton knobColorButton{"Seleccionar Color"};
    
    juce::Label textColorLabel{"", "Color de Texto:"};
    juce::TextButton textColorButton{"Seleccionar Color"};
    
    juce::Label borderWidthLabel{"", "Ancho de Borde:"};
    juce::Slider borderWidthSlider{juce::Slider::LinearHorizontal, juce::Slider::TextBoxRight};
    
    juce::Label cornerRadiusLabel{"", "Radio de Esquina:"};
    juce::Slider cornerRadiusSlider{juce::Slider::LinearHorizontal, juce::Slider::TextBoxRight};
    
    juce::Label fillKnobLabel{"", "Knob Relleno:"};
    juce::ToggleButton fillKnobToggle;
    
    juce::Label knobStyleLabel{"", "Forma de Knob:"};
    juce::ComboBox knobStyleCombo;
    
    juce::Label knobFilmstripLabel{"", "PNG Knob Filmstrip:"};
    juce::TextButton loadKnobFilmstripButton{"Cargar PNG Knob..."};
    juce::Label knobFilmstripPathLabel{"", "Sin imagen"};
    
    juce::Label sliderStyleLabel{"", "Estilo de Slider:"};
    juce::ComboBox sliderStyleCombo;
    
    juce::Label sliderFilmstripLabel{"", "PNG Slider:"};
    juce::TextButton loadSliderFilmstripButton{"Cargar PNG Slider..."};
    juce::Label sliderFilmstripPathLabel{"", "Sin imagen"};
    
    juce::Label buttonStyleLabel{"", "Estilo de Boton:"};
    juce::ComboBox buttonStyleCombo;
    
    juce::Label buttonFilmstripLabel{"", "PNG Boton:"};
    juce::TextButton loadButtonFilmstripButton{"Cargar PNG Boton..."};
    juce::Label buttonFilmstripPathLabel{"", "Sin imagen"};
    
    juce::Label labelStyleLabel{"", "Estilo de Label:"};
    juce::ComboBox labelStyleCombo;
    
    juce::Label labelFilmstripLabel{"", "PNG Label:"};
    juce::TextButton loadLabelFilmstripButton{"Cargar PNG Label..."};
    juce::Label labelFilmstripPathLabel{"", "Sin imagen"};
    
    juce::Label waveformStyleLabel{"", "Estilo de Waveform:"};
    juce::ComboBox waveformStyleCombo;
    juce::Label waveformColorLabel{"", "Color Waveform:"};
    juce::TextButton waveformColorButton{"Seleccionar Color"};
    
    juce::Label meterStyleLabel{"", "Estilo de Meter:"};
    juce::ComboBox meterStyleCombo;
    juce::Label meterColorLabel{"", "Color Meter:"};
    juce::TextButton meterColorButton{"Seleccionar Color"};
    
    juce::Label xyPadStyleLabel{"", "Estilo de XYPad:"};
    juce::ComboBox xyPadStyleCombo;
    juce::Label xyPadColorLabel{"", "Color XYPad:"};
    juce::TextButton xyPadColorButton{"Seleccionar Color"};
    
    juce::Label joystickStyleLabel{"", "Estilo de Joystick:"};
    juce::ComboBox joystickStyleCombo;
    juce::Label joystickColorLabel{"", "Color Joystick:"};
    juce::TextButton joystickColorButton{"Seleccionar Color"};
    
    // Widgets Pro
    juce::Label basicMeterStyleLabel{"", "Estilo de BasicMeter:"};
    juce::ComboBox basicMeterStyleCombo;
    juce::Label basicMeterColorLabel{"", "Color BasicMeter:"};
    juce::TextButton basicMeterColorButton{"Seleccionar Color"};
    
    juce::Label vuMeterStyleLabel{"", "Estilo de VUMeter:"};
    juce::ComboBox vuMeterStyleCombo;
    juce::Label vuMeterColorLabel{"", "Color VUMeter:"};
    juce::TextButton vuMeterColorButton{"Seleccionar Color"};
    
    juce::Label ledMeterStyleLabel{"", "Estilo de LedMeter:"};
    juce::ComboBox ledMeterStyleCombo;
    juce::Label ledMeterColorLabel{"", "Color LedMeter:"};
    juce::TextButton ledMeterColorButton{"Seleccionar Color"};
    
    juce::Label audioScopeStyleLabel{"", "Estilo de AudioScope:"};
    juce::ComboBox audioScopeStyleCombo;
    juce::Label audioScopeColorLabel{"", "Color AudioScope:"};
    juce::TextButton audioScopeColorButton{"Seleccionar Color"};
    
    juce::Label verticalFaderStyleLabel{"", "Estilo de VerticalFader:"};
    juce::ComboBox verticalFaderStyleCombo;
    juce::Label verticalFaderColorLabel{"", "Color VerticalFader:"};
    juce::TextButton verticalFaderColorButton{"Seleccionar Color"};
    
    juce::Label imageButtonStyleLabel{"", "Estilo de ImageButton:"};
    juce::ComboBox imageButtonStyleCombo;
    juce::Label imageButtonColorLabel{"", "Color ImageButton:"};
    juce::TextButton imageButtonColorButton{"Seleccionar Color"};
    
    juce::Label toggleButtonStyleLabel{"", "Estilo de ToggleButton:"};
    juce::ComboBox toggleButtonStyleCombo;
    juce::Label toggleButtonColorLabel{"", "Color ToggleButton:"};
    juce::TextButton toggleButtonColorButton{"Seleccionar Color"};
    
    juce::Label filmstripLabel{"", "Imagen Filmstrip:"};
    juce::TextButton loadFilmstripButton{"Cargar PNG..."};
    juce::Label filmstripPathLabel{"", "Sin imagen"};
    
    // Propiedades de codigo
    juce::Label codeLabel{"", "Conexiones de Codigo"};
    juce::Label varNameLabel{"", "Variable C++:"};
    juce::TextEditor varNameEditor;
    
    juce::Label callbackLabel{"", "Callback:"};
    juce::ComboBox callbackCombo;
    
    // Viewport for scrolling
    juce::Viewport viewport;
    juce::Component contentComponent;

    void updateFromWidget();
    void applyToWidget();
    void showColorSelector(ColorTarget target);
    void showWaveformColorSelector();
    void showMeterColorSelector();
    void showXYPadColorSelector();
    void showJoystickColorSelector();
    void showBasicMeterColorSelector();
    void showVUMeterColorSelector();
    void showLedMeterColorSelector();
    void showAudioScopeColorSelector();
    void showVerticalFaderColorSelector();
    void showImageButtonColorSelector();
    void showToggleButtonColorSelector();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PropertyInspector)
};

/**
 * @class CodeEditor
 * @brief Editor de código integrado con resaltado de sintaxis
 */
class CodeEditorPanel : public juce::Component
{
public:
    CodeEditorPanel();
    ~CodeEditorPanel() override = default;

    void paint(juce::Graphics& g) override;
    void resized() override;

    juce::String getCode() const;
    void setCode(const juce::String& code);

private:
    std::unique_ptr<juce::TextEditor> codeEditor;
    
    juce::Label titleLabel{"", "Editor de Codigo C++"};
    juce::TextButton buildButton{"Compilar"};
    juce::TextButton runButton{"Ejecutar"};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CodeEditorPanel)
};

/**
 * @class DebugConsole
 * @brief Consola de depuración y registro
 */
class DebugConsole : public juce::Component
{
public:
    DebugConsole();
    ~DebugConsole() override = default;

    void paint(juce::Graphics& g) override;
    void resized() override;

    void log(const juce::String& message, bool isError = false);
    void clear();

private:
    juce::TextEditor console;
    juce::Label titleLabel{"", "Consola de Depuracion"};
    juce::TextButton clearButton{"Limpiar"};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DebugConsole)
};

