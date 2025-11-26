#pragma once

#include <juce_audio_utils/juce_audio_utils.h>
#include "AudioEngine.h"
#include "CustomLookAndFeel.h"
#include "DraggableWidget.h"
#include "VisualBuilder.h"
#include "ProjectManager.h"
#include "PluginExporter.h"
#include "IntegratedTerminal.h"

/**
 * @class MainComponent
 * @brief Constructor Visual de GUI Profesional
 * 
 * Entorno de desarrollo completo con:
 * - Paleta de widgets drag & drop
 * - Área de diseño (canvas)
 * - Inspector de propiedades
 * - Editor de código C++
 * - Editor de flujo de audio
 * - Consola de depuración
 */
class MainComponent : public juce::AudioAppComponent,
                      private juce::MidiInputCallback,
                      public juce::MenuBarModel
{
public:
    MainComponent();
    ~MainComponent() override;

    // Métodos de AudioAppComponent
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    // Métodos de Component
    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    // Motor de audio
    std::unique_ptr<AudioEngine> audioEngine;

    // Look and Feel personalizado
    CustomLookAndFeel customLookAndFeel;

    // Panel de Gestión de Proyectos
    ProjectManagerPanel projectManager;

    // Paneles del constructor visual
    PropertyInspector propertyInspector;
    CodeEditorPanel codeEditor;
    DebugConsole debugConsole;
    AudioFlowEditor audioFlowEditor;
    IntegratedTerminal terminal;

    // Contenedor de widgets en el canvas
    juce::OwnedArray<DraggableWidget> widgets;
    
    // Viewport para scroll en canvas
    juce::Viewport canvasViewport;
    juce::Component canvasContainer;
    float zoomLevel = 1.0f;

    // Tabs para cambiar entre vistas
    juce::TabbedComponent viewTabs;

        // Teclado MIDI y botón de mostrar/ocultar
        DraggableMidiKeyboard* midiKeyboardWidget = nullptr;
    
    // Menús desplegables
    juce::MenuBarComponent menuBar;
    
    // Botones de acción global
    juce::TextButton newProjectButton{"Nuevo"};
    juce::TextButton saveButton{"Guardar"};
    juce::TextButton loadButton{"Cargar"};
    juce::TextButton exportButton{"Exportar Plugin"};
    juce::ComboBox frameworkSelector;
    juce::TextButton zoomInButton{"+"};
    juce::TextButton zoomOutButton{"-"};
    juce::TextButton exitButton{"Salir"};
    
    // Métodos para menús
    juce::StringArray getMenuBarNames();
    juce::PopupMenu getMenuForIndex(int topLevelMenuIndex, const juce::String& menuName);
    void menuItemSelected(int menuItemID, int topLevelMenuIndex);

    // Métodos de gestión de widgets
    DraggableWidget* addWidget(DraggableWidget::WidgetType type, const juce::Point<int>& position);
    void zoomIn();
    void zoomOut();
    void updateCanvasZoom();
    void removeWidget(DraggableWidget* widget);
    void selectWidget(DraggableWidget* widget);
    void saveLayout();
    void loadLayout();
    void clearAllWidgets();
    void showAudioSettings();
    void showMidiLearnDialog(DraggableWidget* widget);
    
    // MIDI Learn
    juce::MidiKeyboardState keyboardState;
    void handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message) override;
    void startMidiLearn(DraggableWidget* widget);
    void stopMidiLearn();
    
    bool midiLearnActive = false;
    DraggableWidget* midiLearnWidget = nullptr;
    
    // Sistema completo de proyectos (.dawproj)
    void saveCompleteProject();
    void loadCompleteProject();
    void autoSaveProject();
    juce::ValueTree createProjectTree();
    void loadFromProjectTree(const juce::ValueTree& projectTree);
    
    // Exportación MIDI y Audio
    void exportToMidi();
    void exportToAudio();
    
    // Nuevas funcionalidades de configuración
    void showRecentProjects();
    void showProjectProperties();
    void showModuleManager();
    void showModuleSettings(const juce::String& moduleName);
    void showPlatformExportDialog(const juce::String& platformName);
    void showMidiSettings();
    void showExportPathsSettings();
    void showEditorPreferences();
    void showKeyboardShortcuts();
    
    // Sistema de proyectos recientes
    juce::StringArray recentProjects;
    void addToRecentProjects(const juce::File& projectFile);
    void loadRecentProjects();
    void saveRecentProjects();
    
    // Configuración del proyecto actual
    struct ProjectProperties
    {
        juce::String name = "Sin título";
        juce::String author = "";
        juce::String version = "1.0.0";
        juce::String description = "";
        juce::Time creationDate;
        juce::Time lastModifiedDate;
    } projectProperties;
    
    // Módulos activos
    struct ModuleState
    {
        bool audioEnabled = true;
        bool midiEnabled = true;
        bool dspEnabled = true;
        bool guiEnabled = true;
        bool utilitiesEnabled = true;
    } moduleState;
    
    // Rutas de exportación
    struct ExportPaths
    {
        juce::File pluginsPath;
        juce::File midiPath;
        juce::File audioPath;
        juce::File projectsPath;
    } exportPaths;
    
    // Preferencias del editor
    struct EditorPreferences
    {
        int theme = 0; // 0=Dark, 1=Light, 2=Blue, 3=Custom
        int fontSize = 14;
        int gridSize = 20;
        bool showGrid = true;
        bool snapToGrid = true;
        
        void saveToFile();
        void loadFromFile();
    } editorPreferences;
    
    juce::File currentProjectFile;

    // Contador para nombres únicos
    int widgetCounter = 0;
    DraggableWidget* selectedWidget = nullptr;
    
    // Configuración del grid
    bool showGrid = true;
    bool snapToGrid = true;
    int gridSize = 20;
    
    juce::Point<int> snapToGridPoint(const juce::Point<int>& point) const;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

