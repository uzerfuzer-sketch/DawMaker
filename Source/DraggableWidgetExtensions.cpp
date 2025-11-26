// Este archivo contiene las implementaciones de los nuevos widgets de audio
#include "../Include/DraggableWidget.h"

//==============================================================================
// DraggableBasicMeter Implementation
//==============================================================================
DraggableBasicMeter::DraggableBasicMeter(const juce::String& name)
    : DraggableWidget(BasicMeter, name)
{
    meter = std::make_unique<::BasicMeter>();
    meter->setInterceptsMouseClicks(false, false);
    addAndMakeVisible(*meter);
    setSize(60, 200);
}

void DraggableBasicMeter::resized()
{
    DraggableWidget::resized();
    if (meter)
        meter->setBounds(getLocalBounds().reduced(10));
}

void DraggableBasicMeter::paintWidget(juce::Graphics& g)
{
    auto bounds = getLocalBounds().reduced(10);
    float level = meter ? static_cast<float>(meter->getSlider().getValue()) : 0.75f;
    
    switch (basicMeterStyle)
    {
        case Gradient:   drawGradientStyle(g, bounds, level); break;
        case Segmented:  drawSegmentedStyle(g, bounds, level); break;
        case Neon:       drawNeonStyle(g, bounds, level); break;
        case Vintage:    drawVintageStyle(g, bounds, level); break;
        case Digital:    drawDigitalStyle(g, bounds, level); break;
        case Plasma:     drawPlasmaStyle(g, bounds, level); break;
        case Rainbow:    drawRainbowStyle(g, bounds, level); break;
        case Thermal:    drawThermalStyle(g, bounds, level); break;
        case Crystal:    drawCrystalStyle(g, bounds, level); break;
        case Wave:       drawWaveStyle(g, bounds, level); break;
        default:         drawGradientStyle(g, bounds, level); break;
    }
}

void DraggableBasicMeter::setMeterValue(float value)
{
    if (meter)
        meter->setMeterValue(value);
}

float DraggableBasicMeter::getMeterValue() const
{
    if (meter)
        return meter->getSlider().getValue();
    return 0.0f;
}

void DraggableBasicMeter::updateFromMidiValue(int value)
{
    // Convertir valor MIDI (0-127) al rango del medidor
    float normalizedValue = value / 127.0f;
    setMeterValue(normalizedValue);
}

juce::ValueTree DraggableBasicMeter::toValueTree() const
{
    auto tree = DraggableWidget::toValueTree();
    if (meter)
        tree.setProperty("meterValue", meter->getSlider().getValue(), nullptr);
    return tree;
}

//==============================================================================
// DraggableVUMeter Implementation
//==============================================================================
DraggableVUMeter::DraggableVUMeter(const juce::String& name)
    : DraggableWidget(VUMeter, name)
{
    vuMeter = std::make_unique<::VUMeter>();
    vuMeter->setInterceptsMouseClicks(false, false);
    addAndMakeVisible(*vuMeter);
    setSize(150, 120);
}

void DraggableVUMeter::resized()
{
    DraggableWidget::resized();
    if (vuMeter)
        vuMeter->setBounds(getLocalBounds().reduced(10));
}

void DraggableVUMeter::paintWidget(juce::Graphics& g)
{
    auto bounds = getLocalBounds().reduced(10);
    float level = vuMeter ? static_cast<float>(vuMeter->getVUMeter().getValue()) : 0.75f;
    
    switch (vuMeterStyle)
    {
        case Analog:     drawAnalogStyle(g, bounds, level); break;
        case Modern:     drawModernStyle(g, bounds, level); break;
        case Retro:      drawRetroStyle(g, bounds, level); break;
        case Chrome:     drawChromeStyle(g, bounds, level); break;
        case Wood:       drawWoodStyle(g, bounds, level); break;
        case Steampunk:  drawSteampunkStyle(g, bounds, level); break;
        case Neon:       drawNeonStyle(g, bounds, level); break;
        case Digital:    drawDigitalStyle(g, bounds, level); break;
        case Minimal:    drawMinimalStyle(g, bounds, level); break;
        case Luxury:     drawLuxuryStyle(g, bounds, level); break;
        default:         drawAnalogStyle(g, bounds, level); break;
    }
}

void DraggableVUMeter::setMeterValue(float value)
{
    if (vuMeter)
        vuMeter->getVUMeter().setValue(value);
}

float DraggableVUMeter::getMeterValue() const
{
    if (vuMeter)
        return static_cast<float>(vuMeter->getVUMeter().getValue());
    return 0.0f;
}

void DraggableVUMeter::updateFromMidiValue(int value)
{
    // Convertir valor MIDI (0-127) al rango del medidor VU
    float normalizedValue = value / 127.0f;
    setMeterValue(normalizedValue);
}

juce::ValueTree DraggableVUMeter::toValueTree() const
{
    auto tree = DraggableWidget::toValueTree();
    if (vuMeter)
        tree.setProperty("meterValue", vuMeter->getVUMeter().getValue(), nullptr);
    return tree;
}

//==============================================================================
// DraggableLedMeter Implementation
//==============================================================================
DraggableLedMeter::DraggableLedMeter(const juce::String& name)
    : DraggableWidget(LedMeter, name)
{
    ledMeter = std::make_unique<::LedMeter>();
    ledMeter->setInterceptsMouseClicks(false, false);
    addAndMakeVisible(*ledMeter);
    setSize(40, 200);
}

void DraggableLedMeter::resized()
{
    DraggableWidget::resized();
    if (ledMeter)
        ledMeter->setBounds(getLocalBounds().reduced(10));
}

void DraggableLedMeter::paintWidget(juce::Graphics& g)
{
    auto bounds = getLocalBounds().reduced(10);
    float level = ledMeter ? static_cast<float>(ledMeter->getVUMeter().getValue()) : 0.75f;
    
    switch (ledMeterStyle)
    {
        case Classic:   drawClassicStyle(g, bounds, level); break;
        case Round:     drawRoundStyle(g, bounds, level); break;
        case Square:    drawSquareStyle(g, bounds, level); break;
        case Diamond:   drawDiamondStyle(g, bounds, level); break;
        case Bar:       drawBarStyle(g, bounds, level); break;
        case Dot:       drawDotStyle(g, bounds, level); break;
        case Glow:      drawGlowStyle(g, bounds, level); break;
        case Neon:      drawNeonStyle(g, bounds, level); break;
        case Matrix:    drawMatrixStyle(g, bounds, level); break;
        case Spectrum:  drawSpectrumStyle(g, bounds, level); break;
        default:        drawClassicStyle(g, bounds, level); break;
    }
}

void DraggableLedMeter::setMeterValue(float value)
{
    if (ledMeter)
        ledMeter->setMeterValue(value);
}

void DraggableLedMeter::setNumLEDs(int numLEDs)
{
    if (ledMeter)
        ledMeter->setNumLEDs(numLEDs);
}

juce::ValueTree DraggableLedMeter::toValueTree() const
{
    auto tree = DraggableWidget::toValueTree();
    if (ledMeter)
        tree.setProperty("meterValue", ledMeter->getVUMeter().getValue(), nullptr);
    return tree;
}

//==============================================================================
// DraggableAudioScope Implementation
//==============================================================================
DraggableAudioScope::DraggableAudioScope(const juce::String& name)
    : DraggableWidget(AudioScope, name)
{
    scope = std::make_unique<::AudioScope>(2);
    scope->setInterceptsMouseClicks(false, false);
    addAndMakeVisible(*scope);
    setSize(300, 200);
}

void DraggableAudioScope::resized()
{
    DraggableWidget::resized();
    if (scope)
        scope->setBounds(getLocalBounds().reduced(10));
}

void DraggableAudioScope::paintWidget(juce::Graphics& g)
{
    auto bounds = getLocalBounds().reduced(10);
    
    switch (scopeStyle)
    {
        case Waveform:     drawWaveformStyle(g, bounds); break;
        case Spectrum:     drawSpectrumStyle(g, bounds); break;
        case Vectorscope:  drawVectorscopeStyle(g, bounds); break;
        case Lissajous:    drawLissajousStyle(g, bounds); break;
        case Waterfall:    drawWaterfallStyle(g, bounds); break;
        case Spectrogram:  drawSpectrogramStyle(g, bounds); break;
        case Phase:        drawPhaseStyle(g, bounds); break;
        case Goniometer:   drawGoniometerStyle(g, bounds); break;
        case Oscilloscope: drawOscilloscopeStyle(g, bounds); break;
        case FFT:          drawFFTStyle(g, bounds); break;
        default:           drawWaveformStyle(g, bounds); break;
    }
}

void DraggableAudioScope::fillBuffer(juce::AudioBuffer<float>& buffer)
{
    if (scope)
        scope->fillBuffer(buffer);
}

juce::ValueTree DraggableAudioScope::toValueTree() const
{
    return DraggableWidget::toValueTree();
}

//==============================================================================
// DraggableVerticalFader Implementation
//==============================================================================
DraggableVerticalFader::DraggableVerticalFader(const juce::String& name)
    : DraggableWidget(VerticalFader, name)
{
    fader = std::make_unique<::VerticalFader>();
    fader->setInterceptsMouseClicks(false, false);
    addAndMakeVisible(*fader);
    
    label.setText(name, juce::dontSendNotification);
    label.setJustificationType(juce::Justification::centred);
    label.setColour(juce::Label::textColourId, juce::Colours::white);
    label.setInterceptsMouseClicks(false, false);
    addAndMakeVisible(label);
    
    setSize(80, 250);
}

void DraggableVerticalFader::resized()
{
    DraggableWidget::resized();
    auto bounds = getLocalBounds().reduced(10);
    
    label.setBounds(bounds.removeFromTop(20));
    if (fader)
        fader->setBounds(bounds);
}

void DraggableVerticalFader::paintWidget(juce::Graphics& g)
{
    auto bounds = getLocalBounds().reduced(10);
    float value = fader ? static_cast<float>(fader->getValue()) : 0.5f;
    
    switch (faderStyle)
    {
        case Modern:   drawModernStyle(g, bounds, value); break;
        case Classic:  drawClassicStyle(g, bounds, value); break;
        case Minimal:  drawMinimalStyle(g, bounds, value); break;
        case Studio:   drawStudioStyle(g, bounds, value); break;
        case Vintage:  drawVintageStyle(g, bounds, value); break;
        case Neon:     drawNeonStyle(g, bounds, value); break;
        case Glass:    drawGlassStyle(g, bounds, value); break;
        case Metal:    drawMetalStyle(g, bounds, value); break;
        case Wood:     drawWoodStyle(g, bounds, value); break;
        case Digital:  drawDigitalStyle(g, bounds, value); break;
        default:       drawModernStyle(g, bounds, value); break;
    }
}

void DraggableVerticalFader::updateFromMidiValue(int value)
{
    // Convertir valor MIDI (0-127) al rango del fader
    if (fader)
    {
        double normalizedValue = value / 127.0;
        double min = fader->getMinimum();
        double max = fader->getMaximum();
        double targetValue = min + (normalizedValue * (max - min));
        fader->setValue(targetValue, juce::sendNotificationAsync);
    }
}

juce::ValueTree DraggableVerticalFader::toValueTree() const
{
    auto tree = DraggableWidget::toValueTree();
    if (fader)
        tree.setProperty("faderValue", fader->getValue(), nullptr);
    return tree;
}

//==============================================================================
// DraggableImageButton Implementation
//==============================================================================
DraggableImageButton::DraggableImageButton(const juce::String& name)
    : DraggableWidget(ImageButton, name)
{
    // Crear imágenes placeholder
    juce::Image offImage(juce::Image::ARGB, 48, 48, true);
    juce::Image onImage(juce::Image::ARGB, 48, 48, true);
    
    juce::Graphics gOff(offImage);
    gOff.setColour(juce::Colours::darkgrey);
    gOff.fillRoundedRectangle(offImage.getBounds().toFloat(), 8.0f);
    
    juce::Graphics gOn(onImage);
    gOn.setColour(juce::Colour(0xFF4A90E2));
    gOn.fillRoundedRectangle(onImage.getBounds().toFloat(), 8.0f);
    
    imageButton = std::make_unique<CustomImageButton>(offImage, onImage, "OFF", "ON");
    imageButton->setInterceptsMouseClicks(false, false);
    addAndMakeVisible(*imageButton);
    setSize(80, 80);
}

void DraggableImageButton::resized()
{
    DraggableWidget::resized();
    if (imageButton)
        imageButton->setBounds(getLocalBounds().reduced(10));
}

void DraggableImageButton::mouseDown(const juce::MouseEvent& e)
{
    // Si no estamos redimensionando/arrastrando, activar el botón momentáneamente
    if (!isResizing && !isDragging)
    {
        if (imageButton)
        {
            auto& button = imageButton->getButton();
            button.setToggleState(true, juce::sendNotificationAsync);
            repaint();
        }
    }
    DraggableWidget::mouseDown(e);
}

void DraggableImageButton::mouseDrag(const juce::MouseEvent& e)
{
    DraggableWidget::mouseDrag(e);
}

void DraggableImageButton::mouseUp(const juce::MouseEvent& e)
{
    // Desactivar el botón cuando se suelta el mouse
    if (imageButton && !isDragging && !isResizing)
    {
        auto& button = imageButton->getButton();
        button.setToggleState(false, juce::sendNotificationAsync);
        repaint();
    }
    DraggableWidget::mouseUp(e);
}

void DraggableImageButton::paintWidget(juce::Graphics& g)
{
    auto bounds = getLocalBounds().reduced(10);
    bool isDown = imageButton ? imageButton->getButton().isDown() : false;
    
    switch (buttonStyle)
    {
        case Flat:     drawFlatStyle(g, bounds, isDown); break;
        case Raised:   drawRaisedStyle(g, bounds, isDown); break;
        case Rounded:  drawRoundedStyle(g, bounds, isDown); break;
        case Circle:   drawCircleStyle(g, bounds, isDown); break;
        case Neon:     drawNeonStyle(g, bounds, isDown); break;
        case Glass:    drawGlassStyle(g, bounds, isDown); break;
        case Metal:    drawMetalStyle(g, bounds, isDown); break;
        case Glow:     drawGlowStyle(g, bounds, isDown); break;
        case Shadow:   drawShadowStyle(g, bounds, isDown); break;
        case Gradient: drawGradientStyle(g, bounds, isDown); break;
        default:       drawFlatStyle(g, bounds, isDown); break;
    }
}

juce::ValueTree DraggableImageButton::toValueTree() const
{
    auto tree = DraggableWidget::toValueTree();
    if (imageButton)
        tree.setProperty("toggleState", imageButton->getButton().getToggleState(), nullptr);
    return tree;
}

//==============================================================================
// DraggableToggleButton Implementation
//==============================================================================
DraggableToggleButton::DraggableToggleButton(const juce::String& name)
    : DraggableWidget(ToggleButton, name)
{
    toggleButton = std::make_unique<SimpleToggleButton>(name);
    toggleButton->setInterceptsMouseClicks(false, false);
    addAndMakeVisible(*toggleButton);
    setSize(120, 30);
}

void DraggableToggleButton::resized()
{
    DraggableWidget::resized();
    if (toggleButton)
        toggleButton->setBounds(getLocalBounds().reduced(5));
}

void DraggableToggleButton::paintWidget(juce::Graphics& g)
{
    auto bounds = getLocalBounds().reduced(10);
    bool isOn = toggleButton ? toggleButton->getToggleState() : false;
    
    switch (toggleStyle)
    {
        case Checkbox:  drawCheckboxStyle(g, bounds, isOn); break;
        case Switch:    drawSwitchStyle(g, bounds, isOn); break;
        case Radio:     drawRadioStyle(g, bounds, isOn); break;
        case LED:       drawLEDStyle(g, bounds, isOn); break;
        case Button:    drawButtonStyle(g, bounds, isOn); break;
        case Slider:    drawSliderStyle(g, bounds, isOn); break;
        case Rocker:    drawRockerStyle(g, bounds, isOn); break;
        case Flip:      drawFlipStyle(g, bounds, isOn); break;
        case Power:     drawPowerStyle(g, bounds, isOn); break;
        case Custom:    drawCustomStyle(g, bounds, isOn); break;
        default:        drawCheckboxStyle(g, bounds, isOn); break;
    }
}

juce::ValueTree DraggableToggleButton::toValueTree() const
{
    auto tree = DraggableWidget::toValueTree();
    if (toggleButton)
        tree.setProperty("toggleState", toggleButton->getToggleState(), nullptr);
    return tree;
}

// ============================================
// BasicMeter Style Drawing Methods
// ============================================

void DraggableBasicMeter::drawGradientStyle(juce::Graphics& g, juce::Rectangle<int> area, float level)
{
    juce::ColourGradient gradient(
        meterColour.darker(0.5f), area.toFloat().getBottomLeft(),
        meterColour.brighter(0.3f), area.toFloat().getTopRight(), false);
    g.setGradientFill(gradient);
    auto fillHeight = area.getHeight() * level;
    g.fillRect(area.removeFromBottom(static_cast<int>(fillHeight)));
    
    g.setColour(juce::Colours::black.withAlpha(0.5f));
    g.drawRect(area.toFloat().expanded(1.0f), 1.0f);
}

void DraggableBasicMeter::drawSegmentedStyle(juce::Graphics& g, juce::Rectangle<int> area, float level)
{
    const int numSegments = 20;
    const int gap = 2;
    const float segmentHeight = (area.getHeight() - (numSegments - 1) * gap) / static_cast<float>(numSegments);
    
    for (int i = 0; i < numSegments; ++i)
    {
        float segmentLevel = static_cast<float>(numSegments - i) / numSegments;
        auto segmentBounds = juce::Rectangle<float>(
            static_cast<float>(area.getX()),
            area.getY() + i * (segmentHeight + gap),
            static_cast<float>(area.getWidth()),
            segmentHeight);
        
        if (segmentLevel <= level)
        {
            juce::Colour segmentColour = meterColour;
            if (segmentLevel > 0.8f)
                segmentColour = juce::Colours::red;
            else if (segmentLevel > 0.6f)
                segmentColour = juce::Colours::orange;
            
            g.setColour(segmentColour);
            g.fillRoundedRectangle(segmentBounds, 2.0f);
        }
        else
        {
            g.setColour(juce::Colours::darkgrey);
            g.fillRoundedRectangle(segmentBounds, 2.0f);
        }
    }
}

void DraggableBasicMeter::drawNeonStyle(juce::Graphics& g, juce::Rectangle<int> area, float level)
{
    g.setColour(juce::Colours::black);
    g.fillRect(area);
    
    auto fillHeight = area.getHeight() * level;
    auto fillArea = area.removeFromBottom(static_cast<int>(fillHeight)).toFloat();
    
    for (int i = 3; i >= 0; --i)
    {
        float alpha = 0.3f - i * 0.05f;
        g.setColour(meterColour.withAlpha(alpha));
        g.fillRoundedRectangle(fillArea.expanded(i * 2.0f), 4.0f);
    }
    
    g.setColour(meterColour.brighter(0.5f));
    g.fillRoundedRectangle(fillArea, 4.0f);
    
    g.setColour(meterColour.brighter(1.0f).withAlpha(0.5f));
    g.drawRoundedRectangle(fillArea, 4.0f, 2.0f);
}

void DraggableBasicMeter::drawVintageStyle(juce::Graphics& g, juce::Rectangle<int> area, float level)
{
    juce::Colour vintageBase = juce::Colour(205, 133, 63);
    juce::Colour vintageFill = juce::Colour(139, 69, 19);
    
    g.setColour(vintageBase.darker(0.3f));
    g.fillRect(area);
    
    auto fillHeight = area.getHeight() * level;
    juce::ColourGradient gradient(
        vintageFill, area.toFloat().getBottomLeft(),
        vintageFill.brighter(0.3f), area.toFloat().getTopRight(), false);
    g.setGradientFill(gradient);
    g.fillRect(area.removeFromBottom(static_cast<int>(fillHeight)));
    
    g.setColour(juce::Colour(101, 67, 33));
    g.drawRect(area.toFloat().expanded(2.0f), 3.0f);
}

void DraggableBasicMeter::drawDigitalStyle(juce::Graphics& g, juce::Rectangle<int> area, float level)
{
    g.setColour(juce::Colours::black);
    g.fillRect(area);
    
    const int numBars = 30;
    const int gap = 1;
    const float barHeight = (area.getHeight() - (numBars - 1) * gap) / static_cast<float>(numBars);
    
    for (int i = 0; i < numBars; ++i)
    {
        float barLevel = static_cast<float>(numBars - i) / numBars;
        auto barBounds = juce::Rectangle<float>(
            static_cast<float>(area.getX()),
            area.getY() + i * (barHeight + gap),
            static_cast<float>(area.getWidth()),
            barHeight);
        
        if (barLevel <= level)
        {
            g.setColour(juce::Colour(0, 255, 0));
            g.fillRect(barBounds);
            
            g.setColour(juce::Colour(150, 255, 150));
            g.fillRect(barBounds.reduced(1.0f, 0.0f));
        }
        else
        {
            g.setColour(juce::Colour(20, 40, 20));
            g.fillRect(barBounds);
        }
    }
}

void DraggableBasicMeter::drawPlasmaStyle(juce::Graphics& g, juce::Rectangle<int> area, float level)
{
    g.setColour(juce::Colours::black);
    g.fillRect(area);
    
    auto fillHeight = area.getHeight() * level;
    auto fillArea = area.removeFromBottom(static_cast<int>(fillHeight)).toFloat();
    
    juce::ColourGradient plasma(
        juce::Colours::purple, fillArea.getBottomLeft(),
        juce::Colours::cyan, fillArea.getTopRight(), false);
    plasma.addColour(0.33, juce::Colours::blue);
    plasma.addColour(0.66, juce::Colours::magenta);
    
    g.setGradientFill(plasma);
    g.fillRect(fillArea);
    
    g.setColour(juce::Colours::white.withAlpha(0.3f));
    for (int i = 0; i < 5; ++i)
    {
        float y = fillArea.getY() + fillArea.getHeight() * (i / 5.0f);
        g.drawHorizontalLine(static_cast<int>(y), fillArea.getX(), fillArea.getRight());
    }
}

void DraggableBasicMeter::drawRainbowStyle(juce::Graphics& g, juce::Rectangle<int> area, float level)
{
    g.setColour(juce::Colours::black);
    g.fillRect(area);
    
    auto fillHeight = area.getHeight() * level;
    auto fillArea = area.removeFromBottom(static_cast<int>(fillHeight)).toFloat();
    
    juce::ColourGradient rainbow(
        juce::Colours::red, fillArea.getBottomLeft(),
        juce::Colours::violet, fillArea.getTopRight(), false);
    rainbow.addColour(0.17, juce::Colours::orange);
    rainbow.addColour(0.33, juce::Colours::yellow);
    rainbow.addColour(0.50, juce::Colours::green);
    rainbow.addColour(0.67, juce::Colours::blue);
    rainbow.addColour(0.83, juce::Colours::indigo);
    
    g.setGradientFill(rainbow);
    g.fillRect(fillArea);
    
    g.setColour(juce::Colours::white.withAlpha(0.2f));
    g.drawRect(fillArea, 1.0f);
}

void DraggableBasicMeter::drawThermalStyle(juce::Graphics& g, juce::Rectangle<int> area, float level)
{
    g.setColour(juce::Colours::black);
    g.fillRect(area);
    
    auto fillHeight = area.getHeight() * level;
    auto fillArea = area.removeFromBottom(static_cast<int>(fillHeight)).toFloat();
    
    juce::Colour coldColour = juce::Colour(0, 0, 255);
    juce::Colour warmColour = juce::Colour(255, 255, 0);
    juce::Colour hotColour = juce::Colour(255, 0, 0);
    
    juce::ColourGradient thermal(coldColour, fillArea.getBottomLeft(), hotColour, fillArea.getTopRight(), false);
    thermal.addColour(0.5, warmColour);
    
    g.setGradientFill(thermal);
    g.fillRect(fillArea);
    
    for (int i = 0; i < 10; ++i)
    {
        float y = fillArea.getY() + fillArea.getHeight() * (i / 10.0f);
        g.setColour(juce::Colours::white.withAlpha(0.1f));
        g.drawHorizontalLine(static_cast<int>(y), fillArea.getX(), fillArea.getRight());
    }
}

void DraggableBasicMeter::drawCrystalStyle(juce::Graphics& g, juce::Rectangle<int> area, float level)
{
    g.setColour(juce::Colours::black);
    g.fillRect(area);
    
    auto fillHeight = area.getHeight() * level;
    auto fillArea = area.removeFromBottom(static_cast<int>(fillHeight)).toFloat();
    
    juce::ColourGradient crystalGradient(
        meterColour.withAlpha(0.3f), fillArea.getBottomLeft(),
        meterColour.withAlpha(0.8f), fillArea.getTopRight(), false);
    g.setGradientFill(crystalGradient);
    g.fillRect(fillArea);
    
    g.setColour(meterColour.brighter(1.5f).withAlpha(0.6f));
    juce::Path highlight;
    highlight.addRectangle(fillArea.reduced(2.0f, fillArea.getHeight() * 0.4f));
    g.fillPath(highlight);
    
    g.setColour(juce::Colours::white.withAlpha(0.3f));
    for (int i = 0; i < 3; ++i)
    {
        float x = fillArea.getX() + fillArea.getWidth() * ((i + 1) / 4.0f);
        g.drawVerticalLine(static_cast<int>(x), fillArea.getY(), fillArea.getBottom());
    }
    
    g.setColour(meterColour.brighter(0.5f));
    g.drawRect(fillArea, 1.0f);
}

void DraggableBasicMeter::drawWaveStyle(juce::Graphics& g, juce::Rectangle<int> area, float level)
{
    g.setColour(juce::Colours::black);
    g.fillRect(area);
    
    auto fillHeight = area.getHeight() * level;
    auto fillArea = area.removeFromBottom(static_cast<int>(fillHeight)).toFloat();
    
    juce::Path wavePath;
    float waveAmplitude = fillArea.getWidth() * 0.1f;
    float frequency = 4.0f;
    
    wavePath.startNewSubPath(fillArea.getX(), fillArea.getBottom());
    
    for (float y = fillArea.getBottom(); y >= fillArea.getY(); y -= 2.0f)
    {
        float progress = (fillArea.getBottom() - y) / fillArea.getHeight();
        float xOffset = std::sin(progress * frequency * juce::MathConstants<float>::twoPi) * waveAmplitude;
        wavePath.lineTo(fillArea.getCentreX() + xOffset, y);
    }
    
    wavePath.lineTo(fillArea.getCentreX(), fillArea.getY());
    wavePath.lineTo(fillArea.getX(), fillArea.getY());
    wavePath.closeSubPath();
    
    juce::ColourGradient waveGradient(
        meterColour.darker(0.3f), fillArea.getBottomLeft(),
        meterColour.brighter(0.5f), fillArea.getTopRight(), false);
    g.setGradientFill(waveGradient);
    g.fillPath(wavePath);
    
    g.setColour(meterColour.brighter(1.0f));
    g.strokePath(wavePath, juce::PathStrokeType(2.0f));
}

// ============================================
// VUMeter Style Drawing Methods
// ============================================

void DraggableVUMeter::drawAnalogStyle(juce::Graphics& g, juce::Rectangle<int> area, float level)
{
    g.setColour(juce::Colour(40, 35, 30));
    g.fillRoundedRectangle(area.toFloat(), 10.0f);
    
    auto dialArea = area.reduced(20).toFloat();
    float angle = -2.4f + level * 4.8f;
    auto centre = dialArea.getCentre();
    float radius = juce::jmin(dialArea.getWidth(), dialArea.getHeight()) * 0.4f;
    
    juce::Path needle;
    needle.addLineSegment(juce::Line<float>(centre, centre.getPointOnCircumference(radius, angle)), 3.0f);
    
    g.setColour(juce::Colours::red.darker(0.3f));
    g.fillPath(needle);
    
    g.setColour(juce::Colours::black);
    g.fillEllipse(centre.x - 6.0f, centre.y - 6.0f, 12.0f, 12.0f);
    
    for (int i = 0; i <= 20; ++i)
    {
        float tickAngle = -2.4f + (i / 20.0f) * 4.8f;
        auto outerPt = centre.getPointOnCircumference(radius + 5.0f, tickAngle);
        auto innerPt = centre.getPointOnCircumference(radius, tickAngle);
        
        g.setColour(juce::Colours::white.withAlpha(0.5f));
        g.drawLine(innerPt.x, innerPt.y, outerPt.x, outerPt.y, 1.0f);
    }
}

void DraggableVUMeter::drawModernStyle(juce::Graphics& g, juce::Rectangle<int> area, float level)
{
    g.setColour(juce::Colour(20, 20, 25));
    g.fillRoundedRectangle(area.toFloat(), 8.0f);
    
    auto arcArea = area.reduced(15).toFloat();
    auto centre = arcArea.getCentre();
    float radius = juce::jmin(arcArea.getWidth(), arcArea.getHeight()) * 0.45f;
    float thickness = 10.0f;
    
    juce::Path backgroundArc;
    backgroundArc.addCentredArc(centre.x, centre.y, radius, radius, 0.0f, -2.5f, 2.5f, true);
    g.setColour(juce::Colours::darkgrey.darker());
    g.strokePath(backgroundArc, juce::PathStrokeType(thickness));
    
    juce::Path valueArc;
    float endAngle = -2.5f + level * 5.0f;
    valueArc.addCentredArc(centre.x, centre.y, radius, radius, 0.0f, -2.5f, endAngle, true);
    
    juce::ColourGradient gradient(
        vuColour.darker(0.5f), centre.x - radius, centre.y,
        vuColour.brighter(0.3f), centre.x + radius, centre.y, false);
    g.setGradientFill(gradient);
    g.strokePath(valueArc, juce::PathStrokeType(thickness, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
}

void DraggableVUMeter::drawRetroStyle(juce::Graphics& g, juce::Rectangle<int> area, float level)
{
    juce::Colour sepia(205, 175, 149);
    g.setColour(sepia.darker(0.6f));
    g.fillRect(area);
    
    auto meterArea = area.reduced(10).toFloat();
    float barWidth = meterArea.getWidth() * level;
    
    juce::ColourGradient vintageFill(
        juce::Colour(180, 100, 50), meterArea.getTopLeft(),
        juce::Colour(220, 180, 100), meterArea.getBottomRight(), false);
    g.setGradientFill(vintageFill);
    g.fillRect(meterArea.removeFromLeft(barWidth));
    
    g.setColour(juce::Colour(80, 60, 40));
    g.drawRect(area.toFloat().reduced(5.0f), 3.0f);
    
    for (int i = 1; i < 10; ++i)
    {
        float x = meterArea.getX() + meterArea.getWidth() * (i / 10.0f);
        g.setColour(sepia.darker(0.3f).withAlpha(0.5f));
        g.drawVerticalLine(static_cast<int>(x), meterArea.getY(), meterArea.getBottom());
    }
}

void DraggableVUMeter::drawChromeStyle(juce::Graphics& g, juce::Rectangle<int> area, float level)
{
    juce::ColourGradient chromeGradient(
        juce::Colour(180, 180, 190), area.toFloat().getTopLeft(),
        juce::Colour(100, 100, 110), area.toFloat().getBottomRight(), false);
    chromeGradient.addColour(0.5, juce::Colours::silver);
    g.setGradientFill(chromeGradient);
    g.fillRoundedRectangle(area.toFloat(), 6.0f);
    
    auto meterArea = area.reduced(8).toFloat();
    float barWidth = meterArea.getWidth() * level;
    auto fillRect = meterArea.removeFromLeft(barWidth);
    
    juce::ColourGradient metallic(
        juce::Colour(200, 220, 240), fillRect.getTopLeft(),
        juce::Colour(120, 140, 160), fillRect.getBottomRight(), false);
    g.setGradientFill(metallic);
    g.fillRoundedRectangle(fillRect, 4.0f);
    
    g.setColour(juce::Colours::white.withAlpha(0.3f));
    g.drawLine(fillRect.getX(), fillRect.getY() + 2.0f, fillRect.getRight(), fillRect.getY() + 2.0f, 1.5f);
}

void DraggableVUMeter::drawWoodStyle(juce::Graphics& g, juce::Rectangle<int> area, float level)
{
    juce::Colour woodBase(101, 67, 33);
    juce::Colour woodLight(139, 90, 43);
    
    juce::ColourGradient woodGradient(
        woodBase, area.toFloat().getTopLeft(),
        woodLight, area.toFloat().getBottomRight(), false);
    g.setGradientFill(woodGradient);
    g.fillRoundedRectangle(area.toFloat(), 8.0f);
    
    for (int i = 0; i < 5; ++i)
    {
        float y = area.getY() + area.getHeight() * (i / 5.0f);
        g.setColour(woodBase.darker(0.2f).withAlpha(0.3f));
        g.drawHorizontalLine(static_cast<int>(y), static_cast<float>(area.getX()), static_cast<float>(area.getRight()));
    }
    
    auto meterArea = area.reduced(10).toFloat();
    float barWidth = meterArea.getWidth() * level;
    
    g.setColour(juce::Colour(200, 150, 50).withAlpha(0.8f));
    g.fillRoundedRectangle(meterArea.removeFromLeft(barWidth), 4.0f);
    
    g.setColour(juce::Colour(60, 40, 20));
    g.drawRoundedRectangle(area.toFloat().reduced(2.0f), 8.0f, 2.0f);
}

void DraggableVUMeter::drawSteampunkStyle(juce::Graphics& g, juce::Rectangle<int> area, float level)
{
    juce::Colour brass(181, 166, 66);
    juce::Colour bronze(140, 120, 83);
    
    juce::ColourGradient background(brass.darker(0.5f), area.toFloat().getTopLeft(), bronze.darker(0.3f), area.toFloat().getBottomRight(), false);
    g.setGradientFill(background);
    g.fillRoundedRectangle(area.toFloat(), 10.0f);
    
    g.setColour(bronze.darker(0.7f));
    for (int i = 0; i < 8; ++i)
    {
        int rivetX = area.getX() + 5 + (area.getWidth() - 10) * (i % 4) / 3;
        int rivetY = area.getY() + 5 + 20 * (i / 4);
        g.fillEllipse(static_cast<float>(rivetX - 3), static_cast<float>(rivetY - 3), 6.0f, 6.0f);
        g.setColour(brass.brighter(0.3f).withAlpha(0.5f));
        g.fillEllipse(static_cast<float>(rivetX - 2), static_cast<float>(rivetY - 2), 4.0f, 4.0f);
        g.setColour(bronze.darker(0.7f));
    }
    
    auto dialArea = area.reduced(25).toFloat();
    float angle = -2.2f + level * 4.4f;
    auto centre = dialArea.getCentre();
    float radius = juce::jmin(dialArea.getWidth(), dialArea.getHeight()) * 0.35f;
    
    juce::Path needle;
    needle.addLineSegment(juce::Line<float>(centre, centre.getPointOnCircumference(radius, angle)), 2.5f);
    g.setColour(bronze.darker());
    g.fillPath(needle);
}

void DraggableVUMeter::drawNeonStyle(juce::Graphics& g, juce::Rectangle<int> area, float level)
{
    g.setColour(juce::Colours::black);
    g.fillRect(area);
    
    auto meterArea = area.reduced(12).toFloat();
    float barWidth = meterArea.getWidth() * level;
    auto fillRect = meterArea.removeFromLeft(barWidth);
    
    for (int i = 4; i >= 0; --i)
    {
        g.setColour(vuColour.withAlpha(0.15f * (5 - i)));
        g.fillRoundedRectangle(fillRect.expanded(i * 3.0f), 6.0f);
    }
    
    g.setColour(vuColour.brighter(0.8f));
    g.fillRoundedRectangle(fillRect, 6.0f);
    
    g.setColour(vuColour.brighter(1.5f).withAlpha(0.6f));
    g.drawRoundedRectangle(fillRect, 6.0f, 2.0f);
}

void DraggableVUMeter::drawDigitalStyle(juce::Graphics& g, juce::Rectangle<int> area, float level)
{
    g.setColour(juce::Colours::black);
    g.fillRect(area);
    
    const int numSegments = 50;
    const int gap = 2;
    const float segmentWidth = (area.getWidth() - (numSegments - 1) * gap) / static_cast<float>(numSegments);
    
    for (int i = 0; i < numSegments; ++i)
    {
        float segmentLevel = (i + 1) / static_cast<float>(numSegments);
        auto segmentBounds = juce::Rectangle<float>(
            area.getX() + i * (segmentWidth + gap),
            static_cast<float>(area.getY()),
            segmentWidth,
            static_cast<float>(area.getHeight()));
        
        if (segmentLevel <= level)
        {
            juce::Colour segmentColour = juce::Colour(0, 255, 0);
            if (segmentLevel > 0.9f)
                segmentColour = juce::Colour(255, 0, 0);
            else if (segmentLevel > 0.7f)
                segmentColour = juce::Colour(255, 200, 0);
            
            g.setColour(segmentColour);
            g.fillRect(segmentBounds);
        }
        else
        {
            g.setColour(juce::Colour(15, 25, 15));
            g.fillRect(segmentBounds);
        }
    }
}

void DraggableVUMeter::drawMinimalStyle(juce::Graphics& g, juce::Rectangle<int> area, float level)
{
    g.setColour(juce::Colours::white);
    g.fillRect(area);
    
    auto meterArea = area.reduced(5).toFloat();
    float barWidth = meterArea.getWidth() * level;
    
    g.setColour(vuColour);
    g.fillRect(meterArea.removeFromLeft(barWidth));
    
    g.setColour(juce::Colours::lightgrey);
    g.drawRect(area.toFloat(), 1.0f);
}

void DraggableVUMeter::drawLuxuryStyle(juce::Graphics& g, juce::Rectangle<int> area, float level)
{
    juce::Colour gold(218, 165, 32);
    juce::Colour darkGold(184, 134, 11);
    
    juce::ColourGradient luxuryBg(
        juce::Colour(25, 20, 15), area.toFloat().getTopLeft(),
        juce::Colour(15, 10, 5), area.toFloat().getBottomRight(), false);
    g.setGradientFill(luxuryBg);
    g.fillRoundedRectangle(area.toFloat(), 12.0f);
    
    auto meterArea = area.reduced(15).toFloat();
    float barWidth = meterArea.getWidth() * level;
    auto fillRect = meterArea.removeFromLeft(barWidth);
    
    juce::ColourGradient goldGradient(
        gold, fillRect.getTopLeft(),
        darkGold, fillRect.getBottomRight(), false);
    goldGradient.addColour(0.5, gold.brighter(0.3f));
    g.setGradientFill(goldGradient);
    g.fillRoundedRectangle(fillRect, 5.0f);
    
    g.setColour(gold.brighter(0.5f).withAlpha(0.5f));
    g.drawLine(fillRect.getX(), fillRect.getY() + 3.0f, fillRect.getRight(), fillRect.getY() + 3.0f, 2.0f);
    
    g.setColour(gold.darker(0.3f));
    g.drawRoundedRectangle(area.toFloat().reduced(3.0f), 12.0f, 2.0f);
}

// ============================================
// LedMeter Style Drawing Methods (10)
// ============================================

void DraggableLedMeter::drawClassicStyle(juce::Graphics& g, juce::Rectangle<int> area, float level)
{
    g.setColour(juce::Colours::black);
    g.fillRect(area);
    const int numLeds = 20;
    const int gap = 2;
    const float ledHeight = (area.getHeight() - (numLeds - 1) * gap) / static_cast<float>(numLeds);
    for (int i = 0; i < numLeds; ++i)
    {
        float ledLevel = static_cast<float>(numLeds - i) / numLeds;
        auto ledBounds = juce::Rectangle<float>(static_cast<float>(area.getX()), area.getY() + i * (ledHeight + gap), static_cast<float>(area.getWidth()), ledHeight);
        if (ledLevel <= level)
        {
            juce::Colour ledColour = ledLevel > 0.8f ? juce::Colours::red : (ledLevel > 0.6f ? juce::Colours::yellow : juce::Colours::green);
            g.setColour(ledColour);
            g.fillRect(ledBounds);
        }
        else
        {
            g.setColour(juce::Colour(30, 30, 30));
            g.fillRect(ledBounds);
        }
    }
}

void DraggableLedMeter::drawRoundStyle(juce::Graphics& g, juce::Rectangle<int> area, float level)
{
    g.setColour(juce::Colours::black);
    g.fillRect(area);
    const int numLeds = 15;
    const int gap = 4;
    const float ledDiameter = (area.getHeight() - (numLeds - 1) * gap) / static_cast<float>(numLeds);
    for (int i = 0; i < numLeds; ++i)
    {
        float ledLevel = static_cast<float>(numLeds - i) / numLeds;
        float yPos = area.getY() + i * (ledDiameter + gap);
        if (ledLevel <= level)
        {
            g.setColour(ledColour.brighter(0.5f));
            g.fillEllipse(area.getCentreX() - ledDiameter/2, yPos, ledDiameter, ledDiameter);
        }
        else
        {
            g.setColour(juce::Colour(40, 40, 40));
            g.fillEllipse(area.getCentreX() - ledDiameter/2, yPos, ledDiameter, ledDiameter);
        }
    }
}

void DraggableLedMeter::drawSquareStyle(juce::Graphics& g, juce::Rectangle<int> area, float level)
{
    g.setColour(juce::Colours::darkgrey.darker());
    g.fillRect(area);
    const int numLeds = 18;
    const int gap = 2;
    const float ledHeight = (area.getHeight() - (numLeds - 1) * gap) / static_cast<float>(numLeds);
    for (int i = 0; i < numLeds; ++i)
    {
        float ledLevel = static_cast<float>(numLeds - i) / numLeds;
        auto ledBounds = juce::Rectangle<float>(static_cast<float>(area.getX() + 2), area.getY() + i * (ledHeight + gap), static_cast<float>(area.getWidth() - 4), ledHeight);
        if (ledLevel <= level)
        {
            g.setColour(ledColour);
            g.fillRoundedRectangle(ledBounds, 2.0f);
        }
        else
        {
            g.setColour(juce::Colour(25, 25, 25));
            g.fillRoundedRectangle(ledBounds, 2.0f);
        }
    }
}

void DraggableLedMeter::drawDiamondStyle(juce::Graphics& g, juce::Rectangle<int> area, float level)
{
    g.setColour(juce::Colours::black);
    g.fillRect(area);
    const int numLeds = 12;
    const int gap = 5;
    const float ledSize = (area.getHeight() - (numLeds - 1) * gap) / static_cast<float>(numLeds);
    for (int i = 0; i < numLeds; ++i)
    {
        float ledLevel = static_cast<float>(numLeds - i) / numLeds;
        float yPos = area.getY() + i * (ledSize + gap) + ledSize / 2;
        juce::Path diamond;
        diamond.addQuadrilateral(area.getCentreX(), yPos - ledSize/2, area.getCentreX() + ledSize/2, yPos, area.getCentreX(), yPos + ledSize/2, area.getCentreX() - ledSize/2, yPos);
        if (ledLevel <= level)
        {
            g.setColour(ledColour.brighter(0.3f));
            g.fillPath(diamond);
        }
        else
        {
            g.setColour(juce::Colour(35, 35, 35));
            g.fillPath(diamond);
        }
    }
}

void DraggableLedMeter::drawBarStyle(juce::Graphics& g, juce::Rectangle<int> area, float level)
{
    g.setColour(juce::Colours::black);
    g.fillRect(area);
    auto fillHeight = area.getHeight() * level;
    auto fillArea = area.removeFromBottom(static_cast<int>(fillHeight)).toFloat();
    juce::ColourGradient gradient(ledColour.darker(0.4f), fillArea.getBottomLeft(), ledColour.brighter(0.5f), fillArea.getTopRight(), false);
    g.setGradientFill(gradient);
    g.fillRect(fillArea);
    g.setColour(ledColour.brighter(1.0f).withAlpha(0.4f));
    g.drawRect(fillArea, 1.0f);
}

void DraggableLedMeter::drawDotStyle(juce::Graphics& g, juce::Rectangle<int> area, float level)
{
    g.setColour(juce::Colour(15, 15, 20));
    g.fillRect(area);
    const int numLeds = 25;
    const int gap = 2;
    const float dotSize = (area.getHeight() - (numLeds - 1) * gap) / static_cast<float>(numLeds);
    for (int i = 0; i < numLeds; ++i)
    {
        float ledLevel = static_cast<float>(numLeds - i) / numLeds;
        float yPos = area.getY() + i * (dotSize + gap);
        if (ledLevel <= level)
        {
            g.setColour(ledColour.brighter(0.6f));
            g.fillEllipse(area.getCentreX() - dotSize/2, yPos, dotSize, dotSize);
        }
        else
        {
            g.setColour(juce::Colour(30, 30, 35));
            g.fillEllipse(area.getCentreX() - dotSize/2, yPos, dotSize, dotSize);
        }
    }
}

void DraggableLedMeter::drawGlowStyle(juce::Graphics& g, juce::Rectangle<int> area, float level)
{
    g.setColour(juce::Colours::black);
    g.fillRect(area);
    const int numLeds = 16;
    const int gap = 3;
    const float ledHeight = (area.getHeight() - (numLeds - 1) * gap) / static_cast<float>(numLeds);
    for (int i = 0; i < numLeds; ++i)
    {
        float ledLevel = static_cast<float>(numLeds - i) / numLeds;
        auto ledBounds = juce::Rectangle<float>(static_cast<float>(area.getX() + 3), area.getY() + i * (ledHeight + gap), static_cast<float>(area.getWidth() - 6), ledHeight);
        if (ledLevel <= level)
        {
            for (int j = 3; j >= 0; --j)
            {
                g.setColour(ledColour.withAlpha(0.2f * (4 - j)));
                g.fillRoundedRectangle(ledBounds.expanded(j * 2.0f), 4.0f);
            }
            g.setColour(ledColour.brighter(0.8f));
            g.fillRoundedRectangle(ledBounds, 4.0f);
        }
        else
        {
            g.setColour(juce::Colour(20, 20, 20));
            g.fillRoundedRectangle(ledBounds, 4.0f);
        }
    }
}

void DraggableLedMeter::drawNeonStyle(juce::Graphics& g, juce::Rectangle<int> area, float level)
{
    g.setColour(juce::Colours::black);
    g.fillRect(area);
    const int numLeds = 20;
    const int gap = 2;
    const float ledHeight = (area.getHeight() - (numLeds - 1) * gap) / static_cast<float>(numLeds);
    for (int i = 0; i < numLeds; ++i)
    {
        float ledLevel = static_cast<float>(numLeds - i) / numLeds;
        auto ledBounds = juce::Rectangle<float>(static_cast<float>(area.getX() + 4), area.getY() + i * (ledHeight + gap), static_cast<float>(area.getWidth() - 8), ledHeight);
        if (ledLevel <= level)
        {
            g.setColour(ledColour.withAlpha(0.3f));
            g.fillRoundedRectangle(ledBounds.expanded(4.0f), 5.0f);
            g.setColour(ledColour.brighter(1.0f));
            g.fillRoundedRectangle(ledBounds, 3.0f);
            g.setColour(ledColour.brighter(1.5f).withAlpha(0.5f));
            g.drawRoundedRectangle(ledBounds, 3.0f, 1.5f);
        }
        else
        {
            g.setColour(juce::Colour(10, 15, 10));
            g.fillRoundedRectangle(ledBounds, 3.0f);
        }
    }
}

void DraggableLedMeter::drawMatrixStyle(juce::Graphics& g, juce::Rectangle<int> area, float level)
{
    g.setColour(juce::Colours::black);
    g.fillRect(area);
    const int numRows = 15;
    const int numCols = 3;
    const int gap = 2;
    const float cellHeight = (area.getHeight() - (numRows - 1) * gap) / static_cast<float>(numRows);
    const float cellWidth = (area.getWidth() - (numCols - 1) * gap) / static_cast<float>(numCols);
    for (int row = 0; row < numRows; ++row)
    {
        float rowLevel = static_cast<float>(numRows - row) / numRows;
        for (int col = 0; col < numCols; ++col)
        {
            auto cellBounds = juce::Rectangle<float>(area.getX() + col * (cellWidth + gap), area.getY() + row * (cellHeight + gap), cellWidth, cellHeight);
            if (rowLevel <= level)
            {
                g.setColour(juce::Colour(0, 255, 100).brighter(0.2f));
                g.fillRect(cellBounds);
            }
            else
            {
                g.setColour(juce::Colour(0, 40, 15));
                g.fillRect(cellBounds);
            }
        }
    }
}

void DraggableLedMeter::drawSpectrumStyle(juce::Graphics& g, juce::Rectangle<int> area, float level)
{
    g.setColour(juce::Colours::black);
    g.fillRect(area);
    const int numBands = 20;
    const int gap = 1;
    const float bandHeight = (area.getHeight() - (numBands - 1) * gap) / static_cast<float>(numBands);
    for (int i = 0; i < numBands; ++i)
    {
        float bandLevel = static_cast<float>(numBands - i) / numBands;
        auto bandBounds = juce::Rectangle<float>(static_cast<float>(area.getX()), area.getY() + i * (bandHeight + gap), static_cast<float>(area.getWidth()), bandHeight);
        if (bandLevel <= level)
        {
            float hue = i / static_cast<float>(numBands);
            g.setColour(juce::Colour::fromHSV(hue, 1.0f, 1.0f, 1.0f));
            g.fillRect(bandBounds);
        }
        else
        {
            g.setColour(juce::Colour(20, 20, 20));
            g.fillRect(bandBounds);
        }
    }
}

// ============================================
// AudioScope Style Drawing Methods (10)
// ============================================

void DraggableAudioScope::drawWaveformStyle(juce::Graphics& g, juce::Rectangle<int> area)
{
    g.setColour(juce::Colour(20, 25, 30));
    g.fillRect(area);
    g.setColour(scopeColour);
    juce::Path waveform;
    float centreY = area.getCentreY();
    waveform.startNewSubPath(static_cast<float>(area.getX()), centreY);
    for (int x = area.getX(); x < area.getRight(); x += 2)
    {
        float progress = (x - area.getX()) / static_cast<float>(area.getWidth());
        float y = centreY + std::sin(progress * 8.0f * juce::MathConstants<float>::twoPi) * area.getHeight() * 0.3f;
        waveform.lineTo(static_cast<float>(x), y);
    }
    g.strokePath(waveform, juce::PathStrokeType(2.0f));
}

void DraggableAudioScope::drawSpectrumStyle(juce::Graphics& g, juce::Rectangle<int> area)
{
    g.setColour(juce::Colours::black);
    g.fillRect(area);
    const int numBars = 30;
    const float barWidth = area.getWidth() / static_cast<float>(numBars);
    for (int i = 0; i < numBars; ++i)
    {
        float height = area.getHeight() * (0.2f + 0.6f * std::sin(i * 0.5f));
        auto barBounds = juce::Rectangle<float>(area.getX() + i * barWidth, area.getBottom() - height, barWidth - 2.0f, height);
        juce::ColourGradient gradient(scopeColour.darker(0.5f), barBounds.getBottomLeft(), scopeColour.brighter(0.5f), barBounds.getTopLeft(), false);
        g.setGradientFill(gradient);
        g.fillRect(barBounds);
    }
}

void DraggableAudioScope::drawVectorscopeStyle(juce::Graphics& g, juce::Rectangle<int> area)
{
    g.setColour(juce::Colours::black);
    g.fillRect(area);
    g.setColour(juce::Colours::green.withAlpha(0.3f));
    g.drawEllipse(area.toFloat().reduced(5.0f), 1.0f);
    g.drawLine(static_cast<float>(area.getCentreX()), static_cast<float>(area.getY()), static_cast<float>(area.getCentreX()), static_cast<float>(area.getBottom()), 0.5f);
    g.drawLine(static_cast<float>(area.getX()), static_cast<float>(area.getCentreY()), static_cast<float>(area.getRight()), static_cast<float>(area.getCentreY()), 0.5f);
    g.setColour(scopeColour.brighter(0.5f));
    juce::Path vectorPath;
    auto centre = area.getCentre().toFloat();
    for (float angle = 0; angle < juce::MathConstants<float>::twoPi; angle += 0.1f)
    {
        float radius = area.getWidth() * 0.3f * (0.5f + 0.5f * std::sin(angle * 3.0f));
        auto point = centre.getPointOnCircumference(radius, angle);
        if (angle == 0)
            vectorPath.startNewSubPath(point);
        else
            vectorPath.lineTo(point);
    }
    g.strokePath(vectorPath, juce::PathStrokeType(1.5f));
}

void DraggableAudioScope::drawLissajousStyle(juce::Graphics& g, juce::Rectangle<int> area)
{
    g.setColour(juce::Colour(10, 10, 15));
    g.fillRect(area);
    g.setColour(scopeColour);
    juce::Path lissajous;
    auto centre = area.getCentre().toFloat();
    float radius = juce::jmin(area.getWidth(), area.getHeight()) * 0.35f;
    for (float t = 0; t < juce::MathConstants<float>::twoPi; t += 0.02f)
    {
        float x = centre.x + radius * std::sin(3.0f * t);
        float y = centre.y + radius * std::sin(2.0f * t);
        if (t == 0)
            lissajous.startNewSubPath(x, y);
        else
            lissajous.lineTo(x, y);
    }
    lissajous.closeSubPath();
    g.strokePath(lissajous, juce::PathStrokeType(2.0f));
}

void DraggableAudioScope::drawWaterfallStyle(juce::Graphics& g, juce::Rectangle<int> area)
{
    g.setColour(juce::Colours::black);
    g.fillRect(area);
    const int numLines = 15;
    for (int i = 0; i < numLines; ++i)
    {
        float y = area.getY() + area.getHeight() * (i / static_cast<float>(numLines));
        float alpha = 1.0f - (i / static_cast<float>(numLines));
        g.setColour(scopeColour.withAlpha(alpha));
        juce::Path line;
        line.startNewSubPath(static_cast<float>(area.getX()), y);
        for (int x = area.getX(); x < area.getRight(); x += 5)
        {
            float height = 10.0f * std::sin((x - area.getX()) * 0.1f + i * 0.5f);
            line.lineTo(static_cast<float>(x), y + height);
        }
        g.strokePath(line, juce::PathStrokeType(1.0f));
    }
}

void DraggableAudioScope::drawSpectrogramStyle(juce::Graphics& g, juce::Rectangle<int> area)
{
    g.setColour(juce::Colours::black);
    g.fillRect(area);
    const int gridX = 20, gridY = 15;
    float cellWidth = area.getWidth() / static_cast<float>(gridX);
    float cellHeight = area.getHeight() / static_cast<float>(gridY);
    for (int y = 0; y < gridY; ++y)
    {
        for (int x = 0; x < gridX; ++x)
        {
            float intensity = std::sin((x + y) * 0.5f) * 0.5f + 0.5f;
            g.setColour(scopeColour.withAlpha(intensity));
            g.fillRect(area.getX() + x * cellWidth, area.getY() + y * cellHeight, cellWidth - 1.0f, cellHeight - 1.0f);
        }
    }
}

void DraggableAudioScope::drawPhaseStyle(juce::Graphics& g, juce::Rectangle<int> area)
{
    g.setColour(juce::Colour(15, 15, 20));
    g.fillRect(area);
    g.setColour(juce::Colours::grey.withAlpha(0.3f));
    g.drawLine(area.getX() + area.getWidth() * 0.25f, static_cast<float>(area.getY()), area.getX() + area.getWidth() * 0.25f, static_cast<float>(area.getBottom()), 0.5f);
    g.drawLine(area.getX() + area.getWidth() * 0.75f, static_cast<float>(area.getY()), area.getX() + area.getWidth() * 0.75f, static_cast<float>(area.getBottom()), 0.5f);
    g.setColour(scopeColour.brighter(0.3f));
    juce::Path phaseCurve;
    phaseCurve.startNewSubPath(static_cast<float>(area.getX()), static_cast<float>(area.getCentreY()));
    for (int x = area.getX(); x < area.getRight(); x += 3)
    {
        float progress = (x - area.getX()) / static_cast<float>(area.getWidth());
        float y = area.getCentreY() + std::sin(progress * 4.0f * juce::MathConstants<float>::twoPi) * area.getHeight() * 0.4f;
        phaseCurve.lineTo(static_cast<float>(x), y);
    }
    g.strokePath(phaseCurve, juce::PathStrokeType(2.0f));
}

void DraggableAudioScope::drawGoniometerStyle(juce::Graphics& g, juce::Rectangle<int> area)
{
    g.setColour(juce::Colours::black);
    g.fillRect(area);
    g.setColour(juce::Colours::darkgreen.withAlpha(0.5f));
    g.drawLine(static_cast<float>(area.getX()), static_cast<float>(area.getBottom()), static_cast<float>(area.getRight()), static_cast<float>(area.getY()), 1.0f);
    g.drawLine(static_cast<float>(area.getX()), static_cast<float>(area.getY()), static_cast<float>(area.getRight()), static_cast<float>(area.getBottom()), 1.0f);
    g.setColour(scopeColour);
    auto centre = area.getCentre().toFloat();
    for (int i = 0; i < 50; ++i)
    {
        float angle = (i / 50.0f) * juce::MathConstants<float>::pi - juce::MathConstants<float>::pi * 0.5f;
        float radius = area.getWidth() * 0.4f * (0.3f + 0.7f * std::abs(std::sin(i * 0.3f)));
        auto point = centre.getPointOnCircumference(radius, angle);
        g.fillEllipse(point.x - 2.0f, point.y - 2.0f, 4.0f, 4.0f);
    }
}

void DraggableAudioScope::drawOscilloscopeStyle(juce::Graphics& g, juce::Rectangle<int> area)
{
    g.setColour(juce::Colour(5, 10, 5));
    g.fillRect(area);
    g.setColour(juce::Colours::green.withAlpha(0.2f));
    for (int i = 1; i < 5; ++i)
    {
        g.drawHorizontalLine(static_cast<int>(area.getY() + area.getHeight() * (i / 5.0f)), static_cast<float>(area.getX()), static_cast<float>(area.getRight()));
        g.drawVerticalLine(static_cast<int>(area.getX() + area.getWidth() * (i / 5.0f)), static_cast<float>(area.getY()), static_cast<float>(area.getBottom()));
    }
    g.setColour(juce::Colours::green);
    juce::Path scope;
    float centreY = area.getCentreY();
    scope.startNewSubPath(static_cast<float>(area.getX()), centreY);
    for (int x = area.getX(); x < area.getRight(); x += 2)
    {
        float progress = (x - area.getX()) / static_cast<float>(area.getWidth());
        float y = centreY + std::sin(progress * 6.0f * juce::MathConstants<float>::twoPi) * area.getHeight() * 0.35f;
        scope.lineTo(static_cast<float>(x), y);
    }
    g.strokePath(scope, juce::PathStrokeType(1.5f));
}

void DraggableAudioScope::drawFFTStyle(juce::Graphics& g, juce::Rectangle<int> area)
{
    g.setColour(juce::Colours::black);
    g.fillRect(area);
    const int numBins = 40;
    const float binWidth = area.getWidth() / static_cast<float>(numBins);
    for (int i = 0; i < numBins; ++i)
    {
        float freq = i / static_cast<float>(numBins);
        float magnitude = (1.0f - freq) * std::sin(i * 0.8f) * 0.5f + 0.5f;
        float height = area.getHeight() * magnitude;
        auto binBounds = juce::Rectangle<float>(area.getX() + i * binWidth, area.getBottom() - height, binWidth - 1.0f, height);
        g.setColour(juce::Colour::fromHSV(freq * 0.7f, 1.0f, 1.0f, 1.0f));
        g.fillRect(binBounds);
    }
}

// ============================================
// VerticalFader Style Drawing Methods (10)
// ============================================

void DraggableVerticalFader::drawModernStyle(juce::Graphics& g, juce::Rectangle<int> area, float value)
{
    g.setColour(juce::Colour(30, 30, 35));
    g.fillRoundedRectangle(area.toFloat(), 5.0f);
    auto trackArea = area.reduced(area.getWidth() / 3, 10).toFloat();
    g.setColour(juce::Colour(50, 50, 55));
    g.fillRoundedRectangle(trackArea, 3.0f);
    float thumbY = trackArea.getY() + trackArea.getHeight() * (1.0f - value);
    auto thumbBounds = juce::Rectangle<float>(static_cast<float>(area.getX()), thumbY - 8.0f, static_cast<float>(area.getWidth()), 16.0f);
    g.setColour(faderColour);
    g.fillRoundedRectangle(thumbBounds, 4.0f);
}

void DraggableVerticalFader::drawClassicStyle(juce::Graphics& g, juce::Rectangle<int> area, float value)
{
    g.setColour(juce::Colours::darkgrey);
    g.fillRect(area);
    auto trackArea = area.reduced(area.getWidth() / 3, 5).toFloat();
    juce::ColourGradient trackGradient(juce::Colours::grey.darker(), trackArea.getTopLeft(), juce::Colours::grey, trackArea.getTopRight(), false);
    g.setGradientFill(trackGradient);
    g.fillRect(trackArea);
    float thumbY = trackArea.getY() + trackArea.getHeight() * (1.0f - value);
    auto thumbBounds = juce::Rectangle<float>(static_cast<float>(area.getX()), thumbY - 10.0f, static_cast<float>(area.getWidth()), 20.0f);
    juce::ColourGradient thumbGradient(faderColour.brighter(0.3f), thumbBounds.getTopLeft(), faderColour.darker(0.3f), thumbBounds.getBottomLeft(), false);
    g.setGradientFill(thumbGradient);
    g.fillRect(thumbBounds);
    g.setColour(juce::Colours::black.withAlpha(0.5f));
    g.drawRect(thumbBounds, 1.0f);
}

void DraggableVerticalFader::drawMinimalStyle(juce::Graphics& g, juce::Rectangle<int> area, float value)
{
    g.setColour(juce::Colours::white);
    g.fillRect(area);
    auto trackArea = area.reduced(area.getWidth() / 2 - 2, 10).toFloat();
    g.setColour(juce::Colours::lightgrey);
    g.fillRect(trackArea);
    float thumbY = trackArea.getY() + trackArea.getHeight() * (1.0f - value);
    auto thumbBounds = juce::Rectangle<float>(static_cast<float>(area.getX() + 2), thumbY - 6.0f, static_cast<float>(area.getWidth() - 4), 12.0f);
    g.setColour(faderColour);
    g.fillRect(thumbBounds);
}

void DraggableVerticalFader::drawStudioStyle(juce::Graphics& g, juce::Rectangle<int> area, float value)
{
    g.setColour(juce::Colour(25, 25, 30));
    g.fillRect(area);
    auto trackArea = area.reduced(area.getWidth() / 3, 8).toFloat();
    g.setColour(juce::Colour(45, 45, 50));
    g.fillRect(trackArea);
    for (int i = 0; i <= 10; ++i)
    {
        float y = trackArea.getY() + trackArea.getHeight() * (i / 10.0f);
        g.setColour(juce::Colours::grey.withAlpha(0.5f));
        g.drawHorizontalLine(static_cast<int>(y), trackArea.getX() - 5.0f, trackArea.getRight() + 5.0f);
    }
    float thumbY = trackArea.getY() + trackArea.getHeight() * (1.0f - value);
    auto thumbBounds = juce::Rectangle<float>(static_cast<float>(area.getX()), thumbY - 8.0f, static_cast<float>(area.getWidth()), 16.0f);
    g.setColour(faderColour.brighter(0.2f));
    g.fillRect(thumbBounds);
    g.setColour(faderColour.darker(0.3f));
    g.drawRect(thumbBounds, 1.0f);
}

void DraggableVerticalFader::drawVintageStyle(juce::Graphics& g, juce::Rectangle<int> area, float value)
{
    juce::Colour vintage(205, 175, 149);
    g.setColour(vintage.darker(0.5f));
    g.fillRect(area);
    auto trackArea = area.reduced(area.getWidth() / 4, 10).toFloat();
    juce::ColourGradient vintageBg(juce::Colour(160, 120, 80), trackArea.getTopLeft(), juce::Colour(180, 140, 100), trackArea.getBottomLeft(), false);
    g.setGradientFill(vintageBg);
    g.fillRect(trackArea);
    float thumbY = trackArea.getY() + trackArea.getHeight() * (1.0f - value);
    auto thumbBounds = juce::Rectangle<float>(static_cast<float>(area.getX()), thumbY - 12.0f, static_cast<float>(area.getWidth()), 24.0f);
    g.setColour(juce::Colour(200, 150, 100));
    g.fillRoundedRectangle(thumbBounds, 3.0f);
    g.setColour(juce::Colour(120, 80, 40));
    g.drawRoundedRectangle(thumbBounds, 3.0f, 2.0f);
}

void DraggableVerticalFader::drawNeonStyle(juce::Graphics& g, juce::Rectangle<int> area, float value)
{
    g.setColour(juce::Colours::black);
    g.fillRect(area);
    auto trackArea = area.reduced(area.getWidth() / 3, 10).toFloat();
    g.setColour(juce::Colour(20, 20, 25));
    g.fillRoundedRectangle(trackArea, 4.0f);
    float thumbY = trackArea.getY() + trackArea.getHeight() * (1.0f - value);
    auto thumbBounds = juce::Rectangle<float>(static_cast<float>(area.getX()), thumbY - 10.0f, static_cast<float>(area.getWidth()), 20.0f);
    for (int i = 3; i >= 0; --i)
    {
        g.setColour(faderColour.withAlpha(0.2f * (4 - i)));
        g.fillRoundedRectangle(thumbBounds.expanded(i * 2.5f), 5.0f);
    }
    g.setColour(faderColour.brighter(0.8f));
    g.fillRoundedRectangle(thumbBounds, 5.0f);
}

void DraggableVerticalFader::drawGlassStyle(juce::Graphics& g, juce::Rectangle<int> area, float value)
{
    g.setColour(juce::Colour(40, 45, 50));
    g.fillRoundedRectangle(area.toFloat(), 6.0f);
    auto trackArea = area.reduced(area.getWidth() / 3, 12).toFloat();
    juce::ColourGradient glassTrack(juce::Colours::white.withAlpha(0.1f), trackArea.getTopLeft(), juce::Colours::white.withAlpha(0.3f), trackArea.getBottomLeft(), false);
    g.setGradientFill(glassTrack);
    g.fillRoundedRectangle(trackArea, 4.0f);
    float thumbY = trackArea.getY() + trackArea.getHeight() * (1.0f - value);
    auto thumbBounds = juce::Rectangle<float>(static_cast<float>(area.getX() + 2), thumbY - 10.0f, static_cast<float>(area.getWidth() - 4), 20.0f);
    juce::ColourGradient glassThumb(faderColour.withAlpha(0.6f), thumbBounds.getTopLeft(), faderColour.withAlpha(0.3f), thumbBounds.getBottomLeft(), false);
    g.setGradientFill(glassThumb);
    g.fillRoundedRectangle(thumbBounds, 4.0f);
    g.setColour(juce::Colours::white.withAlpha(0.5f));
    g.drawRoundedRectangle(thumbBounds.reduced(1.0f), 4.0f, 1.0f);
}

void DraggableVerticalFader::drawMetalStyle(juce::Graphics& g, juce::Rectangle<int> area, float value)
{
    juce::ColourGradient metalBg(juce::Colour(120, 120, 130), area.toFloat().getTopLeft(), juce::Colour(80, 80, 90), area.toFloat().getBottomLeft(), false);
    g.setGradientFill(metalBg);
    g.fillRect(area);
    auto trackArea = area.reduced(area.getWidth() / 4, 10).toFloat();
    juce::ColourGradient metalTrack(juce::Colour(60, 60, 70), trackArea.getTopLeft(), juce::Colour(90, 90, 100), trackArea.getBottomLeft(), false);
    g.setGradientFill(metalTrack);
    g.fillRect(trackArea);
    float thumbY = trackArea.getY() + trackArea.getHeight() * (1.0f - value);
    auto thumbBounds = juce::Rectangle<float>(static_cast<float>(area.getX()), thumbY - 10.0f, static_cast<float>(area.getWidth()), 20.0f);
    juce::ColourGradient chrome(juce::Colour(200, 200, 210), thumbBounds.getTopLeft(), juce::Colour(140, 140, 150), thumbBounds.getBottomLeft(), false);
    chrome.addColour(0.5, juce::Colours::silver);
    g.setGradientFill(chrome);
    g.fillRect(thumbBounds);
    g.setColour(juce::Colours::white.withAlpha(0.4f));
    g.drawLine(thumbBounds.getX(), thumbBounds.getY() + 2.0f, thumbBounds.getRight(), thumbBounds.getY() + 2.0f, 1.0f);
}

void DraggableVerticalFader::drawWoodStyle(juce::Graphics& g, juce::Rectangle<int> area, float value)
{
    juce::Colour wood(139, 90, 43);
    g.setColour(wood.darker(0.4f));
    g.fillRect(area);
    auto trackArea = area.reduced(area.getWidth() / 3, 12).toFloat();
    juce::ColourGradient woodGradient(juce::Colour(101, 67, 33), trackArea.getTopLeft(), juce::Colour(120, 80, 40), trackArea.getBottomLeft(), false);
    g.setGradientFill(woodGradient);
    g.fillRect(trackArea);
    float thumbY = trackArea.getY() + trackArea.getHeight() * (1.0f - value);
    auto thumbBounds = juce::Rectangle<float>(static_cast<float>(area.getX()), thumbY - 12.0f, static_cast<float>(area.getWidth()), 24.0f);
    g.setColour(wood);
    g.fillRoundedRectangle(thumbBounds, 4.0f);
    g.setColour(wood.darker(0.5f));
    g.drawRoundedRectangle(thumbBounds, 4.0f, 2.0f);
}

void DraggableVerticalFader::drawDigitalStyle(juce::Graphics& g, juce::Rectangle<int> area, float value)
{
    g.setColour(juce::Colours::black);
    g.fillRect(area);
    auto trackArea = area.reduced(area.getWidth() / 4, 8).toFloat();
    const int numSegments = 20;
    const float segmentHeight = trackArea.getHeight() / numSegments;
    for (int i = 0; i < numSegments; ++i)
    {
        float segmentValue = (numSegments - i) / static_cast<float>(numSegments);
        auto segmentBounds = juce::Rectangle<float>(trackArea.getX(), trackArea.getY() + i * segmentHeight, trackArea.getWidth(), segmentHeight - 1.0f);
        if (segmentValue <= value)
        {
            g.setColour(juce::Colour(0, 200, 255));
            g.fillRect(segmentBounds);
        }
        else
        {
            g.setColour(juce::Colour(0, 30, 40));
            g.fillRect(segmentBounds);
        }
    }
}

// ============================================
// ImageButton Style Drawing Methods (10)
// ============================================

void DraggableImageButton::drawFlatStyle(juce::Graphics& g, juce::Rectangle<int> area, bool isDown)
{
    g.setColour(isDown ? buttonColour.darker(0.3f) : buttonColour);
    g.fillRect(area);
    g.setColour(buttonColour.darker(0.5f));
    g.drawRect(area, 1);
}

void DraggableImageButton::drawRaisedStyle(juce::Graphics& g, juce::Rectangle<int> area, bool isDown)
{
    auto buttonArea = isDown ? area.reduced(2) : area;
    juce::ColourGradient gradient(buttonColour.brighter(0.3f), buttonArea.toFloat().getTopLeft(), buttonColour.darker(0.3f), buttonArea.toFloat().getBottomRight(), false);
    g.setGradientFill(gradient);
    g.fillRect(buttonArea);
    g.setColour(isDown ? juce::Colours::black.withAlpha(0.5f) : juce::Colours::white.withAlpha(0.3f));
    g.drawRect(buttonArea, 2);
}

void DraggableImageButton::drawRoundedStyle(juce::Graphics& g, juce::Rectangle<int> area, bool isDown)
{
    g.setColour(isDown ? buttonColour.darker(0.4f) : buttonColour);
    g.fillRoundedRectangle(area.toFloat(), 10.0f);
    g.setColour(buttonColour.brighter(0.5f).withAlpha(0.3f));
    g.drawRoundedRectangle(area.toFloat().reduced(1.0f), 10.0f, 2.0f);
}

void DraggableImageButton::drawCircleStyle(juce::Graphics& g, juce::Rectangle<int> area, bool isDown)
{
    auto circleBounds = area.toFloat().reduced(5.0f);
    g.setColour(isDown ? buttonColour.darker(0.3f) : buttonColour);
    g.fillEllipse(circleBounds);
    g.setColour(buttonColour.brighter(0.4f));
    g.drawEllipse(circleBounds, 2.0f);
}

void DraggableImageButton::drawNeonStyle(juce::Graphics& g, juce::Rectangle<int> area, bool isDown)
{
    g.setColour(juce::Colours::black);
    g.fillRect(area);
    auto buttonArea = area.toFloat().reduced(8.0f);
    if (isDown)
    {
        for (int i = 5; i >= 0; --i)
        {
            g.setColour(buttonColour.withAlpha(0.15f * (6 - i)));
            g.fillRoundedRectangle(buttonArea.expanded(i * 2.0f), 8.0f);
        }
    }
    g.setColour(isDown ? buttonColour.brighter(0.8f) : buttonColour);
    g.fillRoundedRectangle(buttonArea, 8.0f);
    g.setColour(buttonColour.brighter(1.5f).withAlpha(0.5f));
    g.drawRoundedRectangle(buttonArea, 8.0f, 2.0f);
}

void DraggableImageButton::drawGlassStyle(juce::Graphics& g, juce::Rectangle<int> area, bool isDown)
{
    auto buttonArea = area.toFloat().reduced(3.0f);
    juce::ColourGradient glassGradient(buttonColour.withAlpha(0.6f), buttonArea.getTopLeft(), buttonColour.withAlpha(0.3f), buttonArea.getBottomLeft(), false);
    g.setGradientFill(glassGradient);
    g.fillRoundedRectangle(buttonArea, 6.0f);
    g.setColour(juce::Colours::white.withAlpha(isDown ? 0.2f : 0.4f));
    g.fillRoundedRectangle(buttonArea.removeFromTop(buttonArea.getHeight() * 0.4f), 6.0f);
    g.setColour(buttonColour.brighter(0.5f));
    g.drawRoundedRectangle(buttonArea, 6.0f, 1.5f);
}

void DraggableImageButton::drawMetalStyle(juce::Graphics& g, juce::Rectangle<int> area, bool isDown)
{
    auto buttonArea = area.toFloat().reduced(2.0f);
    juce::ColourGradient metalGradient(juce::Colour(180, 180, 190), buttonArea.getTopLeft(), juce::Colour(100, 100, 110), buttonArea.getBottomRight(), false);
    metalGradient.addColour(0.5, juce::Colours::silver);
    g.setGradientFill(metalGradient);
    g.fillRoundedRectangle(buttonArea, 4.0f);
    if (isDown)
    {
        g.setColour(juce::Colours::black.withAlpha(0.3f));
        g.fillRoundedRectangle(buttonArea, 4.0f);
    }
    g.setColour(juce::Colours::white.withAlpha(0.5f));
    g.drawLine(buttonArea.getX(), buttonArea.getY() + 3.0f, buttonArea.getRight(), buttonArea.getY() + 3.0f, 1.5f);
}

void DraggableImageButton::drawGlowStyle(juce::Graphics& g, juce::Rectangle<int> area, bool isDown)
{
    g.setColour(juce::Colour(20, 20, 25));
    g.fillRect(area);
    auto buttonArea = area.toFloat().reduced(10.0f);
    if (isDown)
    {
        for (int i = 4; i >= 0; --i)
        {
            float alpha = 0.3f - i * 0.05f;
            g.setColour(buttonColour.withAlpha(alpha));
            g.fillEllipse(buttonArea.expanded(i * 4.0f));
        }
    }
    g.setColour(isDown ? buttonColour.brighter(0.5f) : buttonColour);
    g.fillEllipse(buttonArea);
}

void DraggableImageButton::drawShadowStyle(juce::Graphics& g, juce::Rectangle<int> area, bool isDown)
{
    if (!isDown)
    {
        g.setColour(juce::Colours::black.withAlpha(0.5f));
        g.fillRoundedRectangle(area.toFloat().translated(3.0f, 3.0f), 6.0f);
    }
    auto buttonArea = isDown ? area.reduced(2).toFloat() : area.toFloat().reduced(0.0f, 0.0f);
    g.setColour(buttonColour);
    g.fillRoundedRectangle(buttonArea, 6.0f);
    g.setColour(buttonColour.darker(0.4f));
    g.drawRoundedRectangle(buttonArea, 6.0f, 1.5f);
}

void DraggableImageButton::drawGradientStyle(juce::Graphics& g, juce::Rectangle<int> area, bool isDown)
{
    auto buttonArea = area.toFloat().reduced(2.0f);
    juce::ColourGradient gradient(isDown ? buttonColour.darker(0.5f) : buttonColour.brighter(0.3f), buttonArea.getTopLeft(), isDown ? buttonColour.darker(0.2f) : buttonColour.darker(0.4f), buttonArea.getBottomLeft(), false);
    g.setGradientFill(gradient);
    g.fillRoundedRectangle(buttonArea, 5.0f);
    g.setColour(juce::Colours::white.withAlpha(0.2f));
    g.drawRoundedRectangle(buttonArea, 5.0f, 1.0f);
}

// ============================================
// ToggleButton Style Drawing Methods (10)
// ============================================

void DraggableToggleButton::drawCheckboxStyle(juce::Graphics& g, juce::Rectangle<int> area, bool isOn)
{
    auto boxSize = juce::jmin(area.getWidth(), area.getHeight()) - 4;
    auto boxBounds = juce::Rectangle<int>(area.getCentreX() - boxSize/2, area.getCentreY() - boxSize/2, boxSize, boxSize);
    g.setColour(juce::Colours::white);
    g.fillRoundedRectangle(boxBounds.toFloat(), 3.0f);
    g.setColour(juce::Colours::grey);
    g.drawRoundedRectangle(boxBounds.toFloat(), 3.0f, 2.0f);
    if (isOn)
    {
        g.setColour(toggleColour);
        juce::Path checkmark;
        checkmark.startNewSubPath(boxBounds.getX() + boxSize * 0.2f, boxBounds.getCentreY());
        checkmark.lineTo(boxBounds.getCentreX(), boxBounds.getY() + boxSize * 0.7f);
        checkmark.lineTo(boxBounds.getRight() - boxSize * 0.2f, boxBounds.getY() + boxSize * 0.3f);
        g.strokePath(checkmark, juce::PathStrokeType(3.0f));
    }
}

void DraggableToggleButton::drawSwitchStyle(juce::Graphics& g, juce::Rectangle<int> area, bool isOn)
{
    auto switchBounds = area.reduced(5).toFloat();
    float switchHeight = juce::jmin(switchBounds.getHeight(), switchBounds.getWidth() * 0.5f);
    switchBounds = switchBounds.withSizeKeepingCentre(switchBounds.getWidth(), switchHeight);
    g.setColour(isOn ? toggleColour.darker(0.3f) : juce::Colours::grey);
    g.fillRoundedRectangle(switchBounds, switchHeight * 0.5f);
    float knobSize = switchHeight * 0.8f;
    float knobX = isOn ? switchBounds.getRight() - knobSize - 2.0f : switchBounds.getX() + 2.0f;
    g.setColour(juce::Colours::white);
    g.fillEllipse(knobX, switchBounds.getCentreY() - knobSize * 0.5f, knobSize, knobSize);
}

void DraggableToggleButton::drawRadioStyle(juce::Graphics& g, juce::Rectangle<int> area, bool isOn)
{
    auto circleDiameter = juce::jmin(area.getWidth(), area.getHeight()) - 6;
    auto circleBounds = juce::Rectangle<float>(area.getCentreX() - circleDiameter/2.0f, area.getCentreY() - circleDiameter/2.0f, static_cast<float>(circleDiameter), static_cast<float>(circleDiameter));
    g.setColour(juce::Colours::white);
    g.fillEllipse(circleBounds);
    g.setColour(juce::Colours::grey);
    g.drawEllipse(circleBounds, 2.0f);
    if (isOn)
    {
        g.setColour(toggleColour);
        g.fillEllipse(circleBounds.reduced(circleDiameter * 0.25f));
    }
}

void DraggableToggleButton::drawLEDStyle(juce::Graphics& g, juce::Rectangle<int> area, bool isOn)
{
    g.setColour(juce::Colours::black);
    g.fillRect(area);
    auto ledSize = juce::jmin(area.getWidth(), area.getHeight()) * 0.6f;
    auto ledBounds = juce::Rectangle<float>(area.getCentreX() - ledSize/2, area.getCentreY() - ledSize/2, ledSize, ledSize);
    if (isOn)
    {
        for (int i = 3; i >= 0; --i)
        {
            g.setColour(toggleColour.withAlpha(0.2f * (4 - i)));
            g.fillEllipse(ledBounds.expanded(i * 3.0f));
        }
        g.setColour(toggleColour.brighter(0.8f));
        g.fillEllipse(ledBounds);
    }
    else
    {
        g.setColour(juce::Colour(40, 40, 40));
        g.fillEllipse(ledBounds);
    }
}

void DraggableToggleButton::drawButtonStyle(juce::Graphics& g, juce::Rectangle<int> area, bool isOn)
{
    auto buttonArea = area.reduced(3).toFloat();
    g.setColour(isOn ? toggleColour : juce::Colours::grey);
    g.fillRoundedRectangle(buttonArea, 5.0f);
    g.setColour(isOn ? toggleColour.brighter(0.4f) : juce::Colours::lightgrey);
    g.drawRoundedRectangle(buttonArea, 5.0f, 2.0f);
}

void DraggableToggleButton::drawSliderStyle(juce::Graphics& g, juce::Rectangle<int> area, bool isOn)
{
    auto trackBounds = area.reduced(5, area.getHeight() / 3).toFloat();
    g.setColour(juce::Colours::grey.darker());
    g.fillRoundedRectangle(trackBounds, trackBounds.getHeight() * 0.5f);
    float thumbX = isOn ? trackBounds.getRight() - 15.0f : trackBounds.getX();
    auto thumbBounds = juce::Rectangle<float>(thumbX, static_cast<float>(area.getY() + 3), 15.0f, static_cast<float>(area.getHeight() - 6));
    g.setColour(isOn ? toggleColour : juce::Colours::grey);
    g.fillRoundedRectangle(thumbBounds, 3.0f);
}

void DraggableToggleButton::drawRockerStyle(juce::Graphics& g, juce::Rectangle<int> area, bool isOn)
{
    auto rockerBounds = area.reduced(3).toFloat();
    auto topHalf = rockerBounds.removeFromTop(rockerBounds.getHeight() * 0.5f);
    auto bottomHalf = rockerBounds;
    g.setColour(isOn ? juce::Colours::grey.darker() : toggleColour);
    g.fillRect(topHalf);
    g.setColour(isOn ? toggleColour : juce::Colours::grey.darker());
    g.fillRect(bottomHalf);
    g.setColour(juce::Colours::black);
    g.drawRect(area.toFloat(), 2.0f);
}

void DraggableToggleButton::drawFlipStyle(juce::Graphics& g, juce::Rectangle<int> area, bool isOn)
{
    auto flipBounds = area.reduced(4).toFloat();
    juce::ColourGradient gradient(isOn ? toggleColour.brighter(0.3f) : juce::Colours::grey.brighter(0.2f), flipBounds.getTopLeft(), isOn ? toggleColour.darker(0.3f) : juce::Colours::grey.darker(0.3f), flipBounds.getBottomLeft(), false);
    g.setGradientFill(gradient);
    g.fillRoundedRectangle(flipBounds, 6.0f);
    g.setColour(juce::Colours::black.withAlpha(0.5f));
    auto shadowArea = isOn ? flipBounds.removeFromTop(flipBounds.getHeight() * 0.5f) : flipBounds.removeFromBottom(flipBounds.getHeight() * 0.5f);
    g.fillRoundedRectangle(shadowArea, 6.0f);
}

void DraggableToggleButton::drawPowerStyle(juce::Graphics& g, juce::Rectangle<int> area, bool isOn)
{
    g.setColour(juce::Colour(30, 30, 35));
    g.fillEllipse(area.toFloat().reduced(2.0f));
    auto symbolBounds = area.reduced(area.getWidth() / 4).toFloat();
    juce::Path powerSymbol;
    powerSymbol.startNewSubPath(symbolBounds.getCentreX(), symbolBounds.getY());
    powerSymbol.lineTo(symbolBounds.getCentreX(), symbolBounds.getCentreY());
    g.setColour(isOn ? toggleColour.brighter(0.5f) : juce::Colours::grey);
    g.strokePath(powerSymbol, juce::PathStrokeType(3.0f));
    juce::Path arc;
    arc.addCentredArc(symbolBounds.getCentreX(), symbolBounds.getCentreY(), symbolBounds.getWidth() * 0.4f, symbolBounds.getHeight() * 0.4f, 0.0f, 0.8f, juce::MathConstants<float>::twoPi - 0.8f, true);
    g.strokePath(arc, juce::PathStrokeType(3.0f));
    if (isOn)
    {
        g.setColour(toggleColour.withAlpha(0.3f));
        g.fillEllipse(area.toFloat().reduced(5.0f));
    }
}

void DraggableToggleButton::drawCustomStyle(juce::Graphics& g, juce::Rectangle<int> area, bool isOn)
{
    auto customBounds = area.reduced(4).toFloat();
    if (isOn)
    {
        for (int i = 2; i >= 0; --i)
        {
            g.setColour(toggleColour.withAlpha(0.25f * (3 - i)));
            g.fillRoundedRectangle(customBounds.expanded(i * 3.0f), 8.0f);
        }
    }
    juce::ColourGradient gradient(isOn ? toggleColour.brighter(0.4f) : juce::Colours::grey, customBounds.getTopLeft(), isOn ? toggleColour.darker(0.2f) : juce::Colours::darkgrey, customBounds.getBottomRight(), false);
    g.setGradientFill(gradient);
    g.fillRoundedRectangle(customBounds, 8.0f);
    g.setColour(isOn ? toggleColour.brighter(0.8f) : juce::Colours::lightgrey);
    g.drawRoundedRectangle(customBounds, 8.0f, 2.0f);
}

// ============================================================================
// DraggableTransportButton Implementation
// ============================================================================

DraggableTransportButton::DraggableTransportButton(const juce::String& name, TransportType type)
    : DraggableWidget(static_cast<DraggableWidget::WidgetType>(DraggableWidget::TransportPlay + static_cast<int>(type)), name), 
      transportType(type)
{
    setSize(60, 60);
    
    // Set default color based on transport type
    switch (transportType)
    {
        case Play:
            activeColour = juce::Colours::green;
            break;
        case Pause:
            activeColour = juce::Colours::orange;
            break;
        case Stop:
            activeColour = juce::Colours::red;
            break;
        case Record:
            activeColour = juce::Colours::red;
            break;
        case Loop:
            activeColour = juce::Colours::yellow;
            break;
        case Metronome:
            activeColour = juce::Colours::cyan;
            break;
        case ReturnToZero:
            activeColour = juce::Colours::lightblue;
            break;
        case MarkerNext:
        case MarkerPrevious:
            activeColour = juce::Colours::purple;
            break;
        case NudgeForward:
        case NudgeBackward:
            activeColour = juce::Colours::lightgreen;
            break;
        case Drop:
            activeColour = juce::Colours::orangered;
            break;
        case Replace:
            activeColour = juce::Colours::darkred;
            break;
        case AutomationMode:
            activeColour = juce::Colours::magenta;
            break;
        case JogWheel:
            activeColour = juce::Colours::skyblue;
            break;
        case Scrub:
            activeColour = juce::Colours::lightcyan;
            break;
        case PreRoll:
        case PostRoll:
            activeColour = juce::Colours::lightyellow;
            break;
        default:
            activeColour = juce::Colours::lightblue;
            break;
    }
}

void DraggableTransportButton::resized()
{
    DraggableWidget::resized();
}

juce::ValueTree DraggableTransportButton::toValueTree() const
{
    auto tree = DraggableWidget::toValueTree();
    tree.setProperty("transportType", static_cast<int>(transportType), nullptr);
    tree.setProperty("buttonStyle", buttonStyle, nullptr);
    tree.setProperty("active", active, nullptr);
    tree.setProperty("buttonColour", buttonColour.toString(), nullptr);
    tree.setProperty("activeColour", activeColour.toString(), nullptr);
    return tree;
}

void DraggableTransportButton::mouseDown(const juce::MouseEvent& e)
{
    if (!isResizing && !isDragging)
    {
        // Toggle active state for latching buttons
        // Latching: Play, Record, Loop, Metronome, AutomationMode, PreRoll, PostRoll
        if (transportType == Play || transportType == Record || 
            transportType == Loop || transportType == Metronome ||
            transportType == AutomationMode || transportType == PreRoll || 
            transportType == PostRoll)
        {
            active = !active;
            repaint();
        }
        else
        {
            // Momentary for all others
            active = true;
            repaint();
        }
    }
    DraggableWidget::mouseDown(e);
}

void DraggableTransportButton::mouseDrag(const juce::MouseEvent& e)
{
    DraggableWidget::mouseDrag(e);
}

void DraggableTransportButton::mouseUp(const juce::MouseEvent& e)
{
    // Deactivate momentary buttons
    if (!isDragging && !isResizing)
    {
        // Momentary buttons that deactivate on release
        if (transportType == Pause || transportType == Stop || 
            transportType == Rewind || transportType == FastForward ||
            transportType == Tap || transportType == Sync ||
            transportType == ReturnToZero || transportType == MarkerNext ||
            transportType == MarkerPrevious || transportType == NudgeForward ||
            transportType == NudgeBackward || transportType == Drop ||
            transportType == Replace || transportType == JogWheel ||
            transportType == Scrub)
        {
            active = false;
            repaint();
        }
    }
    DraggableWidget::mouseUp(e);
}

void DraggableTransportButton::paintWidget(juce::Graphics& g)
{
    auto bounds = getLocalBounds().reduced(10);
    
    // Draw button background based on style
    auto colour = active ? activeColour : buttonColour;
    
    switch (buttonStyle)
    {
        case Flat:
            g.setColour(colour);
            g.fillRect(bounds);
            break;
            
        case Raised:
            g.setColour(colour.darker(0.3f));
            g.fillRect(bounds.translated(2, 2));
            g.setColour(colour);
            g.fillRect(bounds);
            g.setColour(colour.brighter(0.3f));
            g.drawRect(bounds, 1);
            break;
            
        case Circular:
            g.setColour(colour);
            g.fillEllipse(bounds.toFloat());
            if (active)
            {
                g.setColour(colour.brighter(0.4f));
                g.drawEllipse(bounds.toFloat().reduced(2), 2.0f);
            }
            break;
            
        case Rounded:
            g.setColour(colour);
            g.fillRoundedRectangle(bounds.toFloat(), 8.0f);
            break;
            
        case Neon:
            if (active)
            {
                for (int i = 3; i > 0; --i)
                {
                    g.setColour(colour.withAlpha(0.2f * i));
                    g.fillEllipse(bounds.toFloat().expanded(i * 4.0f));
                }
            }
            g.setColour(colour);
            g.fillEllipse(bounds.toFloat());
            break;
            
        case Retro:
            {
                juce::ColourGradient grad(colour.brighter(0.3f), bounds.getTopLeft().toFloat(),
                                         colour.darker(0.3f), bounds.getBottomRight().toFloat(), false);
                g.setGradientFill(grad);
                g.fillRoundedRectangle(bounds.toFloat(), 4.0f);
                g.setColour(juce::Colours::black.withAlpha(0.3f));
                g.drawRoundedRectangle(bounds.toFloat().reduced(1), 4.0f, 1.0f);
            }
            break;
            
        case Modern:
            g.setColour(colour.withAlpha(0.8f));
            g.fillRoundedRectangle(bounds.toFloat(), 12.0f);
            if (active)
            {
                g.setColour(colour.brighter(0.5f));
                g.drawRoundedRectangle(bounds.toFloat().reduced(2), 10.0f, 2.0f);
            }
            break;
            
        case Minimal:
            g.setColour(colour.withAlpha(active ? 1.0f : 0.3f));
            g.fillRect(bounds);
            break;
            
        case Studio:
            g.setColour(juce::Colour(30, 30, 35));
            g.fillRoundedRectangle(bounds.toFloat(), 6.0f);
            g.setColour(colour);
            g.fillRoundedRectangle(bounds.toFloat().reduced(4), 4.0f);
            break;
            
        case Vintage:
            g.setColour(juce::Colour(60, 50, 40));
            g.fillRoundedRectangle(bounds.toFloat(), 8.0f);
            g.setColour(colour.withAlpha(active ? 1.0f : 0.6f));
            g.fillRoundedRectangle(bounds.toFloat().reduced(6), 4.0f);
            break;
    }
    
    // Draw transport icon
    auto iconArea = bounds.toFloat().reduced(bounds.getWidth() * 0.25f);
    drawTransportIcon(g, iconArea);
    
    // Draw label
    g.setColour(juce::Colours::white);
    g.setFont(12.0f);
    auto labelArea = getLocalBounds().removeFromBottom(15);
    g.drawText(getName(), labelArea, juce::Justification::centred);
}

void DraggableTransportButton::drawTransportIcon(juce::Graphics& g, juce::Rectangle<float> area)
{
    g.setColour(juce::Colours::white);
    
    switch (transportType)
    {
        case Play: drawPlayIcon(g, area); break;
        case Pause: drawPauseIcon(g, area); break;
        case Stop: drawStopIcon(g, area); break;
        case Record: drawRecordIcon(g, area); break;
        case Rewind: drawRewindIcon(g, area); break;
        case FastForward: drawFastForwardIcon(g, area); break;
        case Loop: drawLoopIcon(g, area); break;
        case Metronome: drawMetronomeIcon(g, area); break;
        case Tap: drawTapIcon(g, area); break;
        case Sync: drawSyncIcon(g, area); break;
        case ReturnToZero: drawReturnToZeroIcon(g, area); break;
        case MarkerNext: drawMarkerNextIcon(g, area); break;
        case MarkerPrevious: drawMarkerPreviousIcon(g, area); break;
        case NudgeForward: drawNudgeForwardIcon(g, area); break;
        case NudgeBackward: drawNudgeBackwardIcon(g, area); break;
        case Drop: drawDropIcon(g, area); break;
        case Replace: drawReplaceIcon(g, area); break;
        case AutomationMode: drawAutomationModeIcon(g, area); break;
        case JogWheel: drawJogWheelIcon(g, area); break;
        case Scrub: drawScrubIcon(g, area); break;
        case PreRoll: drawPreRollIcon(g, area); break;
        case PostRoll: drawPostRollIcon(g, area); break;
    }
}

void DraggableTransportButton::drawPlayIcon(juce::Graphics& g, juce::Rectangle<float> area)
{
    juce::Path triangle;
    triangle.addTriangle(area.getX(), area.getY(),
                        area.getX(), area.getBottom(),
                        area.getRight(), area.getCentreY());
    g.fillPath(triangle);
}

void DraggableTransportButton::drawPauseIcon(juce::Graphics& g, juce::Rectangle<float> area)
{
    auto barWidth = area.getWidth() * 0.35f;
    g.fillRect(area.getX(), area.getY(), barWidth, area.getHeight());
    g.fillRect(area.getRight() - barWidth, area.getY(), barWidth, area.getHeight());
}

void DraggableTransportButton::drawStopIcon(juce::Graphics& g, juce::Rectangle<float> area)
{
    g.fillRect(area);
}

void DraggableTransportButton::drawRecordIcon(juce::Graphics& g, juce::Rectangle<float> area)
{
    g.fillEllipse(area);
}

void DraggableTransportButton::drawRewindIcon(juce::Graphics& g, juce::Rectangle<float> area)
{
    auto halfWidth = area.getWidth() * 0.5f;
    juce::Path triangle1, triangle2;
    
    auto left = area.removeFromLeft(halfWidth);
    triangle1.addTriangle(left.getRight(), left.getY(),
                         left.getRight(), left.getBottom(),
                         left.getX(), left.getCentreY());
    
    triangle2.addTriangle(area.getRight(), area.getY(),
                         area.getRight(), area.getBottom(),
                         area.getX(), area.getCentreY());
    
    g.fillPath(triangle1);
    g.fillPath(triangle2);
}

void DraggableTransportButton::drawFastForwardIcon(juce::Graphics& g, juce::Rectangle<float> area)
{
    auto halfWidth = area.getWidth() * 0.5f;
    juce::Path triangle1, triangle2;
    
    auto left = area.removeFromLeft(halfWidth);
    triangle1.addTriangle(left.getX(), left.getY(),
                         left.getX(), left.getBottom(),
                         left.getRight(), left.getCentreY());
    
    triangle2.addTriangle(area.getX(), area.getY(),
                         area.getX(), area.getBottom(),
                         area.getRight(), area.getCentreY());
    
    g.fillPath(triangle1);
    g.fillPath(triangle2);
}

void DraggableTransportButton::drawLoopIcon(juce::Graphics& g, juce::Rectangle<float> area)
{
    juce::Path loop;
    loop.addArrow({area.getX() + area.getWidth() * 0.7f, area.getY() + area.getHeight() * 0.3f,
                   area.getX() + area.getWidth() * 0.3f, area.getY() + area.getHeight() * 0.3f},
                  2.0f, 8.0f, 6.0f);
    loop.addArrow({area.getX() + area.getWidth() * 0.3f, area.getY() + area.getHeight() * 0.7f,
                   area.getX() + area.getWidth() * 0.7f, area.getY() + area.getHeight() * 0.7f},
                  2.0f, 8.0f, 6.0f);
    g.fillPath(loop);
}

void DraggableTransportButton::drawMetronomeIcon(juce::Graphics& g, juce::Rectangle<float> area)
{
    juce::Path metronome;
    metronome.startNewSubPath(area.getCentreX(), area.getY());
    metronome.lineTo(area.getX() + area.getWidth() * 0.3f, area.getBottom());
    metronome.lineTo(area.getX() + area.getWidth() * 0.7f, area.getBottom());
    metronome.closeSubPath();
    g.fillPath(metronome);
    
    // Draw pendulum
    g.drawLine(area.getCentreX(), area.getY() + area.getHeight() * 0.2f,
               area.getCentreX() + area.getWidth() * 0.2f, area.getY() + area.getHeight() * 0.7f, 2.0f);
}

void DraggableTransportButton::drawTapIcon(juce::Graphics& g, juce::Rectangle<float> area)
{
    g.fillEllipse(area.reduced(area.getWidth() * 0.1f));
    g.setColour(buttonColour);
    g.fillEllipse(area.reduced(area.getWidth() * 0.35f));
}

void DraggableTransportButton::drawSyncIcon(juce::Graphics& g, juce::Rectangle<float> area)
{
    juce::Path sync;
    float radius = area.getWidth() * 0.4f;
    sync.addCentredArc(area.getCentreX(), area.getCentreY(), radius, radius,
                       0.0f, 0.0f, juce::MathConstants<float>::pi * 1.5f, true);
    
    // Add arrow at end
    auto endX = area.getCentreX();
    auto endY = area.getCentreY() - radius;
    sync.addArrow({endX + 5, endY, endX, endY}, 2.0f, 6.0f, 4.0f);
    
    g.strokePath(sync, juce::PathStrokeType(2.5f));
}

void DraggableTransportButton::drawReturnToZeroIcon(juce::Graphics& g, juce::Rectangle<float> area)
{
    // Draw "0" or "|<" symbol
    g.drawLine(area.getX(), area.getY(), area.getX(), area.getBottom(), 3.0f);
    
    juce::Path triangle;
    auto triArea = area.withTrimmedLeft(area.getWidth() * 0.3f);
    triangle.addTriangle(triArea.getRight(), triArea.getY(),
                        triArea.getRight(), triArea.getBottom(),
                        triArea.getX(), triArea.getCentreY());
    g.fillPath(triangle);
}

void DraggableTransportButton::drawMarkerNextIcon(juce::Graphics& g, juce::Rectangle<float> area)
{
    // Triangle pointing right with vertical bar
    juce::Path triangle;
    auto triArea = area.withTrimmedRight(area.getWidth() * 0.2f);
    triangle.addTriangle(triArea.getX(), triArea.getY(),
                        triArea.getX(), triArea.getBottom(),
                        triArea.getRight(), triArea.getCentreY());
    g.fillPath(triangle);
    
    g.fillRect(area.getRight() - 3.0f, area.getY(), 3.0f, area.getHeight());
}

void DraggableTransportButton::drawMarkerPreviousIcon(juce::Graphics& g, juce::Rectangle<float> area)
{
    // Triangle pointing left with vertical bar
    g.fillRect(area.getX(), area.getY(), 3.0f, area.getHeight());
    
    juce::Path triangle;
    auto triArea = area.withTrimmedLeft(area.getWidth() * 0.2f);
    triangle.addTriangle(triArea.getRight(), triArea.getY(),
                        triArea.getRight(), triArea.getBottom(),
                        triArea.getX(), triArea.getCentreY());
    g.fillPath(triangle);
}

void DraggableTransportButton::drawNudgeForwardIcon(juce::Graphics& g, juce::Rectangle<float> area)
{
    // Small triangle pointing right
    juce::Path triangle;
    auto smallArea = area.reduced(area.getWidth() * 0.2f);
    triangle.addTriangle(smallArea.getX(), smallArea.getY(),
                        smallArea.getX(), smallArea.getBottom(),
                        smallArea.getRight(), smallArea.getCentreY());
    g.strokePath(triangle, juce::PathStrokeType(2.0f));
}

void DraggableTransportButton::drawNudgeBackwardIcon(juce::Graphics& g, juce::Rectangle<float> area)
{
    // Small triangle pointing left
    juce::Path triangle;
    auto smallArea = area.reduced(area.getWidth() * 0.2f);
    triangle.addTriangle(smallArea.getRight(), smallArea.getY(),
                        smallArea.getRight(), smallArea.getBottom(),
                        smallArea.getX(), smallArea.getCentreY());
    g.strokePath(triangle, juce::PathStrokeType(2.0f));
}

void DraggableTransportButton::drawDropIcon(juce::Graphics& g, juce::Rectangle<float> area)
{
    // Down arrow with circle (drop in/out)
    g.fillEllipse(area.reduced(area.getWidth() * 0.3f).withY(area.getY()));
    
    juce::Path arrow;
    arrow.addArrow({area.getCentreX(), area.getCentreY(),
                   area.getCentreX(), area.getBottom()},
                  2.0f, 8.0f, 6.0f);
    g.fillPath(arrow);
}

void DraggableTransportButton::drawReplaceIcon(juce::Graphics& g, juce::Rectangle<float> area)
{
    // "R" letter or circular arrow
    g.setFont(juce::FontOptions(area.getHeight() * 0.8f, juce::Font::bold));
    g.drawText("R", area, juce::Justification::centred);
}

void DraggableTransportButton::drawAutomationModeIcon(juce::Graphics& g, juce::Rectangle<float> area)
{
    // "A" letter or automation curve
    juce::Path curve;
    curve.startNewSubPath(area.getX(), area.getBottom());
    curve.lineTo(area.getX() + area.getWidth() * 0.3f, area.getY());
    curve.lineTo(area.getX() + area.getWidth() * 0.6f, area.getCentreY());
    curve.lineTo(area.getRight(), area.getY() + area.getHeight() * 0.3f);
    g.strokePath(curve, juce::PathStrokeType(2.5f));
}

void DraggableTransportButton::drawJogWheelIcon(juce::Graphics& g, juce::Rectangle<float> area)
{
    // Circular wheel with notches
    g.drawEllipse(area, 2.5f);
    
    // Draw notches around the wheel
    auto center = area.getCentre();
    auto radius = area.getWidth() * 0.5f;
    for (int i = 0; i < 8; ++i)
    {
        auto angle = i * juce::MathConstants<float>::pi * 0.25f;
        auto x1 = center.x + std::cos(angle) * radius * 0.7f;
        auto y1 = center.y + std::sin(angle) * radius * 0.7f;
        auto x2 = center.x + std::cos(angle) * radius;
        auto y2 = center.y + std::sin(angle) * radius;
        g.drawLine(x1, y1, x2, y2, 2.0f);
    }
}

void DraggableTransportButton::drawScrubIcon(juce::Graphics& g, juce::Rectangle<float> area)
{
    // Waveform with cursor
    juce::Path wave;
    wave.startNewSubPath(area.getX(), area.getCentreY());
    
    for (int i = 0; i < 5; ++i)
    {
        auto x = area.getX() + (i * area.getWidth() / 4.0f);
        auto y = area.getCentreY() + (i % 2 == 0 ? -area.getHeight() * 0.3f : area.getHeight() * 0.3f);
        wave.lineTo(x, y);
    }
    
    g.strokePath(wave, juce::PathStrokeType(2.0f));
    
    // Cursor line
    g.drawLine(area.getCentreX(), area.getY(), area.getCentreX(), area.getBottom(), 2.5f);
}

void DraggableTransportButton::drawPreRollIcon(juce::Graphics& g, juce::Rectangle<float> area)
{
    // Left bracket with arrow
    juce::Path bracket;
    bracket.startNewSubPath(area.getRight() - 3, area.getY());
    bracket.lineTo(area.getX() + 3, area.getY());
    bracket.lineTo(area.getX() + 3, area.getBottom());
    bracket.lineTo(area.getRight() - 3, area.getBottom());
    g.strokePath(bracket, juce::PathStrokeType(2.5f));
    
    // Arrow pointing left
    juce::Path arrow;
    auto arrowArea = area.reduced(area.getWidth() * 0.3f);
    arrow.addTriangle(arrowArea.getRight(), arrowArea.getY() + arrowArea.getHeight() * 0.3f,
                     arrowArea.getRight(), arrowArea.getBottom() - arrowArea.getHeight() * 0.3f,
                     arrowArea.getX(), arrowArea.getCentreY());
    g.fillPath(arrow);
}

void DraggableTransportButton::drawPostRollIcon(juce::Graphics& g, juce::Rectangle<float> area)
{
    // Right bracket with arrow
    juce::Path bracket;
    bracket.startNewSubPath(area.getX() + 3, area.getY());
    bracket.lineTo(area.getRight() - 3, area.getY());
    bracket.lineTo(area.getRight() - 3, area.getBottom());
    bracket.lineTo(area.getX() + 3, area.getBottom());
    g.strokePath(bracket, juce::PathStrokeType(2.5f));
    
    // Arrow pointing right
    juce::Path arrow;
    auto arrowArea = area.reduced(area.getWidth() * 0.3f);
    arrow.addTriangle(arrowArea.getX(), arrowArea.getY() + arrowArea.getHeight() * 0.3f,
                     arrowArea.getX(), arrowArea.getBottom() - arrowArea.getHeight() * 0.3f,
                     arrowArea.getRight(), arrowArea.getCentreY());
    g.fillPath(arrow);
}
