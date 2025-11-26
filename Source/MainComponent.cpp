#include "MainComponent.h"
#include "MidiKeyboardWidget.h"

MainComponent::MainComponent()
    : viewTabs(juce::TabbedButtonBar::TabsAtTop)
{
    // Configurar el tamaño de la ventana
    setSize(1600, 900);

    // Aplicar el Look and Feel personalizado
    setLookAndFeel(&customLookAndFeel);
    
    // Configurar barra de menú
    menuBar.setModel(this);
    addAndMakeVisible(menuBar);

    // Configurar inspector de propiedades
    addAndMakeVisible(propertyInspector);

    // Configurar tabs para diferentes vistas
    viewTabs.setTabBarDepth(35);
    viewTabs.addTab("Editor de Codigo", juce::Colour(0xff16213e), &codeEditor, false);
    viewTabs.addTab("Terminal", juce::Colour(0xff16213e), &terminal, false);
    addAndMakeVisible(viewTabs);

    // Configurar consola de depuración
    addAndMakeVisible(debugConsole);
    debugConsole.log("DawMaker 1.0.1 iniciado");
    
    // Configurar viewport para canvas con scroll
    canvasContainer.setSize(4000, 4000); // Tamaño grande para scroll
    addAndMakeVisible(canvasViewport);
    canvasViewport.setViewedComponent(&canvasContainer, false);
    canvasViewport.setScrollBarsShown(true, true);

        // Crear teclado MIDI (oculto por defecto)
        midiKeyboardWidget = new DraggableMidiKeyboard("Teclado MIDI", keyboardState);
        midiKeyboardWidget->setVisible(false);
        addAndMakeVisible(midiKeyboardWidget);

        // Botón para mostrar/ocultar teclado MIDI
        addAndMakeVisible(toggleMidiKeyboardButton);
        toggleMidiKeyboardButton.onClick = [this]() {
            bool visible = !midiKeyboardWidget->isVisible();
            midiKeyboardWidget->setVisible(visible);
            resized();
        };
    
    // Configurar botones de proyecto en barra superior
    addAndMakeVisible(newProjectButton);
    newProjectButton.onClick = [this]() {
        clearAllWidgets();
        debugConsole.log("Nuevo proyecto creado");
    };
    
    addAndMakeVisible(saveButton);
    saveButton.onClick = [this]() {
        auto chooser = std::make_shared<juce::FileChooser>("Guardar Proyecto",
                                                           juce::File::getSpecialLocation(juce::File::userDocumentsDirectory),
                                                           "*.layout");
        
        auto chooserFlags = juce::FileBrowserComponent::saveMode | juce::FileBrowserComponent::canSelectFiles;
        
        chooser->launchAsync(chooserFlags, [this, chooser](const juce::FileChooser& fc)
        {
            auto file = fc.getResult();
            if (file != juce::File{})
            {
                // Crear ValueTree con todos los widgets
                juce::ValueTree projectTree("Project");
                projectTree.setProperty("name", "GUI Project", nullptr);
                
                for (auto* widget : widgets)
                {
                    if (widget)
                    {
                        projectTree.appendChild(widget->toValueTree(), nullptr);
                    }
                }
                
                // Guardar en archivo
                auto xml = projectTree.createXml();
                if (xml && file.replaceWithText(xml->toString()))
                {
                    debugConsole.log("Proyecto guardado en: " + file.getFullPathName());
                }
                else
                {
                    debugConsole.log("Error al guardar el proyecto");
                }
            }
        });
    };
    
    // Botón cargar proyecto
    addAndMakeVisible(loadButton);
    loadButton.onClick = [this]() {
        auto chooser = std::make_shared<juce::FileChooser>("Cargar Proyecto",
                                                           juce::File::getSpecialLocation(juce::File::userDocumentsDirectory),
                                                           "*.layout");
        
        auto chooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;
        
        chooser->launchAsync(chooserFlags, [this, chooser](const juce::FileChooser& fc)
        {
            auto file = fc.getResult();
            if (file != juce::File{})
            {
                clearAllWidgets();
                // Implementar carga de proyecto
                debugConsole.log("Proyecto cargado desde: " + file.getFullPathName());
            }
        });
    };
    
    // Configurar selector de framework
    addAndMakeVisible(frameworkSelector);
    frameworkSelector.addItem("JUCE", 1);
    frameworkSelector.addItem("iPlug2", 2);
    frameworkSelector.addItem("DPF", 3);
    frameworkSelector.addItem("Elements", 4);
    frameworkSelector.setSelectedId(1);
    frameworkSelector.setTooltip("Seleccionar framework de destino");
    
    addAndMakeVisible(exportButton);
    exportButton.onClick = [this]() {
        auto chooser = std::make_shared<juce::FileChooser>("Exportar Plugin",
                                                           juce::File::getSpecialLocation(juce::File::userDocumentsDirectory),
                                                           "");
        
        auto chooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectDirectories;
        
        chooser->launchAsync(chooserFlags, [this, chooser](const juce::FileChooser& fc)
        {
            auto directory = fc.getResult();
            if (directory != juce::File{})
            {
                // Obtener framework seleccionado
                PluginExporter::FrameworkType framework;
                int selectedId = frameworkSelector.getSelectedId();
                
                switch (selectedId)
                {
                    case 1: framework = PluginExporter::FrameworkType::JUCE; break;
                    case 2: framework = PluginExporter::FrameworkType::IPlug2; break;
                    case 3: framework = PluginExporter::FrameworkType::DPF; break;
                    case 4: framework = PluginExporter::FrameworkType::Elements; break;
                    default: framework = PluginExporter::FrameworkType::JUCE; break;
                }
                
                // Exportar proyecto
                juce::String pluginName = "MyPlugin";
                juce::String manufacturer = "CustomManufacturer";
                
                bool success = PluginExporter::exportProject(
                    framework,
                    directory,
                    widgets,
                    pluginName,
                    manufacturer
                );
                
                if (success)
                {
                    debugConsole.log("Plugin exportado exitosamente a: " + directory.getFullPathName());
                    debugConsole.log("Framework: " + frameworkSelector.getText());
                }
                else
                {
                    debugConsole.log("Error al exportar el plugin");
                }
            }
        });
    };
    
    addAndMakeVisible(exitButton);
    exitButton.onClick = []() {
        juce::JUCEApplication::getInstance()->systemRequestedQuit();
    };
    
    // Botones de zoom
    addAndMakeVisible(zoomInButton);
    zoomInButton.onClick = [this]() { zoomIn(); };
    
    addAndMakeVisible(zoomOutButton);
    zoomOutButton.onClick = [this]() { zoomOut(); };

    // Inicializar el motor de audio
    audioEngine = std::make_unique<AudioEngine>();

    // Configurar el dispositivo de audio
    setAudioChannels(2, 2); // 2 entradas, 2 salidas
    
    // Registrar callback MIDI para MIDI learn
    deviceManager.addMidiInputDeviceCallback(juce::String(), this);
    
    // Cargar configuración de proyectos recientes
    loadRecentProjects();
    
    // Cargar preferencias del editor
    editorPreferences.loadFromFile();
    
    // Aplicar preferencias cargadas
    gridSize = editorPreferences.gridSize;
    showGrid = editorPreferences.showGrid;
    snapToGrid = editorPreferences.snapToGrid;
    
    // Inicializar rutas de exportación predeterminadas
    exportPaths.pluginsPath = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory).getChildFile("DawMaker/Exports/Plugins");
    exportPaths.midiPath = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory).getChildFile("DawMaker/Exports/MIDI");
    exportPaths.audioPath = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory).getChildFile("DawMaker/Exports/Audio");
    exportPaths.projectsPath = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory).getChildFile("DawMaker/Projects");
    
    // Inicializar propiedades del proyecto
    projectProperties.creationDate = juce::Time::getCurrentTime();
    projectProperties.lastModifiedDate = juce::Time::getCurrentTime();
}

MainComponent::~MainComponent()
{
    // Desregistrar callback MIDI
    deviceManager.removeMidiInputDeviceCallback(juce::String(), this);
    
    setLookAndFeel(nullptr);
    shutdownAudio();
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    
    if (audioEngine)
    {
        audioEngine->prepareToPlay(samplesPerBlockExpected, sampleRate);
    }
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Mezclar el audio del engine (si existe)
    if (audioEngine)
    {
        audioEngine->getNextAudioBlock(bufferToFill);
    }
    else
    {
        bufferToFill.clearActiveBufferRegion();
    }
}

void MainComponent::releaseResources()
{
    if (audioEngine)
    {
        audioEngine->releaseResources();
    }
}

void MainComponent::paint(juce::Graphics& g)
{
    // Fondo con gradiente
    auto bounds = getLocalBounds();
    juce::ColourGradient gradient(
        juce::Colour(0xff1a1a2e), 0.0f, 0.0f,
        juce::Colour(0xff16213e), 0.0f, static_cast<float>(bounds.getHeight()),
        false
    );
    g.setGradientFill(gradient);
    g.fillAll();

    // Dibujar grid en el canvasContainer si está habilitado
    if (showGrid)
    {
        g.setColour(juce::Colour(0xff00adb5).withAlpha(0.15f));
        
        // El grid se dibuja relativo al canvasContainer, no a MainComponent
        auto canvasBounds = canvasContainer.getBounds();

        // Lineas verticales
        for (int x = gridSize; x < canvasBounds.getWidth(); x += gridSize)
        {
            g.drawLine(static_cast<float>(x), 0.0f, 
                       static_cast<float>(x), static_cast<float>(canvasBounds.getHeight()), 1.0f);
        }
        
        // Lineas horizontales
        for (int y = gridSize; y < canvasBounds.getHeight(); y += gridSize)
        {
            g.drawLine(0.0f, static_cast<float>(y), 
                       static_cast<float>(canvasBounds.getWidth()), static_cast<float>(y), 1.0f);
        }
    }

    // Texto de ayuda si no hay widgets
    if (widgets.isEmpty())
    {
        g.setColour(juce::Colours::white.withAlpha(0.5f));
        g.setFont(juce::FontOptions(20.0f));
        g.drawText("Usa los menus superiores para anadir widgets",
                   canvasViewport.getBounds().reduced(20),
                   juce::Justification::centred,
                   true);
        
        g.setFont(juce::FontOptions(14.0f));
        g.drawText("Arrastra para mover - Esquinas para redimensionar - X para eliminar",
                   canvasViewport.getBounds().reduced(20).translated(0, 40),
                   juce::Justification::centred,
                   true);
    }
}

void MainComponent::resized()
{
    auto bounds = getLocalBounds();
    
    // Barra de menú en la parte superior
    auto menuArea = bounds.removeFromTop(25);
    menuBar.setBounds(menuArea);
    
    // Botones superiores - Gestión de proyectos y zoom
    auto topBar = bounds.removeFromTop(35);
    exitButton.setBounds(topBar.removeFromRight(70).reduced(4));
    zoomOutButton.setBounds(topBar.removeFromRight(35).reduced(4));
    zoomInButton.setBounds(topBar.removeFromRight(35).reduced(4));
    topBar.removeFromRight(10); // Espaciado
    exportButton.setBounds(topBar.removeFromRight(130).reduced(4));
    frameworkSelector.setBounds(topBar.removeFromRight(110).reduced(4));
    topBar.removeFromRight(10); // Espaciado
    loadButton.setBounds(topBar.removeFromRight(80).reduced(4));
    saveButton.setBounds(topBar.removeFromRight(80).reduced(4));
    newProjectButton.setBounds(topBar.removeFromRight(80).reduced(4));
    

    // Consola de depuración en la parte inferior (reducida)
    auto debugArea = bounds.removeFromBottom(120);
    debugConsole.setBounds(debugArea);

    // Inspector de propiedades a la derecha (más estrecho)
    propertyInspector.setBounds(bounds.removeFromRight(250));

    // Área para el botón y el teclado MIDI
    int midiHeight = midiKeyboardWidget->isVisible() ? 80 : 0;
    int buttonHeight = 32;
    auto midiButtonArea = bounds.removeFromTop(buttonHeight);
    toggleMidiKeyboardButton.setBounds(midiButtonArea.reduced(8));

    auto midiArea = bounds.removeFromTop(midiHeight);
    midiKeyboardWidget->setBounds(midiArea.reduced(8));

    // Área central dividida: Canvas arriba (más grande), Tabs abajo
    auto canvasArea = bounds.removeFromTop(bounds.getHeight() * 0.65f);
    canvasViewport.setBounds(canvasArea);

    // Tabs en la parte inferior
    viewTabs.setBounds(bounds);
}

DraggableWidget* MainComponent::addWidget(DraggableWidget::WidgetType type, const juce::Point<int>& position)
{
    // Aplicar snap-to-grid a la posición
    auto snappedPos = snapToGridPoint(position);
    
    std::unique_ptr<DraggableWidget> widget;
    juce::String name;
    
    switch (type)
    {
        case DraggableWidget::Knob:
            name = "Knob_" + juce::String(++widgetCounter);
            widget = std::make_unique<DraggableKnob>(name);
            widget->setBounds(snappedPos.x, snappedPos.y, 120, 120);
            break;
            
        case DraggableWidget::Slider:
            name = "Slider_" + juce::String(++widgetCounter);
            widget = std::make_unique<DraggableSlider>(name);
            widget->setBounds(snappedPos.x, snappedPos.y, 80, 200);
            break;
            
        case DraggableWidget::Button:
            name = "Button_" + juce::String(++widgetCounter);
            widget = std::make_unique<DraggableButton>(name);
            widget->setBounds(snappedPos.x, snappedPos.y, 120, 50);
            break;
            
        case DraggableWidget::Label:
            name = "Label_" + juce::String(++widgetCounter);
            widget = std::make_unique<DraggableLabel>(name, "Texto");
            widget->setBounds(snappedPos.x, snappedPos.y, 150, 40);
            break;
            
        case DraggableWidget::Waveform:
            name = "Waveform_" + juce::String(++widgetCounter);
            widget = std::make_unique<DraggableWaveform>(name);
            widget->setBounds(snappedPos.x, snappedPos.y, 250, 100);
            break;
            
        case DraggableWidget::Meter:
            name = "Meter_" + juce::String(++widgetCounter);
            widget = std::make_unique<DraggableMeter>(name);
            widget->setBounds(snappedPos.x, snappedPos.y, 60, 200);
            break;
            
        case DraggableWidget::XYPad:
            name = "XYPad_" + juce::String(++widgetCounter);
            widget = std::make_unique<DraggableXYPad>(name);
            widget->setBounds(snappedPos.x, snappedPos.y, 150, 150);
            break;
            
        case DraggableWidget::Joystick:
            name = "Joystick_" + juce::String(++widgetCounter);
            widget = std::make_unique<DraggableJoystick>(name);
            widget->setBounds(snappedPos.x, snappedPos.y, 150, 150);
            break;
            
        case DraggableWidget::PitchWheel:
            name = "PitchWheel_" + juce::String(++widgetCounter);
            widget = std::make_unique<DraggablePitchWheel>(name);
            widget->setBounds(snappedPos.x, snappedPos.y, 60, 200);
            break;
            
        case DraggableWidget::IndexedSlider:
        {
            name = "IndexedSlider_" + juce::String(++widgetCounter);
            juce::StringArray defaultLabels = {"Option 1", "Option 2", "Option 3", "Option 4"};
            widget = std::make_unique<DraggableIndexedSlider>(name, defaultLabels);
            widget->setBounds(snappedPos.x, snappedPos.y, 250, 60);
            break;
        }
        
        case DraggableWidget::BasicMeter:
            name = "BasicMeter_" + juce::String(++widgetCounter);
            widget = std::make_unique<DraggableBasicMeter>(name);
            widget->setBounds(snappedPos.x, snappedPos.y, 80, 200);
            break;
            
        case DraggableWidget::VUMeter:
            name = "VUMeter_" + juce::String(++widgetCounter);
            widget = std::make_unique<DraggableVUMeter>(name);
            widget->setBounds(snappedPos.x, snappedPos.y, 200, 80);
            break;
            
        case DraggableWidget::LedMeter:
            name = "LedMeter_" + juce::String(++widgetCounter);
            widget = std::make_unique<DraggableLedMeter>(name);
            widget->setBounds(snappedPos.x, snappedPos.y, 60, 200);
            break;
            
        case DraggableWidget::AudioScope:
            name = "AudioScope_" + juce::String(++widgetCounter);
            widget = std::make_unique<DraggableAudioScope>(name);
            widget->setBounds(snappedPos.x, snappedPos.y, 250, 150);
            break;
            
        case DraggableWidget::VerticalFader:
            name = "VerticalFader_" + juce::String(++widgetCounter);
            widget = std::make_unique<DraggableVerticalFader>(name);
            widget->setBounds(snappedPos.x, snappedPos.y, 60, 200);
            break;
            
        case DraggableWidget::ImageButton:
            name = "ImageButton_" + juce::String(++widgetCounter);
            widget = std::make_unique<DraggableImageButton>(name);
            widget->setBounds(snappedPos.x, snappedPos.y, 100, 100);
            break;
            
        case DraggableWidget::ToggleButton:
            name = "ToggleButton_" + juce::String(++widgetCounter);
            widget = std::make_unique<DraggableToggleButton>(name);
            widget->setBounds(snappedPos.x, snappedPos.y, 80, 40);
            break;
            
        case DraggableWidget::TransportPlay:
            name = "Play_" + juce::String(++widgetCounter);
            widget = std::make_unique<DraggableTransportButton>(name, DraggableTransportButton::Play);
            widget->setBounds(snappedPos.x, snappedPos.y, 60, 60);
            break;
            
        case DraggableWidget::TransportPause:
            name = "Pause_" + juce::String(++widgetCounter);
            widget = std::make_unique<DraggableTransportButton>(name, DraggableTransportButton::Pause);
            widget->setBounds(snappedPos.x, snappedPos.y, 60, 60);
            break;
            
        case DraggableWidget::TransportStop:
            name = "Stop_" + juce::String(++widgetCounter);
            widget = std::make_unique<DraggableTransportButton>(name, DraggableTransportButton::Stop);
            widget->setBounds(snappedPos.x, snappedPos.y, 60, 60);
            break;
            
        case DraggableWidget::TransportRecord:
            name = "Record_" + juce::String(++widgetCounter);
            widget = std::make_unique<DraggableTransportButton>(name, DraggableTransportButton::Record);
            widget->setBounds(snappedPos.x, snappedPos.y, 60, 60);
            break;
            
        case DraggableWidget::TransportRewind:
            name = "Rewind_" + juce::String(++widgetCounter);
            widget = std::make_unique<DraggableTransportButton>(name, DraggableTransportButton::Rewind);
            widget->setBounds(snappedPos.x, snappedPos.y, 60, 60);
            break;
            
        case DraggableWidget::TransportFastForward:
            name = "FastForward_" + juce::String(++widgetCounter);
            widget = std::make_unique<DraggableTransportButton>(name, DraggableTransportButton::FastForward);
            widget->setBounds(snappedPos.x, snappedPos.y, 60, 60);
            break;
            
        case DraggableWidget::TransportLoop:
            name = "Loop_" + juce::String(++widgetCounter);
            widget = std::make_unique<DraggableTransportButton>(name, DraggableTransportButton::Loop);
            widget->setBounds(snappedPos.x, snappedPos.y, 60, 60);
            break;
            
        case DraggableWidget::TransportMetronome:
            name = "Metronome_" + juce::String(++widgetCounter);
            widget = std::make_unique<DraggableTransportButton>(name, DraggableTransportButton::Metronome);
            widget->setBounds(snappedPos.x, snappedPos.y, 60, 60);
            break;
            
        case DraggableWidget::TransportTap:
            name = "Tap_" + juce::String(++widgetCounter);
            widget = std::make_unique<DraggableTransportButton>(name, DraggableTransportButton::Tap);
            widget->setBounds(snappedPos.x, snappedPos.y, 60, 60);
            break;
            
        case DraggableWidget::TransportSync:
            name = "Sync_" + juce::String(++widgetCounter);
            widget = std::make_unique<DraggableTransportButton>(name, DraggableTransportButton::Sync);
            widget->setBounds(snappedPos.x, snappedPos.y, 60, 60);
            break;
            
        case DraggableWidget::TransportReturnToZero:
            name = "ReturnToZero_" + juce::String(++widgetCounter);
            widget = std::make_unique<DraggableTransportButton>(name, DraggableTransportButton::ReturnToZero);
            widget->setBounds(snappedPos.x, snappedPos.y, 60, 60);
            break;
            
        case DraggableWidget::TransportMarkerNext:
            name = "MarkerNext_" + juce::String(++widgetCounter);
            widget = std::make_unique<DraggableTransportButton>(name, DraggableTransportButton::MarkerNext);
            widget->setBounds(snappedPos.x, snappedPos.y, 60, 60);
            break;
            
        case DraggableWidget::TransportMarkerPrevious:
            name = "MarkerPrev_" + juce::String(++widgetCounter);
            widget = std::make_unique<DraggableTransportButton>(name, DraggableTransportButton::MarkerPrevious);
            widget->setBounds(snappedPos.x, snappedPos.y, 60, 60);
            break;
            
        case DraggableWidget::TransportNudgeForward:
            name = "NudgeFwd_" + juce::String(++widgetCounter);
            widget = std::make_unique<DraggableTransportButton>(name, DraggableTransportButton::NudgeForward);
            widget->setBounds(snappedPos.x, snappedPos.y, 60, 60);
            break;
            
        case DraggableWidget::TransportNudgeBackward:
            name = "NudgeBack_" + juce::String(++widgetCounter);
            widget = std::make_unique<DraggableTransportButton>(name, DraggableTransportButton::NudgeBackward);
            widget->setBounds(snappedPos.x, snappedPos.y, 60, 60);
            break;
            
        case DraggableWidget::TransportDrop:
            name = "Drop_" + juce::String(++widgetCounter);
            widget = std::make_unique<DraggableTransportButton>(name, DraggableTransportButton::Drop);
            widget->setBounds(snappedPos.x, snappedPos.y, 60, 60);
            break;
            
        case DraggableWidget::TransportReplace:
            name = "Replace_" + juce::String(++widgetCounter);
            widget = std::make_unique<DraggableTransportButton>(name, DraggableTransportButton::Replace);
            widget->setBounds(snappedPos.x, snappedPos.y, 60, 60);
            break;
            
        case DraggableWidget::TransportAutomationMode:
            name = "Automation_" + juce::String(++widgetCounter);
            widget = std::make_unique<DraggableTransportButton>(name, DraggableTransportButton::AutomationMode);
            widget->setBounds(snappedPos.x, snappedPos.y, 60, 60);
            break;
            
        case DraggableWidget::TransportJogWheel:
            name = "JogWheel_" + juce::String(++widgetCounter);
            widget = std::make_unique<DraggableTransportButton>(name, DraggableTransportButton::JogWheel);
            widget->setBounds(snappedPos.x, snappedPos.y, 60, 60);
            break;
            
        case DraggableWidget::TransportScrub:
            name = "Scrub_" + juce::String(++widgetCounter);
            widget = std::make_unique<DraggableTransportButton>(name, DraggableTransportButton::Scrub);
            widget->setBounds(snappedPos.x, snappedPos.y, 60, 60);
            break;
            
        case DraggableWidget::TransportPreRoll:
            name = "PreRoll_" + juce::String(++widgetCounter);
            widget = std::make_unique<DraggableTransportButton>(name, DraggableTransportButton::PreRoll);
            widget->setBounds(snappedPos.x, snappedPos.y, 60, 60);
            break;
            
        case DraggableWidget::TransportPostRoll:
            name = "PostRoll_" + juce::String(++widgetCounter);
            widget = std::make_unique<DraggableTransportButton>(name, DraggableTransportButton::PostRoll);
            widget->setBounds(snappedPos.x, snappedPos.y, 60, 60);
            break;
            
        case DraggableWidget::MidiKeyboard:
            name = "MidiKeyboard_" + juce::String(++widgetCounter);
            widget = std::make_unique<DraggableMidiKeyboard>(name, keyboardState);
            widget->setBounds(snappedPos.x, snappedPos.y, 400, 100);
            break;

        default:
            return nullptr;
    }
    
    // Configurar callback de eliminación
    widget->onDeleteRequested = [this](DraggableWidget* w) {
        removeWidget(w);
    };
    
    // Callback de selección al hacer clic
    auto* widgetPtr = widget.get();
    widget->addMouseListener(this, false);
    
    // Callback para clic derecho - configurar
    widget->onConfigRequested = [this](DraggableWidget* w) {
        selectWidget(w);
        debugConsole.log("Configurando widget: " + w->getWidgetName());
    };
    
    // Callback para asignar MIDI
    widget->onMidiLearnRequested = [this](DraggableWidget* w) {
        selectWidget(w);
        showMidiLearnDialog(w);
    };
    
    // Añadir widget al contenedor del canvas (no a MainComponent directamente)
    canvasContainer.addAndMakeVisible(widget.get());
    widgets.add(widget.release());
    
    // Seleccionar automáticamente el nuevo widget
    selectWidget(widgetPtr);
    
    debugConsole.log("Widget creado: " + name);
    repaint();
    
    return widgetPtr;
}

void MainComponent::removeWidget(DraggableWidget* widget)
{
    if (widget == selectedWidget)
    {
        propertyInspector.clearSelection();
        selectedWidget = nullptr;
    }
    
    debugConsole.log("Widget eliminado: " + widget->getWidgetName());
    widgets.removeObject(widget);
    repaint();
}

void MainComponent::selectWidget(DraggableWidget* widget)
{
    selectedWidget = widget;
    propertyInspector.setSelectedWidget(widget);
    debugConsole.log("Widget seleccionado: " + widget->getWidgetName());
}

void MainComponent::saveLayout()
{
    juce::ValueTree layout("Layout");
    
    for (auto* widget : widgets)
    {
        layout.appendChild(widget->toValueTree(), nullptr);
    }
    
    // Crear chooser con lambda callback
    auto chooser = std::make_shared<juce::FileChooser>(
        "Guardar Layout",
        juce::File::getSpecialLocation(juce::File::userDocumentsDirectory),
        "*.xml"
    );
    
    auto flags = juce::FileBrowserComponent::saveMode | juce::FileBrowserComponent::canSelectFiles;
    
    chooser->launchAsync(flags, [layout](const juce::FileChooser& fc)
    {
        auto file = fc.getResult();
        if (file != juce::File())
        {
            auto xml = layout.createXml();
            if (xml != nullptr && xml->writeTo(file))
            {
                juce::NativeMessageBox::showMessageBoxAsync(
                    juce::MessageBoxIconType::InfoIcon,
                    "Guardado",
                    "Layout guardado exitosamente"
                );
            }
        }
    });
}

void MainComponent::loadLayout()
{
    auto chooser = std::make_shared<juce::FileChooser>(
        "Cargar Layout",
        juce::File::getSpecialLocation(juce::File::userDocumentsDirectory),
        "*.xml"
    );
    
    auto flags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;
    
    chooser->launchAsync(flags, [this](const juce::FileChooser& fc)
    {
        auto file = fc.getResult();
        if (file != juce::File())
        {
            auto xml = juce::parseXML(file);
            if (xml != nullptr)
            {
                auto layout = juce::ValueTree::fromXml(*xml);
                if (layout.isValid())
                {
                    clearAllWidgets();
                    
                    for (int i = 0; i < layout.getNumChildren(); ++i)
                    {
                        auto widgetTree = layout.getChild(i);
                        auto widget = DraggableWidget::fromValueTree(widgetTree);
                        
                        if (widget != nullptr)
                        {
                            widget->onDeleteRequested = [this](DraggableWidget* w) {
                                removeWidget(w);
                            };
                            
                            widget->onConfigRequested = [this](DraggableWidget* w) {
                                selectWidget(w);
                                debugConsole.log("Configurando widget: " + w->getWidgetName());
                            };
                            
                            widget->onMidiLearnRequested = [this](DraggableWidget* w) {
                                selectWidget(w);
                                showMidiLearnDialog(w);
                            };
                            
                            addAndMakeVisible(widget.get());
                            widgets.add(widget.release());
                        }
                    }
                    
                    repaint();
                    
                    juce::NativeMessageBox::showMessageBoxAsync(
                        juce::MessageBoxIconType::InfoIcon,
                        "Cargado",
                        "Layout cargado exitosamente"
                    );
                }
            }
        }
    });
}

void MainComponent::clearAllWidgets()
{
    widgets.clear();
    widgetCounter = 0;
    repaint();
}

juce::Point<int> MainComponent::snapToGridPoint(const juce::Point<int>& point) const
{
    if (!snapToGrid)
        return point;
    
    int snappedX = (point.x / gridSize) * gridSize;
    int snappedY = (point.y / gridSize) * gridSize;
    
    return juce::Point<int>(snappedX, snappedY);
}

void MainComponent::showAudioSettings()
{
    // Crear selector de audio usando el deviceManager heredado de AudioAppComponent
    auto* audioSetup = new juce::AudioDeviceSelectorComponent(
        deviceManager,
        0, 256,  // Min/max inputs
        0, 256,  // Min/max outputs
        true,    // Show MIDI input
        true,    // Show MIDI output
        true,    // Show channels as stereo pairs
        false    // Hide advanced options
    );
    
    audioSetup->setSize(500, 450);
    
    // Crear ventana para el selector
    juce::DialogWindow::LaunchOptions options;
    options.content.setOwned(audioSetup);
    options.dialogTitle = "Configuracion de Audio y MIDI";
    options.dialogBackgroundColour = juce::Colour(0xff2d3748);
    options.escapeKeyTriggersCloseButton = true;
    options.useNativeTitleBar = true;
    options.resizable = false;
    
    auto* window = options.launchAsync();
    
    if (window != nullptr)
    {
        window->centreWithSize(550, 500);
    }
    
    debugConsole.log("Configuracion de Audio/MIDI abierta");
}

void MainComponent::showMidiLearnDialog(DraggableWidget* widget)
{
    // Crear el diálogo en el heap para que persista durante el modal
    auto* w = new juce::AlertWindow(
        "Asignar MIDI CC",
        "Configura el control MIDI para " + widget->getWidgetName(),
        juce::MessageBoxIconType::InfoIcon
    );
    
    w->addTextEditor("cc", "1", "Numero de CC (0-127):");
    w->addTextEditor("channel", "1", "Canal MIDI (1-16):");
    
    w->addButton("Asignar", 1, juce::KeyPress(juce::KeyPress::returnKey));
    w->addButton("Aprender", 2);
    w->addButton("Cancelar", 0, juce::KeyPress(juce::KeyPress::escapeKey));
    
    w->enterModalState(true, juce::ModalCallbackFunction::create([this, widget, w](int result) {
        if (result == 1)  // Asignar
        {
            auto ccText = w->getTextEditorContents("cc");
            auto channelText = w->getTextEditorContents("channel");
            
            int cc = ccText.getIntValue();
            int channel = channelText.getIntValue();
            
            // Validar rangos
            if (cc >= 0 && cc <= 127 && channel >= 1 && channel <= 16)
            {
                juce::String message = "MIDI CC " + juce::String(cc) + 
                                      " (Canal " + juce::String(channel) + 
                                      ") asignado a " + widget->getWidgetName();
                
                debugConsole.log(message);
                
                juce::NativeMessageBox::showMessageBoxAsync(
                    juce::MessageBoxIconType::InfoIcon,
                    "MIDI Asignado",
                    message
                );
            }
            else
            {
                debugConsole.log("ERROR: Valores MIDI fuera de rango");
                juce::NativeMessageBox::showMessageBoxAsync(
                    juce::MessageBoxIconType::WarningIcon,
                    "Error",
                    "CC debe estar entre 0-127 y Canal entre 1-16"
                );
            }
        }
        else if (result == 2)  // Aprender
        {
            // Activar modo MIDI learn
            startMidiLearn(widget);
        }
        
        // Eliminar el diálogo después de usarlo
        delete w;
    }), true);
}

// ============================================================================
// Sistema Completo de Proyectos (.dawproj)
// ============================================================================

void MainComponent::saveCompleteProject()
{
    // Si no hay archivo actual, mostrar diálogo "Guardar Como"
    if (!currentProjectFile.existsAsFile())
    {
        auto chooser = std::make_shared<juce::FileChooser>(
            "Guardar Proyecto Completo",
            exportPaths.projectsPath.existsAsFile() ? exportPaths.projectsPath : juce::File::getSpecialLocation(juce::File::userDocumentsDirectory),
            "*.dawproj"
        );
        
        auto flags = juce::FileBrowserComponent::saveMode | juce::FileBrowserComponent::canSelectFiles;
        
        chooser->launchAsync(flags, [this, chooser](const juce::FileChooser& fc)
        {
            auto file = fc.getResult();
            if (file != juce::File())
            {
                if (!file.hasFileExtension(".dawproj"))
                    file = file.withFileExtension(".dawproj");
                
                currentProjectFile = file;
                projectProperties.lastModifiedDate = juce::Time::getCurrentTime();
                
                auto projectTree = createProjectTree();
                auto xml = projectTree.createXml();
                
                if (xml != nullptr && xml->writeTo(file))
                {
                    addToRecentProjects(file);
                    debugConsole.log("Proyecto guardado: " + file.getFullPathName());
                    juce::NativeMessageBox::showMessageBoxAsync(
                        juce::MessageBoxIconType::InfoIcon,
                        "Proyecto Guardado",
                        "Proyecto guardado exitosamente en:\n" + file.getFullPathName()
                    );
                }
            }
        });
    }
    else
    {
        // Guardar directamente en el archivo actual
        projectProperties.lastModifiedDate = juce::Time::getCurrentTime();
        auto projectTree = createProjectTree();
        auto xml = projectTree.createXml();
        
        if (xml != nullptr && xml->writeTo(currentProjectFile))
        {
            debugConsole.log("Proyecto guardado: " + currentProjectFile.getFullPathName());
        }
    }
}

void MainComponent::loadCompleteProject()
{
    auto chooser = std::make_shared<juce::FileChooser>(
        "Cargar Proyecto Completo",
        exportPaths.projectsPath.existsAsFile() ? exportPaths.projectsPath : juce::File::getSpecialLocation(juce::File::userDocumentsDirectory),
        "*.dawproj"
    );
    
    auto flags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;
    
    chooser->launchAsync(flags, [this, chooser](const juce::FileChooser& fc)
    {
        auto file = fc.getResult();
        if (file != juce::File())
        {
            auto xml = juce::parseXML(file);
            if (xml != nullptr)
            {
                auto projectTree = juce::ValueTree::fromXml(*xml);
                if (projectTree.isValid() && projectTree.hasType("DAWProject"))
                {
                    currentProjectFile = file;
                    loadFromProjectTree(projectTree);
                    addToRecentProjects(file);
                    debugConsole.log("Proyecto cargado: " + file.getFullPathName());
                }
            }
        }
    });
}

void MainComponent::autoSaveProject()
{
    if (currentProjectFile.existsAsFile())
    {
        auto projectTree = createProjectTree();
        auto xml = projectTree.createXml();
        if (xml != nullptr)
        {
            auto backupFile = currentProjectFile.getSiblingFile(
                currentProjectFile.getFileNameWithoutExtension() + "_backup.dawproj"
            );
            xml->writeTo(backupFile);
            xml->writeTo(currentProjectFile);
            debugConsole.log("Auto-guardado completado");
        }
    }
}

juce::ValueTree MainComponent::createProjectTree()
{
    juce::ValueTree project("DAWProject");
    project.setProperty("name", projectProperties.name, nullptr);
    project.setProperty("version", projectProperties.version, nullptr);
    project.setProperty("author", projectProperties.author, nullptr);
    project.setProperty("description", projectProperties.description, nullptr);
    project.setProperty("creationDate", projectProperties.creationDate.toISO8601(true), nullptr);
    project.setProperty("lastModified", projectProperties.lastModifiedDate.toISO8601(true), nullptr);
    
    // Guardar estado de módulos
    juce::ValueTree moduleStateTree("ModuleState");
    moduleStateTree.setProperty("audioEnabled", moduleState.audioEnabled, nullptr);
    moduleStateTree.setProperty("midiEnabled", moduleState.midiEnabled, nullptr);
    moduleStateTree.setProperty("dspEnabled", moduleState.dspEnabled, nullptr);
    moduleStateTree.setProperty("guiEnabled", moduleState.guiEnabled, nullptr);
    moduleStateTree.setProperty("utilitiesEnabled", moduleState.utilitiesEnabled, nullptr);
    project.appendChild(moduleStateTree, nullptr);
    
    // Configuración de plataforma
    auto platformConfig = projectManager.getPlatformConfig().getConfiguration();
    juce::ValueTree configTree("Configuration");
    configTree.setProperty("windows", platformConfig.windows, nullptr);
    configTree.setProperty("cppStandard", platformConfig.cppStandard, nullptr);
    configTree.setProperty("enableVST3", platformConfig.enableVST3, nullptr);
    project.appendChild(configTree, nullptr);
    
    // Módulos JUCE
    juce::ValueTree modulesTree("Modules");
    auto selectedModules = projectManager.getModuleManager().getSelectedModules();
    for (const auto& moduleName : selectedModules)
    {
        juce::ValueTree moduleNode("Module");
        moduleNode.setProperty("name", moduleName, nullptr);
        modulesTree.appendChild(moduleNode, nullptr);
    }
    project.appendChild(modulesTree, nullptr);
    
    // Configuración de dispositivos MIDI
    juce::ValueTree midiTree("MIDIDevices");
    
    // Guardar dispositivos MIDI de entrada habilitados
    juce::ValueTree midiInputsTree("MIDIInputs");
    auto availableInputs = juce::MidiInput::getAvailableDevices();
    for (const auto& device : availableInputs)
    {
        if (deviceManager.isMidiInputDeviceEnabled(device.identifier))
        {
            juce::ValueTree inputNode("Input");
            inputNode.setProperty("identifier", device.identifier, nullptr);
            inputNode.setProperty("name", device.name, nullptr);
            midiInputsTree.appendChild(inputNode, nullptr);
        }
    }
    midiTree.appendChild(midiInputsTree, nullptr);
    
    // Guardar el nombre del dispositivo MIDI de salida por defecto si existe
    // Nota: JUCE no tiene una API directa para obtener el dispositivo por defecto,
    // así que guardamos todos los dispositivos de salida disponibles
    juce::ValueTree midiOutputsTree("MIDIOutputs");
    auto availableOutputs = juce::MidiOutput::getAvailableDevices();
    for (const auto& device : availableOutputs)
    {
        juce::ValueTree outputNode("Output");
        outputNode.setProperty("identifier", device.identifier, nullptr);
        outputNode.setProperty("name", device.name, nullptr);
        midiOutputsTree.appendChild(outputNode, nullptr);
    }
    midiTree.appendChild(midiOutputsTree, nullptr);
    
    project.appendChild(midiTree, nullptr);
    
    // Layout de widgets
    juce::ValueTree layoutTree("Layout");
    for (auto* widget : widgets)
        layoutTree.appendChild(widget->toValueTree(), nullptr);
    project.appendChild(layoutTree, nullptr);
    
    return project;
}

void MainComponent::loadFromProjectTree(const juce::ValueTree& projectTree)
{
    clearAllWidgets();
    
    // Restaurar propiedades del proyecto
    projectProperties.name = projectTree.getProperty("name", "Sin título").toString();
    projectProperties.version = projectTree.getProperty("version", "1.0.0").toString();
    projectProperties.author = projectTree.getProperty("author", "").toString();
    projectProperties.description = projectTree.getProperty("description", "").toString();
    
    juce::String creationDateStr = projectTree.getProperty("creationDate", "").toString();
    juce::String lastModifiedStr = projectTree.getProperty("lastModified", "").toString();
    
    if (creationDateStr.isNotEmpty())
        projectProperties.creationDate = juce::Time::fromISO8601(creationDateStr);
    if (lastModifiedStr.isNotEmpty())
        projectProperties.lastModifiedDate = juce::Time::fromISO8601(lastModifiedStr);
    
    // Restaurar estado de módulos
    auto moduleStateTree = projectTree.getChildWithName("ModuleState");
    if (moduleStateTree.isValid())
    {
        moduleState.audioEnabled = moduleStateTree.getProperty("audioEnabled", true);
        moduleState.midiEnabled = moduleStateTree.getProperty("midiEnabled", true);
        moduleState.dspEnabled = moduleStateTree.getProperty("dspEnabled", true);
        moduleState.guiEnabled = moduleStateTree.getProperty("guiEnabled", true);
        moduleState.utilitiesEnabled = moduleStateTree.getProperty("utilitiesEnabled", true);
    }
    
    // Restaurar configuración de dispositivos MIDI
    auto midiTree = projectTree.getChildWithName("MIDIDevices");
    if (midiTree.isValid())
    {
        // Deshabilitar todos los dispositivos MIDI de entrada primero
        auto availableInputs = juce::MidiInput::getAvailableDevices();
        for (const auto& device : availableInputs)
        {
            deviceManager.setMidiInputDeviceEnabled(device.identifier, false);
        }
        
        // Habilitar los dispositivos MIDI de entrada guardados
        auto midiInputsTree = midiTree.getChildWithName("MIDIInputs");
        if (midiInputsTree.isValid())
        {
            for (int i = 0; i < midiInputsTree.getNumChildren(); ++i)
            {
                auto inputNode = midiInputsTree.getChild(i);
                juce::String identifier = inputNode.getProperty("identifier").toString();
                juce::String name = inputNode.getProperty("name").toString();
                
                // Verificar si el dispositivo aún está disponible
                bool deviceFound = false;
                for (const auto& availDevice : availableInputs)
                {
                    if (availDevice.identifier == identifier)
                    {
                        deviceManager.setMidiInputDeviceEnabled(identifier, true);
                        debugConsole.log("Dispositivo MIDI de entrada restaurado: " + name);
                        deviceFound = true;
                        break;
                    }
                }
                
                if (!deviceFound)
                {
                    debugConsole.log("ADVERTENCIA: Dispositivo MIDI no disponible: " + name);
                }
            }
        }
        
        // Log de dispositivos MIDI de salida guardados (informativo)
        auto midiOutputsTree = midiTree.getChildWithName("MIDIOutputs");
        if (midiOutputsTree.isValid())
        {
            debugConsole.log("Dispositivos MIDI de salida guardados en el proyecto:");
            for (int i = 0; i < midiOutputsTree.getNumChildren(); ++i)
            {
                auto outputNode = midiOutputsTree.getChild(i);
                juce::String name = outputNode.getProperty("name").toString();
                debugConsole.log("  - " + name);
            }
        }
    }
    
    // Restaurar widgets
    auto layoutTree = projectTree.getChildWithName("Layout");
    if (layoutTree.isValid())
    {
        for (int i = 0; i < layoutTree.getNumChildren(); ++i)
        {
            auto widgetTree = layoutTree.getChild(i);
            auto widget = DraggableWidget::fromValueTree(widgetTree);
            if (widget != nullptr)
            {
                widget->onDeleteRequested = [this](DraggableWidget* w) { removeWidget(w); };
                widget->onConfigRequested = [this](DraggableWidget* w) { selectWidget(w); };
                widget->onMidiLearnRequested = [this](DraggableWidget* w) { selectWidget(w); showMidiLearnDialog(w); };
                addAndMakeVisible(widget.get());
                widgets.add(widget.release());
            }
        }
    }
    repaint();
}

// ============================================================================
// MIDI Learn
// ============================================================================

void MainComponent::handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message)
{
    // Enviar el mensaje al MidiKeyboardState para actualizar el estado del teclado visual
    keyboardState.processNextMidiEvent(message);

    // Solo procesar mensajes de Control Change
    if (!message.isController())
        return;
    
    int cc = message.getControllerNumber();
    int channel = message.getChannel();
    int value = message.getControllerValue();
    
    // Si el modo MIDI learn está activo, capturar y asignar
    if (midiLearnActive && midiLearnWidget != nullptr)
    {
        juce::MessageManager::callAsync([this, cc, channel, value]()
        {
            if (midiLearnWidget != nullptr)
            {
                // Guardar la asignación en el widget
                midiLearnWidget->setMidiCC(cc, channel);
                
                juce::String widgetName = midiLearnWidget->getWidgetName();
                
                juce::String message = "MIDI CC " + juce::String(cc) + 
                                      " (Canal " + juce::String(channel) + 
                                      ") aprendido y asignado a " + widgetName +
                                      "\nValor recibido: " + juce::String(value);
                
                debugConsole.log(message);
                
                juce::NativeMessageBox::showMessageBoxAsync(
                    juce::MessageBoxIconType::InfoIcon,
                    "MIDI Learn Exitoso",
                    message
                );
            }
            
            // Desactivar modo learn después de capturar
            stopMidiLearn();
        });
        return;
    }
    
    // Procesar mensajes MIDI normales: actualizar widgets asignados
    juce::MessageManager::callAsync([this, cc, channel, value]()
    {
        for (auto* widget : widgets)
        {
            if (widget->hasMidiCC() && 
                widget->getMidiCC() == cc && 
                widget->getMidiChannel() == channel)
            {
                widget->updateFromMidiValue(value);
            }
        }
    });
}

void MainComponent::startMidiLearn(DraggableWidget* widget)
{
    midiLearnActive = true;
    midiLearnWidget = widget;
    
    juce::String message = "Modo MIDI Learn activado para " + widget->getWidgetName() +
                          "\n\nMueve un control MIDI (knob, fader, etc.) para asignarlo..." +
                          "\n\nEl primer CC recibido será asignado automáticamente.";
    
    debugConsole.log("MIDI Learn activado: esperando mensaje CC...");
    
    juce::NativeMessageBox::showMessageBoxAsync(
        juce::MessageBoxIconType::InfoIcon,
        "MIDI Learn Activo",
        message
    );
}

void MainComponent::stopMidiLearn()
{
    midiLearnActive = false;
    midiLearnWidget = nullptr;
    debugConsole.log("MIDI Learn desactivado");
}

// ============================================================================
// Exportación MIDI y Audio
// ============================================================================

void MainComponent::exportToMidi()
{
    auto chooser = std::make_shared<juce::FileChooser>(
        "Exportar MIDI",
        juce::File::getSpecialLocation(juce::File::userDocumentsDirectory),
        "*.mid"
    );
    
    auto flags = juce::FileBrowserComponent::saveMode | juce::FileBrowserComponent::canSelectFiles;
    
    chooser->launchAsync(flags, [this, chooser](const juce::FileChooser& fc)
    {
        auto file = fc.getResult();
        if (file != juce::File())
        {
            if (!file.hasFileExtension(".mid"))
                file = file.withFileExtension(".mid");
            
            // Crear secuencia MIDI
            juce::MidiFile midiFile;
            juce::MidiMessageSequence sequence;
            
            // Recorrer todos los widgets con asignaciones MIDI
            double time = 0.0;
            for (auto* widget : widgets)
            {
                if (widget->hasMidiCC())
                {
                    int cc = widget->getMidiCC();
                    int channel = widget->getMidiChannel();
                    
                    // Agregar mensaje CC con valor actual del widget
                    // Aquí agregamos un valor de ejemplo (64)
                    juce::MidiMessage msg = juce::MidiMessage::controllerEvent(channel, cc, 64);
                    msg.setTimeStamp(time);
                    sequence.addEvent(msg);
                    
                    time += 0.5; // 0.5 segundos entre eventos
                }
            }
            
            midiFile.addTrack(sequence);
            midiFile.setTicksPerQuarterNote(960);
            
            juce::FileOutputStream stream(file);
            if (stream.openedOk())
            {
                midiFile.writeTo(stream);
                debugConsole.log("MIDI exportado exitosamente: " + file.getFullPathName());
                juce::NativeMessageBox::showMessageBoxAsync(
                    juce::MessageBoxIconType::InfoIcon,
                    "Exportación MIDI",
                    "Archivo MIDI exportado exitosamente"
                );
            }
            else
            {
                debugConsole.log("ERROR: No se pudo escribir el archivo MIDI");
            }
        }
    });
}

void MainComponent::exportToAudio()
{
    auto chooser = std::make_shared<juce::FileChooser>(
        "Exportar Audio",
        juce::File::getSpecialLocation(juce::File::userDocumentsDirectory),
        "*.wav"
    );
    
    auto flags = juce::FileBrowserComponent::saveMode | juce::FileBrowserComponent::canSelectFiles;
    
    chooser->launchAsync(flags, [this, chooser](const juce::FileChooser& fc)
    {
        auto file = fc.getResult();
        if (file != juce::File())
        {
            if (!file.hasFileExtension(".wav"))
                file = file.withFileExtension(".wav");
            
            // Parámetros de audio
            const int sampleRate = 44100;
            const int numChannels = 2;
            const int duration = 10; // 10 segundos
            const int numSamples = sampleRate * duration;
            
            // Crear buffer de audio
            juce::AudioBuffer<float> buffer(numChannels, numSamples);
            buffer.clear();
            
            // Generar audio simple (tono de prueba)
            for (int channel = 0; channel < numChannels; ++channel)
            {
                auto* channelData = buffer.getWritePointer(channel);
                for (int i = 0; i < numSamples; ++i)
                {
                    // Tono de 440 Hz (A4)
                    channelData[i] = std::sin(2.0f * juce::MathConstants<float>::pi * 440.0f * i / sampleRate) * 0.5f;
                }
            }
            
            // Escribir archivo WAV
            std::unique_ptr<juce::AudioFormatWriter> writer;
            juce::WavAudioFormat wavFormat;
            
            juce::FileOutputStream* outputStream = new juce::FileOutputStream(file);
            if (outputStream->openedOk())
            {
                writer.reset(wavFormat.createWriterFor(outputStream, sampleRate, numChannels, 16, {}, 0));
                if (writer != nullptr)
                {
                    writer->writeFromAudioSampleBuffer(buffer, 0, numSamples);
                    writer.reset();
                    
                    debugConsole.log("Audio exportado exitosamente: " + file.getFullPathName());
                    juce::NativeMessageBox::showMessageBoxAsync(
                        juce::MessageBoxIconType::InfoIcon,
                        "Exportación Audio",
                        "Archivo de audio exportado exitosamente\\n10 segundos @ 44.1kHz"
                    );
                }
                else
                {
                    debugConsole.log("ERROR: No se pudo crear el escritor de audio");
                    delete outputStream;
                }
            }
            else
            {
                debugConsole.log("ERROR: No se pudo abrir el archivo para escritura");
                delete outputStream;
            }
        }
    });
}

// Métodos de MenuBar
juce::StringArray MainComponent::getMenuBarNames()
{
    return {"Proyecto", "Widgets", "Controles 2D", "Transporte", "Settings"};
}

juce::PopupMenu MainComponent::getMenuForIndex(int topLevelMenuIndex, const juce::String& menuName)
{
    juce::PopupMenu menu;
    
    if (topLevelMenuIndex == 0) // Proyecto
    {
        // Submenú Explorador
        juce::PopupMenu explorerMenu;
        explorerMenu.addItem(1001, "Nuevo Proyecto");
        explorerMenu.addItem(1002, "Abrir Proyecto");
        explorerMenu.addItem(1003, "Proyectos Recientes");
        explorerMenu.addSeparator();
        explorerMenu.addItem(1004, "Guardar Proyecto");
        explorerMenu.addItem(1005, "Guardar Como...");
        explorerMenu.addSeparator();
        explorerMenu.addItem(1006, "Propiedades del Proyecto");
        menu.addSubMenu("Explorador", explorerMenu);
        
        // Submenú Módulos
        juce::PopupMenu modulesMenu;
        modulesMenu.addItem(1010, "Gestor de Modulos");
        modulesMenu.addSeparator();
        modulesMenu.addItem(1011, "Audio");
        modulesMenu.addItem(1012, "MIDI");
        modulesMenu.addItem(1013, "DSP");
        modulesMenu.addItem(1014, "GUI");
        modulesMenu.addItem(1015, "Utilidades");
        menu.addSubMenu("Modulos", modulesMenu);
        
        // Submenú Plataformas
        juce::PopupMenu platformsMenu;
        platformsMenu.addItem(1020, "Windows");
        platformsMenu.addItem(1021, "macOS");
        platformsMenu.addItem(1022, "Linux");
        platformsMenu.addSeparator();
        platformsMenu.addItem(1023, "iOS");
        platformsMenu.addItem(1024, "Android");
        platformsMenu.addSeparator();
        platformsMenu.addItem(1025, "VST3");
        platformsMenu.addItem(1026, "AU");
        platformsMenu.addItem(1027, "AAX");
        platformsMenu.addItem(1028, "Standalone");
        menu.addSubMenu("Plataformas", platformsMenu);
        
        // Submenú Configuración
        juce::PopupMenu configMenu;
        configMenu.addItem(1030, "Configuracion General");
        configMenu.addItem(1031, "Configuracion de Audio");
        configMenu.addItem(1032, "Configuracion MIDI");
        configMenu.addSeparator();
        configMenu.addItem(1033, "Rutas de Exportacion");
        configMenu.addItem(1034, "Preferencias del Editor");
        configMenu.addSeparator();
        configMenu.addItem(1035, "Atajos de Teclado");
        menu.addSubMenu("Configuracion", configMenu);
        
        menu.addSeparator();
        menu.addItem(1040, "Exportar Plugin");
        menu.addItem(1041, "Exportar MIDI");
        menu.addItem(1042, "Exportar Audio");
        menu.addSeparator();
        menu.addItem(1043, "Limpiar Canvas");
        menu.addSeparator();
        menu.addItem(1050, "Salir");
    }
    else if (topLevelMenuIndex == 1) // Widgets
    {
        // Submenú Knobs
        juce::PopupMenu knobsMenu;
        knobsMenu.addItem(2001, "Knob Rotatorio Clásico");
        knobsMenu.addItem(2002, "Knob Vintage");
        knobsMenu.addItem(2003, "Knob Digital");
        knobsMenu.addItem(2004, "Knob Minimalista");
        menu.addSubMenu("Knobs", knobsMenu);
        
        // Submenú Sliders
        juce::PopupMenu slidersMenu;
        slidersMenu.addItem(2010, "Slider Vertical");
        slidersMenu.addItem(2011, "Slider Horizontal");
        slidersMenu.addItem(2012, "Fader Profesional");
        slidersMenu.addItem(2013, "Slider Minimalista");
        slidersMenu.addItem(2014, "Indexed Slider");
        menu.addSubMenu("Sliders", slidersMenu);
        
        // Submenú Buttons
        juce::PopupMenu buttonsMenu;
        buttonsMenu.addItem(2020, "Button Toggle");
        buttonsMenu.addItem(2021, "Button Momentary");
        buttonsMenu.addItem(2022, "Button LED");
        buttonsMenu.addItem(2023, "Button Power");
        buttonsMenu.addItem(2024, "Image Button");
        menu.addSubMenu("Buttons", buttonsMenu);
        
        // Submenú Labels
        juce::PopupMenu labelsMenu;
        labelsMenu.addItem(2030, "Label Texto");
        labelsMenu.addItem(2031, "Label Título");
        labelsMenu.addItem(2032, "Label Valor");
        menu.addSubMenu("Labels", labelsMenu);
        
        // Submenú Visualización
        juce::PopupMenu visualMenu;
        visualMenu.addItem(2040, "Waveform Display");
        visualMenu.addItem(2041, "Audio Scope");
        visualMenu.addItem(2042, "VU Meter");
        visualMenu.addItem(2043, "Level Meter (LED)");
        visualMenu.addItem(2044, "Basic Meter");
        menu.addSubMenu("Visualización", visualMenu);
        
        menu.addSeparator();
        
        // Submenú Widgets Pro (elementos avanzados)
        juce::PopupMenu proMenu;
        
        // Controles Pro
        juce::PopupMenu proControlsMenu;
        proControlsMenu.addItem(2100, "Pitch Wheel");
        proControlsMenu.addItem(2101, "Vertical Fader Pro");
        proMenu.addSubMenu("Controles Pro", proControlsMenu);
        
        // Transport Pro
        juce::PopupMenu proTransportMenu;
        proTransportMenu.addItem(2110, "Transport Play");
        proTransportMenu.addItem(2111, "Transport Pause");
        proTransportMenu.addItem(2112, "Transport Stop");
        proTransportMenu.addItem(2113, "Transport Record");
        proTransportMenu.addItem(2114, "Transport Rewind");
        proTransportMenu.addItem(2115, "Transport Fast Forward");
        proMenu.addSubMenu("Transport Buttons", proTransportMenu);
        
        menu.addSubMenu("Widgets Pro", proMenu);
        
        menu.addSeparator();
        menu.addItem(2050, "Keyboard MIDI");
    }
    else if (topLevelMenuIndex == 2) // Controles 2D
    {
        // Submenú XY Controls
        juce::PopupMenu xyMenu;
        xyMenu.addItem(3001, "XY Pad Clasico");
        xyMenu.addItem(3002, "XY Pad Grid");
        xyMenu.addItem(3003, "XY Pad Vector");
        menu.addSubMenu("XY Pads", xyMenu);
        
        // Submenú Joysticks
        juce::PopupMenu joystickMenu;
        joystickMenu.addItem(3010, "Joystick Circular");
        joystickMenu.addItem(3011, "Joystick Spring Return");
        joystickMenu.addItem(3012, "Joystick Free Float");
        menu.addSubMenu("Joysticks", joystickMenu);
        
        // Otros controles 2D
        menu.addSeparator();
        menu.addItem(3020, "Ribbon Controller");
        menu.addItem(3021, "TouchPad Multi-touch");
        menu.addItem(3022, "Morphing Pad");
    }
    else if (topLevelMenuIndex == 3) // Transporte
    {
        // Controles basicos de transporte
        juce::PopupMenu basicTransport;
        basicTransport.addItem(4001, "Play");
        basicTransport.addItem(4002, "Stop");
        basicTransport.addItem(4003, "Record");
        basicTransport.addItem(4004, "Pause");
        menu.addSubMenu("Controles Basicos", basicTransport);
        
        // Controles de navegacion
        juce::PopupMenu navTransport;
        navTransport.addItem(4010, "Rewind");
        navTransport.addItem(4011, "Forward");
        navTransport.addItem(4012, "Return to Zero");
        navTransport.addItem(4013, "Marker Next");
        navTransport.addItem(4014, "Marker Previous");
        menu.addSubMenu("Navegacion", navTransport);
        
        // Controles de loop y sync
        juce::PopupMenu loopSyncMenu;
        loopSyncMenu.addItem(4020, "Loop On/Off");
        loopSyncMenu.addItem(4021, "Metronome");
        loopSyncMenu.addItem(4022, "Tempo Sync");
        loopSyncMenu.addItem(4023, "Tap Tempo");
        menu.addSubMenu("Loop & Sync", loopSyncMenu);
        
        menu.addSeparator();
        menu.addItem(4030, "Transport Display");
    }
    else if (topLevelMenuIndex == 4) // Settings
    {
        juce::PopupMenu audioMidiMenu;
        audioMidiMenu.addItem(5001, "Configuracion Audio");
        audioMidiMenu.addItem(5002, "Configuracion MIDI");
        menu.addSubMenu("Audio/MIDI", audioMidiMenu);
        
        juce::PopupMenu gridMenu;
        gridMenu.addItem(5003, "Mostrar Grid", true, showGrid);
        gridMenu.addItem(5004, "Snap to Grid", true, snapToGrid);
        gridMenu.addSeparator();
        gridMenu.addItem(5005, "Grid 10px");
        gridMenu.addItem(5006, "Grid 20px");
        gridMenu.addItem(5007, "Grid 30px");
        gridMenu.addItem(5008, "Grid 40px");
        menu.addSubMenu("Grid", gridMenu);
    }
    
    return menu;
}

void MainComponent::menuItemSelected(int menuItemID, int topLevelMenuIndex)
{
    // Proyecto - Explorador
    if (menuItemID == 1001) // Nuevo Proyecto
    {
        clearAllWidgets();
        projectProperties.name = "Sin título";
        projectProperties.creationDate = juce::Time::getCurrentTime();
        projectProperties.lastModifiedDate = juce::Time::getCurrentTime();
        debugConsole.log("Nuevo proyecto creado");
    }
    else if (menuItemID == 1002) // Abrir Proyecto
    {
        loadCompleteProject();
    }
    else if (menuItemID == 1003) // Proyectos Recientes
    {
        showRecentProjects();
    }
    else if (menuItemID == 1004) // Guardar Proyecto
    {
        saveCompleteProject();
    }
    else if (menuItemID == 1005) // Guardar Como
    {
        currentProjectFile = juce::File(); // Resetear para forzar "Guardar Como"
        saveCompleteProject();
    }
    else if (menuItemID == 1006) // Propiedades del Proyecto
    {
        showProjectProperties();
    }
    
    // Proyecto - Módulos
    else if (menuItemID == 1010) // Gestor de Módulos
    {
        showModuleManager();
    }
    else if (menuItemID >= 1011 && menuItemID <= 1015) // Audio, MIDI, DSP, GUI, Utilidades
    {
        juce::String modules[] = {"Audio", "MIDI", "DSP", "GUI", "Utilidades"};
        showModuleSettings(modules[menuItemID - 1011]);
    }
    
    // Proyecto - Plataformas
    else if (menuItemID >= 1020 && menuItemID <= 1028) 
    {
        juce::String platforms[] = {"Windows", "macOS", "Linux", "iOS", "Android", "VST3", "AU", "AAX", "Standalone"};
        int index = (menuItemID == 1023) ? 3 : (menuItemID == 1024) ? 4 : (menuItemID - 1020);
        if (menuItemID >= 1025) index = menuItemID - 1025 + 5;
        showPlatformExportDialog(platforms[index]);
    }
    
    // Proyecto - Configuración
    else if (menuItemID == 1030) showAudioSettings(); // Configuración General
    else if (menuItemID == 1031) showAudioSettings(); // Configuración de Audio
    else if (menuItemID == 1032) showMidiSettings(); // Configuración MIDI
    else if (menuItemID == 1033) showExportPathsSettings(); // Rutas de Exportación
    else if (menuItemID == 1034) showEditorPreferences(); // Preferencias del Editor
    else if (menuItemID == 1035) showKeyboardShortcuts(); // Atajos de Teclado
    
    // Proyecto - Exportación y otros
    else if (menuItemID == 1040) exportButton.triggerClick(); // Exportar Plugin
    else if (menuItemID == 1041) exportToMidi();
    else if (menuItemID == 1042) exportToAudio();
    else if (menuItemID == 1043) clearAllWidgets();
    else if (menuItemID == 1050) juce::JUCEApplication::getInstance()->systemRequestedQuit();
    
    // Widgets - Knobs (con estilos diferentes)
    else if (menuItemID >= 2001 && menuItemID <= 2004)
    {
        auto* widget = addWidget(DraggableWidget::Knob, {200, 100});
        if (auto* knob = dynamic_cast<DraggableKnob*>(widget))
        {
            knob->setKnobStyle(menuItemID - 2001, true);
            juce::String styles[] = {"Clasico", "Vintage", "Digital", "Minimalista"};
            debugConsole.log("Knob " + styles[menuItemID - 2001] + " anadido");
        }
    }
    // Widgets - Sliders (con estilos diferentes)
    else if (menuItemID >= 2010 && menuItemID <= 2013)
    {
        auto* widget = addWidget(DraggableWidget::Slider, {300, 100});
        if (auto* slider = dynamic_cast<DraggableSlider*>(widget))
        {
            slider->setSliderStyle(menuItemID - 2010);
            juce::String styles[] = {"Vertical", "Horizontal", "Fader Profesional", "Minimalista"};
            debugConsole.log("Slider " + styles[menuItemID - 2010] + " anadido");
        }
    }
    else if (menuItemID == 2014) addWidget(DraggableWidget::IndexedSlider, {300, 100});
    
    // Widgets - Buttons (con estilos diferentes)
    else if (menuItemID >= 2020 && menuItemID <= 2023)
    {
        auto* widget = addWidget(DraggableWidget::Button, {400, 100});
        if (auto* button = dynamic_cast<DraggableButton*>(widget))
        {
            button->setButtonStyle(menuItemID - 2020);
            juce::String styles[] = {"Toggle", "Momentary", "LED", "Power"};
            debugConsole.log("Button " + styles[menuItemID - 2020] + " anadido");
        }
    }
    else if (menuItemID == 2024) addWidget(DraggableWidget::ImageButton, {400, 100});
    // Widgets - Labels
    else if (menuItemID >= 2030 && menuItemID <= 2032)
    {
        addWidget(DraggableWidget::Label, {500, 100});
        debugConsole.log("Label añadido: Modelo " + juce::String(menuItemID - 2029));
    }
    // Widgets - Visualización
    else if (menuItemID == 2040) addWidget(DraggableWidget::Waveform, {200, 250});
    else if (menuItemID == 2041) addWidget(DraggableWidget::AudioScope, {200, 250});
    else if (menuItemID == 2042) addWidget(DraggableWidget::VUMeter, {600, 100});
    else if (menuItemID == 2043) addWidget(DraggableWidget::LedMeter, {600, 100});
    else if (menuItemID == 2044) addWidget(DraggableWidget::BasicMeter, {600, 100});
    
    // MIDI Keyboard
    else if (menuItemID == 2050) 
    {
        addWidget(DraggableWidget::MidiKeyboard, {200, 400});
    }
    
    // Widgets Pro - Controles
    else if (menuItemID == 2100) addWidget(DraggableWidget::PitchWheel, {250, 150});
    else if (menuItemID == 2101) addWidget(DraggableWidget::VerticalFader, {300, 150});
    
    // Widgets Pro - Transport Buttons
    else if (menuItemID == 2110) addWidget(DraggableWidget::TransportPlay, {200, 100});
    else if (menuItemID == 2111) addWidget(DraggableWidget::TransportPause, {250, 100});
    else if (menuItemID == 2112) addWidget(DraggableWidget::TransportStop, {300, 100});
    else if (menuItemID == 2113) addWidget(DraggableWidget::TransportRecord, {350, 100});
    else if (menuItemID == 2114) addWidget(DraggableWidget::TransportRewind, {400, 100});
    else if (menuItemID == 2115) addWidget(DraggableWidget::TransportFastForward, {450, 100});
    
    // Controles 2D - XY Pads
    else if (menuItemID >= 3001 && menuItemID <= 3003)
    {
        addWidget(DraggableWidget::XYPad, {200, 100});
        debugConsole.log("XY Pad añadido: Variante " + juce::String(menuItemID - 3000));
    }
    // Controles 2D - Joysticks
    else if (menuItemID >= 3010 && menuItemID <= 3012)
    {
        addWidget(DraggableWidget::Joystick, {350, 100});
        debugConsole.log("Joystick anadido: Tipo " + juce::String(menuItemID - 3009));
    }
    // Controles 2D - Otros (implementar como XYPad por ahora)
    else if (menuItemID == 3020) 
    {
        addWidget(DraggableWidget::XYPad, {200, 100});
        debugConsole.log("Ribbon Controller anadido (XYPad horizontal)");
    }
    else if (menuItemID == 3021) 
    {
        addWidget(DraggableWidget::XYPad, {200, 100});
        debugConsole.log("TouchPad Multi-touch anadido (XYPad)");
    }
    else if (menuItemID == 3022) 
    {
        addWidget(DraggableWidget::XYPad, {200, 100});
        debugConsole.log("Morphing Pad anadido (XYPad)");
    }
    
    // Transporte - Controles basicos
    else if (menuItemID == 4001) addWidget(DraggableWidget::TransportPlay, {200, 100});
    else if (menuItemID == 4002) addWidget(DraggableWidget::TransportStop, {250, 100});
    else if (menuItemID == 4003) addWidget(DraggableWidget::TransportRecord, {300, 100});
    else if (menuItemID == 4004) addWidget(DraggableWidget::TransportPause, {350, 100});
    
    // Transporte - Navegacion
    else if (menuItemID == 4010) addWidget(DraggableWidget::TransportRewind, {200, 150});
    else if (menuItemID == 4011) addWidget(DraggableWidget::TransportFastForward, {250, 150});
    else if (menuItemID == 4012) addWidget(DraggableWidget::TransportReturnToZero, {300, 150});
    else if (menuItemID == 4013) addWidget(DraggableWidget::TransportMarkerNext, {350, 150});
    else if (menuItemID == 4014) addWidget(DraggableWidget::TransportMarkerPrevious, {400, 150});
    
    // Transporte - Loop & Sync
    else if (menuItemID == 4020) addWidget(DraggableWidget::TransportLoop, {200, 200});
    else if (menuItemID == 4021) addWidget(DraggableWidget::TransportMetronome, {250, 200});
    else if (menuItemID == 4022) addWidget(DraggableWidget::TransportSync, {300, 200});
    else if (menuItemID == 4023) addWidget(DraggableWidget::TransportTap, {350, 200});
    
    // Transporte - Display
    else if (menuItemID == 4030) 
    {
        auto* widget = addWidget(DraggableWidget::Label, {200, 100});
        if (widget)
        {
            widget->setWidgetName("Transport Display");
            widget->setSize(300, 60);
            debugConsole.log("Transport Display widget añadido");
        }
    }
    
    // Settings - Audio/MIDI
    else if (menuItemID == 5001) showAudioSettings();
    else if (menuItemID == 5002) debugConsole.log("Configuración MIDI - En desarrollo");
    
    // Settings - Grid
    else if (menuItemID == 5003) { showGrid = !showGrid; repaint(); }
    else if (menuItemID == 5004) { snapToGrid = !snapToGrid; }
    else if (menuItemID == 5005) { gridSize = 10; repaint(); }
    else if (menuItemID == 5006) { gridSize = 20; repaint(); }
    else if (menuItemID == 5007) { gridSize = 30; repaint(); }
    else if (menuItemID == 5008) { gridSize = 40; repaint(); }
}

// Métodos de zoom
void MainComponent::zoomIn()
{
    if (zoomLevel < 3.0f)
    {
        zoomLevel += 0.2f;
        updateCanvasZoom();
        debugConsole.log("Zoom: " + juce::String(zoomLevel * 100, 0) + "%");
    }
}

void MainComponent::zoomOut()
{
    if (zoomLevel > 0.5f)
    {
        zoomLevel -= 0.2f;
        updateCanvasZoom();
        debugConsole.log("Zoom: " + juce::String(zoomLevel * 100, 0) + "%");
    }
}

void MainComponent::updateCanvasZoom()
{
    int newWidth = static_cast<int>(2000 * zoomLevel);
    int newHeight = static_cast<int>(2000 * zoomLevel);
    canvasContainer.setSize(newWidth, newHeight);
    
    // Actualizar posición y tamaño de todos los widgets
    for (auto* widget : widgets)
    {
        auto bounds = widget->getBounds();
        widget->setBounds(bounds);
    }
    
    canvasViewport.repaint();
}

// ============================================================================
// NUEVAS FUNCIONALIDADES - PROYECTO
// ============================================================================

void MainComponent::showRecentProjects()
{
    juce::PopupMenu recentMenu;
    
    if (recentProjects.isEmpty())
    {
        juce::AlertWindow::showMessageBoxAsync(
            juce::MessageBoxIconType::InfoIcon,
            "Proyectos Recientes",
            "No hay proyectos recientes"
        );
        return;
    }
    
    for (int i = 0; i < recentProjects.size(); ++i)
    {
        juce::File projectFile(recentProjects[i]);
        recentMenu.addItem(10000 + i, projectFile.getFileName());
    }
    
    recentMenu.addSeparator();
    recentMenu.addItem(9998, "Limpiar lista");
    
    recentMenu.showMenuAsync(juce::PopupMenu::Options(), [this](int result)
    {
        if (result == 9998)
        {
            recentProjects.clear();
            saveRecentProjects();
            debugConsole.log("Lista de proyectos recientes limpiada");
        }
        else if (result >= 10000)
        {
            int index = result - 10000;
            if (index < recentProjects.size())
            {
                juce::File projectFile(recentProjects[index]);
                if (projectFile.existsAsFile())
                {
                    currentProjectFile = projectFile;
                    
                    // Cargar proyecto directamente
                    auto xml = juce::parseXML(projectFile);
                    if (xml != nullptr)
                    {
                        auto projectTree = juce::ValueTree::fromXml(*xml);
                        if (projectTree.isValid() && projectTree.hasType("DAWProject"))
                        {
                            loadFromProjectTree(projectTree);
                            addToRecentProjects(projectFile);
                            debugConsole.log("Proyecto cargado: " + projectFile.getFullPathName());
                        }
                    }
                }
                else
                {
                    juce::AlertWindow::showMessageBoxAsync(
                        juce::MessageBoxIconType::WarningIcon,
                        "Archivo no encontrado",
                        "El proyecto ya no existe en: " + projectFile.getFullPathName()
                    );
                    recentProjects.remove(index);
                    saveRecentProjects();
                }
            }
        }
    });
}

void MainComponent::showProjectProperties()
{
    juce::String message = "Propiedades del Proyecto:\n\n";
    message += "Nombre: " + projectProperties.name + "\n";
    message += "Autor: " + projectProperties.author + "\n";
    message += "Versión: " + projectProperties.version + "\n";
    message += "Descripción: " + projectProperties.description + "\n\n";
    message += "Creado: " + projectProperties.creationDate.toString(true, true) + "\n";
    message += "Modificado: " + projectProperties.lastModifiedDate.toString(true, true) + "\n\n";
    message += "Para editar las propiedades, use un editor de texto\ny modifique el archivo .dawproj directamente.";
    
    juce::AlertWindow::showMessageBoxAsync(
        juce::MessageBoxIconType::InfoIcon,
        "Propiedades del Proyecto",
        message
    );
}

void MainComponent::showModuleManager()
{
    juce::String message = "Estado de Módulos del Proyecto:\n\n";
    message += "Audio: " + juce::String(moduleState.audioEnabled ? "✓ Activado" : "✗ Desactivado") + "\n";
    message += "MIDI: " + juce::String(moduleState.midiEnabled ? "✓ Activado" : "✗ Desactivado") + "\n";
    message += "DSP: " + juce::String(moduleState.dspEnabled ? "✓ Activado" : "✗ Desactivado") + "\n";
    message += "GUI: " + juce::String(moduleState.guiEnabled ? "✓ Activado" : "✗ Desactivado") + "\n";
    message += "Utilidades: " + juce::String(moduleState.utilitiesEnabled ? "✓ Activado" : "✗ Desactivado") + "\n\n";
    message += "Los módulos están activos por defecto.\nPuede configurarlos en el archivo de proyecto.";
    
    juce::AlertWindow::showMessageBoxAsync(
        juce::MessageBoxIconType::InfoIcon,
        "Gestor de Módulos",
        message
    );
}

void MainComponent::showModuleSettings(const juce::String& moduleName)
{
    juce::String message = "Configuración del módulo " + moduleName + ":\n\n";
    
    if (moduleName == "Audio")
    {
        message += "• Buffer size: 512 samples\n";
        message += "• Sample rate: 44100 Hz\n";
        message += "• Latencia: 11.6 ms\n";
        message += "• Canales entrada: 2\n";
        message += "• Canales salida: 2\n";
    }
    else if (moduleName == "MIDI")
    {
        message += "• Dispositivos MIDI: " + juce::String(juce::MidiInput::getAvailableDevices().size()) + "\n";
        message += "• Canal MIDI: Todos\n";
        message += "• MIDI Learn: Activado\n";
    }
    else if (moduleName == "DSP")
    {
        message += "• Procesadores disponibles: 12\n";
        message += "• FFT size: 2048\n";
        message += "• Oversampling: 2x\n";
    }
    else if (moduleName == "GUI")
    {
        message += "• Widgets disponibles: 50+\n";
        message += "• Temas: Light, Dark\n";
        message += "• Personalización completa\n";
    }
    else if (moduleName == "Utilidades")
    {
        message += "• Exportación MIDI/Audio\n";
        message += "• Gestión de proyectos\n";
        message += "• Sistema de plantillas\n";
    }
    
    juce::AlertWindow::showMessageBoxAsync(
        juce::MessageBoxIconType::InfoIcon,
        "Módulo " + moduleName,
        message
    );
}

void MainComponent::showPlatformExportDialog(const juce::String& platformName)
{
    juce::String message = "Exportar para " + platformName + "\n\n";
    message += "Esta función permitirá:\n";
    message += "• Configurar opciones específicas de " + platformName + "\n";
    message += "• Compilar para la plataforma target\n";
    message += "• Generar instaladores/paquetes\n\n";
    message += "Estado: En desarrollo";
    
    juce::AlertWindow::showMessageBoxAsync(
        juce::MessageBoxIconType::InfoIcon,
        "Exportar - " + platformName,
        message
    );
}

void MainComponent::showMidiSettings()
{
    // Obtener dispositivos MIDI disponibles
    auto midiInputs = juce::MidiInput::getAvailableDevices();
    auto midiOutputs = juce::MidiOutput::getAvailableDevices();
    
    juce::String message = "CONFIGURACIÓN MIDI\n\n";
    message += "Dispositivos de entrada:\n";
    if (midiInputs.isEmpty())
        message += "• No hay dispositivos MIDI de entrada\n";
    else
        for (auto& device : midiInputs)
            message += "• " + device.name + "\n";
    
    message += "\nDispositivos de salida:\n";
    if (midiOutputs.isEmpty())
        message += "• No hay dispositivos MIDI de salida\n";
    else
        for (auto& device : midiOutputs)
            message += "• " + device.name + "\n";
    
    juce::AlertWindow::showMessageBoxAsync(
        juce::MessageBoxIconType::InfoIcon,
        "Configuración MIDI",
        message
    );
    
    debugConsole.log("Configuración MIDI mostrada");
}

void MainComponent::showExportPathsSettings()
{
    // Crear directorios si no existen
    exportPaths.pluginsPath.createDirectory();
    exportPaths.midiPath.createDirectory();
    exportPaths.audioPath.createDirectory();
    exportPaths.projectsPath.createDirectory();
    
    juce::String message = "RUTAS DE EXPORTACIÓN\n\n";
    message += "Plugins:\n" + exportPaths.pluginsPath.getFullPathName() + "\n\n";
    message += "MIDI:\n" + exportPaths.midiPath.getFullPathName() + "\n\n";
    message += "Audio:\n" + exportPaths.audioPath.getFullPathName() + "\n\n";
    message += "Proyectos:\n" + exportPaths.projectsPath.getFullPathName() + "\n\n";
    message += "Todas las rutas han sido creadas y están listas para usar.";
    
    juce::AlertWindow::showMessageBoxAsync(
        juce::MessageBoxIconType::InfoIcon,
        "Rutas de Exportación",
        message
    );
    
    debugConsole.log("Rutas de exportación mostradas");
}

void MainComponent::showEditorPreferences()
{
    // Crear ventana de preferencias personalizada
    class PreferencesWindow : public juce::Component
    {
    public:
        PreferencesWindow(MainComponent& owner) : mainComponent(owner)
        {
            setSize(400, 300);
            
            addAndMakeVisible(themeLabel);
            themeLabel.setText("Tema:", juce::dontSendNotification);
            themeLabel.setBounds(20, 20, 100, 25);
            
            addAndMakeVisible(themeCombo);
            themeCombo.addItem("Oscuro", 1);
            themeCombo.addItem("Claro", 2);
            themeCombo.addItem("Azul Oscuro", 3);
            themeCombo.addItem("Personalizado", 4);
            themeCombo.setSelectedId(mainComponent.editorPreferences.theme + 1);
            themeCombo.setBounds(130, 20, 250, 25);
            
            addAndMakeVisible(fontSizeLabel);
            fontSizeLabel.setText("Tamaño fuente:", juce::dontSendNotification);
            fontSizeLabel.setBounds(20, 60, 100, 25);
            
            addAndMakeVisible(fontSizeSlider);
            fontSizeSlider.setRange(8, 24, 2);
            fontSizeSlider.setValue(mainComponent.editorPreferences.fontSize);
            fontSizeSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 50, 25);
            fontSizeSlider.setBounds(130, 60, 250, 25);
            
            addAndMakeVisible(gridSizeLabel);
            gridSizeLabel.setText("Tamaño grid:", juce::dontSendNotification);
            gridSizeLabel.setBounds(20, 100, 100, 25);
            
            addAndMakeVisible(gridSizeSlider);
            gridSizeSlider.setRange(5, 50, 5);
            gridSizeSlider.setValue(mainComponent.editorPreferences.gridSize);
            gridSizeSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 50, 25);
            gridSizeSlider.setBounds(130, 100, 250, 25);
            
            addAndMakeVisible(showGridToggle);
            showGridToggle.setButtonText("Mostrar Grid");
            showGridToggle.setToggleState(mainComponent.editorPreferences.showGrid, juce::dontSendNotification);
            showGridToggle.setBounds(20, 140, 200, 25);
            
            addAndMakeVisible(snapToGridToggle);
            snapToGridToggle.setButtonText("Snap to Grid");
            snapToGridToggle.setToggleState(mainComponent.editorPreferences.snapToGrid, juce::dontSendNotification);
            snapToGridToggle.setBounds(20, 175, 200, 25);
            
            addAndMakeVisible(saveButton);
            saveButton.setButtonText("Guardar");
            saveButton.setBounds(200, 240, 80, 30);
            saveButton.onClick = [this]() { saveAndClose(); };
            
            addAndMakeVisible(cancelButton);
            cancelButton.setButtonText("Cancelar");
            cancelButton.setBounds(290, 240, 80, 30);
            cancelButton.onClick = [this]() { close(); };
        }
        
        void saveAndClose()
        {
            mainComponent.editorPreferences.theme = themeCombo.getSelectedId() - 1;
            mainComponent.editorPreferences.fontSize = static_cast<int>(fontSizeSlider.getValue());
            mainComponent.editorPreferences.gridSize = static_cast<int>(gridSizeSlider.getValue());
            mainComponent.editorPreferences.showGrid = showGridToggle.getToggleState();
            mainComponent.editorPreferences.snapToGrid = snapToGridToggle.getToggleState();
            
            // Aplicar cambios
            mainComponent.gridSize = mainComponent.editorPreferences.gridSize;
            mainComponent.showGrid = mainComponent.editorPreferences.showGrid;
            mainComponent.snapToGrid = mainComponent.editorPreferences.snapToGrid;
            mainComponent.repaint();
            
            // Guardar a archivo
            mainComponent.editorPreferences.saveToFile();
            
            mainComponent.debugConsole.log("Preferencias guardadas");
            close();
        }
        
        void close()
        {
            if (auto* parent = getParentComponent())
                parent->exitModalState(0);
        }
        
    private:
        MainComponent& mainComponent;
        juce::Label themeLabel, fontSizeLabel, gridSizeLabel;
        juce::ComboBox themeCombo;
        juce::Slider fontSizeSlider, gridSizeSlider;
        juce::ToggleButton showGridToggle, snapToGridToggle;
        juce::TextButton saveButton, cancelButton;
    };
    
    auto* prefsWindow = new PreferencesWindow(*this);
    juce::DialogWindow::LaunchOptions options;
    options.content.setOwned(prefsWindow);
    options.dialogTitle = "Preferencias del Editor";
    options.dialogBackgroundColour = juce::Colour(0xff2b2b2b);
    options.escapeKeyTriggersCloseButton = true;
    options.useNativeTitleBar = true;
    options.resizable = false;
    
    options.launchAsync();
}

void MainComponent::showKeyboardShortcuts()
{
    juce::String shortcuts = 
        "ATAJOS DE TECLADO\n\n"
        "Archivo:\n"
        "  Ctrl+N - Nuevo proyecto\n"
        "  Ctrl+S - Guardar proyecto\n"
        "  Ctrl+O - Abrir proyecto\n"
        "  Ctrl+E - Exportar plugin\n\n"
        "Edición:\n"
        "  Ctrl+Z - Deshacer\n"
        "  Ctrl+Y - Rehacer\n"
        "  Ctrl+C - Copiar widget\n"
        "  Ctrl+V - Pegar widget\n"
        "  Delete - Eliminar widget\n\n"
        "Vista:\n"
        "  Ctrl++ - Zoom in\n"
        "  Ctrl+- - Zoom out\n"
        "  Ctrl+0 - Zoom 100%\n"
        "  Ctrl+G - Mostrar/Ocultar grid\n\n"
        "Audio:\n"
        "  Espacio - Play/Pause\n"
        "  Ctrl+R - Grabar\n"
        "  Ctrl+L - Loop on/off\n";
    
    juce::AlertWindow::showMessageBoxAsync(
        juce::MessageBoxIconType::InfoIcon,
        "Atajos de Teclado",
        shortcuts
    );
}

// ============================================================================
// SISTEMA DE PROYECTOS RECIENTES
// ============================================================================

void MainComponent::addToRecentProjects(const juce::File& projectFile)
{
    if (!projectFile.existsAsFile())
        return;
    
    juce::String path = projectFile.getFullPathName();
    
    // Eliminar si ya existe (para moverlo al principio)
    recentProjects.removeString(path);
    
    // Añadir al principio
    recentProjects.insert(0, path);
    
    // Mantener solo los últimos 10
    while (recentProjects.size() > 10)
        recentProjects.remove(recentProjects.size() - 1);
    
    saveRecentProjects();
}

void MainComponent::loadRecentProjects()
{
    juce::File configFile = juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory)
                                .getChildFile("DawMaker")
                                .getChildFile("recent_projects.txt");
    
    if (configFile.existsAsFile())
    {
        juce::StringArray lines;
        configFile.readLines(lines);
        
        for (auto& line : lines)
        {
            line = line.trim();
            if (line.isNotEmpty() && juce::File(line).existsAsFile())
                recentProjects.add(line);
        }
    }
}

void MainComponent::saveRecentProjects()
{
    juce::File configDir = juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory)
                               .getChildFile("DawMaker");
    
    configDir.createDirectory();
    
    juce::File configFile = configDir.getChildFile("recent_projects.txt");
    
    juce::String content;
    for (auto& path : recentProjects)
        content += path + "\n";
    
    configFile.replaceWithText(content);
}

// ============================================================================
// PREFERENCIAS DEL EDITOR - GUARDAR/CARGAR
// ============================================================================

void MainComponent::EditorPreferences::saveToFile()
{
    juce::File configDir = juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory)
                               .getChildFile("DawMaker");
    
    configDir.createDirectory();
    
    juce::File configFile = configDir.getChildFile("editor_preferences.xml");
    
    juce::ValueTree prefs("EditorPreferences");
    prefs.setProperty("theme", theme, nullptr);
    prefs.setProperty("fontSize", fontSize, nullptr);
    prefs.setProperty("gridSize", gridSize, nullptr);
    prefs.setProperty("showGrid", showGrid, nullptr);
    prefs.setProperty("snapToGrid", snapToGrid, nullptr);
    
    auto xml = prefs.createXml();
    if (xml != nullptr)
        xml->writeTo(configFile);
}

void MainComponent::EditorPreferences::loadFromFile()
{
    juce::File configFile = juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory)
                                .getChildFile("DawMaker")
                                .getChildFile("editor_preferences.xml");
    
    if (configFile.existsAsFile())
    {
        auto xml = juce::parseXML(configFile);
        if (xml != nullptr)
        {
            auto prefs = juce::ValueTree::fromXml(*xml);
            if (prefs.isValid() && prefs.hasType("EditorPreferences"))
            {
                theme = prefs.getProperty("theme", 0);
                fontSize = prefs.getProperty("fontSize", 14);
                gridSize = prefs.getProperty("gridSize", 20);
                showGrid = prefs.getProperty("showGrid", true);
                snapToGrid = prefs.getProperty("snapToGrid", true);
            }
        }
    }
}
