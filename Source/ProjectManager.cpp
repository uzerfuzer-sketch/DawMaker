void ModuleManager::showAddModuleDialog()
{
    auto w = std::make_unique<juce::AlertWindow>("Añadir Módulo", "Completa los datos del nuevo módulo:", juce::MessageBoxIconType::QuestionIcon);
    w->addTextEditor("id", "", "ID:");
    w->addTextEditor("displayName", "", "Nombre:");
    w->addTextEditor("category", "", "Categoría:");
    w->addTextEditor("description", "", "Descripción:");
    w->addButton("Añadir", 1, juce::KeyPress(juce::KeyPress::returnKey));
    w->addButton("Cancelar", 0, juce::KeyPress(juce::KeyPress::escapeKey));
    w->enterModalState(true, juce::ModalCallbackFunction::create([this, windowPtr = w.get()](int result)
    {
        if (result == 1)
        {
            JuceModule mod;
            mod.id = windowPtr->getTextEditorContents("id");
            mod.displayName = windowPtr->getTextEditorContents("displayName");
            mod.category = windowPtr->getTextEditorContents("category");
            mod.description = windowPtr->getTextEditorContents("description");
            mod.detailedHelp = "";
            mod.required = false;
            addModule(mod);
        }
    }), true);
    w.release();
}

void ModuleManager::addModule(const JuceModule& module)
{
    availableModules.push_back(module);
    updateModuleList();
}

void ModuleManager::removeModule(int index)
{
    if (index >= 0 && index < (int)availableModules.size() && !availableModules[index].required)
    {
        availableModules.erase(availableModules.begin() + index);
        updateModuleList();
    }
}

void ModuleManager::showEditModuleDialog(int index)
{
    if (index < 0 || index >= (int)availableModules.size() || availableModules[index].required) return;
    const auto& mod = availableModules[index];
    auto w = std::make_unique<juce::AlertWindow>("Editar Módulo", "Edita los datos del módulo:", juce::MessageBoxIconType::QuestionIcon);
    w->addTextEditor("id", mod.id, "ID:");
    w->addTextEditor("displayName", mod.displayName, "Nombre:");
    w->addTextEditor("category", mod.category, "Categoría:");
    w->addTextEditor("description", mod.description, "Descripción:");
    w->addButton("Guardar", 1, juce::KeyPress(juce::KeyPress::returnKey));
    w->addButton("Cancelar", 0, juce::KeyPress(juce::KeyPress::escapeKey));
    w->enterModalState(true, juce::ModalCallbackFunction::create([this, index, windowPtr = w.get()](int result)
    {
        if (result == 1)
        {
            JuceModule newMod = availableModules[index];
            newMod.id = windowPtr->getTextEditorContents("id");
            newMod.displayName = windowPtr->getTextEditorContents("displayName");
            newMod.category = windowPtr->getTextEditorContents("category");
            newMod.description = windowPtr->getTextEditorContents("description");
            editModule(index, newMod);
        }
    }), true);
    w.release();
}

void ModuleManager::editModule(int index, const JuceModule& module)
{
    if (index >= 0 && index < (int)availableModules.size() && !availableModules[index].required)
    {
        availableModules[index] = module;
        updateModuleList();
    }
}
#include "../Include/ProjectManager.h"
#include <juce_core/juce_core.h>

// ============================================================================
// FileTreeItem - Elemento del árbol de archivos
// ============================================================================
class ProjectExplorer::FileTreeItem : public juce::TreeViewItem
{
public:
    FileTreeItem(const juce::File& f) : file(f) {}
    
    bool mightContainSubItems() override
    {
        return file.isDirectory();
    }
    
    void paintItem(juce::Graphics& g, int width, int height) override
    {
        auto icon = file.isDirectory() ? "[D] " : getIconForFile(file);
        auto colour = isSelected() ? juce::Colours::lightblue : juce::Colours::white;
        
        g.setColour(colour);
        g.setFont(juce::FontOptions(14.0f));
        g.drawText(icon + " " + file.getFileName(), 4, 0, width - 4, height, 
                   juce::Justification::centredLeft, true);
    }
    
    void itemOpened(bool isNowOpen)
    {
        if (isNowOpen && getNumSubItems() == 0)
            refreshSubItems();
    }
    
    void refreshSubItems()
    {
        clearSubItems();
        
        if (file.isDirectory())
        {
            juce::Array<juce::File> files = file.findChildFiles(
                juce::File::findFilesAndDirectories, false);
            
            files.sort();
            
            for (auto& f : files)
                addSubItem(new FileTreeItem(f));
        }
    }
    
    juce::File getFile() const { return file; }

private:
    juce::File file;
    
    juce::String getIconForFile(const juce::File& f) const
    {
        auto ext = f.getFileExtension().toLowerCase();
        
        if (ext == ".cpp" || ext == ".cc") return "[C] ";
        if (ext == ".h" || ext == ".hpp") return "[H] ";
        if (ext == ".txt" || ext == ".cmake") return "[T] ";
        if (ext == ".json" || ext == ".xml") return "[X] ";
        if (ext == ".png" || ext == ".jpg" || ext == ".svg") return "[I] ";
        if (ext == ".wav" || ext == ".mp3" || ext == ".ogg") return "[A] ";
        if (ext == ".jucer") return "[J] ";
        
        return "[F] ";
    }
};

// ============================================================================
// ProjectExplorer
// ============================================================================
ProjectExplorer::ProjectExplorer()
{
    titleLabel.setFont(juce::FontOptions(16.0f, juce::Font::bold));
    titleLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    juce::Component::addAndMakeVisible(titleLabel);
    
    refreshButton.onClick = [this] { refreshTree(); };
    juce::Component::addAndMakeVisible(refreshButton);
    
    newFileButton.onClick = [this] { createNewFile(); };
    juce::Component::addAndMakeVisible(newFileButton);
    
    deleteButton.onClick = [this] { deleteSelectedFile(); };
    juce::Component::addAndMakeVisible(deleteButton);
    
    treeView.setDefaultOpenness(true);
    treeView.setColour(juce::TreeView::backgroundColourId, juce::Colour(0xff2a2a2a));
    juce::Component::addAndMakeVisible(treeView);
}

ProjectExplorer::~ProjectExplorer()
{
    treeView.setRootItem(nullptr);
}

void ProjectExplorer::resized()
{
    auto bounds = juce::Component::getLocalBounds();
    
    auto topBar = bounds.removeFromTop(40);
    topBar.reduce(5, 5);
    titleLabel.setBounds(topBar.removeFromLeft(200));
    topBar.removeFromRight(5);
    deleteButton.setBounds(topBar.removeFromRight(50));
    topBar.removeFromRight(5);
    newFileButton.setBounds(topBar.removeFromRight(80));
    topBar.removeFromRight(5);
    refreshButton.setBounds(topBar.removeFromRight(50));
    
    treeView.setBounds(bounds.reduced(5));
}

void ProjectExplorer::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff1e1e1e));
}

void ProjectExplorer::setProjectRoot(const juce::File& root)
{
    projectRoot = root;
    refreshTree();
}

juce::File ProjectExplorer::getSelectedFile() const
{
    if (auto* selected = dynamic_cast<FileTreeItem*>(treeView.getSelectedItem(0)))
        return selected->getFile();
    
    return {};
}

void ProjectExplorer::refreshTree()
{
    treeView.setRootItem(nullptr);
    
    if (projectRoot.exists())
    {
        rootItem = std::make_unique<FileTreeItem>(projectRoot);
        rootItem->refreshSubItems();
        treeView.setRootItem(rootItem.get());
        rootItem->setOpen(true);
    }
}

void ProjectExplorer::createNewFile()
{
    // Dialogo para crear nuevo archivo
    auto w = std::make_unique<juce::AlertWindow>("Nuevo Archivo", "Introduce el nombre del archivo:", 
                        juce::MessageBoxIconType::QuestionIcon);
    
    w->addTextEditor("filename", "", "Nombre:");
    w->addComboBox("type", {"C++ Source (.cpp)", "C++ Header (.h)", 
                           "CMake (.cmake)", "JSON (.json)"}, "Tipo:");
    w->addButton("Crear", 1, juce::KeyPress(juce::KeyPress::returnKey));
    w->addButton("Cancelar", 0, juce::KeyPress(juce::KeyPress::escapeKey));
    
    w->enterModalState(true, juce::ModalCallbackFunction::create([this, windowPtr = w.get()](int result)
    {
        if (result == 1)
        {
            auto filename = windowPtr->getTextEditorContents("filename");
            auto type = windowPtr->getComboBoxComponent("type")->getSelectedItemIndex();
            
            juce::String extension;
            switch (type)
            {
                case 0: extension = ".cpp"; break;
                case 1: extension = ".h"; break;
                case 2: extension = ".cmake"; break;
                case 3: extension = ".json"; break;
            }
            
            auto newFile = projectRoot.getChildFile(filename + extension);
            newFile.create();
            refreshTree();
        }
    }));
    
    w.release();
}

void ProjectExplorer::deleteSelectedFile()
{
    auto selected = getSelectedFile();
    if (selected.exists())
    {
        juce::NativeMessageBox::showOkCancelBox(
            juce::MessageBoxIconType::WarningIcon,
            "Eliminar Archivo",
            "Seguro que quieres eliminar " + selected.getFileName() + "?",
            nullptr,
            juce::ModalCallbackFunction::create([this, selected](int result)
            {
                if (result == 1)
                {
                    selected.deleteFile();
                    refreshTree();
                }
            }));
    }
}

// ============================================================================
// ModuleManager
// ============================================================================
ModuleManager::ModuleManager()
{
    titleLabel.setFont(juce::FontOptions(16.0f, juce::Font::bold));
    titleLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(titleLabel);
    
    helpButton.setTooltip("Mostrar guia de uso del gestor de modulos");
    helpButton.onClick = [this] { showUserGuide(); };
    helpButton.setColour(juce::TextButton::buttonColourId, juce::Colours::orange);

    addAndMakeVisible(helpButton);

    saveModulesButton.setTooltip("Guardar seleccion actual de modulos");
    saveModulesButton.onClick = [this] { saveModuleSelection(); };
    saveModulesButton.setColour(juce::TextButton::buttonColourId, juce::Colours::green.darker(0.3f));
    addAndMakeVisible(saveModulesButton);

    selectAllButton.setTooltip("Seleccionar todos los modulos disponibles");
    selectAllButton.onClick = [this] 
    { 
        for (auto* toggle : moduleToggles)
            toggle->setToggleState(true, juce::dontSendNotification);
    };
    addAndMakeVisible(selectAllButton);

    selectNoneButton.setTooltip("Deseleccionar todos los modulos (excepto requeridos)");
    selectNoneButton.onClick = [this] 
    { 
        for (auto* toggle : moduleToggles)
            toggle->setToggleState(false, juce::dontSendNotification);
    };
    addAndMakeVisible(selectNoneButton);

    generateCMakeButton.setTooltip("Genera archivo CMakeLists.txt con los modulos seleccionados");
    generateCMakeButton.onClick = [this] { generateCMakeLists(); };
    addAndMakeVisible(generateCMakeButton);

    generateJucerButton.setTooltip("Genera archivo .jucer con la configuracion actual");
    generateJucerButton.onClick = [this] { generateJucerFile(); };
    addAndMakeVisible(generateJucerButton);

    // Botón para añadir módulo
    addModuleButton.setTooltip("Añadir un nuevo módulo personalizado");
    addModuleButton.onClick = [this] { showAddModuleDialog(); };
    addAndMakeVisible(addModuleButton);
    
    viewport.setViewedComponent(&moduleListComponent, false);
    viewport.setScrollBarsShown(true, false);
    addAndMakeVisible(viewport);
    
    initializeModules();
    updateModuleList();
    
    // Cargar selección guardada si existe
    loadModuleSelection();
}

void ModuleManager::resized()
{
    auto bounds = getLocalBounds();
    
    auto topBar = bounds.removeFromTop(40);
    topBar.reduce(5, 5);
    titleLabel.setBounds(topBar.removeFromLeft(200));
    topBar.removeFromRight(5);
    generateJucerButton.setBounds(topBar.removeFromRight(120));
    topBar.removeFromRight(5);
    generateCMakeButton.setBounds(topBar.removeFromRight(150));
    topBar.removeFromRight(5);
    saveModulesButton.setBounds(topBar.removeFromRight(80));
    topBar.removeFromRight(5);
    helpButton.setBounds(topBar.removeFromRight(30));
    
    auto buttonBar = bounds.removeFromTop(35);
    buttonBar.reduce(5, 5);
    selectAllButton.setBounds(buttonBar.removeFromLeft(80));
    buttonBar.removeFromLeft(5);
    selectNoneButton.setBounds(buttonBar.removeFromLeft(80));
    buttonBar.removeFromLeft(5);
    addModuleButton.setBounds(buttonBar.removeFromLeft(130));
    
    viewport.setBounds(bounds.reduced(5));
}

void ModuleManager::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff1e1e1e));
}

void ModuleManager::initializeModules()
{
    availableModules = {
        {"juce_core", "JUCE Core", "Core", "Clases basicas y utilidades", 
         "Modulo fundamental con clases basicas: String, Array, File, Time, Thread, etc. SIEMPRE requerido.", 
         true, {}},
        
        {"juce_events", "JUCE Events", "Core", "Sistema de eventos y mensajes", 
         "Manejo de eventos, timers, mensajes asincronos. Requerido para GUI y audio.",
         true, {"juce_core"}},
        
        {"juce_data_structures", "Data Structures", "Core", "ValueTree y estructuras", 
         "ValueTree para jerarquias de datos, Undoable para deshacer/rehacer. Util para configuracion y proyectos.",
         false, {"juce_core"}},
        
        {"juce_graphics", "Graphics", "GUI", "Renderizado 2D y graficos", 
         "Sistema de renderizado 2D: colores, fuentes, imagenes, paths. Base para toda GUI.",
         false, {"juce_core", "juce_events"}},
        
        {"juce_gui_basics", "GUI Basics", "GUI", "Componentes basicos de UI", 
         "Componentes esenciales: Button, Slider, Label, TextEditor, etc. Necesario para cualquier interfaz.",
         false, {"juce_graphics"}},
        
        {"juce_gui_extra", "GUI Extra", "GUI", "Componentes avanzados de UI", 
         "Componentes avanzados: WebBrowser, CodeEditor, FileBrowser, etc. Para UIs complejas.",
         false, {"juce_gui_basics"}},
        
        {"juce_audio_basics", "Audio Basics", "Audio", "Tipos basicos de audio", 
         "Tipos fundamentales de audio: AudioBuffer, MidiMessage, AudioProcessor. Base para todo audio.",
         false, {"juce_core"}},
        
        {"juce_audio_devices", "Audio Devices", "Audio", "I/O de dispositivos de audio", 
         "Acceso a dispositivos de audio y MIDI del sistema. Necesario para reproduccion y grabacion.",
         false, {"juce_audio_basics"}},
        
        {"juce_audio_formats", "Audio Formats", "Audio", "Codecs de audio (WAV, MP3)", 
         "Lectura/escritura de archivos: WAV, AIFF, MP3, FLAC, OGG. Para importar/exportar audio.",
         false, {"juce_audio_basics"}},
        
        {"juce_audio_processors", "Audio Processors", "Audio", "Framework de plugins", 
         "Framework para crear plugins VST/AU/AAX. Esencial para desarrollo de plugins.",
         false, {"juce_audio_basics"}},
        
        {"juce_audio_utils", "Audio Utils", "Audio", "Utilidades de audio", 
         "Componentes GUI de audio: Waveform, KeyboardComponent, MidiKeyboard. Combina audio + GUI.",
         false, {"juce_audio_basics", "juce_gui_basics"}},
        
        {"juce_dsp", "DSP", "Audio", "Procesamiento de senales", 
         "Algoritmos DSP: filtros, delays, reverbs, FFT, convolution. Para procesamiento avanzado.",
         false, {"juce_audio_basics"}},
        
        {"juce_opengl", "OpenGL", "Graphics", "Aceleracion OpenGL", 
         "Renderizado acelerado por GPU con OpenGL. Para graficos 3D o visualizaciones intensivas.",
         false, {"juce_gui_extra"}},
        
        {"juce_video", "Video", "Media", "Reproduccion de video", 
         "Reproduccion de video en componentes GUI. Para aplicaciones multimedia.",
         false, {"juce_gui_extra"}},
    };
}

void ModuleManager::updateModuleList()
{
    moduleToggles.clear();
    moduleDescriptions.clear();
    
    int yPos = 10;
    
    juce::String currentCategory;
    
    for (size_t i = 0; i < availableModules.size(); ++i)
    {
        const auto& module = availableModules[i];
        // Separador de categoría
        if (module.category != currentCategory)
        {
            currentCategory = module.category;
            auto* categoryLabel = new juce::Label("", "--- " + currentCategory + " ---");
            categoryLabel->setFont(juce::FontOptions(14.0f, juce::Font::bold));
            categoryLabel->setColour(juce::Label::textColourId, juce::Colours::cyan);
            categoryLabel->setBounds(10, yPos, 400, 25);
            moduleListComponent.addAndMakeVisible(categoryLabel);
            moduleDescriptions.add(categoryLabel);
            yPos += 30;
        }

        // Toggle del módulo
        auto* toggle = new juce::ToggleButton(module.displayName);
        toggle->setBounds(20, yPos, 250, 24);
        toggle->setToggleState(module.required, juce::dontSendNotification);
        toggle->setEnabled(!module.required);
        toggle->onClick = [this] { onModuleToggled(); };
        // Doble clic para editar
        if (!module.required)
        {
            toggle->onDoubleClick = [this, idx = (int)i] { showEditModuleDialog(idx); };
        }
        moduleListComponent.addAndMakeVisible(toggle);
        moduleToggles.add(toggle);

        // Botón eliminar si no es requerido
        if (!module.required)
        {
            auto* deleteBtn = new juce::TextButton("Eliminar");
            deleteBtn->setBounds(280, yPos, 70, 24);
            deleteBtn->setColour(juce::TextButton::buttonColourId, juce::Colours::red.darker(0.2f));
            deleteBtn->onClick = [this, idx = (int)i] { removeModule(idx); };
            moduleListComponent.addAndMakeVisible(deleteBtn);
            moduleDescriptions.add(deleteBtn);
        }

        // Descripción
        auto* desc = new juce::Label("", module.description);
        desc->setFont(juce::FontOptions(11.0f));
        desc->setColour(juce::Label::textColourId, juce::Colours::lightgrey);
        desc->setBounds(30, yPos + 22, 350, 18);
        moduleListComponent.addAndMakeVisible(desc);
        moduleDescriptions.add(desc);

        yPos += 50;
    }
    
    moduleListComponent.setSize(400, yPos);
}

juce::StringArray ModuleManager::getSelectedModules() const
{
    juce::StringArray selected;
    
    for (int i = 0; i < moduleToggles.size(); ++i)
    {
        if (moduleToggles[i]->getToggleState())
            selected.add(availableModules[i].id);
    }
    
    return selected;
}

void ModuleManager::setSelectedModules(const juce::StringArray& modules)
{
    for (int i = 0; i < moduleToggles.size(); ++i)
    {
        moduleToggles[i]->setToggleState(
            modules.contains(availableModules[i].id), 
            juce::dontSendNotification);
    }
}

void ModuleManager::generateCMakeLists()
{
    auto selected = getSelectedModules();
    
    juce::String cmake = "cmake_minimum_required(VERSION 3.22)\n\n";
    cmake += "project(CustomDAW VERSION 1.0.0)\n\n";
    cmake += "set(CMAKE_CXX_STANDARD 17)\n";
    cmake += "set(CMAKE_CXX_STANDARD_REQUIRED ON)\n\n";
    cmake += "add_subdirectory(../JUCE JUCE)\n\n";
    cmake += "juce_add_gui_app(CustomDAW\n";
    cmake += "    PRODUCT_NAME \"Custom DAW\"\n";
    cmake += "    COMPANY_NAME \"YourCompany\")\n\n";
    cmake += "target_sources(CustomDAW PRIVATE\n";
    cmake += "    Source/Main.cpp\n";
    cmake += "    # Anade mas archivos aqui\n";
    cmake += "    )\n\n";
    cmake += "target_link_libraries(CustomDAW PRIVATE\n";
    
    for (const auto& module : selected)
        cmake += "    " + module + "\n";
    
    cmake += "    )\n";
    
    juce::AlertWindow::showMessageBoxAsync(
        juce::MessageBoxIconType::InfoIcon,
        "CMakeLists.txt Generado",
        cmake);
}

void ModuleManager::generateJucerFile()
{
    juce::AlertWindow::showMessageBoxAsync(
        juce::MessageBoxIconType::InfoIcon,
        "Generar .jucer",
        "La generacion de archivos .jucer estara disponible proximamente.");
}

void ModuleManager::onModuleToggled()
{
    // Aqui se podria verificar dependencias automaticamente
}

void ModuleManager::showUserGuide()
{
    auto window = new juce::AlertWindow(
        "Guia del Gestor de Modulos JUCE",
        "",
        juce::MessageBoxIconType::InfoIcon
    );
    
    // Crear área de texto con scroll
    auto* textEditor = new juce::TextEditor();
    textEditor->setMultiLine(true);
    textEditor->setReadOnly(true);
    textEditor->setScrollbarsShown(true);
    textEditor->setCaretVisible(false);
    textEditor->setPopupMenuEnabled(false);
    textEditor->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0xff2a2a2a));
    textEditor->setColour(juce::TextEditor::textColourId, juce::Colours::white);
    textEditor->setFont(juce::FontOptions(13.0f));
    
    juce::String guideText = 
        "=== GUIA RAPIDA ===\n\n"
        
        "QUE ES:\n"
        "Selecciona que partes de JUCE incluir en tu proyecto.\n"
        "Solo incluye lo necesario = compilacion mas rapida.\n\n"
        
        "CATEGORIAS:\n\n"
        
        "Core: Clases fundamentales (siempre requeridas)\n"
        "  - juce_core: String, Array, File, Thread\n"
        "  - juce_events: Timers, mensajes\n\n"
        
        "GUI: Interfaz grafica\n"
        "  - juce_graphics: Renderizado 2D\n"
        "  - juce_gui_basics: Button, Slider, Label\n"
        "  - juce_gui_extra: WebBrowser, CodeEditor\n\n"
        
        "Audio: Procesamiento de audio\n"
        "  - juce_audio_basics: AudioBuffer, MIDI\n"
        "  - juce_audio_devices: Hardware I/O\n"
        "  - juce_audio_processors: Plugins VST/AU\n"
        "  - juce_audio_utils: GUI + Audio\n"
        "  - juce_dsp: Filtros, reverbs, FFT\n\n"
        
        "COMO USAR:\n"
        "1. Marca los modulos que necesitas\n"
        "2. Click 'Guardar' para recordar seleccion\n"
        "3. Click 'Generar CMakeLists.txt' para exportar\n\n"
        
        "=== EJEMPLOS EJECUTABLES ===\n\n"
        
        "Haz click en un ejemplo para aplicarlo:\n\n"
        
        "[1] Plugin Simple\n"
        "    Plugin basico con GUI\n\n"
        
        "[2] Plugin Avanzado\n"
        "    Plugin con DSP y controles avanzados\n\n"
        
        "[3] DAW Standalone\n"
        "    Aplicacion completa de audio\n\n"
        
        "[4] Solo GUI\n"
        "    Aplicacion sin audio\n\n"
        
        "[5] Procesador DSP\n"
        "    Solo procesamiento sin GUI\n\n"
        
        "DETALLES:\n\n"
        "Plugin Simple:\n"
        "  core, events, audio_basics,\n"
        "  audio_processors, gui_basics\n\n"
        
        "Plugin Avanzado:\n"
        "  + data_structures, dsp,\n"
        "  gui_extra\n\n"
        
        "DAW Standalone:\n"
        "  + audio_devices, audio_formats,\n"
        "  audio_utils\n\n"
        
        "Solo GUI:\n"
        "  core, events, graphics,\n"
        "  gui_basics\n\n"
        
        "Procesador DSP:\n"
        "  core, audio_basics, dsp\n";
    
    textEditor->setText(guideText);
    textEditor->setSize(600, 500);
    
    window->addCustomComponent(textEditor);
    
    // Botones de ejemplos
    window->addButton("Ejemplo 1: Plugin Simple", 1);
    window->addButton("Ejemplo 2: Plugin Avanzado", 2);
    window->addButton("Ejemplo 3: DAW Standalone", 3);
    window->addButton("Ejemplo 4: Solo GUI", 4);
    window->addButton("Ejemplo 5: DSP Processor", 5);
    window->addButton("Cerrar", 0, juce::KeyPress(juce::KeyPress::escapeKey));
    
    window->enterModalState(true, juce::ModalCallbackFunction::create([this](int result)
    {
        if (result > 0)
        {
            applyExample(result);
        }
    }), true);
}

void ModuleManager::saveModuleSelection()
{
    auto selected = getSelectedModules();
    
    // Guardar en archivo de configuracion
    juce::File configFile = juce::File::getSpecialLocation(
        juce::File::userApplicationDataDirectory)
        .getChildFile("CustomDAW")
        .getChildFile("module_selection.txt");
    
    configFile.create();
    configFile.replaceWithText(selected.joinIntoString("\n"));
    
    juce::NativeMessageBox::showMessageBoxAsync(
        juce::MessageBoxIconType::InfoIcon,
        "Seleccion Guardada",
        "Se guardaron " + juce::String(selected.size()) + " modulos seleccionados.\n\n" +
        "Ubicacion: " + configFile.getFullPathName()
    );
}

void ModuleManager::loadModuleSelection()
{
    juce::File configFile = juce::File::getSpecialLocation(
        juce::File::userApplicationDataDirectory)
        .getChildFile("CustomDAW")
        .getChildFile("module_selection.txt");
    
    if (configFile.existsAsFile())
    {
        juce::StringArray modules;
        modules.addLines(configFile.loadFileAsString());
        setSelectedModules(modules);
    }
}

void ModuleManager::applyExample(int exampleIndex)
{
    // Primero deseleccionar todos
    for (auto* toggle : moduleToggles)
        toggle->setToggleState(false, juce::dontSendNotification);
    
    juce::StringArray modulesToSelect;
    juce::String exampleName;
    
    switch (exampleIndex)
    {
        case 1: // Plugin Simple
            exampleName = "Plugin Simple";
            modulesToSelect = {
                "juce_core",
                "juce_events",
                "juce_audio_basics",
                "juce_audio_processors",
                "juce_graphics",
                "juce_gui_basics"
            };
            break;
            
        case 2: // Plugin Avanzado
            exampleName = "Plugin Avanzado";
            modulesToSelect = {
                "juce_core",
                "juce_events",
                "juce_data_structures",
                "juce_audio_basics",
                "juce_audio_processors",
                "juce_dsp",
                "juce_graphics",
                "juce_gui_basics",
                "juce_gui_extra"
            };
            break;
            
        case 3: // DAW Standalone
            exampleName = "DAW Standalone";
            modulesToSelect = {
                "juce_core",
                "juce_events",
                "juce_data_structures",
                "juce_audio_basics",
                "juce_audio_devices",
                "juce_audio_formats",
                "juce_audio_utils",
                "juce_graphics",
                "juce_gui_basics",
                "juce_gui_extra"
            };
            break;
            
        case 4: // Solo GUI
            exampleName = "Solo GUI";
            modulesToSelect = {
                "juce_core",
                "juce_events",
                "juce_graphics",
                "juce_gui_basics"
            };
            break;
            
        case 5: // DSP Processor
            exampleName = "Procesador DSP";
            modulesToSelect = {
                "juce_core",
                "juce_audio_basics",
                "juce_dsp"
            };
            break;
    }
    
    // Seleccionar los módulos del ejemplo
    for (int i = 0; i < availableModules.size(); ++i)
    {
        if (modulesToSelect.contains(availableModules[i].id))
        {
            moduleToggles[i]->setToggleState(true, juce::dontSendNotification);
        }
    }
    
    juce::NativeMessageBox::showMessageBoxAsync(
        juce::MessageBoxIconType::InfoIcon,
        "Ejemplo Aplicado",
        "Se aplico la configuracion de:\n" + exampleName + "\n\n" +
        "Modulos seleccionados: " + juce::String(modulesToSelect.size())
    );
}

// ============================================================================
// PlatformConfig
// ============================================================================
PlatformConfig::PlatformConfig()
{
    titleLabel.setFont(juce::FontOptions(16.0f, juce::Font::bold));
    titleLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(titleLabel);
    
    initializeComponents();
    updateFromConfig();
}

void PlatformConfig::resized()
{
    auto bounds = getLocalBounds();
    
    auto topBar = bounds.removeFromTop(40);
    topBar.reduce(5, 5);
    titleLabel.setBounds(topBar.removeFromLeft(250));
    topBar.removeFromRight(5);
    exportButton.setBounds(topBar.removeFromRight(140));
    topBar.removeFromRight(5);
    saveButton.setBounds(topBar.removeFromRight(170));
    
    bounds.reduce(10, 0);
    
    // Plataformas
    auto platformSection = bounds.removeFromTop(120);
    platformsGroup.setBounds(platformSection);
    platformSection.reduce(10, 25);
    
    windowsToggle.setBounds(platformSection.removeFromTop(22));
    macosToggle.setBounds(platformSection.removeFromTop(22));
    linuxToggle.setBounds(platformSection.removeFromTop(22));
    auto mobileRow = platformSection.removeFromTop(22);
    iosToggle.setBounds(mobileRow.removeFromLeft(100));
    androidToggle.setBounds(mobileRow);
    
    bounds.removeFromTop(5);
    
    // Build config
    auto buildSection = bounds.removeFromTop(110);
    buildGroup.setBounds(buildSection);
    buildSection.reduce(10, 25);
    
    auto row1 = buildSection.removeFromTop(25);
    cppStandardLabel.setBounds(row1.removeFromLeft(120));
    cppStandardCombo.setBounds(row1.removeFromLeft(100));
    
    auto row2 = buildSection.removeFromTop(25);
    architectureLabel.setBounds(row2.removeFromLeft(120));
    architectureCombo.setBounds(row2.removeFromLeft(100));
    
    auto row3 = buildSection.removeFromTop(25);
    buildTypeLabel.setBounds(row3.removeFromLeft(120));
    buildTypeCombo.setBounds(row3.removeFromLeft(100));
    
    bounds.removeFromTop(5);
    
    // Plugins
    auto pluginSection = bounds.removeFromTop(155);
    pluginGroup.setBounds(pluginSection);
    pluginSection.reduce(10, 25);
    
    vst3Toggle.setBounds(pluginSection.removeFromTop(22));
    vst2Toggle.setBounds(pluginSection.removeFromTop(22));
    auToggle.setBounds(pluginSection.removeFromTop(22));
    auv3Toggle.setBounds(pluginSection.removeFromTop(22));
    aaxToggle.setBounds(pluginSection.removeFromTop(22));
    standaloneToggle.setBounds(pluginSection.removeFromTop(22));
    
    bounds.removeFromTop(5);
    
    // Project info
    auto projectSection = bounds.removeFromTop(110);
    projectGroup.setBounds(projectSection);
    projectSection.reduce(10, 25);
    
    auto prow1 = projectSection.removeFromTop(25);
    projectNameLabel.setBounds(prow1.removeFromLeft(80));
    projectNameEditor.setBounds(prow1);
    
    auto prow2 = projectSection.removeFromTop(25);
    companyNameLabel.setBounds(prow2.removeFromLeft(80));
    companyNameEditor.setBounds(prow2);
    
    auto prow3 = projectSection.removeFromTop(25);
    versionLabel.setBounds(prow3.removeFromLeft(80));
    versionEditor.setBounds(prow3.removeFromLeft(100));
}

void PlatformConfig::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff1e1e1e));
}

void PlatformConfig::initializeComponents()
{
    // Plataformas
    addAndMakeVisible(platformsGroup);
    windowsToggle.setToggleState(true, juce::dontSendNotification);
    addAndMakeVisible(windowsToggle);
    addAndMakeVisible(macosToggle);
    addAndMakeVisible(linuxToggle);
    addAndMakeVisible(iosToggle);
    addAndMakeVisible(androidToggle);
    
    // Build config
    addAndMakeVisible(buildGroup);
    addAndMakeVisible(cppStandardLabel);
    cppStandardCombo.addItemList({"C++11", "C++14", "C++17", "C++20", "C++23"}, 1);
    cppStandardCombo.setSelectedId(3); // C++17
    addAndMakeVisible(cppStandardCombo);
    
    addAndMakeVisible(architectureLabel);
    architectureCombo.addItemList({"x86", "x64", "ARM", "ARM64"}, 1);
    architectureCombo.setSelectedId(2); // x64
    addAndMakeVisible(architectureCombo);
    
    addAndMakeVisible(buildTypeLabel);
    buildTypeCombo.addItemList({"Debug", "Release", "RelWithDebInfo"}, 1);
    buildTypeCombo.setSelectedId(2); // Release
    addAndMakeVisible(buildTypeCombo);
    
    // Plugins
    addAndMakeVisible(pluginGroup);
    vst3Toggle.setToggleState(true, juce::dontSendNotification);
    vst3Toggle.setTooltip("VST3 - Plugin moderno de Steinberg (recomendado)");
    addAndMakeVisible(vst3Toggle);
    vst2Toggle.setTooltip("VST2 - Plugin legacy (requiere SDK de Steinberg)");
    addAndMakeVisible(vst2Toggle);
    auToggle.setTooltip("Audio Unit - Plugin nativo de macOS");
    addAndMakeVisible(auToggle);
    auv3Toggle.setTooltip("AUv3 - Audio Unit moderno para iOS/macOS");
    addAndMakeVisible(auv3Toggle);
    aaxToggle.setTooltip("AAX - Plugin para Pro Tools (requiere SDK de Avid)");
    addAndMakeVisible(aaxToggle);
    standaloneToggle.setToggleState(true, juce::dontSendNotification);
    standaloneToggle.setTooltip("Aplicacion standalone independiente");
    addAndMakeVisible(standaloneToggle);
    
    // Project info
    addAndMakeVisible(projectGroup);
    addAndMakeVisible(projectNameLabel);
    projectNameEditor.setText("CustomDAW");
    addAndMakeVisible(projectNameEditor);
    addAndMakeVisible(companyNameLabel);
    companyNameEditor.setText("YourCompany");
    addAndMakeVisible(companyNameEditor);
    addAndMakeVisible(versionLabel);
    versionEditor.setText("1.0.0");
    addAndMakeVisible(versionEditor);
    
    saveButton.onClick = [this] { saveConfiguration(); };
    addAndMakeVisible(saveButton);
    
    exportButton.onClick = [this] { exportToCMake(); };
    addAndMakeVisible(exportButton);
}

void PlatformConfig::updateFromConfig()
{
    windowsToggle.setToggleState(currentConfig.windows, juce::dontSendNotification);
    macosToggle.setToggleState(currentConfig.macos, juce::dontSendNotification);
    linuxToggle.setToggleState(currentConfig.linux, juce::dontSendNotification);
    iosToggle.setToggleState(currentConfig.ios, juce::dontSendNotification);
    androidToggle.setToggleState(currentConfig.android, juce::dontSendNotification);
}

PlatformConfig::BuildConfiguration PlatformConfig::getConfiguration() const
{
    BuildConfiguration config;
    config.windows = windowsToggle.getToggleState();
    config.macos = macosToggle.getToggleState();
    config.linux = linuxToggle.getToggleState();
    config.ios = iosToggle.getToggleState();
    config.android = androidToggle.getToggleState();
    
    config.cppStandard = juce::String(cppStandardCombo.getSelectedId() + 10);
    config.architecture = architectureCombo.getText();
    config.buildType = buildTypeCombo.getText();
    
    config.enableVST3 = vst3Toggle.getToggleState();
    config.enableAU = auToggle.getToggleState();
    config.enableAAX = aaxToggle.getToggleState();
    config.enableStandalone = standaloneToggle.getToggleState();
    
    config.projectName = projectNameEditor.getText();
    config.companyName = companyNameEditor.getText();
    config.version = versionEditor.getText();
    
    return config;
}

void PlatformConfig::setConfiguration(const BuildConfiguration& config)
{
    currentConfig = config;
    updateFromConfig();
}

void PlatformConfig::saveConfiguration()
{
    currentConfig = getConfiguration();
    
    juce::AlertWindow::showMessageBoxAsync(
        juce::MessageBoxIconType::InfoIcon,
        "Configuracion Guardada",
        "La configuracion se ha guardado correctamente.");
}

void PlatformConfig::exportToCMake()
{
    auto config = getConfiguration();
    
    auto chooser = std::make_shared<juce::FileChooser>(
        "Guardar CMakeLists.txt",
        juce::File::getSpecialLocation(juce::File::userDocumentsDirectory),
        "CMakeLists.txt"
    );
    
    chooser->launchAsync(juce::FileBrowserComponent::saveMode | juce::FileBrowserComponent::canSelectFiles,
        [this, chooser](const juce::FileChooser& fc)
        {
            auto file = fc.getResult();
            if (file != juce::File())
            {
                saveToCMake(file);
                
                juce::NativeMessageBox::showMessageBoxAsync(
                    juce::MessageBoxIconType::InfoIcon,
                    "CMake Exportado",
                    "CMakeLists.txt guardado en:\n" + file.getFullPathName()
                );
            }
        });
}

void PlatformConfig::saveToCMake(const juce::File& cmakeFile)
{
    auto config = getConfiguration();
    
    juce::String cmake;
    cmake << "# Generado automaticamente por Custom DAW\n";
    cmake << "# Fecha: " << juce::Time::getCurrentTime().toString(true, true) << "\n\n";
    cmake << "cmake_minimum_required(VERSION 3.22)\n\n";
    cmake << "project(" << config.projectName << " VERSION " << config.version << ")\n\n";
    cmake << "set(CMAKE_CXX_STANDARD " << config.cppStandard << ")\n";
    cmake << "set(CMAKE_CXX_STANDARD_REQUIRED ON)\n\n";
    
    // Plataformas
    cmake << "# Plataformas habilitadas\n";
    if (config.windows) cmake << "# - Windows\n";
    if (config.macos) cmake << "# - macOS\n";
    if (config.linux) cmake << "# - Linux\n";
    cmake << "\n";
    
    cmake << "add_subdirectory(../JUCE JUCE)\n\n";
    
    // Tipo de target
    if (config.enableVST3 || config.enableAU || config.enableAAX)
    {
        cmake << "juce_add_plugin(" << config.projectName << "\n";
        cmake << "    COMPANY_NAME \"" << config.companyName << "\"\n";
        cmake << "    PLUGIN_MANUFACTURER_CODE Manu\n";
        cmake << "    PLUGIN_CODE Plug\n";
        cmake << "    FORMATS";
        if (config.enableVST3) cmake << " VST3";
        if (config.enableAU) cmake << " AU";
        if (config.enableAAX) cmake << " AAX";
        if (config.enableStandalone) cmake << " Standalone";
        cmake << "\n";
        cmake << "    PRODUCT_NAME \"" << config.projectName << "\")\n\n";
    }
    else
    {
        cmake << "juce_add_gui_app(" << config.projectName << "\n";
        cmake << "    PRODUCT_NAME \"" << config.projectName << "\"\n";
        cmake << "    COMPANY_NAME \"" << config.companyName << "\")\n\n";
    }
    
    cmake << "target_sources(" << config.projectName << " PRIVATE\n";
    cmake << "    Source/Main.cpp\n";
    cmake << "    Source/MainComponent.cpp\n";
    cmake << "    # Agregar mas archivos aqui\n";
    cmake << "    )\n\n";
    
    cmake << "target_compile_definitions(" << config.projectName << " PUBLIC\n";
    cmake << "    JUCE_WEB_BROWSER=0\n";
    cmake << "    JUCE_USE_CURL=0\n";
    cmake << "    JUCE_APPLICATION_NAME_STRING=\"$<TARGET_PROPERTY:" << config.projectName << ",JUCE_PRODUCT_NAME>\")\n\n";
    
    cmake << "target_link_libraries(" << config.projectName << " PRIVATE\n";
    cmake << "    juce::juce_core\n";
    cmake << "    juce::juce_audio_basics\n";
    cmake << "    juce::juce_audio_devices\n";
    cmake << "    juce::juce_gui_basics\n";
    cmake << "    juce::juce_gui_extra\n";
    cmake << "    # Agregar mas modulos aqui\n";
    cmake << "    )\n";
    
    cmakeFile.replaceWithText(cmake);
}

void PlatformConfig::saveToJucer(const juce::File& jucerFile)
{
    auto config = getConfiguration();
    
    juce::String jucer;
    jucer << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\n";
    jucer << "<JUCERPROJECT id=\"" << config.projectName << "\" name=\"" << config.projectName << "\"\n";
    jucer << "              projectType=\"guiapp\" version=\"" << config.version << "\"\n";
    jucer << "              bundleIdentifier=\"com." << config.companyName.toLowerCase() << "." << config.projectName.toLowerCase() << "\"\n";
    jucer << "              companyName=\"" << config.companyName << "\">\n";
    jucer << "  <MAINGROUP id=\"MainGroup\" name=\"" << config.projectName << "\">\n";
    jucer << "    <GROUP id=\"Source\" name=\"Source\">\n";
    jucer << "      <FILE id=\"Main\" name=\"Main.cpp\" compile=\"1\" resource=\"0\" file=\"Source/Main.cpp\"/>\n";
    jucer << "      <FILE id=\"MainComponent\" name=\"MainComponent.cpp\" compile=\"1\" resource=\"0\" file=\"Source/MainComponent.cpp\"/>\n";
    jucer << "      <FILE id=\"MainComponentH\" name=\"MainComponent.h\" compile=\"0\" resource=\"0\" file=\"Include/MainComponent.h\"/>\n";
    jucer << "    </GROUP>\n";
    jucer << "  </MAINGROUP>\n";
    jucer << "  <EXPORTFORMATS>\n";
    
    if (config.windows)
    {
        jucer << "    <VS2019 targetFolder=\"Builds/VisualStudio2019\">\n";
        jucer << "      <CONFIGURATIONS>\n";
        jucer << "        <CONFIGURATION isDebug=\"1\" name=\"Debug\"/>\n";
        jucer << "        <CONFIGURATION isDebug=\"0\" name=\"Release\"/>\n";
        jucer << "      </CONFIGURATIONS>\n";
        jucer << "    </VS2019>\n";
    }
    
    if (config.macos)
    {
        jucer << "    <XCODE_MAC targetFolder=\"Builds/MacOSX\">\n";
        jucer << "      <CONFIGURATIONS>\n";
        jucer << "        <CONFIGURATION isDebug=\"1\" name=\"Debug\"/>\n";
        jucer << "        <CONFIGURATION isDebug=\"0\" name=\"Release\"/>\n";
        jucer << "      </CONFIGURATIONS>\n";
        jucer << "    </XCODE_MAC>\n";
    }
    
    jucer << "  </EXPORTFORMATS>\n";
    jucer << "  <MODULES>\n";
    jucer << "    <MODULE id=\"juce_core\" showAllCode=\"1\" useLocalCopy=\"0\" useGlobalPath=\"1\"/>\n";
    jucer << "    <MODULE id=\"juce_audio_basics\" showAllCode=\"1\" useLocalCopy=\"0\" useGlobalPath=\"1\"/>\n";
    jucer << "    <MODULE id=\"juce_gui_basics\" showAllCode=\"1\" useLocalCopy=\"0\" useGlobalPath=\"1\"/>\n";
    jucer << "  </MODULES>\n";
    jucer << "</JUCERPROJECT>\n";
    
    jucerFile.replaceWithText(jucer);
}

// ============================================================================
// PluginCanvas - Canvas de diseño visual
// ============================================================================
PluginCanvas::PluginCanvas()
{
    titleLabel.setFont(juce::FontOptions(18.0f, juce::Font::bold));
    titleLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(titleLabel);
    
    clearButton.setTooltip("Limpiar todos los widgets del canvas");
    clearButton.onClick = [this] { clearCanvas(); };
    addAndMakeVisible(clearButton);
    
    exportButton.setTooltip("Generar codigo C++ de la interfaz");
    exportButton.onClick = [this] { exportToCode(); };
    addAndMakeVisible(exportButton);
    
    addKnobButton.setTooltip("Agregar un control rotatorio");
    addKnobButton.onClick = [this] { addWidget(CanvasWidget::Knob); };
    addAndMakeVisible(addKnobButton);
    
    addSliderButton.setTooltip("Agregar un slider lineal");
    addSliderButton.onClick = [this] { addWidget(CanvasWidget::Slider); };
    addAndMakeVisible(addSliderButton);
    
    addButtonButton.setTooltip("Agregar un boton");
    addButtonButton.onClick = [this] { addWidget(CanvasWidget::Button); };
    addAndMakeVisible(addButtonButton);
    
    addMeterButton.setTooltip("Agregar un medidor de nivel");
    addMeterButton.onClick = [this] { addWidget(CanvasWidget::Meter); };
    addAndMakeVisible(addMeterButton);
    
    canvasArea.setSize(800, 600);
    canvasViewport.setViewedComponent(&canvasArea, false);
    addAndMakeVisible(canvasViewport);
}

void PluginCanvas::resized()
{
    auto bounds = getLocalBounds();
    
    // Toolbar superior
    auto toolbar = bounds.removeFromTop(50);
    toolbar.reduce(10, 10);
    
    titleLabel.setBounds(toolbar.removeFromLeft(180));
    toolbar.removeFromRight(5);
    exportButton.setBounds(toolbar.removeFromRight(140));
    toolbar.removeFromRight(5);
    clearButton.setBounds(toolbar.removeFromRight(80));
    
    // Toolbar de widgets
    auto widgetBar = bounds.removeFromTop(40);
    widgetBar.reduce(10, 5);
    
    addKnobButton.setBounds(widgetBar.removeFromLeft(90));
    widgetBar.removeFromLeft(5);
    addSliderButton.setBounds(widgetBar.removeFromLeft(90));
    widgetBar.removeFromLeft(5);
    addButtonButton.setBounds(widgetBar.removeFromLeft(90));
    widgetBar.removeFromLeft(5);
    addMeterButton.setBounds(widgetBar.removeFromLeft(90));
    
    // Canvas viewport
    canvasViewport.setBounds(bounds);
}

void PluginCanvas::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff1e1e1e));
    
    // Dibujar canvas
    auto canvasBounds = canvasArea.getBounds();
    g.setColour(juce::Colour(0xff2a2a2a));
    g.fillRect(canvasBounds);
    
    // Grid
    if (showGrid)
        drawGrid(g);
    
    // Dibujar widgets
    for (size_t i = 0; i < widgets.size(); ++i)
    {
        const auto& widget = widgets[i];
        auto bounds = widget.bounds;
        
        // Highlight si está seleccionado
        if (static_cast<int>(i) == selectedWidget)
        {
            g.setColour(juce::Colours::yellow);
            g.drawRect(bounds.expanded(2), 2);
        }
        
        g.setColour(widget.colour);
        g.fillRect(bounds);
        
        // Dibujar según el tipo
        g.setColour(juce::Colours::white);
        
        switch (widget.type)
        {
            case CanvasWidget::Knob:
            {
                auto center = bounds.getCentre();
                auto radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f - 5.0f;
                g.drawEllipse(center.x - radius, center.y - radius, 
                             radius * 2, radius * 2, 2.0f);
                
                // Indicador
                juce::Path p;
                p.addLineSegment(juce::Line<float>(center.x, center.y,
                                                   center.x, center.y - radius), 2.0f);
                g.strokePath(p, juce::PathStrokeType(2.0f));
                break;
            }
            
            case CanvasWidget::Slider:
            {
                auto trackBounds = bounds.reduced(10);
                g.drawRect(trackBounds, 2.0f);
                
                // Thumb
                auto thumbY = trackBounds.getY() + trackBounds.getHeight() / 2;
                g.fillRect(trackBounds.getX(), thumbY - 3, trackBounds.getWidth(), 6);
                break;
            }
            
            case CanvasWidget::Button:
            {
                g.drawRect(bounds.reduced(5), 2.0f);
                g.drawText("BTN", bounds, juce::Justification::centred);
                break;
            }
            
            case CanvasWidget::Meter:
            {
                auto meterBounds = bounds.reduced(10);
                g.drawRect(meterBounds, 2.0f);
                
                // Nivel de ejemplo
                auto level = meterBounds.removeFromBottom(meterBounds.getHeight() * 0.7f);
                g.setColour(juce::Colours::green);
                g.fillRect(level);
                break;
            }
        }
        
        // Nombre del widget
        g.setColour(juce::Colours::white.withAlpha(0.7f));
        g.setFont(juce::FontOptions(10.0f));
        g.drawText(widget.name, bounds.reduced(2), juce::Justification::centredBottom);
    }
}

void PluginCanvas::mouseDown(const juce::MouseEvent& e)
{
    // Verificar si se hizo clic en un widget
    selectedWidget = -1;
    
    for (int i = static_cast<int>(widgets.size()) - 1; i >= 0; --i)
    {
        if (widgets[i].bounds.contains(e.getPosition()))
        {
            selectedWidget = i;
            isDragging = true;
            dragOffset = e.getPosition() - widgets[i].bounds.getPosition();
            repaint();
            break;
        }
    }
}

void PluginCanvas::mouseDrag(const juce::MouseEvent& e)
{
    if (isDragging && selectedWidget >= 0)
    {
        auto newPos = snapToGrid(e.getPosition() - dragOffset);
        widgets[selectedWidget].bounds.setPosition(newPos);
        repaint();
    }
}

void PluginCanvas::mouseUp(const juce::MouseEvent&)
{
    isDragging = false;
}

void PluginCanvas::clearCanvas()
{
    if (widgets.empty())
        return;
    
    juce::NativeMessageBox::showYesNoBox(
        juce::MessageBoxIconType::QuestionIcon,
        "Limpiar Canvas",
        "¿Estas seguro de eliminar todos los widgets?",
        nullptr,
        juce::ModalCallbackFunction::create([this](int result)
        {
            if (result == 1) // Yes
            {
                widgets.clear();
                selectedWidget = -1;
                repaint();
            }
        })
    );
}

void PluginCanvas::exportToCode()
{
    if (widgets.empty())
    {
        juce::NativeMessageBox::showMessageBoxAsync(
            juce::MessageBoxIconType::InfoIcon,
            "Exportar Codigo",
            "El canvas esta vacio. Agrega algunos widgets primero."
        );
        return;
    }
    
    juce::String code;
    code << "// Codigo generado automaticamente desde PluginCanvas\n\n";
    code << "// En tu constructor:\n";
    
    for (size_t i = 0; i < widgets.size(); ++i)
    {
        const auto& w = widgets[i];
        juce::String varName = w.name;
        varName = varName.replaceCharacter(' ', '_').toLowerCase();
        
        switch (w.type)
        {
            case CanvasWidget::Knob:
                code << varName << ".setSliderStyle(juce::Slider::Rotary);\n";
                code << varName << ".setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);\n";
                break;
            case CanvasWidget::Slider:
                code << varName << ".setSliderStyle(juce::Slider::LinearHorizontal);\n";
                code << varName << ".setTextBoxStyle(juce::Slider::TextBoxRight, false, 50, 20);\n";
                break;
            case CanvasWidget::Button:
                code << varName << ".setButtonText(\"" << w.name << "\");\n";
                break;
            case CanvasWidget::Meter:
                code << "// " << varName << " es un medidor de nivel personalizado\n";
                break;
        }
        
        code << varName << ".setBounds(" 
             << w.bounds.getX() << ", " 
             << w.bounds.getY() << ", "
             << w.bounds.getWidth() << ", " 
             << w.bounds.getHeight() << ");\n";
        code << "addAndMakeVisible(" << varName << ");\n\n";
    }
    
    // Mostrar código en ventana
    auto window = new juce::AlertWindow("Codigo Exportado", "", 
                                        juce::MessageBoxIconType::InfoIcon);
    
    auto* editor = new juce::TextEditor();
    editor->setMultiLine(true);
    editor->setReadOnly(true);
    editor->setScrollbarsShown(true);
    editor->setSize(600, 400);
    editor->setText(code);
    editor->setFont(juce::FontOptions(juce::Font::getDefaultMonospacedFontName(), 12.0f, 0));
    
    window->addCustomComponent(editor);
    window->addButton("Copiar al Portapapeles", 1);
    window->addButton("Cerrar", 0);
    
    window->enterModalState(true, juce::ModalCallbackFunction::create([code](int result)
    {
        if (result == 1)
        {
            juce::SystemClipboard::copyTextToClipboard(code);
            juce::NativeMessageBox::showMessageBoxAsync(
                juce::MessageBoxIconType::InfoIcon,
                "Copiado",
                "Codigo copiado al portapapeles!"
            );
        }
    }), true);
}

void PluginCanvas::addWidget(CanvasWidget::Type type)
{
    CanvasWidget widget;
    widget.type = type;
    
    // Tamaño por defecto según el tipo
    int width = 80, height = 80;
    
    switch (type)
    {
        case CanvasWidget::Knob:
            widget.name = "Knob " + juce::String(widgets.size() + 1);
            widget.colour = juce::Colour(0xff404040);
            width = 80;
            height = 80;
            break;
            
        case CanvasWidget::Slider:
            widget.name = "Slider " + juce::String(widgets.size() + 1);
            widget.colour = juce::Colour(0xff404040);
            width = 150;
            height = 40;
            break;
            
        case CanvasWidget::Button:
            widget.name = "Button " + juce::String(widgets.size() + 1);
            widget.colour = juce::Colour(0xff505050);
            width = 80;
            height = 30;
            break;
            
        case CanvasWidget::Meter:
            widget.name = "Meter " + juce::String(widgets.size() + 1);
            widget.colour = juce::Colour(0xff303030);
            width = 30;
            height = 120;
            break;
    }
    
    // Posición inicial (centro del canvas)
    auto centerX = (canvasArea.getWidth() - width) / 2;
    auto centerY = (canvasArea.getHeight() - height) / 2;
    
    widget.bounds = juce::Rectangle<int>(centerX, centerY, width, height);
    
    widgets.push_back(widget);
    selectedWidget = static_cast<int>(widgets.size()) - 1;
    
    repaint();
}

void PluginCanvas::drawGrid(juce::Graphics& g)
{
    g.setColour(juce::Colours::white.withAlpha(0.1f));
    
    auto bounds = canvasArea.getBounds();
    
    // Líneas verticales
    for (int x = gridSize.x; x < bounds.getWidth(); x += gridSize.x)
    {
        g.drawVerticalLine(x, 0, static_cast<float>(bounds.getHeight()));
    }
    
    // Líneas horizontales
    for (int y = gridSize.y; y < bounds.getHeight(); y += gridSize.y)
    {
        g.drawHorizontalLine(y, 0, static_cast<float>(bounds.getWidth()));
    }
}

juce::Point<int> PluginCanvas::snapToGrid(juce::Point<int> pos)
{
    if (!showGrid)
        return pos;
    
    int snappedX = (pos.x / gridSize.x) * gridSize.x;
    int snappedY = (pos.y / gridSize.y) * gridSize.y;
    
    return {snappedX, snappedY};
}

// ============================================================================
// ProjectManagerPanel
// ============================================================================
ProjectManagerPanel::ProjectManagerPanel()
{
    titleLabel.setFont(juce::FontOptions(18.0f, juce::Font::bold));
    titleLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(titleLabel);
    
    openProjectButton.onClick = [this] { browseForProject(); };
    addAndMakeVisible(openProjectButton);
    
    newProjectButton.onClick = [this] { showNewProjectDialog(); };
    addAndMakeVisible(newProjectButton);
    
    tabs.setTabBarDepth(30);
    tabs.addTab("Explorador", juce::Colour(0xff2a2a2a), &explorer, false);
    tabs.addTab("Modulos", juce::Colour(0xff2a2a2a), &moduleManager, false);
    tabs.addTab("Plataformas", juce::Colour(0xff2a2a2a), &platformConfig, false);
    addAndMakeVisible(tabs);
}

void ProjectManagerPanel::resized()
{
    auto bounds = getLocalBounds();
    
    auto topBar = bounds.removeFromTop(50);
    topBar.reduce(10, 10);
    titleLabel.setBounds(topBar.removeFromLeft(220));
    topBar.removeFromRight(5);
    newProjectButton.setBounds(topBar.removeFromRight(140));
    topBar.removeFromRight(5);
    openProjectButton.setBounds(topBar.removeFromRight(140));
    
    tabs.setBounds(bounds);
}

void ProjectManagerPanel::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff1e1e1e));
}

void ProjectManagerPanel::openProject(const juce::File& projectFolder)
{
    currentProjectFile = projectFolder;
    explorer.setProjectRoot(projectFolder);
}

void ProjectManagerPanel::createNewProject(const juce::String& name, const juce::File& location)
{
    auto projectFolder = location.getChildFile(name);
    projectFolder.createDirectory();
    
    projectFolder.getChildFile("Source").createDirectory();
    projectFolder.getChildFile("Include").createDirectory();
    projectFolder.getChildFile("Resources").createDirectory();
    
    openProject(projectFolder);
}

void ProjectManagerPanel::browseForProject()
{
    auto chooser = std::make_shared<juce::FileChooser>("Seleccionar Carpeta del Proyecto");
    
    chooser->launchAsync(juce::FileBrowserComponent::openMode | 
                        juce::FileBrowserComponent::canSelectDirectories,
        [this, chooser](const juce::FileChooser& fc)
        {
            auto folder = fc.getResult();
            if (folder.exists())
                openProject(folder);
        });
}

void ProjectManagerPanel::showNewProjectDialog()
{
    auto w = std::make_unique<juce::AlertWindow>("Nuevo Proyecto", 
                        "Crear un nuevo proyecto JUCE", 
                        juce::MessageBoxIconType::QuestionIcon);
    
    w->addTextEditor("name", "MyPlugin", "Nombre del proyecto:");
    w->addTextEditor("company", "YourCompany", "Nombre de la empresa:");
    
    w->addTextBlock("\nFormatos de Plugin:");
    w->addComboBox("format", {"VST3", "VST2 (Legacy)", "AU (macOS)", "AUv3 (iOS/macOS)", "AAX (Pro Tools)", "Standalone", "VST3 + AU + Standalone"}, "Formato:");
    w->getComboBoxComponent("format")->setSelectedId(7); // VST3 + AU + Standalone por defecto
    
    w->addButton("Crear", 1);
    w->addButton("Cancelar", 0);
    
    w->enterModalState(true, juce::ModalCallbackFunction::create([this, windowPtr = w.get()](int result)
    {
        if (result == 1)
        {
            auto name = windowPtr->getTextEditorContents("name");
            auto company = windowPtr->getTextEditorContents("company");
            auto formatId = windowPtr->getComboBoxComponent("format")->getSelectedId();
            
            // Configurar formatos según la selección
            platformConfig.getConfiguration();
            auto config = platformConfig.getConfiguration();
            config.projectName = name;
            config.companyName = company;
            
            // Resetear todos los formatos
            config.enableVST3 = false;
            config.enableVST2 = false;
            config.enableAU = false;
            config.enableAUv3 = false;
            config.enableAAX = false;
            config.enableStandalone = false;
            
            switch (formatId)
            {
                case 1: config.enableVST3 = true; break;
                case 2: config.enableVST2 = true; break;
                case 3: config.enableAU = true; break;
                case 4: config.enableAUv3 = true; break;
                case 5: config.enableAAX = true; break;
                case 6: config.enableStandalone = true; break;
                case 7: // Combinación común
                    config.enableVST3 = true;
                    config.enableAU = true;
                    config.enableStandalone = true;
                    break;
            }
            
            platformConfig.setConfiguration(config);
            
            auto chooser = std::make_shared<juce::FileChooser>("Ubicacion del Proyecto");
            
            chooser->launchAsync(juce::FileBrowserComponent::openMode | 
                                juce::FileBrowserComponent::canSelectDirectories,
                [this, chooser, name](const juce::FileChooser& fc)
                {
                    auto location = fc.getResult();
                    if (location.exists())
                    {
                        createNewProject(name, location);
                    }
                });
        }
    }));
    
    w.release();
}
