#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_gui_extra/juce_gui_extra.h>
#include <juce_data_structures/juce_data_structures.h>

// ============================================================================
// ProjectExplorer - Árbol de vista jerárquica de archivos del proyecto
// ============================================================================
class ProjectExplorer : public juce::Component,
                        private juce::FileBrowserListener
{
public:
    ProjectExplorer();
    ~ProjectExplorer() override;

    void resized() override;
    void paint(juce::Graphics& g) override;
    
    void setProjectRoot(const juce::File& root);
    juce::File getSelectedFile() const;
    
    // Categorías del proyecto
    enum class FileCategory
    {
        Source,      // Archivos .cpp, .h
        Resources,   // Imágenes, iconos
        Audio,       // Samples, presets
        Config,      // CMakeLists.txt, .jucer, JSON
        Other
    };

private:
    class FileTreeItem;
    
    juce::Label titleLabel{"", "Explorador de Proyectos"};
    juce::TextButton refreshButton{"Actualizar"};
    juce::TextButton newFileButton{"+ Nuevo"};
    juce::TextButton deleteButton{"Eliminar"};
    
    juce::TreeView treeView;
    std::unique_ptr<FileTreeItem> rootItem;
    juce::File projectRoot;
    
    void selectionChanged() override {}
    void fileClicked(const juce::File&, const juce::MouseEvent&) override {}
    void fileDoubleClicked(const juce::File&) override {}
    void browserRootChanged(const juce::File&) override {}
    
    void refreshTree();
    void createNewFile();
    void deleteSelectedFile();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProjectExplorer)
};

// ============================================================================
// ModuleManager - Gestor visual de módulos JUCE
// ============================================================================
class ModuleManager : public juce::Component
{
public:
    ModuleManager();
    ~ModuleManager() override = default;

    void resized() override;
    void paint(juce::Graphics& g) override;
    
    juce::StringArray getSelectedModules() const;
    void setSelectedModules(const juce::StringArray& modules);
    
    void generateCMakeLists();
    void generateJucerFile();
    
    void showUserGuide();

private:
    struct JuceModule
    {
        juce::String id;
        juce::String displayName;
        juce::String category;
        juce::String description;
        juce::String detailedHelp;  // Nueva: ayuda detallada
        bool required;
        juce::StringArray dependencies;
    };
    
    juce::Label titleLabel{"", "Gestor de Modulos JUCE"};
    juce::TextButton helpButton{"?"};  // Nueva: botón de ayuda
    juce::TextButton saveModulesButton{"Guardar"};
    juce::TextButton selectAllButton{"Todos"};
    juce::TextButton selectNoneButton{"Ninguno"};
    juce::TextButton generateCMakeButton{"Generar CMakeLists.txt"};
    juce::TextButton generateJucerButton{"Generar .jucer"};
    juce::TextButton addModuleButton{"+ Añadir módulo"};
    
    juce::Viewport viewport;
    juce::Component moduleListComponent;
    
    juce::OwnedArray<juce::ToggleButton> moduleToggles;
    juce::OwnedArray<juce::Label> moduleDescriptions;
    
    std::vector<JuceModule> availableModules;
    
    void initializeModules();
    void updateModuleList();
    void onModuleToggled();
    void saveModuleSelection();
    void loadModuleSelection();
    void applyExample(int exampleIndex);

    // Métodos para edición de módulos
    void showAddModuleDialog();
    void addModule(const JuceModule& module);
    void removeModule(int index);
    void showEditModuleDialog(int index);
    void editModule(int index, const JuceModule& module);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModuleManager)
};

// ============================================================================
// PlatformConfig - Configuración multiplataforma
// ============================================================================
class PlatformConfig : public juce::Component
{
public:
    PlatformConfig();
    ~PlatformConfig() override = default;

    void resized() override;
    void paint(juce::Graphics& g) override;
    
    struct BuildConfiguration
    {
        bool windows = true;
        bool macos = false;
        bool linux = false;
        bool ios = false;
        bool android = false;
        
        juce::String cppStandard = "17";
        juce::String architecture = "x64";
        juce::String buildType = "Release";
        
        bool enableVST3 = true;
        bool enableVST2 = false;
        bool enableAU = false;
        bool enableAUv3 = false;
        bool enableAAX = false;
        bool enableStandalone = true;
        
        juce::String projectName;
        juce::String companyName;
        juce::String version = "1.0.0";
    };
    
    BuildConfiguration getConfiguration() const;
    void setConfiguration(const BuildConfiguration& config);
    
    void saveToCMake(const juce::File& cmakeFile);
    void saveToJucer(const juce::File& jucerFile);

private:
    juce::Label titleLabel{"", "Configuracion Multiplataforma"};
    
    // Plataformas
    juce::GroupComponent platformsGroup{"", "Plataformas de Destino"};
    juce::ToggleButton windowsToggle{"Windows"};
    juce::ToggleButton macosToggle{"macOS"};
    juce::ToggleButton linuxToggle{"Linux"};
    juce::ToggleButton iosToggle{"iOS"};
    juce::ToggleButton androidToggle{"Android"};
    
    // Configuracion de compilacion
    juce::GroupComponent buildGroup{"", "Configuracion de Compilacion"};
    juce::Label cppStandardLabel{"", "C++ Standard:"};
    juce::ComboBox cppStandardCombo;
    juce::Label architectureLabel{"", "Arquitectura:"};
    juce::ComboBox architectureCombo;
    juce::Label buildTypeLabel{"", "Tipo de Build:"};
    juce::ComboBox buildTypeCombo;
    
    // Formatos de plugin
    juce::GroupComponent pluginGroup{"", "Formatos de Plugin"};
    juce::ToggleButton vst3Toggle{"VST3"};
    juce::ToggleButton vst2Toggle{"VST2 (Legacy)"};
    juce::ToggleButton auToggle{"Audio Unit (AU)"};
    juce::ToggleButton auv3Toggle{"AUv3 (iOS/macOS)"};
    juce::ToggleButton aaxToggle{"AAX (Pro Tools)"};
    juce::ToggleButton standaloneToggle{"Standalone"};
    
    // Informacion del proyecto
    juce::GroupComponent projectGroup{"", "Informacion del Proyecto"};
    juce::Label projectNameLabel{"", "Nombre:"};
    juce::TextEditor projectNameEditor;
    juce::Label companyNameLabel{"", "Empresa:"};
    juce::TextEditor companyNameEditor;
    juce::Label versionLabel{"", "Version:"};
    juce::TextEditor versionEditor;
    
    juce::TextButton saveButton{"Guardar Configuracion"};
    juce::TextButton exportButton{"Exportar CMake"};
    
    BuildConfiguration currentConfig;
    
    void initializeComponents();
    void updateFromConfig();
    void saveConfiguration();
    void exportToCMake();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlatformConfig)
};

// ============================================================================
// PluginCanvas - Canvas de diseño visual para crear interfaces
// ============================================================================
class PluginCanvas : public juce::Component
{
public:
    PluginCanvas();
    ~PluginCanvas() override = default;

    void resized() override;
    void paint(juce::Graphics& g) override;
    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void mouseUp(const juce::MouseEvent& e) override;
    
    void clearCanvas();
    void exportToCode();

private:
    juce::Label titleLabel{"", "Canvas de Diseno"};
    juce::TextButton clearButton{"Limpiar"};
    juce::TextButton exportButton{"Exportar Codigo"};
    juce::TextButton addKnobButton{"+ Knob"};
    juce::TextButton addSliderButton{"+ Slider"};
    juce::TextButton addButtonButton{"+ Button"};
    juce::TextButton addMeterButton{"+ Meter"};
    
    juce::Viewport canvasViewport;
    juce::Component canvasArea;
    
    juce::Point<int> gridSize{10, 10};
    bool showGrid = true;
    
    struct CanvasWidget
    {
        enum Type { Knob, Slider, Button, Meter };
        Type type;
        juce::Rectangle<int> bounds;
        juce::String name;
        juce::Colour colour;
    };
    
    std::vector<CanvasWidget> widgets;
    int selectedWidget = -1;
    bool isDragging = false;
    juce::Point<int> dragOffset;
    
    void addWidget(CanvasWidget::Type type);
    void drawGrid(juce::Graphics& g);
    juce::Point<int> snapToGrid(juce::Point<int> pos);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginCanvas)
};

// ============================================================================
// ProjectManagerPanel - Panel principal que integra todo
// ============================================================================
class ProjectManagerPanel : public juce::Component
{
public:
    ProjectManagerPanel();
    ~ProjectManagerPanel() override = default;

    void resized() override;
    void paint(juce::Graphics& g) override;
    
    void openProject(const juce::File& projectFolder);
    void createNewProject(const juce::String& name, const juce::File& location);
    
    ProjectExplorer& getExplorer() { return explorer; }
    ModuleManager& getModuleManager() { return moduleManager; }
    PlatformConfig& getPlatformConfig() { return platformConfig; }

private:
    juce::Label titleLabel{"", "Gestion de Proyectos"};
    juce::TextButton openProjectButton{"Abrir Proyecto"};
    juce::TextButton newProjectButton{"Nuevo Proyecto"};
    
    juce::TabbedComponent tabs{juce::TabbedButtonBar::TabsAtTop};
    
    ProjectExplorer explorer;
    ModuleManager moduleManager;
    PlatformConfig platformConfig;
    PluginCanvas pluginCanvas;
    
    juce::File currentProjectFile;
    
    void browseForProject();
    void showNewProjectDialog();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProjectManagerPanel)
};
