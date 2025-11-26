#include "VisualBuilder.h"

// ============================================================================
// WidgetPalette - Paleta de Widgets
// ============================================================================

WidgetPalette::WidgetPalette()
{
    // Add all components to contentComponent instead of directly to this
    
    // Configurar etiquetas de categoría
    basicWidgetsLabel.setFont(juce::FontOptions(14.0f, juce::Font::bold));
    basicWidgetsLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    contentComponent.addAndMakeVisible(basicWidgetsLabel);
    
    audioWidgetsLabel.setFont(juce::FontOptions(14.0f, juce::Font::bold));
    audioWidgetsLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    contentComponent.addAndMakeVisible(audioWidgetsLabel);
    
    projectLabel.setFont(juce::FontOptions(14.0f, juce::Font::bold));
    projectLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    contentComponent.addAndMakeVisible(projectLabel);
    
    gridLabel.setFont(juce::FontOptions(14.0f, juce::Font::bold));
    gridLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    contentComponent.addAndMakeVisible(gridLabel);
    
    controlsLabel.setFont(juce::FontOptions(14.0f, juce::Font::bold));
    controlsLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    contentComponent.addAndMakeVisible(controlsLabel);
    
    proWidgetsLabel.setFont(juce::FontOptions(14.0f, juce::Font::bold));
    proWidgetsLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    contentComponent.addAndMakeVisible(proWidgetsLabel);
    
    transportLabel.setFont(juce::FontOptions(14.0f, juce::Font::bold));
    transportLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    contentComponent.addAndMakeVisible(transportLabel);

    // Configurar botones de widgets basicos
    setupButton(knobButton, DraggableWidget::Knob);
    setupButton(sliderButton, DraggableWidget::Slider);
    setupButton(buttonButton, DraggableWidget::Button);
    setupButton(labelButton, DraggableWidget::Label);
    
    // Widgets de audio
    setupButton(waveformButton, DraggableWidget::Waveform);
    setupButton(meterButton, DraggableWidget::Meter);
    
    // Controles 2D (inspirados en AudioKit)
    setupButton(xyPadButton, DraggableWidget::XYPad);
    setupButton(joystickButton, DraggableWidget::Joystick);
    setupButton(pitchWheelButton, DraggableWidget::PitchWheel);
    setupButton(indexedSliderButton, DraggableWidget::IndexedSlider);
    
    // Widgets profesionales (inspirados en ViatorDSP)
    setupButton(basicMeterButton, DraggableWidget::BasicMeter);
    setupButton(vuMeterButton, DraggableWidget::VUMeter);
    setupButton(ledMeterButton, DraggableWidget::LedMeter);
    setupButton(audioScopeButton, DraggableWidget::AudioScope);
    setupButton(verticalFaderButton, DraggableWidget::VerticalFader);
    setupButton(imageButtonButton, DraggableWidget::ImageButton);
    setupButton(toggleButtonButton, DraggableWidget::ToggleButton);
    
    // Botones de transporte
    setupButton(transportPlayButton, DraggableWidget::TransportPlay);
    setupButton(transportPauseButton, DraggableWidget::TransportPause);
    setupButton(transportStopButton, DraggableWidget::TransportStop);
    setupButton(transportRecordButton, DraggableWidget::TransportRecord);
    setupButton(transportRewindButton, DraggableWidget::TransportRewind);
    setupButton(transportFFButton, DraggableWidget::TransportFastForward);
    setupButton(transportLoopButton, DraggableWidget::TransportLoop);
    setupButton(transportMetronomeButton, DraggableWidget::TransportMetronome);
    setupButton(transportTapButton, DraggableWidget::TransportTap);
    setupButton(transportSyncButton, DraggableWidget::TransportSync);
    setupButton(transportReturnToZeroButton, DraggableWidget::TransportReturnToZero);
    setupButton(transportMarkerNextButton, DraggableWidget::TransportMarkerNext);
    setupButton(transportMarkerPrevButton, DraggableWidget::TransportMarkerPrevious);
    setupButton(transportNudgeFwdButton, DraggableWidget::TransportNudgeForward);
    setupButton(transportNudgeBackButton, DraggableWidget::TransportNudgeBackward);
    setupButton(transportDropButton, DraggableWidget::TransportDrop);
    setupButton(transportReplaceButton, DraggableWidget::TransportReplace);
    setupButton(transportAutomationButton, DraggableWidget::TransportAutomationMode);
    setupButton(transportJogWheelButton, DraggableWidget::TransportJogWheel);
    setupButton(transportScrubButton, DraggableWidget::TransportScrub);
    setupButton(transportPreRollButton, DraggableWidget::TransportPreRoll);
    setupButton(transportPostRollButton, DraggableWidget::TransportPostRoll);
    
    // Botones de gestión de proyecto
    saveButton.onClick = [this]() {
        if (onSaveProject) onSaveProject();
    };
    saveButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff00ff00).darker(0.3f));
    contentComponent.addAndMakeVisible(saveButton);
    
    loadButton.onClick = [this]() {
        if (onLoadProject) onLoadProject();
    };
    loadButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff0080ff));
    contentComponent.addAndMakeVisible(loadButton);
    
    clearButton.onClick = [this]() {
        if (onClearProject) onClearProject();
    };
    clearButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xffff4444));
    contentComponent.addAndMakeVisible(clearButton);
    
    // Controles de grid
    gridToggle.setButtonText("Mostrar Grid");
    gridToggle.setToggleState(true, juce::dontSendNotification);
    gridToggle.onClick = [this]() {
        if (onToggleGrid) onToggleGrid(gridToggle.getToggleState());
    };
    contentComponent.addAndMakeVisible(gridToggle);
    
    snapToggle.setButtonText("Snap to Grid");
    snapToggle.setToggleState(true, juce::dontSendNotification);
    snapToggle.onClick = [this]() {
        if (onToggleSnap) onToggleSnap(snapToggle.getToggleState());
    };
    contentComponent.addAndMakeVisible(snapToggle);
    
    gridSizeLabel.setText("Tamano: 20", juce::dontSendNotification);
    gridSizeLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    contentComponent.addAndMakeVisible(gridSizeLabel);
    
    gridSizeSlider.setRange(10, 50, 5);
    gridSizeSlider.setValue(20, juce::dontSendNotification);
    gridSizeSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    gridSizeSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    gridSizeSlider.onValueChange = [this]() {
        int size = static_cast<int>(gridSizeSlider.getValue());
        gridSizeLabel.setText("Tamano: " + juce::String(size), juce::dontSendNotification);
        if (onGridSizeChanged) onGridSizeChanged(size);
    };
    contentComponent.addAndMakeVisible(gridSizeSlider);
    
    // Botón de configuración de audio/MIDI
    audioSettingsButton.onClick = [this]() {
        if (onAudioSettings) onAudioSettings();
    };
    audioSettingsButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xffff8800));
    contentComponent.addAndMakeVisible(audioSettingsButton);
    
    // Setup viewport
    viewport.setViewedComponent(&contentComponent, false);
    viewport.setScrollBarsShown(true, false);
    addAndMakeVisible(viewport);
}

void WidgetPalette::setupButton(juce::TextButton& button, DraggableWidget::WidgetType type)
{
    button.onClick = [this, type]() {
        if (onWidgetDraggedFromPalette)
            onWidgetDraggedFromPalette(type, juce::Point<int>(100, 100));
    };
    contentComponent.addAndMakeVisible(button);
}

void WidgetPalette::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff16213e));
    
    g.setColour(juce::Colour(0xff00adb5));
    g.drawRect(getLocalBounds(), 2);
    
    g.setColour(juce::Colours::white.withAlpha(0.1f));
    g.fillRect(0, 30, getWidth(), 1);
}

void WidgetPalette::resized()
{
    viewport.setBounds(getLocalBounds());
    
    int yPos = 10;
    int contentWidth = getWidth() - 20;
    int leftMargin = 10;
    int rightMargin = 10;
    
    // Widgets basicos
    basicWidgetsLabel.setBounds(leftMargin, yPos, contentWidth - leftMargin - rightMargin, 25);
    yPos += 30;
    
    knobButton.setBounds(leftMargin, yPos, contentWidth - leftMargin - rightMargin, 30);
    yPos += 35;
    sliderButton.setBounds(leftMargin, yPos, contentWidth - leftMargin - rightMargin, 30);
    yPos += 35;
    buttonButton.setBounds(leftMargin, yPos, contentWidth - leftMargin - rightMargin, 30);
    yPos += 35;
    labelButton.setBounds(leftMargin, yPos, contentWidth - leftMargin - rightMargin, 30);
    yPos += 45;
    
    // Widgets de audio
    audioWidgetsLabel.setBounds(leftMargin, yPos, contentWidth - leftMargin - rightMargin, 25);
    yPos += 30;
    
    waveformButton.setBounds(leftMargin, yPos, contentWidth - leftMargin - rightMargin, 30);
    yPos += 35;
    meterButton.setBounds(leftMargin, yPos, contentWidth - leftMargin - rightMargin, 30);
    yPos += 45;
    
    // Controles 2D (AudioKit-inspired)
    controlsLabel.setBounds(leftMargin, yPos, contentWidth - leftMargin - rightMargin, 25);
    yPos += 30;
    
    xyPadButton.setBounds(leftMargin, yPos, contentWidth - leftMargin - rightMargin, 30);
    yPos += 35;
    joystickButton.setBounds(leftMargin, yPos, contentWidth - leftMargin - rightMargin, 30);
    yPos += 35;
    pitchWheelButton.setBounds(leftMargin, yPos, contentWidth - leftMargin - rightMargin, 30);
    yPos += 35;
    indexedSliderButton.setBounds(leftMargin, yPos, contentWidth - leftMargin - rightMargin, 30);
    yPos += 45;
    
    // Widgets profesionales (ViatorDSP-inspired)
    proWidgetsLabel.setBounds(leftMargin, yPos, contentWidth - leftMargin - rightMargin, 25);
    yPos += 30;
    
    basicMeterButton.setBounds(leftMargin, yPos, contentWidth - leftMargin - rightMargin, 30);
    yPos += 35;
    vuMeterButton.setBounds(leftMargin, yPos, contentWidth - leftMargin - rightMargin, 30);
    yPos += 35;
    ledMeterButton.setBounds(leftMargin, yPos, contentWidth - leftMargin - rightMargin, 30);
    yPos += 35;
    audioScopeButton.setBounds(leftMargin, yPos, contentWidth - leftMargin - rightMargin, 30);
    yPos += 35;
    verticalFaderButton.setBounds(leftMargin, yPos, contentWidth - leftMargin - rightMargin, 30);
    yPos += 35;
    imageButtonButton.setBounds(leftMargin, yPos, contentWidth - leftMargin - rightMargin, 30);
    yPos += 35;
    toggleButtonButton.setBounds(leftMargin, yPos, contentWidth - leftMargin - rightMargin, 30);
    yPos += 45;
    
    // Botones de transporte
    transportLabel.setBounds(leftMargin, yPos, contentWidth - leftMargin - rightMargin, 25);
    yPos += 30;
    
    transportPlayButton.setBounds(leftMargin, yPos, contentWidth - leftMargin - rightMargin, 30);
    yPos += 35;
    transportPauseButton.setBounds(leftMargin, yPos, contentWidth - leftMargin - rightMargin, 30);
    yPos += 35;
    transportStopButton.setBounds(leftMargin, yPos, contentWidth - leftMargin - rightMargin, 30);
    yPos += 35;
    transportRecordButton.setBounds(leftMargin, yPos, contentWidth - leftMargin - rightMargin, 30);
    yPos += 35;
    transportRewindButton.setBounds(leftMargin, yPos, contentWidth - leftMargin - rightMargin, 30);
    yPos += 35;
    transportFFButton.setBounds(leftMargin, yPos, contentWidth - leftMargin - rightMargin, 30);
    yPos += 35;
    transportLoopButton.setBounds(leftMargin, yPos, contentWidth - leftMargin - rightMargin, 30);
    yPos += 35;
    transportMetronomeButton.setBounds(leftMargin, yPos, contentWidth - leftMargin - rightMargin, 30);
    yPos += 35;
    transportTapButton.setBounds(leftMargin, yPos, contentWidth - leftMargin - rightMargin, 30);
    yPos += 35;
    transportSyncButton.setBounds(leftMargin, yPos, contentWidth - leftMargin - rightMargin, 30);
    yPos += 35;
    transportReturnToZeroButton.setBounds(leftMargin, yPos, contentWidth - leftMargin - rightMargin, 30);
    yPos += 35;
    transportMarkerNextButton.setBounds(leftMargin, yPos, contentWidth - leftMargin - rightMargin, 30);
    yPos += 35;
    transportMarkerPrevButton.setBounds(leftMargin, yPos, contentWidth - leftMargin - rightMargin, 30);
    yPos += 35;
    transportNudgeFwdButton.setBounds(leftMargin, yPos, contentWidth - leftMargin - rightMargin, 30);
    yPos += 35;
    transportNudgeBackButton.setBounds(leftMargin, yPos, contentWidth - leftMargin - rightMargin, 30);
    yPos += 35;
    transportDropButton.setBounds(leftMargin, yPos, contentWidth - leftMargin - rightMargin, 30);
    yPos += 35;
    transportReplaceButton.setBounds(leftMargin, yPos, contentWidth - leftMargin - rightMargin, 30);
    yPos += 35;
    transportAutomationButton.setBounds(leftMargin, yPos, contentWidth - leftMargin - rightMargin, 30);
    yPos += 35;
    transportJogWheelButton.setBounds(leftMargin, yPos, contentWidth - leftMargin - rightMargin, 30);
    yPos += 35;
    transportScrubButton.setBounds(leftMargin, yPos, contentWidth - leftMargin - rightMargin, 30);
    yPos += 35;
    transportPreRollButton.setBounds(leftMargin, yPos, contentWidth - leftMargin - rightMargin, 30);
    yPos += 35;
    transportPostRollButton.setBounds(leftMargin, yPos, contentWidth - leftMargin - rightMargin, 30);
    yPos += 45;
    
    // Gestión de proyecto
    projectLabel.setBounds(leftMargin, yPos, contentWidth - leftMargin - rightMargin, 25);
    yPos += 30;
    saveButton.setBounds(leftMargin, yPos, contentWidth - leftMargin - rightMargin, 30);
    yPos += 35;
    loadButton.setBounds(leftMargin, yPos, contentWidth - leftMargin - rightMargin, 30);
    yPos += 35;
    clearButton.setBounds(leftMargin, yPos, contentWidth - leftMargin - rightMargin, 30);
    yPos += 45;
    
    // Configuración de Grid
    gridLabel.setBounds(leftMargin, yPos, contentWidth - leftMargin - rightMargin, 25);
    yPos += 30;
    gridToggle.setBounds(leftMargin, yPos, contentWidth - leftMargin - rightMargin, 25);
    yPos += 30;
    snapToggle.setBounds(leftMargin, yPos, contentWidth - leftMargin - rightMargin, 25);
    yPos += 30;
    gridSizeLabel.setBounds(leftMargin, yPos, contentWidth - leftMargin - rightMargin, 20);
    yPos += 23;
    gridSizeSlider.setBounds(leftMargin, yPos, contentWidth - leftMargin - rightMargin, 25);
    yPos += 40;
    
    // Configuración de Audio/MIDI
    audioSettingsButton.setBounds(leftMargin, yPos, contentWidth - leftMargin - rightMargin, 35);
    yPos += 45;
    
    // Set content height based on actual content
    contentComponent.setSize(contentWidth, yPos);
}

// ============================================================================
// PropertyInspector - Inspector de Propiedades
// ============================================================================

PropertyInspector::PropertyInspector()
{
    // Add all components to contentComponent
    
    // Título
    titleLabel.setFont(juce::FontOptions(16.0f, juce::Font::bold));
    titleLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    titleLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(titleLabel);
    
    // Mensaje cuando no hay selección
    noSelectionLabel.setFont(juce::FontOptions(12.0f));
    noSelectionLabel.setColour(juce::Label::textColourId, juce::Colours::grey);
    noSelectionLabel.setJustificationType(juce::Justification::centred);
    contentComponent.addAndMakeVisible(noSelectionLabel);
    
    // Etiquetas de sección
    positionLabel.setFont(juce::FontOptions(13.0f, juce::Font::bold));
    positionLabel.setColour(juce::Label::textColourId, juce::Colour(0xff00adb5));
    contentComponent.addAndMakeVisible(positionLabel);
    
    appearanceLabel.setFont(juce::FontOptions(13.0f, juce::Font::bold));
    appearanceLabel.setColour(juce::Label::textColourId, juce::Colour(0xff00adb5));
    contentComponent.addAndMakeVisible(appearanceLabel);
    
    codeLabel.setFont(juce::FontOptions(13.0f, juce::Font::bold));
    codeLabel.setColour(juce::Label::textColourId, juce::Colour(0xff00adb5));
    contentComponent.addAndMakeVisible(codeLabel);
    
    // Controles de posición
    contentComponent.addAndMakeVisible(xLabel);
    contentComponent.addAndMakeVisible(yLabel);
    contentComponent.addAndMakeVisible(widthLabel);
    contentComponent.addAndMakeVisible(heightLabel);
    
    xSlider.setRange(0, 2000, 1);
    xSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 60, 20);
    contentComponent.addAndMakeVisible(xSlider);
    
    ySlider.setRange(0, 2000, 1);
    ySlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 60, 20);
    contentComponent.addAndMakeVisible(ySlider);
    
    widthSlider.setRange(50, 1000, 1);
    widthSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 60, 20);
    contentComponent.addAndMakeVisible(widthSlider);
    
    heightSlider.setRange(50, 1000, 1);
    heightSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 60, 20);
    contentComponent.addAndMakeVisible(heightSlider);
    
    // Controles de apariencia
    contentComponent.addAndMakeVisible(nameLabel);
    nameEditor.setMultiLine(false);
    nameEditor.onReturnKey = [this]() {
        if (selectedWidget)
        {
            selectedWidget->setWidgetName(nameEditor.getText());
            varNameEditor.setText(nameEditor.getText().replaceCharacter(' ', '_'));
            selectedWidget->repaint();
        }
    };
    nameEditor.onFocusLost = [this]() {
        if (selectedWidget)
        {
            selectedWidget->setWidgetName(nameEditor.getText());
            varNameEditor.setText(nameEditor.getText().replaceCharacter(' ', '_'));
            selectedWidget->repaint();
        }
    };
    nameEditor.onTextChange = [this]() {
        if (selectedWidget)
        {
            selectedWidget->setWidgetName(nameEditor.getText());
            varNameEditor.setText(nameEditor.getText().replaceCharacter(' ', '_'));
            selectedWidget->repaint();
        }
    };
    contentComponent.addAndMakeVisible(nameEditor);
    
    contentComponent.addAndMakeVisible(bgColorLabel);
    bgColorButton.onClick = [this]() { 
        showColorSelector(ColorTarget::Background);
    };
    
    borderColorButton.onClick = [this]() {
        showColorSelector(ColorTarget::Border);
    };
    
    knobColorButton.onClick = [this]() {
        showColorSelector(ColorTarget::Knob);
    };
    
    textColorButton.onClick = [this]() {
        showColorSelector(ColorTarget::Text);
    };
    contentComponent.addAndMakeVisible(bgColorButton);
    
    contentComponent.addAndMakeVisible(borderColorLabel);
    contentComponent.addAndMakeVisible(borderColorButton);
    
    contentComponent.addAndMakeVisible(knobColorLabel);
    contentComponent.addAndMakeVisible(knobColorButton);
    
    contentComponent.addAndMakeVisible(textColorLabel);
    contentComponent.addAndMakeVisible(textColorButton);
    
    contentComponent.addAndMakeVisible(borderWidthLabel);
    contentComponent.addAndMakeVisible(borderWidthSlider);
    
    contentComponent.addAndMakeVisible(cornerRadiusLabel);
    contentComponent.addAndMakeVisible(cornerRadiusSlider);
    
    contentComponent.addAndMakeVisible(fillKnobLabel);
    contentComponent.addAndMakeVisible(fillKnobToggle);
    
    contentComponent.addAndMakeVisible(knobStyleLabel);
    contentComponent.addAndMakeVisible(knobStyleCombo);
    
    contentComponent.addAndMakeVisible(knobFilmstripLabel);
    contentComponent.addAndMakeVisible(loadKnobFilmstripButton);
    contentComponent.addAndMakeVisible(knobFilmstripPathLabel);
    knobFilmstripPathLabel.setColour(juce::Label::textColourId, juce::Colours::grey);
    
    loadKnobFilmstripButton.onClick = [this]() {
        auto chooser = std::make_shared<juce::FileChooser>("Seleccionar PNG filmstrip para Knob",
                                                           juce::File::getSpecialLocation(juce::File::userDesktopDirectory),
                                                           "*.png");
        
        auto chooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;
        
        chooser->launchAsync(chooserFlags, [this, chooser](const juce::FileChooser& fc)
        {
            auto file = fc.getResult();
            if (selectedWidget && file.existsAsFile())
            {
                selectedWidget->style.filmstripPath = file.getFullPathName();
                knobFilmstripPathLabel.setText(file.getFileName(), juce::dontSendNotification);
                selectedWidget->repaint();
            }
        });
    };
    
    contentComponent.addAndMakeVisible(sliderStyleLabel);
    contentComponent.addAndMakeVisible(sliderStyleCombo);
    
    contentComponent.addAndMakeVisible(sliderFilmstripLabel);
    contentComponent.addAndMakeVisible(loadSliderFilmstripButton);
    contentComponent.addAndMakeVisible(sliderFilmstripPathLabel);
    sliderFilmstripPathLabel.setColour(juce::Label::textColourId, juce::Colours::grey);
    
    loadSliderFilmstripButton.onClick = [this]() {
        auto chooser = std::make_shared<juce::FileChooser>("Seleccionar PNG para Slider",
                                                           juce::File::getSpecialLocation(juce::File::userDesktopDirectory),
                                                           "*.png");
        
        auto chooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;
        
        chooser->launchAsync(chooserFlags, [this, chooser](const juce::FileChooser& fc)
        {
            auto file = fc.getResult();
            if (selectedWidget && file.existsAsFile())
            {
                selectedWidget->style.filmstripPath = file.getFullPathName();
                sliderFilmstripPathLabel.setText(file.getFileName(), juce::dontSendNotification);
                selectedWidget->repaint();
            }
        });
    };
    
    contentComponent.addAndMakeVisible(buttonStyleLabel);
    contentComponent.addAndMakeVisible(buttonStyleCombo);
    
    contentComponent.addAndMakeVisible(buttonFilmstripLabel);
    contentComponent.addAndMakeVisible(loadButtonFilmstripButton);
    contentComponent.addAndMakeVisible(buttonFilmstripPathLabel);
    buttonFilmstripPathLabel.setColour(juce::Label::textColourId, juce::Colours::grey);
    
    loadButtonFilmstripButton.onClick = [this]() {
        auto chooser = std::make_shared<juce::FileChooser>("Seleccionar PNG para Boton",
                                                           juce::File::getSpecialLocation(juce::File::userDesktopDirectory),
                                                           "*.png");
        
        auto chooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;
        
        chooser->launchAsync(chooserFlags, [this, chooser](const juce::FileChooser& fc)
        {
            auto file = fc.getResult();
            if (selectedWidget && file.existsAsFile())
            {
                selectedWidget->style.filmstripPath = file.getFullPathName();
                buttonFilmstripPathLabel.setText(file.getFileName(), juce::dontSendNotification);
                selectedWidget->repaint();
            }
        });
    };
    
    contentComponent.addAndMakeVisible(labelStyleLabel);
    contentComponent.addAndMakeVisible(labelStyleCombo);
    
    contentComponent.addAndMakeVisible(labelFilmstripLabel);
    contentComponent.addAndMakeVisible(loadLabelFilmstripButton);
    contentComponent.addAndMakeVisible(labelFilmstripPathLabel);
    labelFilmstripPathLabel.setColour(juce::Label::textColourId, juce::Colours::grey);
    
    loadLabelFilmstripButton.onClick = [this]() {
        auto chooser = std::make_shared<juce::FileChooser>("Seleccionar PNG para Label",
                                                           juce::File::getSpecialLocation(juce::File::userDesktopDirectory),
                                                           "*.png");
        
        auto chooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;
        
        chooser->launchAsync(chooserFlags, [this, chooser](const juce::FileChooser& fc)
        {
            auto file = fc.getResult();
            if (selectedWidget && file.existsAsFile())
            {
                selectedWidget->style.filmstripPath = file.getFullPathName();
                labelFilmstripPathLabel.setText(file.getFileName(), juce::dontSendNotification);
                selectedWidget->repaint();
            }
        });
    };
    
    // Controles de waveform
    contentComponent.addAndMakeVisible(waveformStyleLabel);
    contentComponent.addAndMakeVisible(waveformStyleCombo);
    contentComponent.addAndMakeVisible(waveformColorLabel);
    contentComponent.addAndMakeVisible(waveformColorButton);
    
    // Controles de meter
    contentComponent.addAndMakeVisible(meterStyleLabel);
    contentComponent.addAndMakeVisible(meterStyleCombo);
    contentComponent.addAndMakeVisible(meterColorLabel);
    contentComponent.addAndMakeVisible(meterColorButton);
    
    // Controles de XYPad
    contentComponent.addAndMakeVisible(xyPadStyleLabel);
    contentComponent.addAndMakeVisible(xyPadStyleCombo);
    contentComponent.addAndMakeVisible(xyPadColorLabel);
    contentComponent.addAndMakeVisible(xyPadColorButton);
    
    // Controles de Joystick
    contentComponent.addAndMakeVisible(joystickStyleLabel);
    contentComponent.addAndMakeVisible(joystickStyleCombo);
    contentComponent.addAndMakeVisible(joystickColorLabel);
    contentComponent.addAndMakeVisible(joystickColorButton);
    
    // Controles de BasicMeter
    contentComponent.addAndMakeVisible(basicMeterStyleLabel);
    contentComponent.addAndMakeVisible(basicMeterStyleCombo);
    contentComponent.addAndMakeVisible(basicMeterColorLabel);
    contentComponent.addAndMakeVisible(basicMeterColorButton);
    
    // Controles de VUMeter
    contentComponent.addAndMakeVisible(vuMeterStyleLabel);
    contentComponent.addAndMakeVisible(vuMeterStyleCombo);
    contentComponent.addAndMakeVisible(vuMeterColorLabel);
    contentComponent.addAndMakeVisible(vuMeterColorButton);
    
    // Controles de LedMeter
    contentComponent.addAndMakeVisible(ledMeterStyleLabel);
    contentComponent.addAndMakeVisible(ledMeterStyleCombo);
    contentComponent.addAndMakeVisible(ledMeterColorLabel);
    contentComponent.addAndMakeVisible(ledMeterColorButton);
    
    // Controles de AudioScope
    contentComponent.addAndMakeVisible(audioScopeStyleLabel);
    contentComponent.addAndMakeVisible(audioScopeStyleCombo);
    contentComponent.addAndMakeVisible(audioScopeColorLabel);
    contentComponent.addAndMakeVisible(audioScopeColorButton);
    
    // Controles de VerticalFader
    contentComponent.addAndMakeVisible(verticalFaderStyleLabel);
    contentComponent.addAndMakeVisible(verticalFaderStyleCombo);
    contentComponent.addAndMakeVisible(verticalFaderColorLabel);
    contentComponent.addAndMakeVisible(verticalFaderColorButton);
    
    // Controles de ImageButton
    contentComponent.addAndMakeVisible(imageButtonStyleLabel);
    contentComponent.addAndMakeVisible(imageButtonStyleCombo);
    contentComponent.addAndMakeVisible(imageButtonColorLabel);
    contentComponent.addAndMakeVisible(imageButtonColorButton);
    
    // Controles de ToggleButton
    contentComponent.addAndMakeVisible(toggleButtonStyleLabel);
    contentComponent.addAndMakeVisible(toggleButtonStyleCombo);
    contentComponent.addAndMakeVisible(toggleButtonColorLabel);
    contentComponent.addAndMakeVisible(toggleButtonColorButton);
    
    waveformColorButton.onClick = [this]() {
        if (selectedWidget && selectedWidget->getWidgetType() == DraggableWidget::Waveform)
        {
            auto* waveform = dynamic_cast<DraggableWaveform*>(selectedWidget);
            if (waveform)
            {
                // Mostrar selector de color simple con menú popup
                showWaveformColorSelector();
            }
        }
    };
    
    // Controles de filmstrip
    contentComponent.addAndMakeVisible(filmstripLabel);
    contentComponent.addAndMakeVisible(loadFilmstripButton);
    contentComponent.addAndMakeVisible(filmstripPathLabel);
    filmstripPathLabel.setColour(juce::Label::textColourId, juce::Colours::grey);
    
    loadFilmstripButton.onClick = [this]() {
        auto chooser = std::make_shared<juce::FileChooser>("Seleccionar imagen filmstrip PNG",
                                                           juce::File::getSpecialLocation(juce::File::userDesktopDirectory),
                                                           "*.png");
        
        auto chooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;
        
        chooser->launchAsync(chooserFlags, [this, chooser](const juce::FileChooser& fc)
        {
            auto file = fc.getResult();
            if (selectedWidget && file.existsAsFile())
            {
                selectedWidget->style.filmstripPath = file.getFullPathName();
                filmstripPathLabel.setText(file.getFileName(), juce::dontSendNotification);
                selectedWidget->repaint();
            }
        });
    };
    
    // Controles de código
    contentComponent.addAndMakeVisible(varNameLabel);
    varNameEditor.setMultiLine(false);
    contentComponent.addAndMakeVisible(varNameEditor);
    
    contentComponent.addAndMakeVisible(callbackLabel);
    callbackCombo.addItem("onClick", 1);
    callbackCombo.addItem("onValueChanged", 2);
    callbackCombo.addItem("onDrag", 3);
    contentComponent.addAndMakeVisible(callbackCombo);
    
    // Configurar sliders de estilo
    borderWidthSlider.setRange(0.5, 10.0, 0.5);
    borderWidthSlider.setValue(2.0);
    borderWidthSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 50, 20);
    
    cornerRadiusSlider.setRange(0.0, 20.0, 1.0);
    cornerRadiusSlider.setValue(5.0);
    cornerRadiusSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 50, 20);
    
    fillKnobToggle.setToggleState(true, juce::dontSendNotification);
    
    // Setup viewport
    viewport.setViewedComponent(&contentComponent, false);
    viewport.setScrollBarsShown(true, false);
    addAndMakeVisible(viewport);
    
    knobStyleCombo.addItem("Circular", 1);
    knobStyleCombo.addItem("Rectangular", 2);
    knobStyleCombo.addItem("Diamante", 3);
    knobStyleCombo.addItem("Clasico", 4);
    knobStyleCombo.addItem("Vintage", 5);
    knobStyleCombo.addItem("Neon", 6);
    knobStyleCombo.addItem("Minimal", 7);
    knobStyleCombo.addItem("Dot", 8);
    knobStyleCombo.addItem("Arc Pie", 9);
    knobStyleCombo.addItem("Filmstrip", 10);
    knobStyleCombo.setSelectedId(1);
    
    sliderStyleCombo.addItem("Moderno", 1);
    sliderStyleCombo.addItem("Minimalista", 2);
    sliderStyleCombo.addItem("Volumetrico", 3);
    sliderStyleCombo.addItem("LED Bar", 4);
    sliderStyleCombo.addItem("Neon", 5);
    sliderStyleCombo.addItem("Groove", 6);
    sliderStyleCombo.addItem("Flat", 7);
    sliderStyleCombo.addItem("Vintage", 8);
    sliderStyleCombo.addItem("Circulos", 9);
    sliderStyleCombo.addItem("Lineas", 10);
    sliderStyleCombo.setSelectedId(1);
    
    buttonStyleCombo.addItem("Moderno", 1);
    buttonStyleCombo.addItem("Flat", 2);
    buttonStyleCombo.addItem("Relieve 3D", 3);
    buttonStyleCombo.addItem("Neon", 4);
    buttonStyleCombo.addItem("Outlined", 5);
    buttonStyleCombo.addItem("Pill", 6);
    buttonStyleCombo.addItem("Glass", 7);
    buttonStyleCombo.addItem("Segmentado", 8);
    buttonStyleCombo.addItem("Circular", 9);
    buttonStyleCombo.addItem("Hexagonal", 10);
    buttonStyleCombo.setSelectedId(1);
    
    labelStyleCombo.addItem("Normal", 1);
    labelStyleCombo.addItem("Bold", 2);
    labelStyleCombo.addItem("Italic", 3);
    labelStyleCombo.addItem("Large", 4);
    labelStyleCombo.addItem("Small", 5);
    labelStyleCombo.addItem("Monospace", 6);
    labelStyleCombo.addItem("Left", 7);
    labelStyleCombo.addItem("Right", 8);
    labelStyleCombo.addItem("Title", 9);
    labelStyleCombo.addItem("Subtitle", 10);
    labelStyleCombo.setSelectedId(1);
    
    waveformStyleCombo.addItem("Line", 1);
    waveformStyleCombo.addItem("Line Glow", 2);
    waveformStyleCombo.addItem("Filled", 3);
    waveformStyleCombo.addItem("Mirror", 4);
    waveformStyleCombo.addItem("Bars", 5);
    waveformStyleCombo.addItem("Dots", 6);
    waveformStyleCombo.addItem("Gradient", 7);
    waveformStyleCombo.addItem("Stepped", 8);
    waveformStyleCombo.addItem("Frequency", 9);
    waveformStyleCombo.addItem("Vectorscope", 10);
    waveformStyleCombo.setSelectedId(1);
    
    meterStyleCombo.addItem("Vertical", 1);
    meterStyleCombo.addItem("Horizontal", 2);
    meterStyleCombo.addItem("LED", 3);
    meterStyleCombo.addItem("VU Meter", 4);
    meterStyleCombo.addItem("Peak Meter", 5);
    meterStyleCombo.addItem("RMS", 6);
    meterStyleCombo.addItem("Gradient", 7);
    meterStyleCombo.addItem("Segmented", 8);
    meterStyleCombo.addItem("Circular", 9);
    meterStyleCombo.addItem("Radial", 10);
    meterStyleCombo.setSelectedId(1);
    
    xyPadStyleCombo.addItem("Circle", 1);
    xyPadStyleCombo.addItem("Square", 2);
    xyPadStyleCombo.addItem("Orbital", 3);
    xyPadStyleCombo.addItem("Crosshair", 4);
    xyPadStyleCombo.addItem("Grid", 5);
    xyPadStyleCombo.addItem("Glow", 6);
    xyPadStyleCombo.addItem("Vector", 7);
    xyPadStyleCombo.addItem("Radar", 8);
    xyPadStyleCombo.addItem("Diamond", 9);
    xyPadStyleCombo.addItem("Nebula", 10);
    xyPadStyleCombo.setSelectedId(1);
    
    joystickStyleCombo.addItem("Classic", 1);
    joystickStyleCombo.addItem("Arcade", 2);
    joystickStyleCombo.addItem("Flight", 3);
    joystickStyleCombo.addItem("Minimal", 4);
    joystickStyleCombo.addItem("Neon", 5);
    joystickStyleCombo.addItem("Mechanical", 6);
    joystickStyleCombo.addItem("Orb", 7);
    joystickStyleCombo.addItem("Target", 8);
    joystickStyleCombo.addItem("Compass", 9);
    joystickStyleCombo.addItem("Galaxy", 10);
    joystickStyleCombo.setSelectedId(1);
    
    // BasicMeter
    basicMeterStyleCombo.addItem("Gradient", 1);
    basicMeterStyleCombo.addItem("Segmented", 2);
    basicMeterStyleCombo.addItem("Neon", 3);
    basicMeterStyleCombo.addItem("Vintage", 4);
    basicMeterStyleCombo.addItem("Digital", 5);
    basicMeterStyleCombo.addItem("Plasma", 6);
    basicMeterStyleCombo.addItem("Rainbow", 7);
    basicMeterStyleCombo.addItem("Thermal", 8);
    basicMeterStyleCombo.addItem("Crystal", 9);
    basicMeterStyleCombo.addItem("Wave", 10);
    basicMeterStyleCombo.onChange = [this]() { applyToWidget(); };
    basicMeterColorButton.onClick = [this]() { showBasicMeterColorSelector(); };
    
    // VUMeter
    vuMeterStyleCombo.addItem("Analog", 1);
    vuMeterStyleCombo.addItem("Modern", 2);
    vuMeterStyleCombo.addItem("Retro", 3);
    vuMeterStyleCombo.addItem("Chrome", 4);
    vuMeterStyleCombo.addItem("Wood", 5);
    vuMeterStyleCombo.addItem("Steampunk", 6);
    vuMeterStyleCombo.addItem("Neon", 7);
    vuMeterStyleCombo.addItem("Digital", 8);
    vuMeterStyleCombo.addItem("Minimal", 9);
    vuMeterStyleCombo.addItem("Luxury", 10);
    vuMeterStyleCombo.onChange = [this]() { applyToWidget(); };
    vuMeterColorButton.onClick = [this]() { showVUMeterColorSelector(); };
    
    // LedMeter
    ledMeterStyleCombo.addItem("Classic", 1);
    ledMeterStyleCombo.addItem("Round", 2);
    ledMeterStyleCombo.addItem("Square", 3);
    ledMeterStyleCombo.addItem("Diamond", 4);
    ledMeterStyleCombo.addItem("Bar", 5);
    ledMeterStyleCombo.addItem("Dot", 6);
    ledMeterStyleCombo.addItem("Glow", 7);
    ledMeterStyleCombo.addItem("Neon", 8);
    ledMeterStyleCombo.addItem("Matrix", 9);
    ledMeterStyleCombo.addItem("Spectrum", 10);
    ledMeterStyleCombo.onChange = [this]() { applyToWidget(); };
    ledMeterColorButton.onClick = [this]() { showLedMeterColorSelector(); };
    
    // AudioScope
    audioScopeStyleCombo.addItem("Waveform", 1);
    audioScopeStyleCombo.addItem("Spectrum", 2);
    audioScopeStyleCombo.addItem("Vectorscope", 3);
    audioScopeStyleCombo.addItem("Lissajous", 4);
    audioScopeStyleCombo.addItem("Waterfall", 5);
    audioScopeStyleCombo.addItem("Spectrogram", 6);
    audioScopeStyleCombo.addItem("Phase", 7);
    audioScopeStyleCombo.addItem("Goniometer", 8);
    audioScopeStyleCombo.addItem("Oscilloscope", 9);
    audioScopeStyleCombo.addItem("FFT", 10);
    audioScopeStyleCombo.onChange = [this]() { applyToWidget(); };
    audioScopeColorButton.onClick = [this]() { showAudioScopeColorSelector(); };
    
    // VerticalFader
    verticalFaderStyleCombo.addItem("Modern", 1);
    verticalFaderStyleCombo.addItem("Classic", 2);
    verticalFaderStyleCombo.addItem("Minimal", 3);
    verticalFaderStyleCombo.addItem("Studio", 4);
    verticalFaderStyleCombo.addItem("Vintage", 5);
    verticalFaderStyleCombo.addItem("Neon", 6);
    verticalFaderStyleCombo.addItem("Glass", 7);
    verticalFaderStyleCombo.addItem("Metal", 8);
    verticalFaderStyleCombo.addItem("Wood", 9);
    verticalFaderStyleCombo.addItem("Digital", 10);
    verticalFaderStyleCombo.onChange = [this]() { applyToWidget(); };
    verticalFaderColorButton.onClick = [this]() { showVerticalFaderColorSelector(); };
    
    // ImageButton
    imageButtonStyleCombo.addItem("Flat", 1);
    imageButtonStyleCombo.addItem("Raised", 2);
    imageButtonStyleCombo.addItem("Rounded", 3);
    imageButtonStyleCombo.addItem("Circle", 4);
    imageButtonStyleCombo.addItem("Neon", 5);
    imageButtonStyleCombo.addItem("Glass", 6);
    imageButtonStyleCombo.addItem("Metal", 7);
    imageButtonStyleCombo.addItem("Glow", 8);
    imageButtonStyleCombo.addItem("Shadow", 9);
    imageButtonStyleCombo.addItem("Gradient", 10);
    imageButtonStyleCombo.onChange = [this]() { applyToWidget(); };
    imageButtonColorButton.onClick = [this]() { showImageButtonColorSelector(); };
    
    // ToggleButton
    toggleButtonStyleCombo.addItem("Checkbox", 1);
    toggleButtonStyleCombo.addItem("Switch", 2);
    toggleButtonStyleCombo.addItem("Radio", 3);
    toggleButtonStyleCombo.addItem("LED", 4);
    toggleButtonStyleCombo.addItem("Button", 5);
    toggleButtonStyleCombo.addItem("Slider", 6);
    toggleButtonStyleCombo.addItem("Rocker", 7);
    toggleButtonStyleCombo.addItem("Flip", 8);
    toggleButtonStyleCombo.addItem("Power", 9);
    toggleButtonStyleCombo.addItem("Custom", 10);
    toggleButtonStyleCombo.onChange = [this]() { applyToWidget(); };
    toggleButtonColorButton.onClick = [this]() { showToggleButtonColorSelector(); };
    
    // Callbacks para actualizar el widget
    xSlider.onValueChange = [this]() { applyToWidget(); };
    ySlider.onValueChange = [this]() { applyToWidget(); };
    widthSlider.onValueChange = [this]() { applyToWidget(); };
    heightSlider.onValueChange = [this]() { applyToWidget(); };
    
    borderWidthSlider.onValueChange = [this]() { applyToWidget(); };
    cornerRadiusSlider.onValueChange = [this]() { applyToWidget(); };
    fillKnobToggle.onClick = [this]() { applyToWidget(); };
    knobStyleCombo.onChange = [this]() { applyToWidget(); };
    sliderStyleCombo.onChange = [this]() { applyToWidget(); };
    buttonStyleCombo.onChange = [this]() { applyToWidget(); };
    labelStyleCombo.onChange = [this]() { applyToWidget(); };
    waveformStyleCombo.onChange = [this]() { applyToWidget(); };
    meterStyleCombo.onChange = [this]() { applyToWidget(); };
    xyPadStyleCombo.onChange = [this]() { applyToWidget(); };
    joystickStyleCombo.onChange = [this]() { applyToWidget(); };
    
    waveformColorButton.onClick = [this]() { showWaveformColorSelector(); };
    meterColorButton.onClick = [this]() { showMeterColorSelector(); };
    xyPadColorButton.onClick = [this]() { showXYPadColorSelector(); };
    joystickColorButton.onClick = [this]() { showJoystickColorSelector(); };
    
    clearSelection();
}

void PropertyInspector::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff1a1a2e));
    
    g.setColour(juce::Colour(0xff00adb5));
    g.drawRect(getLocalBounds(), 2);
}

void PropertyInspector::resized()
{
    auto bounds = getLocalBounds();
    
    titleLabel.setBounds(bounds.removeFromTop(40));
    
    viewport.setBounds(bounds);
    
    int yPos = 10;
    int contentWidth = getWidth() - 20;
    int leftMargin = 10;
    
    if (selectedWidget == nullptr)
    {
        noSelectionLabel.setBounds(leftMargin, yPos, contentWidth - 20, 50);
        contentComponent.setSize(contentWidth, yPos + 60);
        return;
    }
    
    // Posición y tamaño
    positionLabel.setBounds(leftMargin, yPos, contentWidth - 20, 25);
    yPos += 30;
    
    xLabel.setBounds(leftMargin, yPos, 50, 25);
    xSlider.setBounds(leftMargin + 50, yPos, contentWidth - 70, 25);
    yPos += 30;
    
    yLabel.setBounds(leftMargin, yPos, 50, 25);
    ySlider.setBounds(leftMargin + 50, yPos, contentWidth - 70, 25);
    yPos += 30;
    
    widthLabel.setBounds(leftMargin, yPos, 50, 25);
    widthSlider.setBounds(leftMargin + 50, yPos, contentWidth - 70, 25);
    yPos += 30;
    
    heightLabel.setBounds(leftMargin, yPos, 50, 25);
    heightSlider.setBounds(leftMargin + 50, yPos, contentWidth - 70, 25);
    yPos += 35;
    
    // Apariencia
    appearanceLabel.setBounds(leftMargin, yPos, contentWidth - 20, 25);
    yPos += 30;
    
    nameLabel.setBounds(leftMargin, yPos, contentWidth - 20, 20);
    yPos += 20;
    nameEditor.setBounds(leftMargin, yPos, contentWidth - 20, 25);
    yPos += 30;
    
    bgColorLabel.setBounds(leftMargin, yPos, contentWidth - 20, 20);
    yPos += 20;
    bgColorButton.setBounds(leftMargin, yPos, contentWidth - 20, 30);
    yPos += 35;
    
    borderColorLabel.setBounds(leftMargin, yPos, contentWidth - 20, 20);
    yPos += 20;
    borderColorButton.setBounds(leftMargin, yPos, contentWidth - 20, 30);
    yPos += 35;
    
    knobColorLabel.setBounds(leftMargin, yPos, contentWidth - 20, 20);
    yPos += 20;
    knobColorButton.setBounds(leftMargin, yPos, contentWidth - 20, 30);
    yPos += 35;
    
    textColorLabel.setBounds(leftMargin, yPos, contentWidth - 20, 20);
    yPos += 20;
    textColorButton.setBounds(leftMargin, yPos, contentWidth - 20, 30);
    yPos += 35;
    
    borderWidthLabel.setBounds(leftMargin, yPos, contentWidth - 20, 20);
    yPos += 20;
    borderWidthSlider.setBounds(leftMargin, yPos, contentWidth - 20, 25);
    yPos += 30;
    
    cornerRadiusLabel.setBounds(leftMargin, yPos, contentWidth - 20, 20);
    yPos += 20;
    cornerRadiusSlider.setBounds(leftMargin, yPos, contentWidth - 20, 25);
    yPos += 30;
    
    fillKnobLabel.setBounds(leftMargin, yPos, contentWidth - 20, 20);
    yPos += 20;
    fillKnobToggle.setBounds(leftMargin, yPos, contentWidth - 20, 25);
    yPos += 30;
    
    knobStyleLabel.setBounds(leftMargin, yPos, contentWidth - 20, 20);
    yPos += 20;
    knobStyleCombo.setBounds(leftMargin, yPos, contentWidth - 20, 25);
    yPos += 30;
    
    knobFilmstripLabel.setBounds(leftMargin, yPos, contentWidth - 20, 20);
    yPos += 20;
    loadKnobFilmstripButton.setBounds(leftMargin, yPos, contentWidth - 20, 30);
    yPos += 30;
    knobFilmstripPathLabel.setBounds(leftMargin, yPos, contentWidth - 20, 20);
    yPos += 35;
    
    sliderStyleLabel.setBounds(leftMargin, yPos, contentWidth - 20, 20);
    yPos += 20;
    sliderStyleCombo.setBounds(leftMargin, yPos, contentWidth - 20, 25);
    yPos += 30;
    
    sliderFilmstripLabel.setBounds(leftMargin, yPos, contentWidth - 20, 20);
    yPos += 20;
    loadSliderFilmstripButton.setBounds(leftMargin, yPos, contentWidth - 20, 30);
    yPos += 30;
    sliderFilmstripPathLabel.setBounds(leftMargin, yPos, contentWidth - 20, 20);
    yPos += 35;
    
    buttonStyleLabel.setBounds(leftMargin, yPos, contentWidth - 20, 20);
    yPos += 20;
    buttonStyleCombo.setBounds(leftMargin, yPos, contentWidth - 20, 25);
    yPos += 30;
    
    buttonFilmstripLabel.setBounds(leftMargin, yPos, contentWidth - 20, 20);
    yPos += 20;
    loadButtonFilmstripButton.setBounds(leftMargin, yPos, contentWidth - 20, 30);
    yPos += 30;
    buttonFilmstripPathLabel.setBounds(leftMargin, yPos, contentWidth - 20, 20);
    yPos += 35;
    
    labelStyleLabel.setBounds(leftMargin, yPos, contentWidth - 20, 20);
    yPos += 20;
    labelStyleCombo.setBounds(leftMargin, yPos, contentWidth - 20, 25);
    yPos += 30;
    
    labelFilmstripLabel.setBounds(leftMargin, yPos, contentWidth - 20, 20);
    yPos += 20;
    loadLabelFilmstripButton.setBounds(leftMargin, yPos, contentWidth - 20, 30);
    yPos += 30;
    labelFilmstripPathLabel.setBounds(leftMargin, yPos, contentWidth - 20, 20);
    yPos += 35;
    
    waveformStyleLabel.setBounds(leftMargin, yPos, contentWidth - 20, 20);
    yPos += 20;
    waveformStyleCombo.setBounds(leftMargin, yPos, contentWidth - 20, 25);
    yPos += 30;
    waveformColorLabel.setBounds(leftMargin, yPos, contentWidth - 20, 20);
    yPos += 20;
    waveformColorButton.setBounds(leftMargin, yPos, contentWidth - 20, 30);
    yPos += 35;
    
    meterStyleLabel.setBounds(leftMargin, yPos, contentWidth - 20, 20);
    yPos += 20;
    meterStyleCombo.setBounds(leftMargin, yPos, contentWidth - 20, 25);
    yPos += 30;
    meterColorLabel.setBounds(leftMargin, yPos, contentWidth - 20, 20);
    yPos += 20;
    meterColorButton.setBounds(leftMargin, yPos, contentWidth - 20, 30);
    yPos += 35;
    
    xyPadStyleLabel.setBounds(leftMargin, yPos, contentWidth - 20, 20);
    yPos += 20;
    xyPadStyleCombo.setBounds(leftMargin, yPos, contentWidth - 20, 25);
    yPos += 30;
    xyPadColorLabel.setBounds(leftMargin, yPos, contentWidth - 20, 20);
    yPos += 20;
    xyPadColorButton.setBounds(leftMargin, yPos, contentWidth - 20, 30);
    yPos += 35;
    
    joystickStyleLabel.setBounds(leftMargin, yPos, contentWidth - 20, 20);
    yPos += 20;
    joystickStyleCombo.setBounds(leftMargin, yPos, contentWidth - 20, 25);
    yPos += 30;
    joystickColorLabel.setBounds(leftMargin, yPos, contentWidth - 20, 20);
    yPos += 20;
    joystickColorButton.setBounds(leftMargin, yPos, contentWidth - 20, 30);
    yPos += 35;
    
    basicMeterStyleLabel.setBounds(leftMargin, yPos, contentWidth - 20, 20);
    yPos += 20;
    basicMeterStyleCombo.setBounds(leftMargin, yPos, contentWidth - 20, 25);
    yPos += 30;
    basicMeterColorLabel.setBounds(leftMargin, yPos, contentWidth - 20, 20);
    yPos += 20;
    basicMeterColorButton.setBounds(leftMargin, yPos, contentWidth - 20, 30);
    yPos += 35;
    
    vuMeterStyleLabel.setBounds(leftMargin, yPos, contentWidth - 20, 20);
    yPos += 20;
    vuMeterStyleCombo.setBounds(leftMargin, yPos, contentWidth - 20, 25);
    yPos += 30;
    vuMeterColorLabel.setBounds(leftMargin, yPos, contentWidth - 20, 20);
    yPos += 20;
    vuMeterColorButton.setBounds(leftMargin, yPos, contentWidth - 20, 30);
    yPos += 35;
    
    ledMeterStyleLabel.setBounds(leftMargin, yPos, contentWidth - 20, 20);
    yPos += 20;
    ledMeterStyleCombo.setBounds(leftMargin, yPos, contentWidth - 20, 25);
    yPos += 30;
    ledMeterColorLabel.setBounds(leftMargin, yPos, contentWidth - 20, 20);
    yPos += 20;
    ledMeterColorButton.setBounds(leftMargin, yPos, contentWidth - 20, 30);
    yPos += 35;
    
    audioScopeStyleLabel.setBounds(leftMargin, yPos, contentWidth - 20, 20);
    yPos += 20;
    audioScopeStyleCombo.setBounds(leftMargin, yPos, contentWidth - 20, 25);
    yPos += 30;
    audioScopeColorLabel.setBounds(leftMargin, yPos, contentWidth - 20, 20);
    yPos += 20;
    audioScopeColorButton.setBounds(leftMargin, yPos, contentWidth - 20, 30);
    yPos += 35;
    
    verticalFaderStyleLabel.setBounds(leftMargin, yPos, contentWidth - 20, 20);
    yPos += 20;
    verticalFaderStyleCombo.setBounds(leftMargin, yPos, contentWidth - 20, 25);
    yPos += 30;
    verticalFaderColorLabel.setBounds(leftMargin, yPos, contentWidth - 20, 20);
    yPos += 20;
    verticalFaderColorButton.setBounds(leftMargin, yPos, contentWidth - 20, 30);
    yPos += 35;
    
    imageButtonStyleLabel.setBounds(leftMargin, yPos, contentWidth - 20, 20);
    yPos += 20;
    imageButtonStyleCombo.setBounds(leftMargin, yPos, contentWidth - 20, 25);
    yPos += 30;
    imageButtonColorLabel.setBounds(leftMargin, yPos, contentWidth - 20, 20);
    yPos += 20;
    imageButtonColorButton.setBounds(leftMargin, yPos, contentWidth - 20, 30);
    yPos += 35;
    
    toggleButtonStyleLabel.setBounds(leftMargin, yPos, contentWidth - 20, 20);
    yPos += 20;
    toggleButtonStyleCombo.setBounds(leftMargin, yPos, contentWidth - 20, 25);
    yPos += 30;
    toggleButtonColorLabel.setBounds(leftMargin, yPos, contentWidth - 20, 20);
    yPos += 20;
    toggleButtonColorButton.setBounds(leftMargin, yPos, contentWidth - 20, 30);
    yPos += 35;
    
    filmstripLabel.setBounds(leftMargin, yPos, contentWidth - 20, 20);
    yPos += 20;
    loadFilmstripButton.setBounds(leftMargin, yPos, contentWidth - 20, 30);
    yPos += 30;
    filmstripPathLabel.setBounds(leftMargin, yPos, contentWidth - 20, 20);
    yPos += 35;
    
    // Código
    codeLabel.setBounds(leftMargin, yPos, contentWidth - 20, 25);
    yPos += 30;
    
    varNameLabel.setBounds(leftMargin, yPos, contentWidth - 20, 20);
    yPos += 20;
    varNameEditor.setBounds(leftMargin, yPos, contentWidth - 20, 25);
    yPos += 30;
    
    callbackLabel.setBounds(leftMargin, yPos, contentWidth - 20, 20);
    yPos += 20;
    callbackCombo.setBounds(leftMargin, yPos, contentWidth - 20, 25);
    yPos += 40;
    
    // Set content height
    contentComponent.setSize(contentWidth, yPos);
}

void PropertyInspector::setSelectedWidget(DraggableWidget* widget)
{
    selectedWidget = widget;
    updateFromWidget();
    resized();
    repaint();
}

void PropertyInspector::clearSelection()
{
    selectedWidget = nullptr;
    
    // Ocultar todos los controles
    positionLabel.setVisible(false);
    xLabel.setVisible(false);
    yLabel.setVisible(false);
    widthLabel.setVisible(false);
    heightLabel.setVisible(false);
    xSlider.setVisible(false);
    ySlider.setVisible(false);
    widthSlider.setVisible(false);
    heightSlider.setVisible(false);
    
    appearanceLabel.setVisible(false);
    nameLabel.setVisible(false);
    nameEditor.setVisible(false);
    bgColorLabel.setVisible(false);
    bgColorButton.setVisible(false);
    borderColorLabel.setVisible(false);
    borderColorButton.setVisible(false);
    knobColorLabel.setVisible(false);
    knobColorButton.setVisible(false);
    textColorLabel.setVisible(false);
    textColorButton.setVisible(false);
    borderWidthLabel.setVisible(false);
    borderWidthSlider.setVisible(false);
    cornerRadiusLabel.setVisible(false);
    cornerRadiusSlider.setVisible(false);
    fillKnobLabel.setVisible(false);
    fillKnobToggle.setVisible(false);
    knobStyleLabel.setVisible(false);
    knobStyleCombo.setVisible(false);
    knobFilmstripLabel.setVisible(false);
    loadKnobFilmstripButton.setVisible(false);
    knobFilmstripPathLabel.setVisible(false);
    sliderStyleLabel.setVisible(false);
    sliderStyleCombo.setVisible(false);
    sliderFilmstripLabel.setVisible(false);
    loadSliderFilmstripButton.setVisible(false);
    sliderFilmstripPathLabel.setVisible(false);
    buttonStyleLabel.setVisible(false);
    buttonStyleCombo.setVisible(false);
    buttonFilmstripLabel.setVisible(false);
    loadButtonFilmstripButton.setVisible(false);
    buttonFilmstripPathLabel.setVisible(false);
    labelStyleLabel.setVisible(false);
    labelStyleCombo.setVisible(false);
    labelFilmstripLabel.setVisible(false);
    loadLabelFilmstripButton.setVisible(false);
    labelFilmstripPathLabel.setVisible(false);
    waveformStyleLabel.setVisible(false);
    waveformStyleCombo.setVisible(false);
    waveformColorLabel.setVisible(false);
    waveformColorButton.setVisible(false);
    meterStyleLabel.setVisible(false);
    meterStyleCombo.setVisible(false);
    meterColorLabel.setVisible(false);
    meterColorButton.setVisible(false);
    xyPadStyleLabel.setVisible(false);
    xyPadStyleCombo.setVisible(false);
    xyPadColorLabel.setVisible(false);
    xyPadColorButton.setVisible(false);
    joystickStyleLabel.setVisible(false);
    joystickStyleCombo.setVisible(false);
    joystickColorLabel.setVisible(false);
    joystickColorButton.setVisible(false);
    basicMeterStyleLabel.setVisible(false);
    basicMeterStyleCombo.setVisible(false);
    basicMeterColorLabel.setVisible(false);
    basicMeterColorButton.setVisible(false);
    vuMeterStyleLabel.setVisible(false);
    vuMeterStyleCombo.setVisible(false);
    vuMeterColorLabel.setVisible(false);
    vuMeterColorButton.setVisible(false);
    ledMeterStyleLabel.setVisible(false);
    ledMeterStyleCombo.setVisible(false);
    ledMeterColorLabel.setVisible(false);
    ledMeterColorButton.setVisible(false);
    audioScopeStyleLabel.setVisible(false);
    audioScopeStyleCombo.setVisible(false);
    audioScopeColorLabel.setVisible(false);
    audioScopeColorButton.setVisible(false);
    verticalFaderStyleLabel.setVisible(false);
    verticalFaderStyleCombo.setVisible(false);
    verticalFaderColorLabel.setVisible(false);
    verticalFaderColorButton.setVisible(false);
    imageButtonStyleLabel.setVisible(false);
    imageButtonStyleCombo.setVisible(false);
    imageButtonColorLabel.setVisible(false);
    imageButtonColorButton.setVisible(false);
    toggleButtonStyleLabel.setVisible(false);
    toggleButtonStyleCombo.setVisible(false);
    toggleButtonColorLabel.setVisible(false);
    toggleButtonColorButton.setVisible(false);
    buttonStyleLabel.setVisible(false);
    buttonStyleCombo.setVisible(false);
    labelStyleLabel.setVisible(false);
    labelStyleCombo.setVisible(false);
    filmstripLabel.setVisible(false);
    loadFilmstripButton.setVisible(false);
    filmstripPathLabel.setVisible(false);
    
    codeLabel.setVisible(false);
    varNameLabel.setVisible(false);
    varNameEditor.setVisible(false);
    callbackLabel.setVisible(false);
    callbackCombo.setVisible(false);
    
    noSelectionLabel.setVisible(true);
    repaint();
}

void PropertyInspector::updateFromWidget()
{
    if (selectedWidget == nullptr)
        return;
    
    // Mostrar controles
    positionLabel.setVisible(true);
    xLabel.setVisible(true);
    yLabel.setVisible(true);
    widthLabel.setVisible(true);
    heightLabel.setVisible(true);
    xSlider.setVisible(true);
    ySlider.setVisible(true);
    widthSlider.setVisible(true);
    heightSlider.setVisible(true);
    
    appearanceLabel.setVisible(true);
    nameLabel.setVisible(true);
    nameEditor.setVisible(true);
    bgColorLabel.setVisible(true);
    bgColorButton.setVisible(true);
    borderColorLabel.setVisible(true);
    borderColorButton.setVisible(true);
    knobColorLabel.setVisible(true);
    knobColorButton.setVisible(true);
    textColorLabel.setVisible(true);
    textColorButton.setVisible(true);
    knobColorLabel.setVisible(true);
    knobColorButton.setVisible(true);
    textColorLabel.setVisible(true);
    textColorButton.setVisible(true);
    borderWidthLabel.setVisible(true);
    borderWidthSlider.setVisible(true);
    cornerRadiusLabel.setVisible(true);
    cornerRadiusSlider.setVisible(true);
    fillKnobLabel.setVisible(true);
    fillKnobToggle.setVisible(true);
    
    // Mostrar combos según el tipo de widget
    auto widgetType = selectedWidget->getWidgetType();
    bool isKnob = (widgetType == DraggableWidget::WidgetType::Knob);
    bool isSlider = (widgetType == DraggableWidget::WidgetType::Slider);
    bool isButton = (widgetType == DraggableWidget::WidgetType::Button);
    bool isLabel = (widgetType == DraggableWidget::WidgetType::Label);
    bool isWaveform = (widgetType == DraggableWidget::WidgetType::Waveform);
    bool isMeter = (widgetType == DraggableWidget::WidgetType::Meter);
    bool isXYPad = (widgetType == DraggableWidget::WidgetType::XYPad);
    bool isJoystick = (widgetType == DraggableWidget::WidgetType::Joystick);
    bool isBasicMeter = (widgetType == DraggableWidget::WidgetType::BasicMeter);
    bool isVUMeter = (widgetType == DraggableWidget::WidgetType::VUMeter);
    bool isLedMeter = (widgetType == DraggableWidget::WidgetType::LedMeter);
    bool isAudioScope = (widgetType == DraggableWidget::WidgetType::AudioScope);
    bool isVerticalFader = (widgetType == DraggableWidget::WidgetType::VerticalFader);
    bool isImageButton = (widgetType == DraggableWidget::WidgetType::ImageButton);
    bool isToggleButton = (widgetType == DraggableWidget::WidgetType::ToggleButton);
    
    fillKnobLabel.setVisible(isKnob);
    fillKnobToggle.setVisible(isKnob);
    knobStyleLabel.setVisible(isKnob);
    knobStyleCombo.setVisible(isKnob);
    knobFilmstripLabel.setVisible(isKnob);
    loadKnobFilmstripButton.setVisible(isKnob);
    knobFilmstripPathLabel.setVisible(isKnob);
    sliderStyleLabel.setVisible(isSlider);
    sliderStyleCombo.setVisible(isSlider);
    sliderFilmstripLabel.setVisible(isSlider);
    loadSliderFilmstripButton.setVisible(isSlider);
    sliderFilmstripPathLabel.setVisible(isSlider);
    buttonStyleLabel.setVisible(isButton);
    buttonStyleCombo.setVisible(isButton);
    buttonFilmstripLabel.setVisible(isButton);
    loadButtonFilmstripButton.setVisible(isButton);
    buttonFilmstripPathLabel.setVisible(isButton);
    labelStyleLabel.setVisible(isLabel);
    labelStyleCombo.setVisible(isLabel);
    labelFilmstripLabel.setVisible(isLabel);
    loadLabelFilmstripButton.setVisible(isLabel);
    labelFilmstripPathLabel.setVisible(isLabel);
    waveformStyleLabel.setVisible(isWaveform);
    waveformStyleCombo.setVisible(isWaveform);
    waveformColorLabel.setVisible(isWaveform);
    waveformColorButton.setVisible(isWaveform);
    meterStyleLabel.setVisible(isMeter);
    meterStyleCombo.setVisible(isMeter);
    meterColorLabel.setVisible(isMeter);
    meterColorButton.setVisible(isMeter);
    xyPadStyleLabel.setVisible(isXYPad);
    xyPadStyleCombo.setVisible(isXYPad);
    xyPadColorLabel.setVisible(isXYPad);
    xyPadColorButton.setVisible(isXYPad);
    joystickStyleLabel.setVisible(isJoystick);
    joystickStyleCombo.setVisible(isJoystick);
    joystickColorLabel.setVisible(isJoystick);
    joystickColorButton.setVisible(isJoystick);
    basicMeterStyleLabel.setVisible(isBasicMeter);
    basicMeterStyleCombo.setVisible(isBasicMeter);
    basicMeterColorLabel.setVisible(isBasicMeter);
    basicMeterColorButton.setVisible(isBasicMeter);
    vuMeterStyleLabel.setVisible(isVUMeter);
    vuMeterStyleCombo.setVisible(isVUMeter);
    vuMeterColorLabel.setVisible(isVUMeter);
    vuMeterColorButton.setVisible(isVUMeter);
    ledMeterStyleLabel.setVisible(isLedMeter);
    ledMeterStyleCombo.setVisible(isLedMeter);
    ledMeterColorLabel.setVisible(isLedMeter);
    ledMeterColorButton.setVisible(isLedMeter);
    audioScopeStyleLabel.setVisible(isAudioScope);
    audioScopeStyleCombo.setVisible(isAudioScope);
    audioScopeColorLabel.setVisible(isAudioScope);
    audioScopeColorButton.setVisible(isAudioScope);
    verticalFaderStyleLabel.setVisible(isVerticalFader);
    verticalFaderStyleCombo.setVisible(isVerticalFader);
    verticalFaderColorLabel.setVisible(isVerticalFader);
    verticalFaderColorButton.setVisible(isVerticalFader);
    imageButtonStyleLabel.setVisible(isImageButton);
    imageButtonStyleCombo.setVisible(isImageButton);
    imageButtonColorLabel.setVisible(isImageButton);
    imageButtonColorButton.setVisible(isImageButton);
    toggleButtonStyleLabel.setVisible(isToggleButton);
    toggleButtonStyleCombo.setVisible(isToggleButton);
    toggleButtonColorLabel.setVisible(isToggleButton);
    toggleButtonColorButton.setVisible(isToggleButton);
    
    filmstripLabel.setVisible(true);
    loadFilmstripButton.setVisible(true);
    filmstripPathLabel.setVisible(true);
    
    codeLabel.setVisible(true);
    varNameLabel.setVisible(true);
    varNameEditor.setVisible(true);
    callbackLabel.setVisible(true);
    callbackCombo.setVisible(true);
    
    noSelectionLabel.setVisible(false);
    
    // Actualizar valores de posición y tamaño
    auto bounds = selectedWidget->getBounds();
    xSlider.setValue(bounds.getX(), juce::dontSendNotification);
    ySlider.setValue(bounds.getY(), juce::dontSendNotification);
    widthSlider.setValue(bounds.getWidth(), juce::dontSendNotification);
    heightSlider.setValue(bounds.getHeight(), juce::dontSendNotification);
    
    // Actualizar valores de estilo
    auto style = selectedWidget->getStyle();
    bgColorButton.setColour(juce::TextButton::buttonColourId, style.bgColor);
    borderColorButton.setColour(juce::TextButton::buttonColourId, style.borderColor);
    knobColorButton.setColour(juce::TextButton::buttonColourId, style.knobColor);
    textColorButton.setColour(juce::TextButton::buttonColourId, style.textColor);
    
    borderWidthSlider.setValue(style.borderWidth, juce::dontSendNotification);
    cornerRadiusSlider.setValue(style.cornerRadius, juce::dontSendNotification);
    fillKnobToggle.setToggleState(style.fillKnob, juce::dontSendNotification);
    knobStyleCombo.setSelectedId(style.knobStyle + 1, juce::dontSendNotification);
    sliderStyleCombo.setSelectedId(style.sliderStyle + 1, juce::dontSendNotification);
    buttonStyleCombo.setSelectedId(style.buttonStyle + 1, juce::dontSendNotification);
    labelStyleCombo.setSelectedId(style.labelStyle + 1, juce::dontSendNotification);
    
    // Actualizar estilo de waveform si es un waveform
    if (isWaveform)
    {
        auto* waveform = dynamic_cast<DraggableWaveform*>(selectedWidget);
        if (waveform)
        {
            waveformStyleCombo.setSelectedId(waveform->getStyle() + 1, juce::dontSendNotification);
        }
    }
    
    if (isMeter)
    {
        auto* meter = dynamic_cast<DraggableMeter*>(selectedWidget);
        if (meter)
        {
            meterStyleCombo.setSelectedId(meter->getStyle() + 1, juce::dontSendNotification);
        }
    }
    
    if (isXYPad)
    {
        auto* xypad = dynamic_cast<DraggableXYPad*>(selectedWidget);
        if (xypad)
        {
            xyPadStyleCombo.setSelectedId(xypad->getStyle() + 1, juce::dontSendNotification);
        }
    }
    
    if (isJoystick)
    {
        auto* joystick = dynamic_cast<DraggableJoystick*>(selectedWidget);
        if (joystick)
        {
            joystickStyleCombo.setSelectedId(joystick->getStyle() + 1, juce::dontSendNotification);
        }
    }
    
    if (isBasicMeter)
    {
        auto* basicMeter = dynamic_cast<DraggableBasicMeter*>(selectedWidget);
        if (basicMeter)
        {
            basicMeterStyleCombo.setSelectedId(basicMeter->getStyle() + 1, juce::dontSendNotification);
        }
    }
    
    if (isVUMeter)
    {
        auto* vuMeter = dynamic_cast<DraggableVUMeter*>(selectedWidget);
        if (vuMeter)
        {
            vuMeterStyleCombo.setSelectedId(vuMeter->getStyle() + 1, juce::dontSendNotification);
        }
    }
    
    if (isLedMeter)
    {
        auto* ledMeter = dynamic_cast<DraggableLedMeter*>(selectedWidget);
        if (ledMeter)
        {
            ledMeterStyleCombo.setSelectedId(ledMeter->getStyle() + 1, juce::dontSendNotification);
        }
    }
    
    if (isAudioScope)
    {
        auto* audioScope = dynamic_cast<DraggableAudioScope*>(selectedWidget);
        if (audioScope)
        {
            audioScopeStyleCombo.setSelectedId(audioScope->getStyle() + 1, juce::dontSendNotification);
        }
    }
    
    if (isVerticalFader)
    {
        auto* verticalFader = dynamic_cast<DraggableVerticalFader*>(selectedWidget);
        if (verticalFader)
        {
            verticalFaderStyleCombo.setSelectedId(verticalFader->getStyle() + 1, juce::dontSendNotification);
        }
    }
    
    if (isImageButton)
    {
        auto* imageButton = dynamic_cast<DraggableImageButton*>(selectedWidget);
        if (imageButton)
        {
            imageButtonStyleCombo.setSelectedId(imageButton->getStyle() + 1, juce::dontSendNotification);
        }
    }
    
    if (isToggleButton)
    {
        auto* toggleButton = dynamic_cast<DraggableToggleButton*>(selectedWidget);
        if (toggleButton)
        {
            toggleButtonStyleCombo.setSelectedId(toggleButton->getStyle() + 1, juce::dontSendNotification);
        }
    }
    
    // Actualizar filmstrip path
    if (style.filmstripPath.isNotEmpty())
    {
        juce::File imageFile(style.filmstripPath);
        filmstripPathLabel.setText(imageFile.getFileName(), juce::dontSendNotification);
        knobFilmstripPathLabel.setText(imageFile.getFileName(), juce::dontSendNotification);
        sliderFilmstripPathLabel.setText(imageFile.getFileName(), juce::dontSendNotification);
        buttonFilmstripPathLabel.setText(imageFile.getFileName(), juce::dontSendNotification);
        labelFilmstripPathLabel.setText(imageFile.getFileName(), juce::dontSendNotification);
    }
    else
    {
        filmstripPathLabel.setText("Sin imagen", juce::dontSendNotification);
        knobFilmstripPathLabel.setText("Sin imagen", juce::dontSendNotification);
        sliderFilmstripPathLabel.setText("Sin imagen", juce::dontSendNotification);
        buttonFilmstripPathLabel.setText("Sin imagen", juce::dontSendNotification);
        labelFilmstripPathLabel.setText("Sin imagen", juce::dontSendNotification);
    }
    
    nameEditor.setText(selectedWidget->getWidgetName());
    varNameEditor.setText(selectedWidget->getWidgetName().replaceCharacter(' ', '_'));
}

void PropertyInspector::applyToWidget()
{
    if (selectedWidget == nullptr)
        return;
    
    // Aplicar posición y tamaño
    selectedWidget->setBounds(
        static_cast<int>(xSlider.getValue()),
        static_cast<int>(ySlider.getValue()),
        static_cast<int>(widthSlider.getValue()),
        static_cast<int>(heightSlider.getValue())
    );
    
    // Aplicar estilo
    auto style = selectedWidget->getStyle();
    style.borderWidth = static_cast<float>(borderWidthSlider.getValue());
    style.cornerRadius = static_cast<float>(cornerRadiusSlider.getValue());
    style.fillKnob = fillKnobToggle.getToggleState();
    style.knobStyle = knobStyleCombo.getSelectedId() - 1;
    style.sliderStyle = sliderStyleCombo.getSelectedId() - 1;
    style.buttonStyle = buttonStyleCombo.getSelectedId() - 1;
    style.labelStyle = labelStyleCombo.getSelectedId() - 1;
    selectedWidget->setStyle(style);
    
    // Aplicar estilo de waveform si es necesario
    if (selectedWidget->getWidgetType() == DraggableWidget::WidgetType::Waveform)
    {
        auto* waveform = dynamic_cast<DraggableWaveform*>(selectedWidget);
        if (waveform)
        {
            waveform->setStyle(waveformStyleCombo.getSelectedId() - 1);
        }
    }
    
    // Aplicar estilo de meter si es necesario
    if (selectedWidget->getWidgetType() == DraggableWidget::WidgetType::Meter)
    {
        auto* meter = dynamic_cast<DraggableMeter*>(selectedWidget);
        if (meter)
        {
            meter->setStyle(meterStyleCombo.getSelectedId() - 1);
        }
    }
    
    // Aplicar estilo de XYPad si es necesario
    if (selectedWidget->getWidgetType() == DraggableWidget::WidgetType::XYPad)
    {
        auto* xypad = dynamic_cast<DraggableXYPad*>(selectedWidget);
        if (xypad)
        {
            xypad->setStyle(xyPadStyleCombo.getSelectedId() - 1);
        }
    }
    
    // Aplicar estilo de Joystick si es necesario
    if (selectedWidget->getWidgetType() == DraggableWidget::WidgetType::Joystick)
    {
        auto* joystick = dynamic_cast<DraggableJoystick*>(selectedWidget);
        if (joystick)
        {
            joystick->setStyle(joystickStyleCombo.getSelectedId() - 1);
        }
    }
    
    // Aplicar estilo de BasicMeter si es necesario
    if (selectedWidget->getWidgetType() == DraggableWidget::WidgetType::BasicMeter)
    {
        auto* basicMeter = dynamic_cast<DraggableBasicMeter*>(selectedWidget);
        if (basicMeter)
        {
            basicMeter->setStyle(basicMeterStyleCombo.getSelectedId() - 1);
        }
    }
    
    // Aplicar estilo de VUMeter si es necesario
    if (selectedWidget->getWidgetType() == DraggableWidget::WidgetType::VUMeter)
    {
        auto* vuMeter = dynamic_cast<DraggableVUMeter*>(selectedWidget);
        if (vuMeter)
        {
            vuMeter->setStyle(vuMeterStyleCombo.getSelectedId() - 1);
        }
    }
    
    // Aplicar estilo de LedMeter si es necesario
    if (selectedWidget->getWidgetType() == DraggableWidget::WidgetType::LedMeter)
    {
        auto* ledMeter = dynamic_cast<DraggableLedMeter*>(selectedWidget);
        if (ledMeter)
        {
            ledMeter->setStyle(ledMeterStyleCombo.getSelectedId() - 1);
        }
    }
    
    // Aplicar estilo de AudioScope si es necesario
    if (selectedWidget->getWidgetType() == DraggableWidget::WidgetType::AudioScope)
    {
        auto* audioScope = dynamic_cast<DraggableAudioScope*>(selectedWidget);
        if (audioScope)
        {
            audioScope->setStyle(audioScopeStyleCombo.getSelectedId() - 1);
        }
    }
    
    // Aplicar estilo de VerticalFader si es necesario
    if (selectedWidget->getWidgetType() == DraggableWidget::WidgetType::VerticalFader)
    {
        auto* verticalFader = dynamic_cast<DraggableVerticalFader*>(selectedWidget);
        if (verticalFader)
        {
            verticalFader->setStyle(verticalFaderStyleCombo.getSelectedId() - 1);
        }
    }
    
    // Aplicar estilo de ImageButton si es necesario
    if (selectedWidget->getWidgetType() == DraggableWidget::WidgetType::ImageButton)
    {
        auto* imageButton = dynamic_cast<DraggableImageButton*>(selectedWidget);
        if (imageButton)
        {
            imageButton->setStyle(imageButtonStyleCombo.getSelectedId() - 1);
        }
    }
    
    // Aplicar estilo de ToggleButton si es necesario
    if (selectedWidget->getWidgetType() == DraggableWidget::WidgetType::ToggleButton)
    {
        auto* toggleButton = dynamic_cast<DraggableToggleButton*>(selectedWidget);
        if (toggleButton)
        {
            toggleButton->setStyle(toggleButtonStyleCombo.getSelectedId() - 1);
        }
    }
    
    selectedWidget->repaint();
}

void PropertyInspector::showColorSelector(ColorTarget target)
{
    if (!selectedWidget)
        return;
    
    juce::PopupMenu menu;
    
    auto style = selectedWidget->getStyle();
    juce::Colour currentColor;
    
    switch (target)
    {
        case ColorTarget::Background: currentColor = style.bgColor; break;
        case ColorTarget::Border: currentColor = style.borderColor; break;
        case ColorTarget::Knob: currentColor = style.knobColor; break;
        case ColorTarget::Text: currentColor = style.textColor; break;
    }
    
    menu.addItem(1, "Azul Oscuro", true, currentColor == juce::Colour(0xff2d2d44));
    menu.addItem(2, "Verde Oscuro", true, currentColor == juce::Colour(0xff2d442d));
    menu.addItem(3, "Rojo Oscuro", true, currentColor == juce::Colour(0xff442d2d));
    menu.addItem(4, "Morado", true, currentColor == juce::Colour(0xff442d44));
    menu.addSeparator();
    menu.addItem(5, "Azul Claro", true, currentColor == juce::Colour(0xff4d4d88));
    menu.addItem(6, "Verde Claro", true, currentColor == juce::Colour(0xff4d884d));
    menu.addItem(7, "Rojo Claro", true, currentColor == juce::Colour(0xff884d4d));
    menu.addItem(8, "Cyan", true, currentColor == juce::Colour(0xff00adb5));
    menu.addSeparator();
    menu.addItem(9, "Gris Oscuro", true, currentColor == juce::Colour(0xff3d3d3d));
    menu.addItem(10, "Gris Claro", true, currentColor == juce::Colour(0xff7d7d7d));
    menu.addItem(11, "Blanco", true, currentColor == juce::Colours::white);
    menu.addItem(12, "Negro", true, currentColor == juce::Colours::black);
    menu.addSeparator();
    menu.addItem(13, "Transparente", true, currentColor.isTransparent());
    
    menu.showMenuAsync(juce::PopupMenu::Options(),
        [this, target](int result) {
            if (!selectedWidget || result == 0)
                return;
            
            juce::Colour newColor;
            switch (result)
            {
                case 1: newColor = juce::Colour(0xff2d2d44); break;
                case 2: newColor = juce::Colour(0xff2d442d); break;
                case 3: newColor = juce::Colour(0xff442d2d); break;
                case 4: newColor = juce::Colour(0xff442d44); break;
                case 5: newColor = juce::Colour(0xff4d4d88); break;
                case 6: newColor = juce::Colour(0xff4d884d); break;
                case 7: newColor = juce::Colour(0xff884d4d); break;
                case 8: newColor = juce::Colour(0xff00adb5); break;
                case 9: newColor = juce::Colour(0xff3d3d3d); break;
                case 10: newColor = juce::Colour(0xff7d7d7d); break;
                case 11: newColor = juce::Colours::white; break;
                case 12: newColor = juce::Colours::black; break;
                case 13: newColor = juce::Colours::transparentBlack; break;
                default: return;
            }
            
            auto style = selectedWidget->getStyle();
            
            switch (target)
            {
                case ColorTarget::Background: 
                    style.bgColor = newColor;
                    bgColorButton.setColour(juce::TextButton::buttonColourId, newColor);
                    break;
                case ColorTarget::Border: 
                    style.borderColor = newColor;
                    borderColorButton.setColour(juce::TextButton::buttonColourId, newColor);
                    break;
                case ColorTarget::Knob: 
                    style.knobColor = newColor;
                    knobColorButton.setColour(juce::TextButton::buttonColourId, newColor);
                    break;
                case ColorTarget::Text: 
                    style.textColor = newColor;
                    textColorButton.setColour(juce::TextButton::buttonColourId, newColor);
                    break;
            }
            
            selectedWidget->setStyle(style);
            selectedWidget->repaint();
        }
    );
}

void PropertyInspector::showWaveformColorSelector()
{
    if (!selectedWidget || selectedWidget->getWidgetType() != DraggableWidget::Waveform)
        return;
    
    auto* waveform = dynamic_cast<DraggableWaveform*>(selectedWidget);
    if (!waveform)
        return;
    
    juce::PopupMenu menu;
    menu.addItem(1, "Cyan", true);
    menu.addItem(2, "Verde Neon", true);
    menu.addItem(3, "Azul", true);
    menu.addItem(4, "Rojo", true);
    menu.addItem(5, "Amarillo", true);
    menu.addItem(6, "Magenta", true);
    menu.addItem(7, "Naranja", true);
    menu.addItem(8, "Morado", true);
    
    menu.showMenuAsync(juce::PopupMenu::Options(),
        [this, waveform](int result) {
            if (!waveform || result == 0)
                return;
            
            juce::Colour newColor;
            switch (result)
            {
                case 1: newColor = juce::Colour(0xff00ffff); break; // Cyan
                case 2: newColor = juce::Colour(0xff00ff00); break; // Verde
                case 3: newColor = juce::Colour(0xff0080ff); break; // Azul
                case 4: newColor = juce::Colour(0xffff0040); break; // Rojo
                case 5: newColor = juce::Colour(0xffffff00); break; // Amarillo
                case 6: newColor = juce::Colour(0xffff00ff); break; // Magenta
                case 7: newColor = juce::Colour(0xffff8000); break; // Naranja
                case 8: newColor = juce::Colour(0xff8000ff); break; // Morado
                default: return;
            }
            
            waveform->setWaveformColor(newColor);
            waveformColorButton.setColour(juce::TextButton::buttonColourId, newColor);
        }
    );
}

void PropertyInspector::showMeterColorSelector()
{
    if (!selectedWidget || selectedWidget->getWidgetType() != DraggableWidget::Meter)
        return;
    
    auto* meter = dynamic_cast<DraggableMeter*>(selectedWidget);
    if (!meter)
        return;
    
    juce::PopupMenu menu;
    menu.addItem(1, "Verde", true);
    menu.addItem(2, "Azul", true);
    menu.addItem(3, "Rojo", true);
    menu.addItem(4, "Amarillo", true);
    menu.addItem(5, "Cyan", true);
    menu.addItem(6, "Magenta", true);
    menu.addItem(7, "Naranja", true);
    menu.addItem(8, "Blanco", true);
    
    menu.showMenuAsync(juce::PopupMenu::Options(),
        [this, meter](int result) {
            if (!meter || result == 0)
                return;
            
            juce::Colour newColor;
            switch (result)
            {
                case 1: newColor = juce::Colour(0xff00ff00); break; // Verde
                case 2: newColor = juce::Colour(0xff0080ff); break; // Azul
                case 3: newColor = juce::Colour(0xffff0000); break; // Rojo
                case 4: newColor = juce::Colour(0xffffff00); break; // Amarillo
                case 5: newColor = juce::Colour(0xff00ffff); break; // Cyan
                case 6: newColor = juce::Colour(0xffff00ff); break; // Magenta
                case 7: newColor = juce::Colour(0xffff8000); break; // Naranja
                case 8: newColor = juce::Colour(0xffffffff); break; // Blanco
                default: return;
            }
            
            meter->setMeterColor(newColor);
            meterColorButton.setColour(juce::TextButton::buttonColourId, newColor);
        }
    );
}

void PropertyInspector::showXYPadColorSelector()
{
    if (!selectedWidget || selectedWidget->getWidgetType() != DraggableWidget::XYPad)
        return;
    
    auto* xypad = dynamic_cast<DraggableXYPad*>(selectedWidget);
    if (!xypad)
        return;
    
    juce::PopupMenu menu;
    menu.addItem(1, "Cyan", true);
    menu.addItem(2, "Magenta", true);
    menu.addItem(3, "Amarillo", true);
    menu.addItem(4, "Verde", true);
    menu.addItem(5, "Azul", true);
    menu.addItem(6, "Rojo", true);
    menu.addItem(7, "Morado", true);
    menu.addItem(8, "Naranja", true);
    
    menu.showMenuAsync(juce::PopupMenu::Options(),
        [this, xypad](int result) {
            if (!xypad || result == 0)
                return;
            
            juce::Colour newColor;
            switch (result)
            {
                case 1: newColor = juce::Colour(0xff00ffff); break; // Cyan
                case 2: newColor = juce::Colour(0xffff00ff); break; // Magenta
                case 3: newColor = juce::Colour(0xffffff00); break; // Amarillo
                case 4: newColor = juce::Colour(0xff00ff00); break; // Verde
                case 5: newColor = juce::Colour(0xff0080ff); break; // Azul
                case 6: newColor = juce::Colour(0xffff0040); break; // Rojo
                case 7: newColor = juce::Colour(0xff8000ff); break; // Morado
                case 8: newColor = juce::Colour(0xffff8000); break; // Naranja
                default: return;
            }
            
            xypad->setXYColor(newColor);
            xyPadColorButton.setColour(juce::TextButton::buttonColourId, newColor);
        }
    );
}

void PropertyInspector::showJoystickColorSelector()
{
    if (!selectedWidget || selectedWidget->getWidgetType() != DraggableWidget::Joystick)
        return;
    
    auto* joystick = dynamic_cast<DraggableJoystick*>(selectedWidget);
    if (!joystick)
        return;
    
    juce::PopupMenu menu;
    menu.addItem(1, "Verde", true);
    menu.addItem(2, "Azul", true);
    menu.addItem(3, "Rojo", true);
    menu.addItem(4, "Cyan", true);
    menu.addItem(5, "Amarillo", true);
    menu.addItem(6, "Magenta", true);
    menu.addItem(7, "Morado", true);
    menu.addItem(8, "Blanco", true);
    
    menu.showMenuAsync(juce::PopupMenu::Options(),
        [this, joystick](int result) {
            if (!joystick || result == 0)
                return;
            
            juce::Colour newColor;
            switch (result)
            {
                case 1: newColor = juce::Colour(0xff00ff00); break; // Verde
                case 2: newColor = juce::Colour(0xff0080ff); break; // Azul
                case 3: newColor = juce::Colour(0xffff0000); break; // Rojo
                case 4: newColor = juce::Colour(0xff00ffff); break; // Cyan
                case 5: newColor = juce::Colour(0xffffff00); break; // Amarillo
                case 6: newColor = juce::Colour(0xffff00ff); break; // Magenta
                case 7: newColor = juce::Colour(0xff8000ff); break; // Morado
                case 8: newColor = juce::Colour(0xffffffff); break; // Blanco
                default: return;
            }
            
            joystick->setJoystickColor(newColor);
            joystickColorButton.setColour(juce::TextButton::buttonColourId, newColor);
        }
    );
}

void PropertyInspector::showBasicMeterColorSelector()
{
    if (!selectedWidget || selectedWidget->getWidgetType() != DraggableWidget::BasicMeter)
        return;
    
    auto* basicMeter = dynamic_cast<DraggableBasicMeter*>(selectedWidget);
    if (!basicMeter)
        return;
    
    juce::PopupMenu menu;
    menu.addItem(1, "Verde", true);
    menu.addItem(2, "Azul", true);
    menu.addItem(3, "Rojo", true);
    menu.addItem(4, "Cyan", true);
    menu.addItem(5, "Amarillo", true);
    menu.addItem(6, "Magenta", true);
    menu.addItem(7, "Naranja", true);
    menu.addItem(8, "Blanco", true);
    
    menu.showMenuAsync(juce::PopupMenu::Options(),
        [this, basicMeter](int result) {
            if (!basicMeter || result == 0)
                return;
            
            juce::Colour newColor;
            switch (result)
            {
                case 1: newColor = juce::Colour(0xff00ff00); break; // Verde
                case 2: newColor = juce::Colour(0xff0080ff); break; // Azul
                case 3: newColor = juce::Colour(0xffff0000); break; // Rojo
                case 4: newColor = juce::Colour(0xff00ffff); break; // Cyan
                case 5: newColor = juce::Colour(0xffffff00); break; // Amarillo
                case 6: newColor = juce::Colour(0xffff00ff); break; // Magenta
                case 7: newColor = juce::Colour(0xffff8000); break; // Naranja
                case 8: newColor = juce::Colour(0xffffffff); break; // Blanco
                default: return;
            }
            
            basicMeter->setMeterColor(newColor);
            basicMeterColorButton.setColour(juce::TextButton::buttonColourId, newColor);
        }
    );
}

void PropertyInspector::showVUMeterColorSelector()
{
    if (!selectedWidget || selectedWidget->getWidgetType() != DraggableWidget::VUMeter)
        return;
    
    auto* vuMeter = dynamic_cast<DraggableVUMeter*>(selectedWidget);
    if (!vuMeter)
        return;
    
    juce::PopupMenu menu;
    menu.addItem(1, "Verde", true);
    menu.addItem(2, "Azul", true);
    menu.addItem(3, "Rojo", true);
    menu.addItem(4, "Cyan", true);
    menu.addItem(5, "Amarillo", true);
    menu.addItem(6, "Magenta", true);
    menu.addItem(7, "Naranja", true);
    menu.addItem(8, "Blanco", true);
    
    menu.showMenuAsync(juce::PopupMenu::Options(),
        [this, vuMeter](int result) {
            if (!vuMeter || result == 0)
                return;
            
            juce::Colour newColor;
            switch (result)
            {
                case 1: newColor = juce::Colour(0xff00ff00); break; // Verde
                case 2: newColor = juce::Colour(0xff0080ff); break; // Azul
                case 3: newColor = juce::Colour(0xffff0000); break; // Rojo
                case 4: newColor = juce::Colour(0xff00ffff); break; // Cyan
                case 5: newColor = juce::Colour(0xffffff00); break; // Amarillo
                case 6: newColor = juce::Colour(0xffff00ff); break; // Magenta
                case 7: newColor = juce::Colour(0xffff8000); break; // Naranja
                case 8: newColor = juce::Colour(0xffffffff); break; // Blanco
                default: return;
            }
            
            vuMeter->setVUColor(newColor);
            vuMeterColorButton.setColour(juce::TextButton::buttonColourId, newColor);
        }
    );
}

void PropertyInspector::showLedMeterColorSelector()
{
    if (!selectedWidget || selectedWidget->getWidgetType() != DraggableWidget::LedMeter)
        return;
    
    auto* ledMeter = dynamic_cast<DraggableLedMeter*>(selectedWidget);
    if (!ledMeter)
        return;
    
    juce::PopupMenu menu;
    menu.addItem(1, "Verde", true);
    menu.addItem(2, "Azul", true);
    menu.addItem(3, "Rojo", true);
    menu.addItem(4, "Cyan", true);
    menu.addItem(5, "Amarillo", true);
    menu.addItem(6, "Magenta", true);
    menu.addItem(7, "Naranja", true);
    menu.addItem(8, "Blanco", true);
    
    menu.showMenuAsync(juce::PopupMenu::Options(),
        [this, ledMeter](int result) {
            if (!ledMeter || result == 0)
                return;
            
            juce::Colour newColor;
            switch (result)
            {
                case 1: newColor = juce::Colour(0xff00ff00); break; // Verde
                case 2: newColor = juce::Colour(0xff0080ff); break; // Azul
                case 3: newColor = juce::Colour(0xffff0000); break; // Rojo
                case 4: newColor = juce::Colour(0xff00ffff); break; // Cyan
                case 5: newColor = juce::Colour(0xffffff00); break; // Amarillo
                case 6: newColor = juce::Colour(0xffff00ff); break; // Magenta
                case 7: newColor = juce::Colour(0xffff8000); break; // Naranja
                case 8: newColor = juce::Colour(0xffffffff); break; // Blanco
                default: return;
            }
            
            ledMeter->setLedColor(newColor);
            ledMeterColorButton.setColour(juce::TextButton::buttonColourId, newColor);
        }
    );
}

void PropertyInspector::showAudioScopeColorSelector()
{
    if (!selectedWidget || selectedWidget->getWidgetType() != DraggableWidget::AudioScope)
        return;
    
    auto* audioScope = dynamic_cast<DraggableAudioScope*>(selectedWidget);
    if (!audioScope)
        return;
    
    juce::PopupMenu menu;
    menu.addItem(1, "Cyan", true);
    menu.addItem(2, "Verde Neon", true);
    menu.addItem(3, "Azul", true);
    menu.addItem(4, "Rojo", true);
    menu.addItem(5, "Amarillo", true);
    menu.addItem(6, "Magenta", true);
    menu.addItem(7, "Naranja", true);
    menu.addItem(8, "Blanco", true);
    
    menu.showMenuAsync(juce::PopupMenu::Options(),
        [this, audioScope](int result) {
            if (!audioScope || result == 0)
                return;
            
            juce::Colour newColor;
            switch (result)
            {
                case 1: newColor = juce::Colour(0xff00ffff); break; // Cyan
                case 2: newColor = juce::Colour(0xff00ff00); break; // Verde
                case 3: newColor = juce::Colour(0xff0080ff); break; // Azul
                case 4: newColor = juce::Colour(0xffff0040); break; // Rojo
                case 5: newColor = juce::Colour(0xffffff00); break; // Amarillo
                case 6: newColor = juce::Colour(0xffff00ff); break; // Magenta
                case 7: newColor = juce::Colour(0xffff8000); break; // Naranja
                case 8: newColor = juce::Colour(0xffffffff); break; // Blanco
                default: return;
            }
            
            audioScope->setScopeColor(newColor);
            audioScopeColorButton.setColour(juce::TextButton::buttonColourId, newColor);
        }
    );
}

void PropertyInspector::showVerticalFaderColorSelector()
{
    if (!selectedWidget || selectedWidget->getWidgetType() != DraggableWidget::VerticalFader)
        return;
    
    auto* verticalFader = dynamic_cast<DraggableVerticalFader*>(selectedWidget);
    if (!verticalFader)
        return;
    
    juce::PopupMenu menu;
    menu.addItem(1, "Azul", true);
    menu.addItem(2, "Verde", true);
    menu.addItem(3, "Rojo", true);
    menu.addItem(4, "Cyan", true);
    menu.addItem(5, "Amarillo", true);
    menu.addItem(6, "Magenta", true);
    menu.addItem(7, "Naranja", true);
    menu.addItem(8, "Blanco", true);
    
    menu.showMenuAsync(juce::PopupMenu::Options(),
        [this, verticalFader](int result) {
            if (!verticalFader || result == 0)
                return;
            
            juce::Colour newColor;
            switch (result)
            {
                case 1: newColor = juce::Colour(0xff0080ff); break; // Azul
                case 2: newColor = juce::Colour(0xff00ff00); break; // Verde
                case 3: newColor = juce::Colour(0xffff0000); break; // Rojo
                case 4: newColor = juce::Colour(0xff00ffff); break; // Cyan
                case 5: newColor = juce::Colour(0xffffff00); break; // Amarillo
                case 6: newColor = juce::Colour(0xffff00ff); break; // Magenta
                case 7: newColor = juce::Colour(0xffff8000); break; // Naranja
                case 8: newColor = juce::Colour(0xffffffff); break; // Blanco
                default: return;
            }
            
            verticalFader->setFaderColor(newColor);
            verticalFaderColorButton.setColour(juce::TextButton::buttonColourId, newColor);
        }
    );
}

void PropertyInspector::showImageButtonColorSelector()
{
    if (!selectedWidget || selectedWidget->getWidgetType() != DraggableWidget::ImageButton)
        return;
    
    auto* imageButton = dynamic_cast<DraggableImageButton*>(selectedWidget);
    if (!imageButton)
        return;
    
    juce::PopupMenu menu;
    menu.addItem(1, "Azul", true);
    menu.addItem(2, "Verde", true);
    menu.addItem(3, "Rojo", true);
    menu.addItem(4, "Cyan", true);
    menu.addItem(5, "Amarillo", true);
    menu.addItem(6, "Magenta", true);
    menu.addItem(7, "Naranja", true);
    menu.addItem(8, "Gris", true);
    
    menu.showMenuAsync(juce::PopupMenu::Options(),
        [this, imageButton](int result) {
            if (!imageButton || result == 0)
                return;
            
            juce::Colour newColor;
            switch (result)
            {
                case 1: newColor = juce::Colour(0xff0080ff); break; // Azul
                case 2: newColor = juce::Colour(0xff00ff00); break; // Verde
                case 3: newColor = juce::Colour(0xffff0000); break; // Rojo
                case 4: newColor = juce::Colour(0xff00ffff); break; // Cyan
                case 5: newColor = juce::Colour(0xffffff00); break; // Amarillo
                case 6: newColor = juce::Colour(0xffff00ff); break; // Magenta
                case 7: newColor = juce::Colour(0xffff8000); break; // Naranja
                case 8: newColor = juce::Colour(0xff808080); break; // Gris
                default: return;
            }
            
            imageButton->setButtonColor(newColor);
            imageButtonColorButton.setColour(juce::TextButton::buttonColourId, newColor);
        }
    );
}

void PropertyInspector::showToggleButtonColorSelector()
{
    if (!selectedWidget || selectedWidget->getWidgetType() != DraggableWidget::ToggleButton)
        return;
    
    auto* toggleButton = dynamic_cast<DraggableToggleButton*>(selectedWidget);
    if (!toggleButton)
        return;
    
    juce::PopupMenu menu;
    menu.addItem(1, "Verde", true);
    menu.addItem(2, "Azul", true);
    menu.addItem(3, "Rojo", true);
    menu.addItem(4, "Cyan", true);
    menu.addItem(5, "Amarillo", true);
    menu.addItem(6, "Magenta", true);
    menu.addItem(7, "Naranja", true);
    menu.addItem(8, "Blanco", true);
    
    menu.showMenuAsync(juce::PopupMenu::Options(),
        [this, toggleButton](int result) {
            if (!toggleButton || result == 0)
                return;
            
            juce::Colour newColor;
            switch (result)
            {
                case 1: newColor = juce::Colour(0xff00ff00); break; // Verde
                case 2: newColor = juce::Colour(0xff0080ff); break; // Azul
                case 3: newColor = juce::Colour(0xffff0000); break; // Rojo
                case 4: newColor = juce::Colour(0xff00ffff); break; // Cyan
                case 5: newColor = juce::Colour(0xffffff00); break; // Amarillo
                case 6: newColor = juce::Colour(0xffff00ff); break; // Magenta
                case 7: newColor = juce::Colour(0xffff8000); break; // Naranja
                case 8: newColor = juce::Colour(0xffffffff); break; // Blanco
                default: return;
            }
            
            toggleButton->setToggleColor(newColor);
            toggleButtonColorButton.setColour(juce::TextButton::buttonColourId, newColor);
        }
    );
}

// ============================================================================
// CodeEditorPanel - Editor de Código
// ============================================================================

CodeEditorPanel::CodeEditorPanel()
{
    titleLabel.setFont(juce::FontOptions(14.0f, juce::Font::bold));
    titleLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(titleLabel);
    
    addAndMakeVisible(buildButton);
    addAndMakeVisible(runButton);
    
    codeEditor = std::make_unique<juce::TextEditor>();
    codeEditor->setMultiLine(true);
    codeEditor->setReturnKeyStartsNewLine(true);
    codeEditor->setTabKeyUsedAsCharacter(true);
    codeEditor->setFont(juce::FontOptions(juce::Font::getDefaultMonospacedFontName(), 14.0f, juce::Font::plain));
    codeEditor->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0xff1e1e1e));
    codeEditor->setColour(juce::TextEditor::textColourId, juce::Colour(0xffd4d4d4));
    addAndMakeVisible(codeEditor.get());
    
    // Código de ejemplo
    codeEditor->setText(
        "// Codigo C++ personalizado\n"
        "// Este codigo se integrara con tu GUI\n\n"
        "void customButtonClicked()\n"
        "{\n"
        "    // Tu logica aqui\n"
        "    DBG(\"Boton presionado!\");\n"
        "}\n"
    );
}

void CodeEditorPanel::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff1a1a2e));
}

void CodeEditorPanel::resized()
{
    auto bounds = getLocalBounds().reduced(5);
    
    auto topBar = bounds.removeFromTop(35);
    titleLabel.setBounds(topBar.removeFromLeft(200));
    topBar.removeFromRight(5);
    runButton.setBounds(topBar.removeFromRight(100));
    topBar.removeFromRight(5);
    buildButton.setBounds(topBar.removeFromRight(100));
    
    codeEditor->setBounds(bounds);
}

juce::String CodeEditorPanel::getCode() const
{
    return codeEditor->getText();
}

void CodeEditorPanel::setCode(const juce::String& code)
{
    codeEditor->setText(code);
}

// ============================================================================
// DebugConsole - Consola de Depuración
// ============================================================================

DebugConsole::DebugConsole()
{
    titleLabel.setFont(juce::FontOptions(14.0f, juce::Font::bold));
    titleLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(titleLabel);
    
    console.setMultiLine(true);
    console.setReadOnly(true);
    console.setFont(juce::FontOptions(juce::Font::getDefaultMonospacedFontName(), 12.0f, juce::Font::plain));
    console.setColour(juce::TextEditor::backgroundColourId, juce::Colour(0xff0a0a0a));
    console.setColour(juce::TextEditor::textColourId, juce::Colour(0xff00ff00));
    addAndMakeVisible(console);
    
    clearButton.onClick = [this]() { clear(); };
    addAndMakeVisible(clearButton);
    
    log("=== Consola de Depuracion Iniciada ===");
}

void DebugConsole::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff16213e));
}

void DebugConsole::resized()
{
    auto bounds = getLocalBounds().reduced(5);
    
    auto topBar = bounds.removeFromTop(30);
    titleLabel.setBounds(topBar.removeFromLeft(200));
    topBar.removeFromRight(5);
    clearButton.setBounds(topBar.removeFromRight(80));
    
    console.setBounds(bounds);
}

void DebugConsole::log(const juce::String& message, bool isError)
{
    auto timestamp = juce::Time::getCurrentTime().toString(true, true, true, true);
    auto prefix = isError ? "[ERROR] " : "[INFO] ";
    console.insertTextAtCaret(timestamp + " " + prefix + message + "\n");
}

void DebugConsole::clear()
{
    console.clear();
    log("Consola limpiada");
}

