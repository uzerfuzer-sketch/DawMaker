#include "DraggableWidget.h"

// ============================================================================
// CustomKnobLookAndFeel - Renderizado de knobs con formas personalizadas
// ============================================================================

void CustomKnobLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                                             float sliderPosProportional, float rotaryStartAngle,
                                             float rotaryEndAngle, juce::Slider& slider)
{
    auto bounds = juce::Rectangle<int>(x, y, width, height).toFloat().reduced(10);
    auto radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
    auto centerX = bounds.getCentreX();
    auto centerY = bounds.getCentreY();
    auto angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);
    
    auto fillColour = slider.findColour(juce::Slider::rotarySliderFillColourId);
    auto outlineColour = slider.findColour(juce::Slider::rotarySliderOutlineColourId);
    
    // Renderizar segun el estilo
    if (knobStyle == 0) // Circular - Diseño mejorado con gradiente y sombra
    {
        // Sombra exterior
        g.setColour(juce::Colours::black.withAlpha(0.3f));
        g.fillEllipse(bounds.translated(2, 2));
        
        // Gradiente radial para profundidad
        if (fillKnob)
        {
            juce::ColourGradient gradient(
                fillColour.brighter(0.4f), centerX, centerY - radius * 0.3f,
                fillColour.darker(0.3f), centerX, centerY + radius * 0.5f,
                true
            );
            g.setGradientFill(gradient);
            g.fillEllipse(bounds);
        }
        
        // Borde principal
        g.setColour(outlineColour);
        g.drawEllipse(bounds, 2.5f);
        
        // Borde interior brillante
        g.setColour(outlineColour.brighter(0.6f).withAlpha(0.5f));
        g.drawEllipse(bounds.reduced(3), 1.0f);
        
        // Arco de progreso
        juce::Path valueArc;
        valueArc.addCentredArc(centerX, centerY, radius * 0.85f, radius * 0.85f,
                              0.0f, rotaryStartAngle, angle, true);
        g.setColour(outlineColour.brighter(0.8f));
        g.strokePath(valueArc, juce::PathStrokeType(3.0f));
        
        // Indicador mejorado con punto
        auto pointerLength = radius * 0.6f;
        auto pointerThickness = 3.5f;
        juce::Path pointer;
        pointer.addRectangle(-pointerThickness * 0.5f, -pointerLength, pointerThickness, pointerLength * 0.8f);
        pointer.applyTransform(juce::AffineTransform::rotation(angle).translated(centerX, centerY));
        
        g.setColour(outlineColour.brighter(1.2f));
        g.fillPath(pointer);
        
        // Punto central brillante
        g.setColour(outlineColour.brighter(1.5f));
        g.fillEllipse(centerX - 4, centerY - 4, 8, 8);
        g.setColour(fillColour.brighter(0.8f));
        g.fillEllipse(centerX - 2, centerY - 2, 4, 4);
    }
    else if (knobStyle == 1) // Rectangular - Diseño minimalista moderno
    {
        auto rectBounds = bounds.reduced(5);
        
        // Sombra
        g.setColour(juce::Colours::black.withAlpha(0.25f));
        g.fillRoundedRectangle(rectBounds.translated(1.5f, 1.5f), 4.0f);
        
        // Gradiente de fondo
        if (fillKnob)
        {
            juce::ColourGradient gradient(
                fillColour.brighter(0.3f), centerX, rectBounds.getY(),
                fillColour.darker(0.2f), centerX, rectBounds.getBottom(),
                false
            );
            g.setGradientFill(gradient);
            g.fillRoundedRectangle(rectBounds, 4.0f);
        }
        
        // Borde
        g.setColour(outlineColour);
        g.drawRoundedRectangle(rectBounds, 4.0f, 2.0f);
        
        // Barra de progreso vertical
        auto progressHeight = rectBounds.getHeight() * sliderPosProportional;
        auto progressBounds = juce::Rectangle<float>(
            rectBounds.getX() + 3,
            rectBounds.getBottom() - progressHeight - 3,
            rectBounds.getWidth() - 6,
            progressHeight
        );
        
        g.setColour(outlineColour.brighter(0.6f));
        g.fillRoundedRectangle(progressBounds, 2.0f);
        
        // Indicador rotatorio en la parte superior
        juce::Path indicator;
        auto indicatorLength = radius * 0.5f;
        indicator.addLineSegment(juce::Line<float>(centerX, centerY - indicatorLength, 
                                                   centerX, centerY - 5), 3.0f);
        indicator.applyTransform(juce::AffineTransform::rotation(angle, centerX, centerY - radius * 0.3f));
        g.setColour(outlineColour.brighter(1.0f));
        g.fillPath(indicator);
    }
    else if (knobStyle == 2) // Diamante - Diseño cristalino
    {
        // Sombra
        juce::Path diamondShadow;
        diamondShadow.addTriangle(centerX, centerY - radius,
                                 centerX + radius, centerY,
                                 centerX, centerY + radius);
        diamondShadow.addTriangle(centerX, centerY - radius,
                                 centerX - radius, centerY,
                                 centerX, centerY + radius);
        g.setColour(juce::Colours::black.withAlpha(0.3f));
        g.fillPath(diamondShadow, juce::AffineTransform::translation(2, 2));
        
        // Forma del diamante con gradiente
        juce::Path diamond;
        diamond.addTriangle(centerX, centerY - radius,
                           centerX + radius, centerY,
                           centerX, centerY + radius);
        diamond.addTriangle(centerX, centerY - radius,
                           centerX - radius, centerY,
                           centerX, centerY + radius);
        
        if (fillKnob)
        {
            juce::ColourGradient gradient(
                fillColour.brighter(0.5f), centerX - radius * 0.3f, centerY - radius * 0.3f,
                fillColour.darker(0.3f), centerX + radius * 0.3f, centerY + radius * 0.3f,
                false
            );
            g.setGradientFill(gradient);
            g.fillPath(diamond);
        }
        
        // Borde brillante
        g.setColour(outlineColour);
        g.strokePath(diamond, juce::PathStrokeType(2.5f));
        
        // Reflejo brillante
        juce::Path highlight;
        highlight.addTriangle(centerX, centerY - radius * 0.8f,
                            centerX + radius * 0.4f, centerY - radius * 0.2f,
                            centerX - radius * 0.4f, centerY - radius * 0.2f);
        g.setColour(outlineColour.brighter(1.0f).withAlpha(0.4f));
        g.fillPath(highlight);
        
        // Indicador
        juce::Path indicator;
        auto indicatorLength = radius * 0.6f;
        indicator.addLineSegment(juce::Line<float>(centerX, centerY - indicatorLength, 
                                                   centerX, centerY), 3.5f);
        indicator.applyTransform(juce::AffineTransform::rotation(angle, centerX, centerY));
        g.setColour(outlineColour.brighter(1.3f));
        g.fillPath(indicator);
        
        // Punto central
        g.setColour(outlineColour.brighter(1.5f));
        g.fillEllipse(centerX - 3, centerY - 3, 6, 6);
    }
    else if (knobStyle == 3) // Clasico - Estilo simple y limpio
    {
        const float rx = centerX - radius;
        const float ry = centerY - radius;
        const float rw = radius * 2.0f;
        
        // Relleno simple
        if (fillKnob)
        {
            g.setColour(fillColour);
            g.fillEllipse(rx, ry, rw, rw);
        }
        
        // Contorno
        g.setColour(outlineColour);
        g.drawEllipse(rx, ry, rw, rw, 2.0f);
        
        // Puntero rectangular simple
        juce::Path pointer;
        const float pointerLength = radius * 0.33f;
        const float pointerThickness = 3.0f;
        pointer.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
        pointer.applyTransform(juce::AffineTransform::rotation(angle).translated(centerX, centerY));
        
        g.setColour(outlineColour.brighter(0.8f));
        g.fillPath(pointer);
    }
    else if (knobStyle == 4) // Vintage - Estilo analogico con marcas de tick
    {
        // Textura de fondo con efecto madera/metal
        if (fillKnob)
        {
            juce::ColourGradient gradient(
                fillColour.darker(0.6f), centerX - radius * 0.5f, centerY - radius * 0.5f,
                fillColour.darker(0.2f), centerX + radius * 0.5f, centerY + radius * 0.5f,
                true
            );
            g.setGradientFill(gradient);
            g.fillEllipse(bounds);
        }
        
        // Anillo de tick marks (marcas de escala)
        g.setColour(outlineColour.darker(0.3f));
        for (int i = 0; i < 12; ++i)
        {
            auto tickAngle = juce::MathConstants<float>::pi * 0.25f + (i / 12.0f) * juce::MathConstants<float>::pi * 1.5f;
            auto tickLength = (i % 3 == 0) ? 8.0f : 5.0f; // Marcas principales mas largas
            auto tickWidth = (i % 3 == 0) ? 2.5f : 1.5f;
            
            auto startX = centerX + std::cos(tickAngle) * (radius - tickLength);
            auto startY = centerY + std::sin(tickAngle) * (radius - tickLength);
            auto endX = centerX + std::cos(tickAngle) * radius;
            auto endY = centerY + std::sin(tickAngle) * radius;
            
            g.drawLine(startX, startY, endX, endY, tickWidth);
        }
        
        // Borde con efecto metalico
        g.setColour(outlineColour.darker(0.4f));
        g.drawEllipse(bounds, 3.0f);
        g.setColour(outlineColour.brighter(0.3f));
        g.drawEllipse(bounds.reduced(2), 1.0f);
        
        // Puntero estilo aguja vintage
        juce::Path needle;
        needle.startNewSubPath(centerX, centerY);
        needle.lineTo(centerX + std::cos(angle - juce::MathConstants<float>::pi * 0.5f) * radius * 0.7f,
                     centerY + std::sin(angle - juce::MathConstants<float>::pi * 0.5f) * radius * 0.7f);
        
        g.setColour(juce::Colours::darkred.brighter(0.3f));
        g.strokePath(needle, juce::PathStrokeType(3.5f, juce::PathStrokeType::curved));
        
        // Tornillo central
        g.setColour(outlineColour.darker(0.5f));
        g.fillEllipse(centerX - 6, centerY - 6, 12, 12);
        g.setColour(outlineColour.brighter(0.2f));
        g.drawEllipse(centerX - 6, centerY - 6, 12, 12, 1.0f);
        g.drawLine(centerX - 4, centerY, centerX + 4, centerY, 1.0f);
        g.drawLine(centerX, centerY - 4, centerX, centerY + 4, 1.0f);
    }
    else if (knobStyle == 5) // Neon - Estilo brillante futurista
    {
        // Resplandor exterior
        for (int i = 3; i >= 0; --i)
        {
            float glowAlpha = 0.15f * (4 - i) / 4.0f;
            g.setColour(fillColour.brighter(0.6f).withAlpha(glowAlpha));
            g.drawEllipse(bounds.expanded(i * 3.0f), 2.0f);
        }
        
        // Anillo neon principal
        juce::Path outerRing;
        outerRing.addEllipse(bounds);
        g.setColour(fillColour.brighter(0.8f));
        g.strokePath(outerRing, juce::PathStrokeType(3.5f));
        
        // Arco de progreso brillante
        juce::Path progressArc;
        progressArc.addCentredArc(centerX, centerY, radius * 0.75f, radius * 0.75f,
                                 0.0f, rotaryStartAngle, angle, true);
        
        juce::ColourGradient arcGradient(
            fillColour.brighter(1.2f), centerX, centerY - radius,
            fillColour.brighter(0.4f), centerX, centerY + radius,
            false
        );
        g.setGradientFill(arcGradient);
        g.strokePath(progressArc, juce::PathStrokeType(6.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
        
        // Linea indicadora brillante
        auto indicatorLength = radius * 0.55f;
        juce::Path indicator;
        indicator.startNewSubPath(centerX, centerY);
        indicator.lineTo(centerX + std::cos(angle - juce::MathConstants<float>::pi * 0.5f) * indicatorLength,
                        centerY + std::sin(angle - juce::MathConstants<float>::pi * 0.5f) * indicatorLength);
        
        g.setColour(juce::Colours::white.withAlpha(0.9f));
        g.strokePath(indicator, juce::PathStrokeType(4.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
        
        // Punto central brillante
        g.setColour(juce::Colours::white);
        g.fillEllipse(centerX - 5, centerY - 5, 10, 10);
        g.setColour(fillColour.brighter(1.5f));
        g.fillEllipse(centerX - 3, centerY - 3, 6, 6);
    }
    else if (knobStyle == 6) // Minimal - Ultra simple con linea
    {
        // Sin relleno - solo contorno fino
        g.setColour(outlineColour.withAlpha(0.6f));
        g.drawEllipse(bounds, 1.5f);
        
        // Linea indicadora simple y elegante
        auto indicatorLength = radius * 0.65f;
        auto endX = centerX + std::cos(angle - juce::MathConstants<float>::pi * 0.5f) * indicatorLength;
        auto endY = centerY + std::sin(angle - juce::MathConstants<float>::pi * 0.5f) * indicatorLength;
        
        g.setColour(outlineColour);
        g.drawLine(centerX, centerY, endX, endY, 2.5f);
        
        // Punto en el extremo de la linea
        g.fillEllipse(endX - 3, endY - 3, 6, 6);
        
        // Pequeño circulo central
        g.drawEllipse(centerX - 4, centerY - 4, 8, 8, 1.0f);
    }
    else if (knobStyle == 7) // Dot - Anillo de puntos estilo Ableton
    {
        int numDots = 24;
        float dotRadius = 3.5f;
        float ringRadius = radius * 0.85f;
        
        // Calcular cuantos puntos deben estar iluminados
        float progress = (angle - rotaryStartAngle) / (rotaryEndAngle - rotaryStartAngle);
        int activeDots = (int)(progress * numDots);
        
        for (int i = 0; i < numDots; ++i)
        {
            auto dotAngle = rotaryStartAngle + (i / (float)numDots) * (rotaryEndAngle - rotaryStartAngle);
            auto dotX = centerX + std::cos(dotAngle - juce::MathConstants<float>::pi * 0.5f) * ringRadius;
            auto dotY = centerY + std::sin(dotAngle - juce::MathConstants<float>::pi * 0.5f) * ringRadius;
            
            // Puntos activos vs inactivos
            if (i <= activeDots)
            {
                // Punto activo - brillante
                g.setColour(fillColour.brighter(0.6f));
                g.fillEllipse(dotX - dotRadius, dotY - dotRadius, dotRadius * 2, dotRadius * 2);
                
                // Resplandor sutil
                g.setColour(fillColour.brighter(0.3f).withAlpha(0.3f));
                g.fillEllipse(dotX - dotRadius * 1.5f, dotY - dotRadius * 1.5f, dotRadius * 3, dotRadius * 3);
            }
            else
            {
                // Punto inactivo - tenue
                g.setColour(outlineColour.darker(0.5f).withAlpha(0.3f));
                g.fillEllipse(dotX - dotRadius * 0.7f, dotY - dotRadius * 0.7f, dotRadius * 1.4f, dotRadius * 1.4f);
            }
        }
        
        // Circulo central
        g.setColour(outlineColour);
        g.drawEllipse(centerX - radius * 0.2f, centerY - radius * 0.2f, radius * 0.4f, radius * 0.4f, 2.0f);
        
        if (fillKnob)
        {
            g.setColour(fillColour.darker(0.4f));
            g.fillEllipse(centerX - radius * 0.18f, centerY - radius * 0.18f, radius * 0.36f, radius * 0.36f);
        }
    }
    else if (knobStyle == 8) // Arc Pie - Estilo arco relleno con puntero
    {
        // Arco de fondo completo
        g.setColour(outlineColour.darker(0.6f));
        juce::Path backgroundArc;
        backgroundArc.addPieSegment(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight(), 
                                   rotaryStartAngle, rotaryEndAngle, 0.6f);
        g.fillPath(backgroundArc);
        
        // Arco de progreso
        g.setColour(fillColour.brighter(0.4f));
        juce::Path progressArc;
        progressArc.addPieSegment(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight(),
                                 rotaryStartAngle, angle, 0.6f);
        g.fillPath(progressArc);
        
        // Puntero triangular
        juce::Path pointer;
        const float pointerLength = radius * 0.63f;
        const float pointerThickness = radius * 0.2f;
        pointer.addRectangle(-pointerThickness * 0.5f, -radius - 1, pointerThickness, pointerLength);
        pointer.applyTransform(juce::AffineTransform::rotation(angle).translated(centerX, centerY));
        
        g.setColour(outlineColour.darker(0.4f));
        g.fillPath(pointer);
        
        // Circulo central grande
        const float dotRadius = radius * 0.4f;
        const float dotDiameter = dotRadius * 2.0f;
        g.setColour(outlineColour.darker(0.4f));
        g.fillEllipse(centerX - dotRadius, centerY - dotRadius, dotDiameter, dotDiameter);
    }
    else if (knobStyle == 9) // Filmstrip - Knob basado en imagen PNG
    {
        if (filmstripImage.isValid())
        {
            // Calcular el frame basado en la posicion del slider
            const int numFrames = filmstripImage.getHeight() / filmstripImage.getWidth();
            const int frameId = juce::jmin((int)(sliderPosProportional * numFrames), numFrames - 1);
            
            const float radius = juce::jmin(width / 2.0f, height / 2.0f);
            const float centerX = x + width * 0.5f;
            const float centerY = y + height * 0.5f;
            const float rx = centerX - radius - 1.0f;
            const float ry = centerY - radius;
            
            // Dibujar el frame correspondiente del filmstrip
            g.drawImage(filmstripImage,
                       (int)rx, (int)ry,
                       2 * (int)radius, 2 * (int)radius,
                       0, frameId * filmstripImage.getWidth(),
                       filmstripImage.getWidth(), filmstripImage.getWidth());
        }
        else
        {
            // Sin imagen - mostrar mensaje
            g.setColour(outlineColour.darker(0.5f));
            g.drawEllipse(bounds, 2.0f);
            
            g.setColour(juce::Colours::white.withAlpha(0.7f));
            g.drawFittedText("No Image\nLoad PNG", 
                           bounds.reduced(10).toNearestInt(),
                           juce::Justification::centred, 2);
        }
    }
}

// ============================================================================
// CustomSliderLookAndFeel - 10 estilos de slider
// ============================================================================

void CustomSliderLookAndFeel::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
                                               float sliderPos, float minSliderPos, float maxSliderPos,
                                               const juce::Slider::SliderStyle sliderType, juce::Slider& slider)
{
    auto trackBounds = (sliderType == juce::Slider::LinearVertical)
                       ? juce::Rectangle<float>(x + width * 0.4f, y, width * 0.2f, height)
                       : juce::Rectangle<float>(x, y + height * 0.4f, width, height * 0.2f);
    
    auto thumbBounds = (sliderType == juce::Slider::LinearVertical)
                       ? juce::Rectangle<float>(x, sliderPos - 10, width, 20)
                       : juce::Rectangle<float>(sliderPos - 10, y, 20, height);
    
    auto trackColor = slider.findColour(juce::Slider::trackColourId);
    auto thumbColor = slider.findColour(juce::Slider::thumbColourId);
    auto bgColor = slider.findColour(juce::Slider::backgroundColourId);
    
    bool isVertical = (sliderType == juce::Slider::LinearVertical);
    
    switch(sliderStyle)
    {
        case 0: // Moderno con gradiente
        {
            // Track de fondo
            g.setColour(bgColor.withAlpha(0.3f));
            g.fillRoundedRectangle(trackBounds, 4.0f);
            
            // Track activo (progreso)
            auto progressBounds = trackBounds;
            if (isVertical)
                progressBounds.setTop(sliderPos);
            else
                progressBounds.setWidth(sliderPos - trackBounds.getX());
            
            juce::ColourGradient gradient(
                trackColor.brighter(0.3f), 
                isVertical ? trackBounds.getCentreX() : trackBounds.getX(),
                isVertical ? trackBounds.getY() : trackBounds.getCentreY(),
                trackColor.darker(0.2f),
                isVertical ? trackBounds.getCentreX() : trackBounds.getRight(),
                isVertical ? trackBounds.getBottom() : trackBounds.getCentreY(),
                false
            );
            g.setGradientFill(gradient);
            g.fillRoundedRectangle(progressBounds, 4.0f);
            
            // Thumb
            g.setColour(juce::Colours::black.withAlpha(0.3f));
            g.fillEllipse(thumbBounds.translated(2, 2));
            g.setColour(thumbColor);
            g.fillEllipse(thumbBounds);
            g.setColour(thumbColor.brighter(0.6f));
            g.drawEllipse(thumbBounds, 2.0f);
            break;
        }
        
        case 1: // Minimalista
        {
            // Track fino
            auto thinTrack = trackBounds.reduced(isVertical ? width * 0.35f : 0, 
                                                 isVertical ? 0 : height * 0.35f);
            g.setColour(bgColor.withAlpha(0.4f));
            g.fillRoundedRectangle(thinTrack, 2.0f);
            
            // Línea de progreso
            auto progressLine = thinTrack;
            if (isVertical)
                progressLine.setTop(sliderPos);
            else
                progressLine.setWidth(sliderPos - thinTrack.getX());
            g.setColour(trackColor);
            g.fillRoundedRectangle(progressLine, 2.0f);
            
            // Thumb rectangular pequeño
            auto smallThumb = isVertical 
                ? juce::Rectangle<float>(x + width * 0.2f, sliderPos - 3, width * 0.6f, 6)
                : juce::Rectangle<float>(sliderPos - 3, y + height * 0.2f, 6, height * 0.6f);
            g.setColour(thumbColor);
            g.fillRoundedRectangle(smallThumb, 2.0f);
            break;
        }
        
        case 2: // Volumétrico con sombra 3D
        {
            // Track con efecto 3D
            g.setColour(juce::Colours::black.withAlpha(0.2f));
            g.fillRoundedRectangle(trackBounds.translated(1, 1), 3.0f);
            
            g.setColour(bgColor.darker(0.3f));
            g.fillRoundedRectangle(trackBounds, 3.0f);
            
            // Borde superior brillante
            g.setColour(bgColor.brighter(0.3f));
            g.drawRoundedRectangle(trackBounds.reduced(1), 3.0f, 1.0f);
            
            // Progreso
            auto progressBounds = trackBounds.reduced(2);
            if (isVertical)
                progressBounds.setTop(sliderPos);
            else
                progressBounds.setWidth(sliderPos - trackBounds.getX() - 2);
            
            g.setColour(trackColor);
            g.fillRoundedRectangle(progressBounds, 2.0f);
            
            // Thumb con relieve
            auto thumbRect = thumbBounds.reduced(2);
            g.setColour(juce::Colours::black.withAlpha(0.4f));
            g.fillRoundedRectangle(thumbRect.translated(2, 2), 4.0f);
            g.setColour(thumbColor);
            g.fillRoundedRectangle(thumbRect, 4.0f);
            g.setColour(thumbColor.brighter(0.8f));
            g.drawRoundedRectangle(thumbRect, 4.0f, 1.5f);
            break;
        }
        
        case 3: // LED Bar (segmentado)
        {
            int segments = isVertical ? 20 : 30;
            float segmentSize = (isVertical ? trackBounds.getHeight() : trackBounds.getWidth()) / segments;
            float progress = (sliderPos - (isVertical ? trackBounds.getY() : trackBounds.getX())) / 
                           (isVertical ? trackBounds.getHeight() : trackBounds.getWidth());
            
            if (!isVertical) progress = 1.0f - progress;
            
            for (int i = 0; i < segments; ++i)
            {
                float segProgress = (float)i / segments;
                bool isLit = (isVertical ? (1.0f - segProgress) <= (1.0f - progress) : segProgress <= progress);
                
                auto segBounds = isVertical
                    ? juce::Rectangle<float>(trackBounds.getX() + 2, trackBounds.getY() + i * segmentSize + 1,
                                            trackBounds.getWidth() - 4, segmentSize - 2)
                    : juce::Rectangle<float>(trackBounds.getX() + i * segmentSize + 1, trackBounds.getY() + 2,
                                            segmentSize - 2, trackBounds.getHeight() - 4);
                
                if (isLit)
                {
                    // Color según la posición (verde -> amarillo -> rojo)
                    juce::Colour ledColor;
                    if (segProgress < 0.6f)
                        ledColor = juce::Colour(0xff00ff00);
                    else if (segProgress < 0.8f)
                        ledColor = juce::Colour(0xffffff00);
                    else
                        ledColor = juce::Colour(0xffff0000);
                    
                    g.setColour(ledColor);
                    g.fillRoundedRectangle(segBounds, 1.0f);
                }
                else
                {
                    g.setColour(bgColor.withAlpha(0.2f));
                    g.fillRoundedRectangle(segBounds, 1.0f);
                }
            }
            break;
        }
        
        case 4: // Neón brillante
        {
            // Glow exterior
            for (int i = 0; i < 3; ++i)
            {
                g.setColour(trackColor.withAlpha(0.1f * (3 - i)));
                g.drawRoundedRectangle(trackBounds.expanded(i * 2), 6.0f, 2.0f);
            }
            
            // Track
            g.setColour(bgColor.darker(0.8f));
            g.fillRoundedRectangle(trackBounds, 5.0f);
            
            // Progreso con brillo
            auto progressBounds = trackBounds.reduced(1);
            if (isVertical)
                progressBounds.setTop(sliderPos);
            else
                progressBounds.setWidth(sliderPos - trackBounds.getX() - 1);
            
            g.setColour(trackColor);
            g.fillRoundedRectangle(progressBounds, 4.0f);
            
            // Glow del progreso
            g.setColour(trackColor.withAlpha(0.4f));
            g.drawRoundedRectangle(progressBounds.expanded(2), 6.0f, 3.0f);
            
            // Thumb brillante
            g.setColour(thumbColor.brighter(1.0f));
            g.fillEllipse(thumbBounds.reduced(2));
            
            for (int i = 0; i < 2; ++i)
            {
                g.setColour(thumbColor.withAlpha(0.3f * (2 - i)));
                g.drawEllipse(thumbBounds.expanded(i * 3), 2.0f);
            }
            break;
        }
        
        case 5: // Groove profundo
        {
            // Surco oscuro
            g.setColour(juce::Colours::black.withAlpha(0.5f));
            g.fillRoundedRectangle(trackBounds, 4.0f);
            
            // Borde del surco
            g.setColour(juce::Colours::black.withAlpha(0.3f));
            g.drawRoundedRectangle(trackBounds.reduced(1), 3.0f, 1.0f);
            
            // Progreso con relieve
            auto progressBounds = trackBounds.reduced(3);
            if (isVertical)
                progressBounds.setTop(sliderPos + 3);
            else
                progressBounds.setWidth(sliderPos - trackBounds.getX() - 3);
            
            g.setColour(trackColor.darker(0.3f));
            g.fillRoundedRectangle(progressBounds, 2.0f);
            g.setColour(trackColor);
            g.fillRoundedRectangle(progressBounds.reduced(1), 2.0f);
            
            // Thumb elevado
            g.setColour(juce::Colours::black.withAlpha(0.3f));
            g.fillRoundedRectangle(thumbBounds.translated(1, 1).reduced(1), 6.0f);
            g.setColour(thumbColor.darker(0.2f));
            g.fillRoundedRectangle(thumbBounds.reduced(1), 6.0f);
            g.setColour(thumbColor.brighter(0.4f));
            g.fillRoundedRectangle(thumbBounds.reduced(4), 4.0f);
            break;
        }
        
        case 6: // Flat material design
        {
            // Track plano
            g.setColour(bgColor.withAlpha(0.3f));
            g.fillRoundedRectangle(trackBounds, 2.0f);
            
            // Progreso
            auto progressBounds = trackBounds;
            if (isVertical)
                progressBounds.setTop(sliderPos);
            else
                progressBounds.setWidth(sliderPos - trackBounds.getX());
            
            g.setColour(trackColor);
            g.fillRoundedRectangle(progressBounds, 2.0f);
            
            // Thumb cuadrado
            auto squareThumb = isVertical
                ? juce::Rectangle<float>(x, sliderPos - 8, width, 16)
                : juce::Rectangle<float>(sliderPos - 8, y, 16, height);
            
            g.setColour(thumbColor);
            g.fillRoundedRectangle(squareThumb, 2.0f);
            break;
        }
        
        case 7: // Vintage con muescas
        {
            // Background metálico
            juce::ColourGradient metalGrad(
                juce::Colour(0xff888888), trackBounds.getX(), trackBounds.getY(),
                juce::Colour(0xff666666), trackBounds.getRight(), trackBounds.getBottom(),
                false
            );
            g.setGradientFill(metalGrad);
            g.fillRoundedRectangle(trackBounds, 3.0f);
            
            // Muescas decorativas
            int notches = isVertical ? 15 : 20;
            for (int i = 0; i < notches; ++i)
            {
                float pos = i * (isVertical ? trackBounds.getHeight() : trackBounds.getWidth()) / notches;
                g.setColour(juce::Colours::black.withAlpha(0.3f));
                
                if (isVertical)
                    g.drawLine(trackBounds.getX(), trackBounds.getY() + pos,
                             trackBounds.getRight(), trackBounds.getY() + pos, 1.0f);
                else
                    g.drawLine(trackBounds.getX() + pos, trackBounds.getY(),
                             trackBounds.getX() + pos, trackBounds.getBottom(), 1.0f);
            }
            
            // Thumb vintage
            g.setColour(juce::Colour(0xffffcc66));
            g.fillRoundedRectangle(thumbBounds.reduced(1), 3.0f);
            g.setColour(juce::Colour(0xffcc9944));
            g.drawRoundedRectangle(thumbBounds.reduced(1), 3.0f, 2.0f);
            break;
        }
        
        case 8: // Círculos concéntricos
        {
            // Track simple
            g.setColour(bgColor.withAlpha(0.2f));
            g.fillRoundedRectangle(trackBounds, 3.0f);
            
            // Progreso
            auto progressBounds = trackBounds.reduced(1);
            if (isVertical)
                progressBounds.setTop(sliderPos);
            else
                progressBounds.setWidth(sliderPos - trackBounds.getX() - 1);
            g.setColour(trackColor);
            g.fillRoundedRectangle(progressBounds, 2.0f);
            
            // Thumb con círculos concéntricos
            auto centerX = thumbBounds.getCentreX();
            auto centerY = thumbBounds.getCentreY();
            auto radius = juce::jmin(thumbBounds.getWidth(), thumbBounds.getHeight()) / 2;
            
            for (int i = 3; i >= 0; --i)
            {
                float r = radius * (1.0f - i * 0.2f);
                g.setColour(thumbColor.withAlpha(0.3f + i * 0.15f));
                g.fillEllipse(centerX - r, centerY - r, r * 2, r * 2);
            }
            break;
        }
        
        case 9: // Línea de progreso animada
        {
            // Track transparente
            g.setColour(bgColor.withAlpha(0.1f));
            g.fillRoundedRectangle(trackBounds, 2.0f);
            g.setColour(bgColor.withAlpha(0.3f));
            g.drawRoundedRectangle(trackBounds, 2.0f, 1.0f);
            
            // Progreso con patrón de líneas
            auto progressBounds = trackBounds.reduced(2);
            if (isVertical)
                progressBounds.setTop(sliderPos);
            else
                progressBounds.setWidth(sliderPos - trackBounds.getX() - 2);
            
            g.setColour(trackColor);
            g.fillRoundedRectangle(progressBounds, 1.0f);
            
            // Patrón de líneas diagonales
            g.setColour(trackColor.brighter(0.3f));
            int stripes = isVertical ? (int)progressBounds.getHeight() / 4 : (int)progressBounds.getWidth() / 4;
            for (int i = 0; i < stripes; ++i)
            {
                if (isVertical)
                    g.drawLine(progressBounds.getX(), progressBounds.getY() + i * 4,
                             progressBounds.getRight(), progressBounds.getY() + i * 4, 1.0f);
                else
                    g.drawLine(progressBounds.getX() + i * 4, progressBounds.getY(),
                             progressBounds.getX() + i * 4, progressBounds.getBottom(), 1.0f);
            }
            
            // Thumb diamante
            juce::Path diamond;
            auto centerX = thumbBounds.getCentreX();
            auto centerY = thumbBounds.getCentreY();
            auto size = juce::jmin(thumbBounds.getWidth(), thumbBounds.getHeight()) / 2;
            diamond.addTriangle(centerX, centerY - size, 
                              centerX + size, centerY,
                              centerX, centerY + size);
            diamond.addTriangle(centerX, centerY - size,
                              centerX - size, centerY,
                              centerX, centerY + size);
            g.setColour(thumbColor);
            g.fillPath(diamond);
            g.setColour(thumbColor.brighter(0.5f));
            g.strokePath(diamond, juce::PathStrokeType(2.0f));
            break;
        }
    }
}

// ============================================================================
// CustomButtonLookAndFeel - 10 estilos de botón
// ============================================================================

void CustomButtonLookAndFeel::drawButtonBackground(juce::Graphics& g, juce::Button& button,
                                                    const juce::Colour& backgroundColour,
                                                    bool shouldDrawButtonAsHighlighted,
                                                    bool shouldDrawButtonAsDown)
{
    auto bounds = button.getLocalBounds().toFloat().reduced(2);
    auto baseColour = backgroundColour.withMultipliedSaturation(button.hasKeyboardFocus(true) ? 1.3f : 0.9f)
                                     .withMultipliedAlpha(button.isEnabled() ? 1.0f : 0.5f);
    
    if (shouldDrawButtonAsDown)
        baseColour = baseColour.darker(0.2f);
    else if (shouldDrawButtonAsHighlighted)
        baseColour = baseColour.brighter(0.1f);
    
    switch(buttonStyle)
    {
        case 0: // Moderno con gradiente
        {
            juce::ColourGradient gradient(
                baseColour.brighter(0.3f), bounds.getCentreX(), bounds.getY(),
                baseColour.darker(0.2f), bounds.getCentreX(), bounds.getBottom(),
                false
            );
            g.setGradientFill(gradient);
            g.fillRoundedRectangle(bounds, 6.0f);
            
            g.setColour(baseColour.brighter(0.5f));
            g.drawRoundedRectangle(bounds, 6.0f, 1.5f);
            break;
        }
        
        case 1: // Flat material
        {
            g.setColour(baseColour);
            g.fillRoundedRectangle(bounds, 3.0f);
            
            if (shouldDrawButtonAsDown)
            {
                g.setColour(juce::Colours::black.withAlpha(0.2f));
                g.fillRoundedRectangle(bounds, 3.0f);
            }
            break;
        }
        
        case 2: // Relieve 3D
        {
            // Sombra
            g.setColour(juce::Colours::black.withAlpha(0.3f));
            g.fillRoundedRectangle(bounds.translated(0, shouldDrawButtonAsDown ? 1 : 2), 5.0f);
            
            // Base
            auto buttonBounds = bounds.translated(0, shouldDrawButtonAsDown ? 2 : 0);
            juce::ColourGradient gradient(
                baseColour.brighter(0.4f), buttonBounds.getCentreX(), buttonBounds.getY(),
                baseColour.darker(0.1f), buttonBounds.getCentreX(), buttonBounds.getBottom(),
                false
            );
            g.setGradientFill(gradient);
            g.fillRoundedRectangle(buttonBounds, 5.0f);
            
            // Highlight superior
            g.setColour(baseColour.brighter(0.8f).withAlpha(0.3f));
            g.fillRoundedRectangle(buttonBounds.removeFromTop(buttonBounds.getHeight() * 0.4f), 5.0f);
            
            // Borde
            g.setColour(baseColour.darker(0.3f));
            g.drawRoundedRectangle(buttonBounds, 5.0f, 1.5f);
            break;
        }
        
        case 3: // Neón
        {
            // Glow exterior
            for (int i = 0; i < 4; ++i)
            {
                g.setColour(baseColour.withAlpha(0.15f * (4 - i)));
                g.drawRoundedRectangle(bounds.expanded(i * 2), 8.0f, 2.0f);
            }
            
            // Base oscura
            g.setColour(juce::Colours::black.withAlpha(0.7f));
            g.fillRoundedRectangle(bounds, 6.0f);
            
            // Borde brillante
            g.setColour(baseColour);
            g.drawRoundedRectangle(bounds, 6.0f, 2.5f);
            
            // Glow interior
            g.setColour(baseColour.withAlpha(0.2f));
            g.fillRoundedRectangle(bounds.reduced(3), 4.0f);
            break;
        }
        
        case 4: // Outlined
        {
            g.setColour(baseColour.withAlpha(0.1f));
            g.fillRoundedRectangle(bounds, 4.0f);
            
            g.setColour(baseColour);
            g.drawRoundedRectangle(bounds, 4.0f, 2.5f);
            
            if (shouldDrawButtonAsDown || shouldDrawButtonAsHighlighted)
            {
                g.setColour(baseColour.withAlpha(0.3f));
                g.fillRoundedRectangle(bounds.reduced(2), 3.0f);
            }
            break;
        }
        
        case 5: // Pill (cápsula)
        {
            float radius = bounds.getHeight() / 2;
            
            g.setColour(baseColour);
            g.fillRoundedRectangle(bounds, radius);
            
            // Highlight
            g.setColour(baseColour.brighter(0.4f).withAlpha(0.4f));
            g.fillRoundedRectangle(bounds.removeFromTop(bounds.getHeight() * 0.5f), radius);
            
            // Borde
            g.setColour(baseColour.darker(0.3f));
            g.drawRoundedRectangle(bounds, radius, 1.5f);
            break;
        }
        
        case 6: // Glass (vidrio)
        {
            // Base translúcida
            g.setColour(baseColour.withAlpha(0.6f));
            g.fillRoundedRectangle(bounds, 8.0f);
            
            // Reflejo superior
            auto topHalf = bounds.removeFromTop(bounds.getHeight() * 0.5f);
            juce::ColourGradient reflectionGrad(
                juce::Colours::white.withAlpha(0.3f), topHalf.getCentreX(), topHalf.getY(),
                juce::Colours::white.withAlpha(0.0f), topHalf.getCentreX(), topHalf.getBottom(),
                false
            );
            g.setGradientFill(reflectionGrad);
            g.fillRoundedRectangle(topHalf, 8.0f);
            
            // Borde
            g.setColour(baseColour.brighter(0.6f));
            g.drawRoundedRectangle(bounds, 8.0f, 1.0f);
            break;
        }
        
        case 7: // Segmented (segmentado)
        {
            g.setColour(baseColour);
            g.fillRoundedRectangle(bounds, 2.0f);
            
            // Líneas de segmentación
            int segments = 3;
            g.setColour(baseColour.darker(0.3f));
            for (int i = 1; i < segments; ++i)
            {
                float x = bounds.getX() + i * bounds.getWidth() / segments;
                g.drawLine(x, bounds.getY(), x, bounds.getBottom(), 1.0f);
            }
            
            // Borde
            g.drawRoundedRectangle(bounds, 2.0f, 1.5f);
            break;
        }
        
        case 8: // Circular
        {
            // Hacer el botón circular
            auto size = juce::jmin(bounds.getWidth(), bounds.getHeight());
            auto circleBounds = bounds.withSizeKeepingCentre(size, size);
            
            // Sombra
            g.setColour(juce::Colours::black.withAlpha(0.3f));
            g.fillEllipse(circleBounds.translated(2, 2));
            
            // Base con gradiente radial
            juce::ColourGradient radialGrad(
                baseColour.brighter(0.4f), circleBounds.getCentreX(), circleBounds.getCentreY() - size * 0.2f,
                baseColour.darker(0.2f), circleBounds.getCentreX(), circleBounds.getCentreY() + size * 0.3f,
                true
            );
            g.setGradientFill(radialGrad);
            g.fillEllipse(circleBounds);
            
            // Borde
            g.setColour(baseColour.darker(0.4f));
            g.drawEllipse(circleBounds, 2.0f);
            break;
        }
        
        case 9: // Hexagonal
        {
            juce::Path hexagon;
            auto centerX = bounds.getCentreX();
            auto centerY = bounds.getCentreY();
            auto radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.2f;
            
            for (int i = 0; i < 6; ++i)
            {
                float angle = i * juce::MathConstants<float>::pi / 3.0f;
                float x = centerX + radius * std::cos(angle);
                float y = centerY + radius * std::sin(angle);
                
                if (i == 0)
                    hexagon.startNewSubPath(x, y);
                else
                    hexagon.lineTo(x, y);
            }
            hexagon.closeSubPath();
            
            // Sombra
            g.setColour(juce::Colours::black.withAlpha(0.3f));
            g.fillPath(hexagon, juce::AffineTransform::translation(2, 2));
            
            // Base
            g.setColour(baseColour);
            g.fillPath(hexagon);
            
            // Borde
            g.setColour(baseColour.darker(0.4f));
            g.strokePath(hexagon, juce::PathStrokeType(2.0f));
            break;
        }
    }
}

void CustomButtonLookAndFeel::drawButtonText(juce::Graphics& g, juce::TextButton& button,
                                             bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    auto font = juce::FontOptions(16.0f);
    g.setFont(font);
    
    auto textColour = button.findColour(juce::TextButton::textColourOffId);
    
    if (!button.isEnabled())
        textColour = textColour.withAlpha(0.4f);
    
    g.setColour(textColour);
    
    auto yIndent = juce::jmin(4, button.proportionOfHeight(0.3f));
    auto cornerSize = juce::jmin(button.getHeight(), button.getWidth()) / 2;
    
    auto fontHeight = juce::roundToInt(font.getHeight() * 0.6f);
    auto leftIndent = juce::jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnLeft() ? 4 : 2));
    auto rightIndent = juce::jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnRight() ? 4 : 2));
    auto textWidth = button.getWidth() - leftIndent - rightIndent;
    
    if (textWidth > 0)
        g.drawFittedText(button.getButtonText(),
                        leftIndent, yIndent, textWidth, button.getHeight() - yIndent * 2,
                        juce::Justification::centred, 2);
}

// ============================================================================
// DraggableWidget - Clase Base
// ============================================================================

DraggableWidget::DraggableWidget(WidgetType type, const juce::String& name)
    : widgetType(type),
      widgetName(name),
      resizer(this, &constrainer),
      deleteButton("X")
{
    // Configurar constrainer para limitar tamaño mínimo
    constrainer.setMinimumSize(50, 50);
    
    // Añadir componente de redimensionamiento
    addAndMakeVisible(resizer);
    
    // Configurar botón de eliminar
    deleteButton.setSize(20, 20);
    deleteButton.onClick = [this] { deleteButtonClicked(); };
    addAndMakeVisible(deleteButton);
    
    setSize(100, 100);
}

void DraggableWidget::paint(juce::Graphics& g)
{
    // Fondo del widget con color personalizable (solo si no es transparente)
    if (!style.bgColor.isTransparent())
    {
        auto bgColour = isHovered ? style.bgColor.brighter(0.1f) : style.bgColor;
        g.setColour(bgColour);
        g.fillRoundedRectangle(getLocalBounds().toFloat(), style.cornerRadius);
    }
    
    // Borde con color y grosor personalizables (solo si no es transparente)
    if (!style.borderColor.isTransparent())
    {
        auto borderColour = isHovered ? style.borderColor.brighter(0.2f) : style.borderColor;
        auto borderWidth = isHovered ? style.borderWidth + 1.0f : style.borderWidth;
        g.setColour(borderColour);
        g.drawRoundedRectangle(getLocalBounds().toFloat().reduced(1), style.cornerRadius, borderWidth);
    }
    
    // Pintar el widget específico
    paintWidget(g);
}

void DraggableWidget::resized()
{
    auto bounds = getLocalBounds();
    
    // Posicionar botón de eliminar en la esquina superior derecha
    deleteButton.setBounds(bounds.getWidth() - 22, 2, 20, 20);
}

void DraggableWidget::mouseDown(const juce::MouseEvent& e)
{
    // Clic derecho abre menu contextual
    if (e.mods.isPopupMenu())
    {
        juce::PopupMenu menu;
        menu.addItem(1, "Configurar Propiedades");
        menu.addSeparator();
        menu.addItem(2, "Asignar MIDI CC");
        menu.addItem(3, "Duplicar");
        menu.addItem(4, "Traer al Frente");
        menu.addItem(5, "Enviar al Fondo");
        menu.addSeparator();
        menu.addItem(6, "Eliminar");
        
        menu.showMenuAsync(juce::PopupMenu::Options(), [this](int result) {
            switch (result)
            {
                case 1: // Configurar
                    if (onConfigRequested)
                        onConfigRequested(this);
                    break;
                case 2: // Asignar MIDI
                    if (onMidiLearnRequested)
                        onMidiLearnRequested(this);
                    break;
                case 3: // Duplicar (futuro)
                    break;
                case 4: // Traer al frente
                    toFront(true);
                    break;
                case 5: // Enviar al fondo
                    toBack();
                    break;
                case 6: // Eliminar
                    if (onDeleteRequested)
                        onDeleteRequested(this);
                    break;
            }
        });
        return;
    }
    
    // Si hacemos clic en el borde, estamos redimensionando
    auto border = 10;
    auto bounds = getLocalBounds();
    
    if (e.x < border || e.x > bounds.getWidth() - border ||
        e.y < border || e.y > bounds.getHeight() - border)
    {
        isResizing = true;
    }
    else
    {
        isDragging = true;
        dragger.startDraggingComponent(this, e);
    }
    
    toFront(true);
}

void DraggableWidget::mouseDrag(const juce::MouseEvent& e)
{
    if (isDragging)
    {
        dragger.dragComponent(this, e, nullptr);
        repaint();
    }
}

void DraggableWidget::mouseUp(const juce::MouseEvent&)
{
    isDragging = false;
    isResizing = false;
}

void DraggableWidget::mouseEnter(const juce::MouseEvent&)
{
    isHovered = true;
    repaint();
}

void DraggableWidget::mouseExit(const juce::MouseEvent&)
{
    isHovered = false;
    repaint();
}

void DraggableWidget::deleteButtonClicked()
{
    if (onDeleteRequested)
        onDeleteRequested(this);
}

juce::ValueTree DraggableWidget::toValueTree() const
{
    juce::ValueTree tree("Widget");
    tree.setProperty("type", static_cast<int>(widgetType), nullptr);
    tree.setProperty("name", widgetName, nullptr);
    tree.setProperty("x", getX(), nullptr);
    tree.setProperty("y", getY(), nullptr);
    tree.setProperty("width", getWidth(), nullptr);
    tree.setProperty("height", getHeight(), nullptr);
    
    // Guardar propiedades de estilo
    tree.setProperty("bgColor", style.bgColor.toString(), nullptr);
    tree.setProperty("borderColor", style.borderColor.toString(), nullptr);
    tree.setProperty("knobColor", style.knobColor.toString(), nullptr);
    tree.setProperty("textColor", style.textColor.toString(), nullptr);
    tree.setProperty("borderWidth", style.borderWidth, nullptr);
    tree.setProperty("cornerRadius", style.cornerRadius, nullptr);
    tree.setProperty("fillKnob", style.fillKnob, nullptr);
    tree.setProperty("knobStyle", style.knobStyle, nullptr);
    tree.setProperty("filmstripPath", style.filmstripPath, nullptr);
    tree.setProperty("sliderStyle", style.sliderStyle, nullptr);
    tree.setProperty("buttonStyle", style.buttonStyle, nullptr);
    tree.setProperty("labelStyle", style.labelStyle, nullptr);
    
    // Guardar asignación MIDI
    tree.setProperty("hasMidiAssignment", hasMidiAssignment, nullptr);
    tree.setProperty("midiCC", midiCC, nullptr);
    tree.setProperty("midiChannel", midiChannel, nullptr);
    
    return tree;
}

std::unique_ptr<DraggableWidget> DraggableWidget::fromValueTree(const juce::ValueTree& tree)
{
    auto type = static_cast<WidgetType>(int(tree.getProperty("type")));
    auto name = tree.getProperty("name").toString();
    
    std::unique_ptr<DraggableWidget> widget;
    
    switch (type)
    {
        case Knob:
            widget = std::make_unique<DraggableKnob>(name);
            break;
        case Slider:
            widget = std::make_unique<DraggableSlider>(name);
            break;
        case Button:
            widget = std::make_unique<DraggableButton>(name);
            break;
        case Label:
            widget = std::make_unique<DraggableLabel>(name, "Label");
            break;
        case XYPad:
        {
            widget = std::make_unique<DraggableXYPad>(name);
            auto* xypad = static_cast<DraggableXYPad*>(widget.get());
            if (tree.hasProperty("xValue"))
                xypad->setXValue(tree.getProperty("xValue"));
            if (tree.hasProperty("yValue"))
                xypad->setYValue(tree.getProperty("yValue"));
            break;
        }
        case Joystick:
        {
            widget = std::make_unique<DraggableJoystick>(name);
            // Los valores se restauran automaticamente al cargar
            break;
        }
        case PitchWheel:
        {
            widget = std::make_unique<DraggablePitchWheel>(name);
            break;
        }
        case IndexedSlider:
        {
            juce::StringArray loadedLabels;
            if (tree.hasProperty("labels"))
            {
                juce::String labelsString = tree.getProperty("labels").toString();
                loadedLabels.addTokens(labelsString, ",", "");
            }
            if (loadedLabels.isEmpty())
                loadedLabels.add("Option 1");
            
            widget = std::make_unique<DraggableIndexedSlider>(name, loadedLabels);
            auto* indexedSlider = static_cast<DraggableIndexedSlider*>(widget.get());
            if (tree.hasProperty("currentIndex"))
                indexedSlider->setIndex(tree.getProperty("currentIndex"));
            break;
        }
        case BasicMeter:
        {
            widget = std::make_unique<DraggableBasicMeter>(name);
            auto* meter = static_cast<DraggableBasicMeter*>(widget.get());
            if (tree.hasProperty("meterValue"))
                meter->setMeterValue(tree.getProperty("meterValue"));
            break;
        }
        case VUMeter:
        {
            widget = std::make_unique<DraggableVUMeter>(name);
            auto* vuMeter = static_cast<DraggableVUMeter*>(widget.get());
            if (tree.hasProperty("meterValue"))
                vuMeter->setMeterValue(tree.getProperty("meterValue"));
            break;
        }
        case LedMeter:
        {
            widget = std::make_unique<DraggableLedMeter>(name);
            auto* ledMeter = static_cast<DraggableLedMeter*>(widget.get());
            if (tree.hasProperty("meterValue"))
                ledMeter->setMeterValue(tree.getProperty("meterValue"));
            break;
        }
        case AudioScope:
        {
            widget = std::make_unique<DraggableAudioScope>(name);
            break;
        }
        case VerticalFader:
        {
            widget = std::make_unique<DraggableVerticalFader>(name);
            break;
        }
        case ImageButton:
        {
            widget = std::make_unique<DraggableImageButton>(name);
            break;
        }
        case ToggleButton:
        {
            widget = std::make_unique<DraggableToggleButton>(name);
            break;
        }
        default:
            return nullptr;
    }
    
    widget->setBounds(tree.getProperty("x"),
                     tree.getProperty("y"),
                     tree.getProperty("width"),
                     tree.getProperty("height"));
    
    // Restaurar propiedades de estilo si existen
    if (tree.hasProperty("bgColor"))
    {
        widget->style.bgColor = juce::Colour::fromString(tree.getProperty("bgColor").toString());
        widget->style.borderColor = juce::Colour::fromString(tree.getProperty("borderColor").toString());
        widget->style.knobColor = juce::Colour::fromString(tree.getProperty("knobColor").toString());
        widget->style.textColor = juce::Colour::fromString(tree.getProperty("textColor").toString());
        widget->style.borderWidth = tree.getProperty("borderWidth");
        widget->style.cornerRadius = tree.getProperty("cornerRadius");
        widget->style.fillKnob = tree.getProperty("fillKnob");
        widget->style.knobStyle = tree.getProperty("knobStyle");
        widget->style.filmstripPath = tree.getProperty("filmstripPath").toString();
        widget->style.sliderStyle = tree.getProperty("sliderStyle", 0);
        widget->style.buttonStyle = tree.getProperty("buttonStyle", 0);
        widget->style.labelStyle = tree.getProperty("labelStyle", 0);
    }
    
    // Restaurar asignación MIDI
    if (tree.hasProperty("hasMidiAssignment") && tree.getProperty("hasMidiAssignment"))
    {
        int cc = tree.getProperty("midiCC");
        int channel = tree.getProperty("midiChannel");
        widget->setMidiCC(cc, channel);
    }
    
    return widget;
}

// ============================================================================
// DraggableKnob
// ============================================================================

DraggableKnob::DraggableKnob(const juce::String& name)
    : DraggableWidget(WidgetType::Knob, name)
{
    knob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    knob.setRange(0.0, 1.0, 0.01);
    knob.setValue(0.5);
    knob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    knob.setLookAndFeel(&knobLookAndFeel);
    
    // CRÍTICO: Los hijos NO deben interceptar eventos de mouse
    knob.setInterceptsMouseClicks(false, false);
    knob.setMouseClickGrabsKeyboardFocus(false);
    
    addAndMakeVisible(knob);
    
    label.setText(name, juce::dontSendNotification);
    label.setJustificationType(juce::Justification::centred);
    label.setEditable(false, false, false);
    label.setInterceptsMouseClicks(false, false);
    addAndMakeVisible(label);
}

void DraggableKnob::resized()
{
    DraggableWidget::resized();
    
    auto bounds = getLocalBounds().reduced(10);
    label.setBounds(bounds.removeFromBottom(20));
    knob.setBounds(bounds);
}

void DraggableKnob::paintWidget(juce::Graphics&)
{
    // Actualizar el look and feel con el estilo actual
    knobLookAndFeel.setKnobStyle(style.knobStyle, style.fillKnob);
    
    // Si el estilo es filmstrip (9) y hay una ruta de imagen, cargarla
    if (style.knobStyle == 9 && style.filmstripPath.isNotEmpty())
    {
        juce::File imageFile(style.filmstripPath);
        if (imageFile.existsAsFile())
        {
            auto image = juce::ImageCache::getFromFile(imageFile);
            if (image.isValid())
            {
                knobLookAndFeel.setFilmstripImage(image);
            }
        }
    }
    
    // Aplicar colores del estilo al knob
    knob.setColour(juce::Slider::rotarySliderFillColourId, style.knobColor);
    knob.setColour(juce::Slider::rotarySliderOutlineColourId, style.borderColor);
    knob.setColour(juce::Slider::thumbColourId, style.knobColor.brighter(0.3f));
    
    label.setColour(juce::Label::textColourId, style.textColor);
}

void DraggableKnob::setWidgetName(const juce::String& newName)
{
    DraggableWidget::setWidgetName(newName);
    label.setText(newName, juce::dontSendNotification);
}

juce::ValueTree DraggableKnob::toValueTree() const
{
    auto tree = DraggableWidget::toValueTree();
    tree.setProperty("value", knob.getValue(), nullptr);
    return tree;
}

void DraggableKnob::mouseDown(const juce::MouseEvent& e)
{
    DraggableWidget::mouseDown(e);
}

void DraggableKnob::mouseDrag(const juce::MouseEvent& e)
{
    DraggableWidget::mouseDrag(e);
}

void DraggableKnob::mouseUp(const juce::MouseEvent& e)
{
    DraggableWidget::mouseUp(e);
}

void DraggableKnob::updateFromMidiValue(int value)
{
    // Convertir valor MIDI (0-127) al rango del knob
    double normalizedValue = value / 127.0;
    double min = knob.getMinimum();
    double max = knob.getMaximum();
    double targetValue = min + (normalizedValue * (max - min));
    
    knob.setValue(targetValue, juce::sendNotificationAsync);
}

// ============================================================================
// DraggableSlider
// ============================================================================

DraggableSlider::DraggableSlider(const juce::String& name, bool isVertical)
    : DraggableWidget(WidgetType::Slider, name),
      vertical(isVertical)
{
    slider.setSliderStyle(vertical ? juce::Slider::LinearVertical : juce::Slider::LinearHorizontal);
    slider.setRange(0.0, 1.0, 0.01);
    slider.setValue(0.5);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    slider.setLookAndFeel(&sliderLookAndFeel);
    
    // Los hijos NO deben interceptar eventos de mouse
    slider.setInterceptsMouseClicks(false, false);
    addAndMakeVisible(slider);
    
    label.setText(name, juce::dontSendNotification);
    label.setJustificationType(juce::Justification::centred);
    label.setInterceptsMouseClicks(false, false);
    addAndMakeVisible(label);
}

void DraggableSlider::resized()
{
    DraggableWidget::resized();
    
    auto bounds = getLocalBounds().reduced(10);
    
    if (vertical)
    {
        label.setBounds(bounds.removeFromBottom(20));
        slider.setBounds(bounds);
    }
    else
    {
        label.setBounds(bounds.removeFromLeft(60));
        slider.setBounds(bounds);
    }
}

void DraggableSlider::paintWidget(juce::Graphics&)
{
    // Aplicar estilo al slider
    sliderLookAndFeel.setSliderStyle(style.sliderStyle);
    
    // Aplicar colores del estilo
    slider.setColour(juce::Slider::backgroundColourId, style.bgColor);
    slider.setColour(juce::Slider::trackColourId, style.knobColor);
    slider.setColour(juce::Slider::thumbColourId, style.borderColor);
    
    label.setColour(juce::Label::textColourId, style.textColor);
}

void DraggableSlider::setWidgetName(const juce::String& newName)
{
    DraggableWidget::setWidgetName(newName);
    label.setText(newName, juce::dontSendNotification);
}

juce::ValueTree DraggableSlider::toValueTree() const
{
    auto tree = DraggableWidget::toValueTree();
    tree.setProperty("value", slider.getValue(), nullptr);
    tree.setProperty("vertical", vertical, nullptr);
    return tree;
}

void DraggableSlider::mouseDown(const juce::MouseEvent& e)
{
    DraggableWidget::mouseDown(e);
}

void DraggableSlider::mouseDrag(const juce::MouseEvent& e)
{
    DraggableWidget::mouseDrag(e);
}

void DraggableSlider::mouseUp(const juce::MouseEvent& e)
{
    DraggableWidget::mouseUp(e);
}

void DraggableSlider::updateFromMidiValue(int value)
{
    // Convertir valor MIDI (0-127) al rango del slider
    double normalizedValue = value / 127.0;
    double min = slider.getMinimum();
    double max = slider.getMaximum();
    double targetValue = min + (normalizedValue * (max - min));
    
    slider.setValue(targetValue, juce::sendNotificationAsync);
}

// ============================================================================
// DraggableButton
// ============================================================================

DraggableButton::DraggableButton(const juce::String& name)
    : DraggableWidget(WidgetType::Button, name)
{
    button.setButtonText(name);
    button.setLookAndFeel(&buttonLookAndFeel);
    
    // El botón NO debe interceptar eventos de mouse
    button.setInterceptsMouseClicks(false, false);
    addAndMakeVisible(button);
}

void DraggableButton::resized()
{
    DraggableWidget::resized();
    
    auto bounds = getLocalBounds().reduced(10);
    button.setBounds(bounds);
}

void DraggableButton::paintWidget(juce::Graphics&)
{
    // Aplicar estilo al botón
    buttonLookAndFeel.setButtonStyle(style.buttonStyle);
    
    // Aplicar colores del estilo
    button.setColour(juce::TextButton::buttonColourId, style.knobColor);
    button.setColour(juce::TextButton::textColourOffId, style.textColor);
    button.setColour(juce::TextButton::textColourOnId, style.textColor.brighter(0.2f));
}

void DraggableButton::setWidgetName(const juce::String& newName)
{
    DraggableWidget::setWidgetName(newName);
    button.setButtonText(newName);
}

juce::ValueTree DraggableButton::toValueTree() const
{
    return DraggableWidget::toValueTree();
}

void DraggableButton::mouseDown(const juce::MouseEvent& e)
{
    DraggableWidget::mouseDown(e);
}

void DraggableButton::mouseDrag(const juce::MouseEvent& e)
{
    DraggableWidget::mouseDrag(e);
}

void DraggableButton::mouseUp(const juce::MouseEvent& e)
{
    DraggableWidget::mouseUp(e);
}

// ============================================================================
// DraggableLabel
// ============================================================================

DraggableLabel::DraggableLabel(const juce::String& name, const juce::String& text)
    : DraggableWidget(WidgetType::Label, name)
{
    label.setText(text, juce::dontSendNotification);
    label.setJustificationType(juce::Justification::centred);
    label.setEditable(true);
    
    // El label NO debe interceptar eventos de mouse para arrastrar
    // pero SÍ necesita capturar clics para edición
    label.setInterceptsMouseClicks(false, false);
    addAndMakeVisible(label);
}

void DraggableLabel::resized()
{
    DraggableWidget::resized();
    
    auto bounds = getLocalBounds().reduced(10);
    label.setBounds(bounds);
}

void DraggableLabel::paintWidget(juce::Graphics& g)
{
    // Aplicar colores y estilos del label
    label.setColour(juce::Label::textColourId, style.textColor);
    label.setColour(juce::Label::backgroundColourId, style.bgColor);
    label.setColour(juce::Label::outlineColourId, style.borderColor);
    
    // Aplicar diferentes estilos de fuente y alineación según labelStyle
    switch (style.labelStyle)
    {
        case 0: // Normal
            label.setFont(juce::FontOptions(14.0f));
            label.setJustificationType(juce::Justification::centred);
            break;
        case 1: // Bold
            label.setFont(juce::FontOptions(14.0f, juce::Font::bold));
            label.setJustificationType(juce::Justification::centred);
            break;
        case 2: // Italic
            label.setFont(juce::FontOptions(14.0f, juce::Font::italic));
            label.setJustificationType(juce::Justification::centred);
            break;
        case 3: // Large
            label.setFont(juce::FontOptions(20.0f, juce::Font::bold));
            label.setJustificationType(juce::Justification::centred);
            break;
        case 4: // Small
            label.setFont(juce::FontOptions(10.0f));
            label.setJustificationType(juce::Justification::centred);
            break;
        case 5: // Monospace
            label.setFont(juce::FontOptions(juce::Font::getDefaultMonospacedFontName(), 13.0f, juce::Font::plain));
            label.setJustificationType(juce::Justification::left);
            break;
        case 6: // Left aligned
            label.setFont(juce::FontOptions(14.0f));
            label.setJustificationType(juce::Justification::left);
            break;
        case 7: // Right aligned
            label.setFont(juce::FontOptions(14.0f));
            label.setJustificationType(juce::Justification::right);
            break;
        case 8: // Title (large centered)
            label.setFont(juce::FontOptions(24.0f, juce::Font::bold));
            label.setJustificationType(juce::Justification::centred);
            break;
        case 9: // Subtitle (medium italic)
            label.setFont(juce::FontOptions(16.0f, juce::Font::italic));
            label.setJustificationType(juce::Justification::centred);
            break;
    }
}

void DraggableLabel::setWidgetName(const juce::String& newName)
{
    DraggableWidget::setWidgetName(newName);
    // Note: DraggableLabel uses the text property, not the widget name
    // So we don't update the label text here
}

juce::ValueTree DraggableLabel::toValueTree() const
{
    auto tree = DraggableWidget::toValueTree();
    tree.setProperty("text", label.getText(), nullptr);
    return tree;
}

void DraggableLabel::mouseDown(const juce::MouseEvent& e)
{
    DraggableWidget::mouseDown(e);
}

void DraggableLabel::mouseDrag(const juce::MouseEvent& e)
{
    DraggableWidget::mouseDrag(e);
}

void DraggableLabel::mouseUp(const juce::MouseEvent& e)
{
    DraggableWidget::mouseUp(e);
}

// ============================================================================
// DraggableMeter - Medidor de nivel de audio
// ============================================================================

DraggableMeter::DraggableMeter(const juce::String& name)
    : DraggableWidget(WidgetType::Meter, name)
{
    setSize(60, 200);
}

void DraggableMeter::resized()
{
    DraggableWidget::resized();
}

void DraggableMeter::setMeterValue(float value)
{
    currentValue = juce::jlimit(0.0f, 1.0f, value);
    if (currentValue > peakValue)
    {
        peakValue = currentValue;
        peakHoldTime = 2000.0f; // 2 segundos
    }
    repaint();
}

juce::Colour DraggableMeter::getColorForLevel(float level)
{
    if (level > 0.9f)
        return juce::Colours::red;
    else if (level > 0.7f)
        return juce::Colours::yellow;
    else
        return meterColour;
}

void DraggableMeter::paintWidget(juce::Graphics& g)
{
    auto bounds = getLocalBounds().reduced(15).toFloat();
    
    // Fondo oscuro
    g.setColour(juce::Colour(0xff1a1a1a));
    g.fillRoundedRectangle(bounds, 5.0f);
    
    g.setColour(juce::Colour(0xff444444));
    g.drawRoundedRectangle(bounds, 5.0f, 1.5f);
    
    // Dibujar según estilo
    switch (meterStyle)
    {
        case 0: drawVerticalMeter(g, bounds); break;
        case 1: drawHorizontalMeter(g, bounds); break;
        case 2: drawLEDMeter(g, bounds); break;
        case 3: drawVUMeter(g, bounds); break;
        case 4: drawPeakMeter(g, bounds); break;
        case 5: drawRMSMeter(g, bounds); break;
        case 6: drawGradientMeter(g, bounds); break;
        case 7: drawSegmentedMeter(g, bounds); break;
        case 8: drawCircularMeter(g, bounds); break;
        case 9: drawRadialMeter(g, bounds); break;
        default: drawVerticalMeter(g, bounds); break;
    }
    
    // Etiqueta
    g.setColour(juce::Colours::white.withAlpha(0.7f));
    g.setFont(juce::FontOptions(10.0f));
    g.drawText(getWidgetName(), bounds.reduced(3), juce::Justification::topLeft);
    
    // Indicador de estilo
    g.setFont(juce::FontOptions(9.0f));
    g.drawText("Style: " + juce::String(meterStyle), bounds.reduced(3), juce::Justification::bottomRight);
}

// Estilo 0: Barra vertical clasica
void DraggableMeter::drawVerticalMeter(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    auto meterArea = bounds.reduced(10, 15);
    float height = currentValue * meterArea.getHeight();
    float yPos = meterArea.getBottom() - height;
    
    // Fondo del meter
    g.setColour(juce::Colour(0xff2a2a2a));
    g.fillRect(meterArea);
    
    // Nivel actual con gradiente
    juce::ColourGradient gradient(
        getColorForLevel(0.0f), meterArea.getX(), meterArea.getBottom(),
        getColorForLevel(currentValue), meterArea.getX(), yPos,
        false
    );
    gradient.addColour(0.7, juce::Colours::yellow.withAlpha(0.8f));
    gradient.addColour(0.9, juce::Colours::orange);
    
    g.setGradientFill(gradient);
    g.fillRect(meterArea.getX(), yPos, meterArea.getWidth(), height);
    
    // Marcas de escala
    g.setColour(juce::Colours::grey);
    for (int i = 0; i <= 10; ++i)
    {
        float y = meterArea.getY() + (meterArea.getHeight() * i / 10.0f);
        g.drawLine(meterArea.getRight() + 2, y, meterArea.getRight() + 5, y, 1.0f);
    }
}

// Estilo 1: Barra horizontal
void DraggableMeter::drawHorizontalMeter(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    auto meterArea = bounds.reduced(15, 10);
    float width = currentValue * meterArea.getWidth();
    
    g.setColour(juce::Colour(0xff2a2a2a));
    g.fillRect(meterArea);
    
    g.setColour(getColorForLevel(currentValue));
    g.fillRect(meterArea.getX(), meterArea.getY(), width, meterArea.getHeight());
    
    // Peak hold indicator
    if (peakValue > 0.0f)
    {
        float peakX = meterArea.getX() + (peakValue * meterArea.getWidth());
        g.setColour(juce::Colours::white);
        g.drawLine(peakX, meterArea.getY(), peakX, meterArea.getBottom(), 2.0f);
    }
}

// Estilo 2: LEDs discretos
void DraggableMeter::drawLEDMeter(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    auto meterArea = bounds.reduced(10, 15);
    int numLEDs = 20;
    float ledHeight = (meterArea.getHeight() - (numLEDs - 1) * 2) / numLEDs;
    int litLEDs = static_cast<int>(currentValue * numLEDs);
    
    for (int i = 0; i < numLEDs; ++i)
    {
        float y = meterArea.getBottom() - ((i + 1) * (ledHeight + 2));
        bool isLit = i < litLEDs;
        
        juce::Colour ledColor;
        if (i >= numLEDs * 0.9f)
            ledColor = juce::Colours::red;
        else if (i >= numLEDs * 0.7f)
            ledColor = juce::Colours::yellow;
        else
            ledColor = meterColour;
        
        if (isLit)
        {
            g.setColour(ledColor);
            g.fillRoundedRectangle(meterArea.getX(), y, meterArea.getWidth(), ledHeight, 2.0f);
        }
        else
        {
            g.setColour(ledColor.withAlpha(0.2f));
            g.fillRoundedRectangle(meterArea.getX(), y, meterArea.getWidth(), ledHeight, 2.0f);
        }
    }
}

// Estilo 3: VU meter analogico
void DraggableMeter::drawVUMeter(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    auto center = bounds.getCentre();
    float radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) * 0.4f;
    
    // Dibujar arco de fondo
    float startAngle = juce::MathConstants<float>::pi * 0.75f;
    float endAngle = juce::MathConstants<float>::pi * 2.25f;
    
    juce::Path backgroundArc;
    backgroundArc.addCentredArc(center.x, center.y, radius, radius, 0.0f, startAngle, endAngle, true);
    
    g.setColour(juce::Colour(0xff2a2a2a));
    g.strokePath(backgroundArc, juce::PathStrokeType(8.0f));
    
    // Aguja
    float needleAngle = startAngle + (endAngle - startAngle) * currentValue;
    float needleLength = radius * 0.8f;
    
    juce::Point<float> needleEnd(
        center.x + std::cos(needleAngle) * needleLength,
        center.y + std::sin(needleAngle) * needleLength
    );
    
    g.setColour(juce::Colours::red);
    g.drawLine(center.x, center.y, needleEnd.x, needleEnd.y, 3.0f);
    g.fillEllipse(center.x - 5, center.y - 5, 10, 10);
    
    // Marcas
    g.setColour(juce::Colours::white.withAlpha(0.7f));
    for (int i = 0; i <= 10; ++i)
    {
        float angle = startAngle + (endAngle - startAngle) * (i / 10.0f);
        float markStart = radius * 0.9f;
        float markEnd = radius;
        
        juce::Point<float> start(center.x + std::cos(angle) * markStart, center.y + std::sin(angle) * markStart);
        juce::Point<float> end(center.x + std::cos(angle) * markEnd, center.y + std::sin(angle) * markEnd);
        
        g.drawLine(start.x, start.y, end.x, end.y, 1.5f);
    }
}

// Estilo 4: Peak meter con hold
void DraggableMeter::drawPeakMeter(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    auto meterArea = bounds.reduced(10, 15);
    float height = currentValue * meterArea.getHeight();
    float yPos = meterArea.getBottom() - height;
    
    g.setColour(juce::Colour(0xff2a2a2a));
    g.fillRect(meterArea);
    
    // Nivel RMS (mas oscuro)
    g.setColour(getColorForLevel(currentValue).darker(0.5f));
    g.fillRect(meterArea.getX(), yPos, meterArea.getWidth(), height);
    
    // Picos mas brillantes
    g.setColour(getColorForLevel(currentValue));
    float peakHeight = height * 0.3f;
    g.fillRect(meterArea.getX(), yPos, meterArea.getWidth(), peakHeight);
    
    // Peak hold line
    if (peakValue > 0.0f)
    {
        float peakY = meterArea.getBottom() - (peakValue * meterArea.getHeight());
        g.setColour(juce::Colours::white);
        g.fillRect(meterArea.getX(), peakY - 1.0f, meterArea.getWidth(), 2.0f);
    }
}

// Estilo 5: RMS con suavizado
void DraggableMeter::drawRMSMeter(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    auto meterArea = bounds.reduced(10, 15);
    float height = currentValue * meterArea.getHeight();
    float yPos = meterArea.getBottom() - height;
    
    g.setColour(juce::Colour(0xff2a2a2a));
    g.fillRoundedRectangle(meterArea, 3.0f);
    
    // Gradiente suave
    juce::ColourGradient gradient(
        meterColour.darker(0.3f), meterArea.getX(), meterArea.getBottom(),
        meterColour.brighter(0.3f), meterArea.getX(), yPos,
        false
    );
    
    g.setGradientFill(gradient);
    g.fillRoundedRectangle(meterArea.getX(), yPos, meterArea.getWidth(), height, 3.0f);
    
    // Borde brillante
    g.setColour(meterColour.brighter());
    g.drawRoundedRectangle(meterArea.getX(), yPos, meterArea.getWidth(), height, 3.0f, 1.0f);
}

// Estilo 6: Gradiente continuo
void DraggableMeter::drawGradientMeter(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    auto meterArea = bounds.reduced(10, 15);
    float height = currentValue * meterArea.getHeight();
    float yPos = meterArea.getBottom() - height;
    
    juce::ColourGradient rainbow(
        juce::Colours::green, meterArea.getX(), meterArea.getBottom(),
        juce::Colours::red, meterArea.getX(), meterArea.getY(),
        false
    );
    rainbow.addColour(0.3, juce::Colours::yellow);
    rainbow.addColour(0.6, juce::Colours::orange);
    
    g.setGradientFill(rainbow);
    g.fillRect(meterArea.getX(), yPos, meterArea.getWidth(), height);
}

// Estilo 7: Segmentado con gaps
void DraggableMeter::drawSegmentedMeter(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    auto meterArea = bounds.reduced(10, 15);
    int numSegments = 15;
    float segmentHeight = (meterArea.getHeight() - (numSegments - 1) * 3) / numSegments;
    int litSegments = static_cast<int>(currentValue * numSegments);
    
    for (int i = 0; i < numSegments; ++i)
    {
        float y = meterArea.getBottom() - ((i + 1) * (segmentHeight + 3));
        bool isLit = i < litSegments;
        
        float levelRatio = (float)(i + 1) / numSegments;
        juce::Colour segColor = getColorForLevel(levelRatio);
        
        if (isLit)
            g.setColour(segColor);
        else
            g.setColour(segColor.withAlpha(0.15f));
        
        g.fillRect(meterArea.getX(), y, meterArea.getWidth(), segmentHeight);
    }
}

// Estilo 8: Meter circular
void DraggableMeter::drawCircularMeter(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    auto center = bounds.getCentre();
    float radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) * 0.35f;
    
    // Circulo de fondo
    g.setColour(juce::Colour(0xff2a2a2a));
    g.fillEllipse(center.x - radius, center.y - radius, radius * 2, radius * 2);
    
    // Arco de nivel
    float startAngle = -juce::MathConstants<float>::pi * 0.75f;
    float endAngle = startAngle + (juce::MathConstants<float>::pi * 1.5f * currentValue);
    
    juce::Path arc;
    arc.addCentredArc(center.x, center.y, radius * 0.8f, radius * 0.8f, 0.0f, startAngle, endAngle, true);
    
    g.setColour(getColorForLevel(currentValue));
    g.strokePath(arc, juce::PathStrokeType(12.0f));
    
    // Valor en el centro
    g.setColour(juce::Colours::white);
    g.setFont(juce::FontOptions(14.0f, juce::Font::bold));
    g.drawText(juce::String(static_cast<int>(currentValue * 100)) + "%", 
               bounds.reduced(5), juce::Justification::centred);
}

// Estilo 9: Radial desde centro
void DraggableMeter::drawRadialMeter(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    auto center = bounds.getCentre();
    float maxRadius = juce::jmin(bounds.getWidth(), bounds.getHeight()) * 0.4f;
    float currentRadius = currentValue * maxRadius;
    
    // Circulos concentricos
    int numRings = 10;
    for (int i = 0; i < numRings; ++i)
    {
        float ringRadius = maxRadius * (i + 1) / numRings;
        bool isLit = ringRadius <= currentRadius;
        
        float levelRatio = (float)(i + 1) / numRings;
        juce::Colour ringColor = getColorForLevel(levelRatio);
        
        if (isLit)
            g.setColour(ringColor.withAlpha(0.7f));
        else
            g.setColour(ringColor.withAlpha(0.1f));
        
        float thickness = maxRadius / numRings * 0.8f;
        g.drawEllipse(center.x - ringRadius, center.y - ringRadius, 
                     ringRadius * 2, ringRadius * 2, thickness);
    }
}

juce::ValueTree DraggableMeter::toValueTree() const
{
    auto tree = DraggableWidget::toValueTree();
    tree.setProperty("meterStyle", meterStyle, nullptr);
    tree.setProperty("meterColor", meterColour.toString(), nullptr);
    tree.setProperty("currentValue", currentValue, nullptr);
    return tree;
}

// ============================================================================
// DraggableXYPad - Control XY bidimensional
// ============================================================================
// DraggableWaveform - Visualizador de forma de onda
// ============================================================================

DraggableWaveform::DraggableWaveform(const juce::String& name)
    : DraggableWidget(WidgetType::Waveform, name)
    , audioBuffer(2, bufferSize)
{
    setSize(250, 100);
    audioBuffer.clear();
}

void DraggableWaveform::resized()
{
    DraggableWidget::resized();
}

void DraggableWaveform::pushSample(float sample)
{
    if (writePosition < bufferSize)
    {
        audioBuffer.setSample(0, writePosition, sample);
        writePosition = (writePosition + 1) % bufferSize;
        
        // Repintar cada ciertos samples para rendimiento
        if (writePosition % 16 == 0)
            repaint();
    }
}

void DraggableWaveform::pushBuffer(const float* samples, int numSamples)
{
    for (int i = 0; i < numSamples; ++i)
    {
        audioBuffer.setSample(0, writePosition, samples[i]);
        writePosition = (writePosition + 1) % bufferSize;
    }
    repaint();
}

void DraggableWaveform::clear()
{
    audioBuffer.clear();
    writePosition = 0;
    repaint();
}

void DraggableWaveform::paintWidget(juce::Graphics& g)
{
    auto bounds = getLocalBounds().reduced(15).toFloat();
    
    // Fondo oscuro con borde
    g.setColour(juce::Colour(0xff1a1a1a));
    g.fillRoundedRectangle(bounds, 5.0f);
    
    g.setColour(juce::Colour(0xff444444));
    g.drawRoundedRectangle(bounds, 5.0f, 1.5f);
    
    // Línea central (0 dB)
    g.setColour(juce::Colour(0xff333333));
    float centerY = bounds.getCentreY();
    g.drawLine(bounds.getX(), centerY, bounds.getRight(), centerY, 1.0f);
    
    // Dibujar grid horizontal
    for (int i = 1; i <= 3; ++i)
    {
        float y1 = bounds.getY() + (bounds.getHeight() * i / 4.0f);
        g.setColour(juce::Colour(0xff222222));
        g.drawLine(bounds.getX(), y1, bounds.getRight(), y1, 0.5f);
    }
    
    // Dibujar forma de onda según estilo
    switch (waveformStyle)
    {
        case 0: drawLineWaveform(g, bounds); break;
        case 1: drawLineGlowWaveform(g, bounds); break;
        case 2: drawFilledWaveform(g, bounds); break;
        case 3: drawMirrorWaveform(g, bounds); break;
        case 4: drawBarsWaveform(g, bounds); break;
        case 5: drawDotsWaveform(g, bounds); break;
        case 6: drawGradientWaveform(g, bounds); break;
        case 7: drawSteppedWaveform(g, bounds); break;
        case 8: drawFrequencyWaveform(g, bounds); break;
        case 9: drawVectorscopeWaveform(g, bounds); break;
        default: drawLineWaveform(g, bounds); break;
    }
    
    // Etiqueta
    g.setColour(juce::Colours::white.withAlpha(0.7f));
    g.setFont(juce::FontOptions(12.0f));
    g.drawText(getWidgetName(), bounds.reduced(5), juce::Justification::topLeft);
    
    // Indicador de estilo y zoom
    juce::String info = "Style: " + juce::String(waveformStyle);
    if (zoomLevel != 1.0f)
        info += " | Zoom: " + juce::String(zoomLevel, 1) + "x";
    
    g.setFont(juce::FontOptions(10.0f));
    g.drawText(info, bounds.reduced(5), juce::Justification::bottomRight);
}

// Estilo 0: Línea simple
void DraggableWaveform::drawLineWaveform(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    juce::Path waveformPath;
    int samplesToDisplay = static_cast<int>(bufferSize / zoomLevel);
    float xStep = bounds.getWidth() / static_cast<float>(samplesToDisplay);
    
    for (int i = 0; i < samplesToDisplay; ++i)
    {
        int sampleIndex = (writePosition + i) % bufferSize;
        float sample = audioBuffer.getSample(0, sampleIndex);
        float y = bounds.getCentreY() - (sample * bounds.getHeight() * 0.4f);
        float x = bounds.getX() + (i * xStep);
        
        if (i == 0) waveformPath.startNewSubPath(x, y);
        else waveformPath.lineTo(x, y);
    }
    
    g.setColour(waveformColour);
    g.strokePath(waveformPath, juce::PathStrokeType(1.5f));
}

// Estilo 1: Línea con glow
void DraggableWaveform::drawLineGlowWaveform(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    juce::Path waveformPath;
    int samplesToDisplay = static_cast<int>(bufferSize / zoomLevel);
    float xStep = bounds.getWidth() / static_cast<float>(samplesToDisplay);
    
    for (int i = 0; i < samplesToDisplay; ++i)
    {
        int sampleIndex = (writePosition + i) % bufferSize;
        float sample = audioBuffer.getSample(0, sampleIndex);
        float y = bounds.getCentreY() - (sample * bounds.getHeight() * 0.4f);
        float x = bounds.getX() + (i * xStep);
        
        if (i == 0) waveformPath.startNewSubPath(x, y);
        else waveformPath.lineTo(x, y);
    }
    
    // Glow effect con múltiples capas
    g.setColour(waveformColour.withAlpha(0.1f));
    g.strokePath(waveformPath, juce::PathStrokeType(8.0f));
    g.setColour(waveformColour.withAlpha(0.3f));
    g.strokePath(waveformPath, juce::PathStrokeType(4.0f));
    g.setColour(waveformColour);
    g.strokePath(waveformPath, juce::PathStrokeType(1.5f));
}

// Estilo 2: Área rellena
void DraggableWaveform::drawFilledWaveform(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    juce::Path waveformPath;
    int samplesToDisplay = static_cast<int>(bufferSize / zoomLevel);
    float xStep = bounds.getWidth() / static_cast<float>(samplesToDisplay);
    
    waveformPath.startNewSubPath(bounds.getX(), bounds.getCentreY());
    
    for (int i = 0; i < samplesToDisplay; ++i)
    {
        int sampleIndex = (writePosition + i) % bufferSize;
        float sample = audioBuffer.getSample(0, sampleIndex);
        float y = bounds.getCentreY() - (sample * bounds.getHeight() * 0.4f);
        float x = bounds.getX() + (i * xStep);
        waveformPath.lineTo(x, y);
    }
    
    waveformPath.lineTo(bounds.getRight(), bounds.getCentreY());
    waveformPath.closeSubPath();
    
    g.setColour(waveformColour.withAlpha(0.3f));
    g.fillPath(waveformPath);
    g.setColour(waveformColour);
    g.strokePath(waveformPath, juce::PathStrokeType(1.0f));
}

// Estilo 3: Espejo vertical
void DraggableWaveform::drawMirrorWaveform(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    int samplesToDisplay = static_cast<int>(bufferSize / zoomLevel);
    float xStep = bounds.getWidth() / static_cast<float>(samplesToDisplay);
    float centerY = bounds.getCentreY();
    
    for (int i = 0; i < samplesToDisplay; ++i)
    {
        int sampleIndex = (writePosition + i) % bufferSize;
        float sample = audioBuffer.getSample(0, sampleIndex);
        float amplitude = sample * bounds.getHeight() * 0.4f;
        float x = bounds.getX() + (i * xStep);
        
        g.setColour(waveformColour.withAlpha(0.8f));
        g.drawLine(x, centerY - amplitude, x, centerY + amplitude, 1.0f);
    }
}

// Estilo 4: Barras verticales
void DraggableWaveform::drawBarsWaveform(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    int samplesToDisplay = static_cast<int>(bufferSize / zoomLevel);
    float xStep = bounds.getWidth() / static_cast<float>(samplesToDisplay);
    float centerY = bounds.getCentreY();
    float barWidth = juce::jmax(1.0f, xStep * 0.8f);
    
    for (int i = 0; i < samplesToDisplay; ++i)
    {
        int sampleIndex = (writePosition + i) % bufferSize;
        float sample = audioBuffer.getSample(0, sampleIndex);
        float height = std::abs(sample) * bounds.getHeight() * 0.4f;
        float x = bounds.getX() + (i * xStep);
        
        juce::Colour barColour = sample > 0.0f ? waveformColour : waveformColour.darker(0.3f);
        g.setColour(barColour.withAlpha(0.7f));
        
        if (sample > 0.0f)
            g.fillRect(x, centerY - height, barWidth, height);
        else
            g.fillRect(x, centerY, barWidth, height);
    }
}

// Estilo 5: Puntos
void DraggableWaveform::drawDotsWaveform(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    int samplesToDisplay = static_cast<int>(bufferSize / zoomLevel);
    float xStep = bounds.getWidth() / static_cast<float>(samplesToDisplay);
    
    for (int i = 0; i < samplesToDisplay; ++i)
    {
        int sampleIndex = (writePosition + i) % bufferSize;
        float sample = audioBuffer.getSample(0, sampleIndex);
        float y = bounds.getCentreY() - (sample * bounds.getHeight() * 0.4f);
        float x = bounds.getX() + (i * xStep);
        
        float dotSize = 2.0f + std::abs(sample) * 3.0f;
        g.setColour(waveformColour.withAlpha(0.6f + std::abs(sample) * 0.4f));
        g.fillEllipse(x - dotSize/2, y - dotSize/2, dotSize, dotSize);
    }
}

// Estilo 6: Gradiente de color
void DraggableWaveform::drawGradientWaveform(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    int samplesToDisplay = static_cast<int>(bufferSize / zoomLevel);
    float xStep = bounds.getWidth() / static_cast<float>(samplesToDisplay);
    
    for (int i = 0; i < samplesToDisplay - 1; ++i)
    {
        int sampleIndex = (writePosition + i) % bufferSize;
        float sample1 = audioBuffer.getSample(0, sampleIndex);
        float sample2 = audioBuffer.getSample(0, (sampleIndex + 1) % bufferSize);
        
        float y1 = bounds.getCentreY() - (sample1 * bounds.getHeight() * 0.4f);
        float y2 = bounds.getCentreY() - (sample2 * bounds.getHeight() * 0.4f);
        float x1 = bounds.getX() + (i * xStep);
        float x2 = bounds.getX() + ((i + 1) * xStep);
        
        // Color basado en amplitud
        float intensity = (std::abs(sample1) + std::abs(sample2)) * 0.5f;
        juce::Colour lineColour = waveformColour.interpolatedWith(
            juce::Colours::red, intensity * 0.5f);
        
        g.setColour(lineColour);
        g.drawLine(x1, y1, x2, y2, 2.0f);
    }
}

// Estilo 7: Escalonado (stepped)
void DraggableWaveform::drawSteppedWaveform(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    juce::Path waveformPath;
    int samplesToDisplay = static_cast<int>(bufferSize / zoomLevel);
    float xStep = bounds.getWidth() / static_cast<float>(samplesToDisplay);
    
    for (int i = 0; i < samplesToDisplay; ++i)
    {
        int sampleIndex = (writePosition + i) % bufferSize;
        float sample = audioBuffer.getSample(0, sampleIndex);
        float y = bounds.getCentreY() - (sample * bounds.getHeight() * 0.4f);
        float x = bounds.getX() + (i * xStep);
        
        if (i == 0)
            waveformPath.startNewSubPath(x, y);
        else
        {
            // Línea horizontal primero, luego vertical
            waveformPath.lineTo(x, waveformPath.getCurrentPosition().y);
            waveformPath.lineTo(x, y);
        }
    }
    
    g.setColour(waveformColour);
    g.strokePath(waveformPath, juce::PathStrokeType(1.5f));
}

// Estilo 8: Espectro de frecuencias simulado
void DraggableWaveform::drawFrequencyWaveform(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    int numBands = 32;
    float bandWidth = bounds.getWidth() / numBands;
    
    // Simular bandas de frecuencia agrupando samples
    for (int band = 0; band < numBands; ++band)
    {
        float magnitude = 0.0f;
        int samplesPerBand = bufferSize / numBands;
        
        for (int i = 0; i < samplesPerBand; ++i)
        {
            int sampleIndex = (band * samplesPerBand + i) % bufferSize;
            magnitude += std::abs(audioBuffer.getSample(0, sampleIndex));
        }
        magnitude /= samplesPerBand;
        
        float height = magnitude * bounds.getHeight() * 0.8f;
        float x = bounds.getX() + (band * bandWidth);
        
        // Color basado en frecuencia (graves=azul, agudos=rojo)
        float hue = (float)band / numBands;
        juce::Colour bandColour = juce::Colour::fromHSV(hue * 0.7f, 0.8f, 0.9f, 1.0f);
        
        g.setColour(bandColour.withAlpha(0.7f));
        g.fillRect(x + 1, bounds.getBottom() - height, bandWidth - 2, height);
    }
}

// Estilo 9: Vectorscopio (Lissajous)
void DraggableWaveform::drawVectorscopeWaveform(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    if (numChannels < 2)
    {
        // Si solo hay un canal, mostrar círculo
        float radius = bounds.getHeight() * 0.3f;
        g.setColour(waveformColour.withAlpha(0.3f));
        g.drawEllipse(bounds.getCentreX() - radius, bounds.getCentreY() - radius,
                     radius * 2, radius * 2, 1.0f);
        
        int samplesToDisplay = juce::jmin(512, bufferSize);
        for (int i = 0; i < samplesToDisplay; ++i)
        {
            int sampleIndex = (writePosition + i) % bufferSize;
            float sample = audioBuffer.getSample(0, sampleIndex);
            float angle = (float)i / samplesToDisplay * juce::MathConstants<float>::twoPi;
            
            float x = bounds.getCentreX() + std::cos(angle) * sample * bounds.getWidth() * 0.3f;
            float y = bounds.getCentreY() + std::sin(angle) * sample * bounds.getHeight() * 0.3f;
            
            float alpha = (float)i / samplesToDisplay;
            g.setColour(waveformColour.withAlpha(alpha * 0.5f));
            g.fillEllipse(x - 1, y - 1, 2, 2);
        }
    }
    else
    {
        // Lissajous con dos canales
        juce::Path lissajousPath;
        int samplesToDisplay = juce::jmin(512, bufferSize);
        
        for (int i = 0; i < samplesToDisplay; ++i)
        {
            int sampleIndex = (writePosition + i) % bufferSize;
            float left = audioBuffer.getSample(0, sampleIndex);
            float right = audioBuffer.getNumChannels() > 1 ? 
                         audioBuffer.getSample(1, sampleIndex) : left;
            
            float x = bounds.getCentreX() + left * bounds.getWidth() * 0.4f;
            float y = bounds.getCentreY() - right * bounds.getHeight() * 0.4f;
            
            if (i == 0) lissajousPath.startNewSubPath(x, y);
            else lissajousPath.lineTo(x, y);
        }
        
        g.setColour(waveformColour.withAlpha(0.6f));
        g.strokePath(lissajousPath, juce::PathStrokeType(1.5f));
    }
}

juce::ValueTree DraggableWaveform::toValueTree() const
{
    auto tree = DraggableWidget::toValueTree();
    tree.setProperty("zoom", zoomLevel, nullptr);
    tree.setProperty("channels", numChannels, nullptr);
    tree.setProperty("style", waveformStyle, nullptr);
    tree.setProperty("waveformColor", waveformColour.toString(), nullptr);
    return tree;
}

// ============================================================================
// DraggableXYPad - Pad XY 2D
// ============================================================================

DraggableXYPad::DraggableXYPad(const juce::String& name)
    : DraggableWidget(WidgetType::XYPad, name)
{
    setSize(150, 150);
    indicatorPos = juce::Point<float>(0.5f, 0.5f);
}

void DraggableXYPad::resized()
{
    DraggableWidget::resized();
}

void DraggableXYPad::mouseDown(const juce::MouseEvent& e)
{
    // Si el clic esta en el area del widget (no en bordes de resize)
    if (!isResizing)
    {
        updateIndicatorFromMouse(e.getPosition());
    }
    DraggableWidget::mouseDown(e);
}

void DraggableXYPad::mouseDrag(const juce::MouseEvent& e)
{
    if (!isResizing && !isDragging)
    {
        updateIndicatorFromMouse(e.getPosition());
    }
    else
    {
        DraggableWidget::mouseDrag(e);
    }
}

void DraggableXYPad::updateIndicatorFromMouse(const juce::Point<int>& mousePos)
{
    auto bounds = getLocalBounds().reduced(15).toFloat();
    
    // Limitar a los bordes del pad
    float x = juce::jlimit(bounds.getX() + indicatorSize/2, 
                          bounds.getRight() - indicatorSize/2, 
                          (float)mousePos.x);
    float y = juce::jlimit(bounds.getY() + indicatorSize/2, 
                          bounds.getBottom() - indicatorSize/2, 
                          (float)mousePos.y);
    
    // Convertir a valores normalizados 0-1
    xValue = (x - bounds.getX() - indicatorSize/2) / (bounds.getWidth() - indicatorSize);
    yValue = 1.0f - (y - bounds.getY() - indicatorSize/2) / (bounds.getHeight() - indicatorSize);
    
    indicatorPos = juce::Point<float>(x, y);
    repaint();
}

void DraggableXYPad::paintWidget(juce::Graphics& g)
{
    auto bounds = getLocalBounds().reduced(15).toFloat();
    
    // Actualizar posicion del indicador basado en xValue/yValue
    float x = bounds.getX() + indicatorSize/2 + xValue * (bounds.getWidth() - indicatorSize);
    float y = bounds.getY() + indicatorSize/2 + (1.0f - yValue) * (bounds.getHeight() - indicatorSize);
    indicatorPos = juce::Point<float>(x, y);
    
    // Dibujar según estilo
    switch (xyPadStyle)
    {
        case 0: drawCircleStyle(g, bounds); break;
        case 1: drawSquareStyle(g, bounds); break;
        case 2: drawOrbitalStyle(g, bounds); break;
        case 3: drawCrosshairStyle(g, bounds); break;
        case 4: drawGridStyle(g, bounds); break;
        case 5: drawGlowStyle(g, bounds); break;
        case 6: drawVectorStyle(g, bounds); break;
        case 7: drawRadarStyle(g, bounds); break;
        case 8: drawDiamondStyle(g, bounds); break;
        case 9: drawNebulaStyle(g, bounds); break;
        default: drawCircleStyle(g, bounds); break;
    }
    
    // Valores de texto
    g.setColour(style.textColor);
    g.setFont(juce::FontOptions(10.0f));
    juce::String xText = "X: " + juce::String(xValue, 2);
    juce::String yText = "Y: " + juce::String(yValue, 2);
    g.drawText(xText, bounds.getX(), bounds.getBottom() + 5, 60, 15, juce::Justification::left);
    g.drawText(yText, bounds.getRight() - 60, bounds.getBottom() + 5, 60, 15, juce::Justification::right);
}

// Estilo 0: Circle - Círculo con crosshair
void DraggableXYPad::drawCircleStyle(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    // Fondo con gradiente
    juce::ColourGradient gradient(
        style.bgColor.withAlpha(0.3f), bounds.getCentreX(), bounds.getY(),
        style.bgColor.withAlpha(0.6f), bounds.getCentreX(), bounds.getBottom(),
        false
    );
    g.setGradientFill(gradient);
    g.fillRoundedRectangle(bounds, style.cornerRadius);
    
    g.setColour(style.borderColor);
    g.drawRoundedRectangle(bounds, style.cornerRadius, style.borderWidth);
    
    // Crosshair en el centro
    g.setColour(style.borderColor.withAlpha(0.3f));
    g.drawLine(bounds.getCentreX(), bounds.getY(), bounds.getCentreX(), bounds.getBottom(), 1.0f);
    g.drawLine(bounds.getX(), bounds.getCentreY(), bounds.getRight(), bounds.getCentreY(), 1.0f);
    
    // Indicador circular
    g.setColour(juce::Colours::black.withAlpha(0.4f));
    g.fillEllipse(indicatorPos.x - indicatorSize/2 + 2, indicatorPos.y - indicatorSize/2 + 2, indicatorSize, indicatorSize);
    
    g.setColour(xyColour);
    g.fillEllipse(indicatorPos.x - indicatorSize/2, indicatorPos.y - indicatorSize/2, indicatorSize, indicatorSize);
    
    g.setColour(xyColour.brighter(0.5f));
    g.drawEllipse(indicatorPos.x - indicatorSize/2, indicatorPos.y - indicatorSize/2, indicatorSize, indicatorSize, 2.0f);
}

// Estilo 1: Square - Cuadrado con grid
void DraggableXYPad::drawSquareStyle(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    g.setColour(style.bgColor.withAlpha(0.5f));
    g.fillRect(bounds);
    
    g.setColour(style.borderColor);
    g.drawRect(bounds, style.borderWidth);
    
    // Grid 5x5
    g.setColour(style.borderColor.withAlpha(0.2f));
    for (int i = 1; i < 5; ++i)
    {
        float x = bounds.getX() + (bounds.getWidth() * i / 5.0f);
        float y = bounds.getY() + (bounds.getHeight() * i / 5.0f);
        g.drawLine(x, bounds.getY(), x, bounds.getBottom(), 1.0f);
        g.drawLine(bounds.getX(), y, bounds.getRight(), y, 1.0f);
    }
    
    // Indicador cuadrado
    float squareSize = indicatorSize * 0.8f;
    g.setColour(xyColour.withAlpha(0.8f));
    g.fillRect(indicatorPos.x - squareSize/2, indicatorPos.y - squareSize/2, squareSize, squareSize);
    
    g.setColour(xyColour);
    g.drawRect(indicatorPos.x - squareSize/2, indicatorPos.y - squareSize/2, squareSize, squareSize, 2.0f);
}

// Estilo 2: Orbital - Con trail orbital
void DraggableXYPad::drawOrbitalStyle(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    g.setColour(juce::Colour(0xff0a0a0a));
    g.fillRoundedRectangle(bounds, 8.0f);
    
    g.setColour(style.borderColor.withAlpha(0.5f));
    g.drawRoundedRectangle(bounds, 8.0f, 2.0f);
    
    auto center = bounds.getCentre();
    
    // Órbitas circulares
    for (int i = 1; i <= 3; ++i)
    {
        float radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) * i / 6.0f;
        g.setColour(xyColour.withAlpha(0.2f));
        g.drawEllipse(center.x - radius, center.y - radius, radius * 2, radius * 2, 1.0f);
    }
    
    // Trail effect
    juce::Path trail;
    trail.startNewSubPath(center);
    trail.lineTo(indicatorPos);
    
    g.setColour(xyColour.withAlpha(0.4f));
    g.strokePath(trail, juce::PathStrokeType(2.0f));
    
    // Indicador con glow
    for (int i = 3; i > 0; --i)
    {
        g.setColour(xyColour.withAlpha(0.2f * i / 3.0f));
        g.fillEllipse(indicatorPos.x - indicatorSize/2 * i/2, indicatorPos.y - indicatorSize/2 * i/2, 
                     indicatorSize * i/2, indicatorSize * i/2);
    }
    
    g.setColour(xyColour);
    g.fillEllipse(indicatorPos.x - indicatorSize/2, indicatorPos.y - indicatorSize/2, indicatorSize, indicatorSize);
}

// Estilo 3: Crosshair - Crosshair grande y prominente
void DraggableXYPad::drawCrosshairStyle(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    g.setColour(style.bgColor.withAlpha(0.4f));
    g.fillRect(bounds);
    
    g.setColour(style.borderColor);
    g.drawRect(bounds, 1.5f);
    
    // Crosshair que sigue al indicador
    g.setColour(xyColour.withAlpha(0.6f));
    g.drawLine(indicatorPos.x, bounds.getY(), indicatorPos.x, bounds.getBottom(), 2.0f);
    g.drawLine(bounds.getX(), indicatorPos.y, bounds.getRight(), indicatorPos.y, 2.0f);
    
    // Punto central
    g.setColour(xyColour);
    g.fillEllipse(indicatorPos.x - 8, indicatorPos.y - 8, 16, 16);
    
    g.setColour(juce::Colours::black);
    g.fillEllipse(indicatorPos.x - 4, indicatorPos.y - 4, 8, 8);
}

// Estilo 4: Grid - Grid detallado con punto
void DraggableXYPad::drawGridStyle(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    g.setColour(juce::Colour(0xff1a1a1a));
    g.fillRect(bounds);
    
    // Grid denso 10x10
    g.setColour(xyColour.withAlpha(0.15f));
    for (int i = 0; i <= 10; ++i)
    {
        float x = bounds.getX() + (bounds.getWidth() * i / 10.0f);
        float y = bounds.getY() + (bounds.getHeight() * i / 10.0f);
        
        float thickness = (i % 5 == 0) ? 1.5f : 0.5f;
        g.drawLine(x, bounds.getY(), x, bounds.getBottom(), thickness);
        g.drawLine(bounds.getX(), y, bounds.getRight(), y, thickness);
    }
    
    g.setColour(style.borderColor);
    g.drawRect(bounds, 2.0f);
    
    // Punto con círculos concéntricos
    for (int i = 2; i >= 0; --i)
    {
        float size = indicatorSize * (1 + i * 0.3f);
        g.setColour(xyColour.withAlpha(0.3f / (i + 1)));
        g.drawEllipse(indicatorPos.x - size/2, indicatorPos.y - size/2, size, size, 2.0f);
    }
    
    g.setColour(xyColour);
    g.fillEllipse(indicatorPos.x - indicatorSize/2, indicatorPos.y - indicatorSize/2, indicatorSize, indicatorSize);
}

// Estilo 5: Glow - Punto con efecto glow intenso
void DraggableXYPad::drawGlowStyle(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    g.setColour(juce::Colours::black);
    g.fillRoundedRectangle(bounds, 5.0f);
    
    g.setColour(style.borderColor.withAlpha(0.3f));
    g.drawRoundedRectangle(bounds, 5.0f, 1.0f);
    
    // Múltiples capas de glow
    for (int i = 5; i > 0; --i)
    {
        float alpha = 0.15f * (6 - i) / 5.0f;
        float size = indicatorSize * (1 + i * 0.5f);
        
        g.setColour(xyColour.withAlpha(alpha));
        g.fillEllipse(indicatorPos.x - size/2, indicatorPos.y - size/2, size, size);
    }
    
    // Núcleo brillante
    g.setColour(xyColour);
    g.fillEllipse(indicatorPos.x - indicatorSize/2, indicatorPos.y - indicatorSize/2, indicatorSize, indicatorSize);
    
    g.setColour(juce::Colours::white);
    g.fillEllipse(indicatorPos.x - indicatorSize/4, indicatorPos.y - indicatorSize/4, indicatorSize/2, indicatorSize/2);
}

// Estilo 6: Vector - Flecha desde el centro
void DraggableXYPad::drawVectorStyle(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    g.setColour(style.bgColor.withAlpha(0.5f));
    g.fillRect(bounds);
    
    g.setColour(style.borderColor);
    g.drawRect(bounds, 2.0f);
    
    auto center = bounds.getCentre();
    
    // Ejes X/Y
    g.setColour(style.borderColor.withAlpha(0.2f));
    g.drawLine(bounds.getCentreX(), bounds.getY(), bounds.getCentreX(), bounds.getBottom(), 1.0f);
    g.drawLine(bounds.getX(), bounds.getCentreY(), bounds.getRight(), bounds.getCentreY(), 1.0f);
    
    // Vector desde centro a indicador
    juce::Path arrow;
    arrow.startNewSubPath(center);
    arrow.lineTo(indicatorPos);
    
    g.setColour(xyColour.withAlpha(0.7f));
    g.strokePath(arrow, juce::PathStrokeType(3.0f));
    
    // Punta de flecha
    float angle = std::atan2(indicatorPos.y - center.y, indicatorPos.x - center.x);
    float arrowSize = 10.0f;
    
    juce::Path arrowHead;
    arrowHead.startNewSubPath(indicatorPos.x, indicatorPos.y);
    arrowHead.lineTo(indicatorPos.x - arrowSize * std::cos(angle - 0.5f), 
                    indicatorPos.y - arrowSize * std::sin(angle - 0.5f));
    arrowHead.lineTo(indicatorPos.x - arrowSize * std::cos(angle + 0.5f), 
                    indicatorPos.y - arrowSize * std::sin(angle + 0.5f));
    arrowHead.closeSubPath();
    
    g.setColour(xyColour);
    g.fillPath(arrowHead);
    
    // Punto final
    g.fillEllipse(indicatorPos.x - 6, indicatorPos.y - 6, 12, 12);
}

// Estilo 7: Radar - Estilo radar circular
void DraggableXYPad::drawRadarStyle(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    auto center = bounds.getCentre();
    float maxRadius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
    
    g.setColour(juce::Colour(0xff0a0a0a));
    g.fillEllipse(bounds);
    
    // Círculos concéntricos tipo radar
    for (int i = 1; i <= 5; ++i)
    {
        float radius = maxRadius * i / 5.0f;
        g.setColour(xyColour.withAlpha(0.15f));
        g.drawEllipse(center.x - radius, center.y - radius, radius * 2, radius * 2, 1.0f);
    }
    
    // Líneas radiales
    for (int i = 0; i < 8; ++i)
    {
        float angle = i * juce::MathConstants<float>::pi / 4.0f;
        float x2 = center.x + maxRadius * std::cos(angle);
        float y2 = center.y + maxRadius * std::sin(angle);
        
        g.setColour(xyColour.withAlpha(0.1f));
        g.drawLine(center.x, center.y, x2, y2, 1.0f);
    }
    
    g.setColour(style.borderColor);
    g.drawEllipse(bounds, 2.0f);
    
    // Punto con efecto radar sweep
    g.setColour(xyColour.withAlpha(0.3f));
    g.fillEllipse(indicatorPos.x - indicatorSize, indicatorPos.y - indicatorSize, 
                 indicatorSize * 2, indicatorSize * 2);
    
    g.setColour(xyColour);
    g.fillEllipse(indicatorPos.x - indicatorSize/2, indicatorPos.y - indicatorSize/2, 
                 indicatorSize, indicatorSize);
}

// Estilo 8: Diamond - Forma de diamante
void DraggableXYPad::drawDiamondStyle(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    auto center = bounds.getCentre();
    
    g.setColour(style.bgColor.withAlpha(0.4f));
    g.fillRect(bounds);
    
    // Diamante rotado
    juce::Path diamond;
    diamond.startNewSubPath(center.x, bounds.getY());
    diamond.lineTo(bounds.getRight(), center.y);
    diamond.lineTo(center.x, bounds.getBottom());
    diamond.lineTo(bounds.getX(), center.y);
    diamond.closeSubPath();
    
    g.setColour(xyColour.withAlpha(0.1f));
    g.fillPath(diamond);
    
    g.setColour(style.borderColor);
    g.strokePath(diamond, juce::PathStrokeType(2.0f));
    
    // Diamantes concéntricos
    for (int i = 1; i <= 2; ++i)
    {
        juce::Path innerDiamond;
        float scale = 1.0f - (i * 0.3f);
        float w = bounds.getWidth() * scale / 2;
        float h = bounds.getHeight() * scale / 2;
        
        innerDiamond.startNewSubPath(center.x, center.y - h);
        innerDiamond.lineTo(center.x + w, center.y);
        innerDiamond.lineTo(center.x, center.y + h);
        innerDiamond.lineTo(center.x - w, center.y);
        innerDiamond.closeSubPath();
        
        g.setColour(xyColour.withAlpha(0.2f));
        g.strokePath(innerDiamond, juce::PathStrokeType(1.0f));
    }
    
    // Indicador diamante
    juce::Path indicatorDiamond;
    float size = indicatorSize / 1.5f;
    indicatorDiamond.startNewSubPath(indicatorPos.x, indicatorPos.y - size);
    indicatorDiamond.lineTo(indicatorPos.x + size, indicatorPos.y);
    indicatorDiamond.lineTo(indicatorPos.x, indicatorPos.y + size);
    indicatorDiamond.lineTo(indicatorPos.x - size, indicatorPos.y);
    indicatorDiamond.closeSubPath();
    
    g.setColour(xyColour);
    g.fillPath(indicatorDiamond);
}

// Estilo 9: Nebula - Efecto nebulosa con partículas
void DraggableXYPad::drawNebulaStyle(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    g.setColour(juce::Colours::black);
    g.fillRoundedRectangle(bounds, 8.0f);
    
    auto center = bounds.getCentre();
    
    // Partículas de fondo basadas en posición
    juce::Random random(static_cast<int>(xValue * 1000 + yValue * 1000));
    for (int i = 0; i < 50; ++i)
    {
        float x = bounds.getX() + random.nextFloat() * bounds.getWidth();
        float y = bounds.getY() + random.nextFloat() * bounds.getHeight();
        float size = random.nextFloat() * 2.0f + 0.5f;
        float brightness = random.nextFloat();
        
        g.setColour(xyColour.withAlpha(brightness * 0.3f));
        g.fillEllipse(x, y, size, size);
    }
    
    // Nebulosa central
    for (int i = 4; i > 0; --i)
    {
        float size = indicatorSize * i * 1.5f;
        float alpha = 0.1f * (5 - i) / 4.0f;
        
        g.setColour(xyColour.withAlpha(alpha));
        g.fillEllipse(indicatorPos.x - size/2, indicatorPos.y - size/2, size, size);
    }
    
    // Núcleo brillante
    g.setColour(xyColour.brighter(0.5f));
    g.fillEllipse(indicatorPos.x - indicatorSize/2, indicatorPos.y - indicatorSize/2, 
                 indicatorSize, indicatorSize);
    
    g.setColour(juce::Colours::white);
    g.fillEllipse(indicatorPos.x - indicatorSize/3, indicatorPos.y - indicatorSize/3, 
                 indicatorSize * 2/3, indicatorSize * 2/3);
    
    g.setColour(style.borderColor.withAlpha(0.4f));
    g.drawRoundedRectangle(bounds, 8.0f, 1.5f);
}

void DraggableXYPad::updateFromMidiValue(int value)
{
    // Para XY Pad, usar el valor MIDI para controlar el eje X
    // (Se podría asignar otro CC para el eje Y)
    xValue = value / 127.0f;
    repaint();
}

juce::ValueTree DraggableXYPad::toValueTree() const
{
    auto tree = DraggableWidget::toValueTree();
    tree.setProperty("xValue", xValue, nullptr);
    tree.setProperty("yValue", yValue, nullptr);
    tree.setProperty("xyPadStyle", xyPadStyle, nullptr);
    tree.setProperty("xyColor", xyColour.toString(), nullptr);
    return tree;
}

// ============================================================================
// DraggableJoystick - Control polar con retorno al centro
// ============================================================================

DraggableJoystick::DraggableJoystick(const juce::String& name)
    : DraggableWidget(WidgetType::Joystick, name)
{
    setSize(150, 150);
    stickPos = juce::Point<float>(0, 0);
}

void DraggableJoystick::resized()
{
    DraggableWidget::resized();
}

void DraggableJoystick::mouseDown(const juce::MouseEvent& e)
{
    if (!isResizing)
    {
        updateFromMouse(e.getPosition());
    }
    DraggableWidget::mouseDown(e);
}

void DraggableJoystick::mouseDrag(const juce::MouseEvent& e)
{
    if (!isResizing && !isDragging)
    {
        updateFromMouse(e.getPosition());
    }
    else
    {
        DraggableWidget::mouseDrag(e);
    }
}

void DraggableJoystick::mouseUp(const juce::MouseEvent& e)
{
    if (!isResizing && !isDragging)
    {
        returnToCenter();
    }
    DraggableWidget::mouseUp(e);
}

void DraggableJoystick::updateFromMouse(const juce::Point<int>& mousePos)
{
    auto bounds = getLocalBounds().reduced(20).toFloat();
    auto center = bounds.getCentre();
    auto maxRadius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
    
    // Calcular distancia y angulo desde el centro
    float deltaX = mousePos.x - center.x;
    float deltaY = mousePos.y - center.y;
    float distance = std::sqrt(deltaX * deltaX + deltaY * deltaY);
    
    // Limitar al radio maximo
    radius = juce::jlimit(0.0f, 1.0f, distance / maxRadius);
    
    // Calcular angulo (0 = arriba, 0.25 = derecha, 0.5 = abajo, 0.75 = izquierda)
    float angleRad = std::atan2(deltaX, -deltaY);
    angle = (angleRad / juce::MathConstants<float>::twoPi) + 0.5f;
    if (angle < 0.0f) angle += 1.0f;
    if (angle >= 1.0f) angle -= 1.0f;
    
    // Actualizar posicion del stick
    float actualRadius = radius * maxRadius;
    stickPos.x = center.x + actualRadius * std::sin(angleRad);
    stickPos.y = center.y - actualRadius * std::cos(angleRad);
    
    repaint();
}

void DraggableJoystick::returnToCenter()
{
    radius = 0.0f;
    angle = 0.0f;
    
    auto bounds = getLocalBounds().reduced(20).toFloat();
    stickPos = bounds.getCentre();
    
    repaint();
}

void DraggableJoystick::paintWidget(juce::Graphics& g)
{
    auto bounds = getLocalBounds().reduced(20).toFloat();
    auto center = bounds.getCentre();
    auto maxRadius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
    
    // Si radius > 0, actualizar posicion del stick
    if (radius > 0.0f)
    {
        float angleRad = (angle - 0.5f) * juce::MathConstants<float>::twoPi;
        float actualRadius = radius * maxRadius;
        stickPos.x = center.x + actualRadius * std::sin(angleRad);
        stickPos.y = center.y - actualRadius * std::cos(angleRad);
    }
    else
    {
        stickPos = center;
    }
    
    // Dibujar según estilo
    switch (joystickStyle)
    {
        case 0: drawClassicStyle(g, bounds); break;
        case 1: drawArcadeStyle(g, bounds); break;
        case 2: drawFlightStyle(g, bounds); break;
        case 3: drawMinimalStyle(g, bounds); break;
        case 4: drawNeonStyle(g, bounds); break;
        case 5: drawMechanicalStyle(g, bounds); break;
        case 6: drawOrbStyle(g, bounds); break;
        case 7: drawTargetStyle(g, bounds); break;
        case 8: drawCompassStyle(g, bounds); break;
        case 9: drawGalaxyStyle(g, bounds); break;
        default: drawClassicStyle(g, bounds); break;
    }
    
    // Valores de texto
    g.setColour(style.textColor);
    g.setFont(juce::FontOptions(10.0f));
    juce::String radiusText = "R: " + juce::String(radius, 2);
    juce::String angleText = "A: " + juce::String(angle * 360.0f, 0) + juce::String::charToString(L'\u00B0');
    g.drawText(radiusText, bounds.getX(), bounds.getBottom() + 5, 60, 15, juce::Justification::left);
    g.drawText(angleText, bounds.getRight() - 60, bounds.getBottom() + 5, 60, 15, juce::Justification::right);
}

// Estilo 0: Classic - Joystick clásico con círculo
void DraggableJoystick::drawClassicStyle(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    auto center = bounds.getCentre();
    auto maxRadius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
    
    // Fondo circular con gradiente
    juce::ColourGradient gradient(
        style.bgColor.withAlpha(0.4f), center.x, center.y - maxRadius,
        style.bgColor.withAlpha(0.7f), center.x, center.y + maxRadius,
        false
    );
    g.setGradientFill(gradient);
    g.fillEllipse(bounds);
    
    g.setColour(style.borderColor);
    g.drawEllipse(bounds, style.borderWidth);
    
    // Círculos concéntricos
    g.setColour(style.borderColor.withAlpha(0.2f));
    g.drawEllipse(bounds.reduced(maxRadius * 0.33f), 1.0f);
    g.drawEllipse(bounds.reduced(maxRadius * 0.66f), 1.0f);
    
    // Cruz central
    g.setColour(style.borderColor.withAlpha(0.3f));
    g.drawLine(center.x, bounds.getY(), center.x, bounds.getBottom(), 1.0f);
    g.drawLine(bounds.getX(), center.y, bounds.getRight(), center.y, 1.0f);
    
    // Stick con sombra
    float stickSize = maxRadius * 0.25f;
    g.setColour(juce::Colours::black.withAlpha(0.4f));
    g.fillEllipse(stickPos.x - stickSize/2 + 2, stickPos.y - stickSize/2 + 2, stickSize, stickSize);
    
    g.setColour(joystickColour);
    g.fillEllipse(stickPos.x - stickSize/2, stickPos.y - stickSize/2, stickSize, stickSize);
    
    g.setColour(joystickColour.brighter(0.6f));
    g.drawEllipse(stickPos.x - stickSize/2, stickPos.y - stickSize/2, stickSize, stickSize, 2.0f);
}

// Estilo 1: Arcade - Estilo arcade con base
void DraggableJoystick::drawArcadeStyle(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    auto center = bounds.getCentre();
    auto maxRadius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
    
    // Base cuadrada con esquinas redondeadas
    g.setColour(juce::Colour(0xff2a2a2a));
    g.fillRoundedRectangle(bounds, 10.0f);
    
    g.setColour(style.borderColor);
    g.drawRoundedRectangle(bounds, 10.0f, 2.0f);
    
    // Guías diagonales
    g.setColour(style.borderColor.withAlpha(0.2f));
    g.drawLine(bounds.getX(), bounds.getY(), bounds.getRight(), bounds.getBottom(), 1.0f);
    g.drawLine(bounds.getRight(), bounds.getY(), bounds.getX(), bounds.getBottom(), 1.0f);
    g.drawLine(center.x, bounds.getY(), center.x, bounds.getBottom(), 1.0f);
    g.drawLine(bounds.getX(), center.y, bounds.getRight(), center.y, 1.0f);
    
    // Stick estilo arcade con base
    float stickSize = maxRadius * 0.3f;
    
    // Base del stick
    g.setColour(juce::Colour(0xff404040));
    g.fillEllipse(stickPos.x - stickSize * 0.6f, stickPos.y - stickSize * 0.6f, 
                 stickSize * 1.2f, stickSize * 1.2f);
    
    // Stick principal
    g.setColour(joystickColour);
    g.fillEllipse(stickPos.x - stickSize/2, stickPos.y - stickSize/2, stickSize, stickSize);
    
    // Top brillante
    g.setColour(joystickColour.brighter(0.8f));
    g.fillEllipse(stickPos.x - stickSize/4, stickPos.y - stickSize/4, stickSize/2, stickSize/2);
}

// Estilo 2: Flight - Stick de vuelo
void DraggableJoystick::drawFlightStyle(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    auto center = bounds.getCentre();
    auto maxRadius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
    
    // Fondo oscuro
    g.setColour(juce::Colour(0xff0a0a0a));
    g.fillEllipse(bounds);
    
    // Horizon line
    g.setColour(joystickColour.withAlpha(0.3f));
    g.drawLine(bounds.getX(), center.y, bounds.getRight(), center.y, 2.0f);
    
    // Pitch indicators
    for (int i = -2; i <= 2; ++i)
    {
        if (i == 0) continue;
        float y = center.y + (i * maxRadius / 3.0f);
        float lineWidth = bounds.getWidth() * 0.3f;
        g.setColour(joystickColour.withAlpha(0.2f));
        g.drawLine(center.x - lineWidth/2, y, center.x + lineWidth/2, y, 1.0f);
    }
    
    g.setColour(style.borderColor);
    g.drawEllipse(bounds, 2.0f);
    
    // Stick con trail
    juce::Path trail;
    trail.startNewSubPath(center);
    trail.lineTo(stickPos);
    
    g.setColour(joystickColour.withAlpha(0.3f));
    g.strokePath(trail, juce::PathStrokeType(2.0f));
    
    float stickSize = maxRadius * 0.2f;
    g.setColour(joystickColour);
    g.fillEllipse(stickPos.x - stickSize/2, stickPos.y - stickSize/2, stickSize, stickSize);
    
    // Crosshair en stick
    g.setColour(juce::Colours::black);
    g.drawLine(stickPos.x - stickSize/4, stickPos.y, stickPos.x + stickSize/4, stickPos.y, 1.5f);
    g.drawLine(stickPos.x, stickPos.y - stickSize/4, stickPos.x, stickPos.y + stickSize/4, 1.5f);
}

// Estilo 3: Minimal - Minimalista
void DraggableJoystick::drawMinimalStyle(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    auto center = bounds.getCentre();
    
    // Borde simple
    g.setColour(style.borderColor.withAlpha(0.3f));
    g.drawEllipse(bounds, 1.0f);
    
    // Punto central
    g.setColour(style.borderColor.withAlpha(0.5f));
    g.fillEllipse(center.x - 3, center.y - 3, 6, 6);
    
    // Stick minimalista
    float stickSize = 12.0f;
    
    g.setColour(joystickColour.withAlpha(0.8f));
    g.fillEllipse(stickPos.x - stickSize/2, stickPos.y - stickSize/2, stickSize, stickSize);
    
    g.setColour(joystickColour);
    g.drawEllipse(stickPos.x - stickSize/2, stickPos.y - stickSize/2, stickSize, stickSize, 2.0f);
}

// Estilo 4: Neon - Neon con glow
void DraggableJoystick::drawNeonStyle(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    auto center = bounds.getCentre();
    auto maxRadius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
    
    g.setColour(juce::Colours::black);
    g.fillEllipse(bounds);
    
    // Anillos neon
    for (int i = 0; i < 3; ++i)
    {
        float radius = maxRadius * (1.0f - i * 0.3f);
        g.setColour(joystickColour.withAlpha(0.3f / (i + 1)));
        g.drawEllipse(center.x - radius, center.y - radius, radius * 2, radius * 2, 2.0f);
    }
    
    // Trail neon
    juce::Path trail;
    trail.startNewSubPath(center);
    trail.lineTo(stickPos);
    
    for (int i = 3; i > 0; --i)
    {
        g.setColour(joystickColour.withAlpha(0.2f * i / 3.0f));
        g.strokePath(trail, juce::PathStrokeType(i * 2.0f));
    }
    
    // Stick con glow
    float stickSize = maxRadius * 0.25f;
    
    for (int i = 4; i > 0; --i)
    {
        float size = stickSize * (1 + i * 0.3f);
        g.setColour(joystickColour.withAlpha(0.2f * (5 - i) / 4.0f));
        g.fillEllipse(stickPos.x - size/2, stickPos.y - size/2, size, size);
    }
    
    g.setColour(joystickColour);
    g.fillEllipse(stickPos.x - stickSize/2, stickPos.y - stickSize/2, stickSize, stickSize);
}

// Estilo 5: Mechanical - Mecánico con detalles
void DraggableJoystick::drawMechanicalStyle(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    auto center = bounds.getCentre();
    auto maxRadius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
    
    // Base metálica
    juce::ColourGradient metalGradient(
        juce::Colour(0xff505050), center.x, bounds.getY(),
        juce::Colour(0xff2a2a2a), center.x, bounds.getBottom(),
        false
    );
    g.setGradientFill(metalGradient);
    g.fillEllipse(bounds);
    
    g.setColour(juce::Colour(0xff707070));
    g.drawEllipse(bounds, 2.0f);
    
    // Tornillos decorativos
    for (int i = 0; i < 4; ++i)
    {
        float angle = i * juce::MathConstants<float>::halfPi;
        float screwDist = maxRadius * 0.85f;
        float x = center.x + screwDist * std::cos(angle);
        float y = center.y + screwDist * std::sin(angle);
        
        g.setColour(juce::Colour(0xff404040));
        g.fillEllipse(x - 4, y - 4, 8, 8);
        
        g.setColour(juce::Colour(0xff606060));
        g.drawEllipse(x - 4, y - 4, 8, 8, 1.0f);
        g.drawLine(x - 3, y, x + 3, y, 1.0f);
    }
    
    // Grid mecánico
    g.setColour(joystickColour.withAlpha(0.15f));
    for (int i = -3; i <= 3; ++i)
    {
        float offset = i * maxRadius / 4.0f;
        g.drawLine(bounds.getX(), center.y + offset, bounds.getRight(), center.y + offset, 0.5f);
        g.drawLine(center.x + offset, bounds.getY(), center.x + offset, bounds.getBottom(), 0.5f);
    }
    
    // Stick mecánico
    float stickSize = maxRadius * 0.28f;
    
    g.setColour(juce::Colour(0xff505050));
    g.fillEllipse(stickPos.x - stickSize/2, stickPos.y - stickSize/2, stickSize, stickSize);
    
    g.setColour(joystickColour);
    g.fillEllipse(stickPos.x - stickSize * 0.35f, stickPos.y - stickSize * 0.35f, 
                 stickSize * 0.7f, stickSize * 0.7f);
    
    g.setColour(joystickColour.brighter(0.5f));
    g.drawEllipse(stickPos.x - stickSize/2, stickPos.y - stickSize/2, stickSize, stickSize, 1.5f);
}

// Estilo 6: Orb - Orbe flotante
void DraggableJoystick::drawOrbStyle(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    auto center = bounds.getCentre();
    auto maxRadius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
    
    // Fondo oscuro
    g.setColour(juce::Colour(0xff0a0a0a));
    g.fillEllipse(bounds);
    
    // Anillos energéticos
    for (int i = 0; i < 4; ++i)
    {
        float radius = maxRadius * (0.3f + i * 0.2f);
        float alpha = 0.2f / (i + 1);
        
        g.setColour(joystickColour.withAlpha(alpha));
        g.drawEllipse(center.x - radius, center.y - radius, radius * 2, radius * 2, 1.5f);
    }
    
    g.setColour(style.borderColor.withAlpha(0.4f));
    g.drawEllipse(bounds, 1.5f);
    
    // Orbe con capas de energía
    float orbSize = maxRadius * 0.35f;
    
    for (int i = 3; i > 0; --i)
    {
        float size = orbSize * (1 + i * 0.4f);
        g.setColour(joystickColour.withAlpha(0.15f * (4 - i) / 3.0f));
        g.fillEllipse(stickPos.x - size/2, stickPos.y - size/2, size, size);
    }
    
    // Orbe principal con gradiente
    juce::ColourGradient orbGradient(
        joystickColour.brighter(0.5f), stickPos.x - orbSize/4, stickPos.y - orbSize/4,
        joystickColour, stickPos.x + orbSize/4, stickPos.y + orbSize/4,
        true
    );
    g.setGradientFill(orbGradient);
    g.fillEllipse(stickPos.x - orbSize/2, stickPos.y - orbSize/2, orbSize, orbSize);
    
    // Highlight
    g.setColour(juce::Colours::white.withAlpha(0.8f));
    g.fillEllipse(stickPos.x - orbSize/3, stickPos.y - orbSize/3, orbSize/3, orbSize/3);
}

// Estilo 7: Target - Target con mira
void DraggableJoystick::drawTargetStyle(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    auto center = bounds.getCentre();
    auto maxRadius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
    
    g.setColour(juce::Colour(0xff1a1a1a));
    g.fillEllipse(bounds);
    
    // Círculos target
    for (int i = 1; i <= 4; ++i)
    {
        float radius = maxRadius * i / 4.0f;
        g.setColour(joystickColour.withAlpha(0.2f));
        g.drawEllipse(center.x - radius, center.y - radius, radius * 2, radius * 2, 1.0f);
    }
    
    // Crosshair principal
    g.setColour(joystickColour.withAlpha(0.5f));
    g.drawLine(center.x, bounds.getY(), center.x, bounds.getBottom(), 1.5f);
    g.drawLine(bounds.getX(), center.y, bounds.getRight(), center.y, 1.5f);
    
    g.setColour(style.borderColor);
    g.drawEllipse(bounds, 2.0f);
    
    // Crosshair en stick
    g.setColour(joystickColour);
    float lineLen = 15.0f;
    g.drawLine(stickPos.x - lineLen, stickPos.y, stickPos.x + lineLen, stickPos.y, 2.0f);
    g.drawLine(stickPos.x, stickPos.y - lineLen, stickPos.x, stickPos.y + lineLen, 2.0f);
    
    // Centro del target
    g.fillEllipse(stickPos.x - 6, stickPos.y - 6, 12, 12);
    
    g.setColour(juce::Colours::black);
    g.fillEllipse(stickPos.x - 3, stickPos.y - 3, 6, 6);
}

// Estilo 8: Compass - Brújula
void DraggableJoystick::drawCompassStyle(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    auto center = bounds.getCentre();
    auto maxRadius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
    
    // Fondo tipo brújula
    juce::ColourGradient compassGradient(
        juce::Colour(0xff2a2a1a), center.x, bounds.getY(),
        juce::Colour(0xff1a1a0a), center.x, bounds.getBottom(),
        false
    );
    g.setGradientFill(compassGradient);
    g.fillEllipse(bounds);
    
    g.setColour(juce::Colour(0xff8b7355));
    g.drawEllipse(bounds, 2.0f);
    
    // Marcas cardinales
    g.setFont(juce::FontOptions(14.0f, juce::Font::bold));
    juce::String cardinals[] = {"N", "E", "S", "W"};
    
    for (int i = 0; i < 4; ++i)
    {
        float angle = i * juce::MathConstants<float>::halfPi - juce::MathConstants<float>::halfPi;
        float textRadius = maxRadius * 0.75f;
        float x = center.x + textRadius * std::cos(angle);
        float y = center.y + textRadius * std::sin(angle);
        
        g.setColour(joystickColour);
        g.drawText(cardinals[i], static_cast<int>(x - 10), static_cast<int>(y - 10), 
                  20, 20, juce::Justification::centred);
    }
    
    // Marcas de grados
    for (int i = 0; i < 360; i += 30)
    {
        float angle = i * juce::MathConstants<float>::pi / 180.0f - juce::MathConstants<float>::halfPi;
        float innerRadius = maxRadius * 0.9f;
        float outerRadius = maxRadius * 0.95f;
        
        float x1 = center.x + innerRadius * std::cos(angle);
        float y1 = center.y + innerRadius * std::sin(angle);
        float x2 = center.x + outerRadius * std::cos(angle);
        float y2 = center.y + outerRadius * std::sin(angle);
        
        g.setColour(joystickColour.withAlpha(0.5f));
        g.drawLine(x1, y1, x2, y2, (i % 90 == 0) ? 2.0f : 1.0f);
    }
    
    // Aguja de brújula
    juce::Path needle;
    needle.startNewSubPath(center.x, center.y);
    needle.lineTo(stickPos.x, stickPos.y);
    
    g.setColour(juce::Colours::red);
    g.strokePath(needle, juce::PathStrokeType(3.0f));
    
    g.fillEllipse(stickPos.x - 8, stickPos.y - 8, 16, 16);
    
    // Centro
    g.setColour(juce::Colour(0xff8b7355));
    g.fillEllipse(center.x - 5, center.y - 5, 10, 10);
}

// Estilo 9: Galaxy - Galaxia con estrellas
void DraggableJoystick::drawGalaxyStyle(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    auto center = bounds.getCentre();
    auto maxRadius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
    
    g.setColour(juce::Colours::black);
    g.fillEllipse(bounds);
    
    // Estrellas de fondo
    juce::Random random(static_cast<int>(radius * 1000 + angle * 1000));
    for (int i = 0; i < 80; ++i)
    {
        float r = random.nextFloat() * maxRadius;
        float a = random.nextFloat() * juce::MathConstants<float>::twoPi;
        float x = center.x + r * std::cos(a);
        float y = center.y + r * std::sin(a);
        float size = random.nextFloat() * 1.5f + 0.5f;
        float brightness = random.nextFloat();
        
        g.setColour(juce::Colours::white.withAlpha(brightness * 0.8f));
        g.fillEllipse(x, y, size, size);
    }
    
    // Espiral galáctica
    for (int i = 0; i < 3; ++i)
    {
        juce::Path spiral;
        float startAngle = i * juce::MathConstants<float>::twoPi / 3.0f;
        
        spiral.startNewSubPath(center.x, center.y);
        for (float t = 0; t < juce::MathConstants<float>::twoPi * 1.5f; t += 0.1f)
        {
            float r = t * maxRadius / (juce::MathConstants<float>::twoPi * 1.5f);
            float a = startAngle + t;
            spiral.lineTo(center.x + r * std::cos(a), center.y + r * std::sin(a));
        }
        
        g.setColour(joystickColour.withAlpha(0.2f));
        g.strokePath(spiral, juce::PathStrokeType(2.0f));
    }
    
    g.setColour(style.borderColor.withAlpha(0.3f));
    g.drawEllipse(bounds, 1.0f);
    
    // Planeta/orbe principal
    float orbSize = maxRadius * 0.4f;
    
    juce::ColourGradient planetGradient(
        joystickColour.brighter(0.3f), stickPos.x - orbSize/3, stickPos.y - orbSize/3,
        joystickColour.darker(0.3f), stickPos.x + orbSize/3, stickPos.y + orbSize/3,
        true
    );
    g.setGradientFill(planetGradient);
    g.fillEllipse(stickPos.x - orbSize/2, stickPos.y - orbSize/2, orbSize, orbSize);
    
    // Anillos
    g.setColour(joystickColour.withAlpha(0.4f));
    g.drawEllipse(stickPos.x - orbSize * 0.7f, stickPos.y - orbSize * 0.35f, 
                 orbSize * 1.4f, orbSize * 0.7f, 2.0f);
    
    // Glow exterior
    for (int i = 2; i > 0; --i)
    {
        float glowSize = orbSize * (1 + i * 0.3f);
        g.setColour(joystickColour.withAlpha(0.1f * (3 - i) / 2.0f));
        g.fillEllipse(stickPos.x - glowSize/2, stickPos.y - glowSize/2, glowSize, glowSize);
    }
}

void DraggableJoystick::updateFromMidiValue(int value)
{
    // Para joystick, usar el valor MIDI para controlar el radio
    radius = value / 127.0f;
    
    // Actualizar posición del stick basado en radio y ángulo
    auto bounds = getLocalBounds().reduced(15).toFloat();
    auto center = bounds.getCentre();
    float maxRadius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f - 20.0f;
    
    float angleRadians = angle * juce::MathConstants<float>::twoPi;
    stickPos.x = center.x + std::cos(angleRadians) * (radius * maxRadius);
    stickPos.y = center.y + std::sin(angleRadians) * (radius * maxRadius);
    
    repaint();
}

juce::ValueTree DraggableJoystick::toValueTree() const
{
    auto tree = DraggableWidget::toValueTree();
    tree.setProperty("radius", radius, nullptr);
    tree.setProperty("angle", angle, nullptr);
    tree.setProperty("joystickStyle", joystickStyle, nullptr);
    tree.setProperty("joystickColor", joystickColour.toString(), nullptr);
    return tree;
}

// ============================================================================
// DraggablePitchWheel - Slider vertical con retorno al centro
// ============================================================================

DraggablePitchWheel::DraggablePitchWheel(const juce::String& name)
    : DraggableWidget(WidgetType::PitchWheel, name)
{
    setSize(60, 200);
}

void DraggablePitchWheel::resized()
{
    DraggableWidget::resized();
}

void DraggablePitchWheel::mouseDown(const juce::MouseEvent& e)
{
    if (!isResizing)
    {
        updateFromMouse(e.getPosition());
    }
    DraggableWidget::mouseDown(e);
}

void DraggablePitchWheel::mouseDrag(const juce::MouseEvent& e)
{
    if (!isResizing && !isDragging)
    {
        updateFromMouse(e.getPosition());
    }
    else
    {
        DraggableWidget::mouseDrag(e);
    }
}

void DraggablePitchWheel::mouseUp(const juce::MouseEvent& e)
{
    if (!isResizing && !isDragging)
    {
        returnToCenter();
    }
    DraggableWidget::mouseUp(e);
}

void DraggablePitchWheel::updateFromMouse(const juce::Point<int>& mousePos)
{
    auto bounds = getLocalBounds().reduced(10, 15).toFloat();
    
    // Calcular valor basado en Y (invertido: arriba = 1, abajo = 0)
    float y = juce::jlimit(bounds.getY(), bounds.getBottom(), (float)mousePos.y);
    value = 1.0f - ((y - bounds.getY()) / bounds.getHeight());
    
    repaint();
}

void DraggablePitchWheel::returnToCenter()
{
    value = 0.5f;
    repaint();
}

void DraggablePitchWheel::paintWidget(juce::Graphics& g)
{
    auto bounds = getLocalBounds().reduced(10, 15).toFloat();
    
    // Fondo con gradiente
    juce::ColourGradient gradient(
        style.bgColor.withAlpha(0.4f), bounds.getCentreX(), bounds.getY(),
        style.bgColor.withAlpha(0.7f), bounds.getCentreX(), bounds.getBottom(),
        false
    );
    g.setGradientFill(gradient);
    g.fillRoundedRectangle(bounds, style.cornerRadius);
    
    // Borde
    g.setColour(style.borderColor);
    g.drawRoundedRectangle(bounds, style.cornerRadius, style.borderWidth);
    
    // Linea central (posicion neutra)
    g.setColour(style.borderColor.withAlpha(0.4f));
    float centerY = bounds.getCentreY();
    g.drawLine(bounds.getX() + 5, centerY, bounds.getRight() - 5, centerY, 1.5f);
    
    // Indicador
    float indicatorY = bounds.getY() + (1.0f - value) * bounds.getHeight();
    auto indicatorBounds = juce::Rectangle<float>(
        bounds.getX() + 5,
        indicatorY - indicatorHeight/2,
        bounds.getWidth() - 10,
        indicatorHeight
    );
    
    // Limitar el indicador a los bordes
    if (indicatorBounds.getY() < bounds.getY())
        indicatorBounds.setY(bounds.getY());
    if (indicatorBounds.getBottom() > bounds.getBottom())
        indicatorBounds.setY(bounds.getBottom() - indicatorHeight);
    
    // Sombra del indicador
    g.setColour(juce::Colours::black.withAlpha(0.4f));
    g.fillRoundedRectangle(indicatorBounds.translated(1, 1), style.cornerRadius * 0.5f);
    
    // Indicador principal
    g.setColour(style.knobColor);
    g.fillRoundedRectangle(indicatorBounds, style.cornerRadius * 0.5f);
    
    // Borde del indicador
    g.setColour(style.knobColor.brighter(0.5f));
    g.drawRoundedRectangle(indicatorBounds, style.cornerRadius * 0.5f, 1.5f);
    
    // Valor de texto
    g.setColour(style.textColor);
    g.setFont(11.0f);
    int pitchValue = (int)((value - 0.5f) * 200.0f); // -100 a +100
    juce::String valueText = (pitchValue >= 0 ? "+" : "") + juce::String(pitchValue);
    g.drawText(valueText, bounds.getX(), bounds.getBottom() + 5, bounds.getWidth(), 15, juce::Justification::centred);
}

void DraggablePitchWheel::updateFromMidiValue(int midiValue)
{
    // Convertir valor MIDI (0-127) a rango del pitch wheel (0-1, centro en 0.5)
    value = midiValue / 127.0f;
    repaint();
}

juce::ValueTree DraggablePitchWheel::toValueTree() const
{
    auto tree = DraggableWidget::toValueTree();
    tree.setProperty("value", value, nullptr);
    return tree;
}

// ============================================================================
// DraggableIndexedSlider - Slider con posiciones discretas
// ============================================================================

DraggableIndexedSlider::DraggableIndexedSlider(const juce::String& name, const juce::StringArray& initialLabels)
    : DraggableWidget(WidgetType::IndexedSlider, name),
      labels(initialLabels),
      currentIndex(0)
{
    setSize(250, 60);
    if (labels.isEmpty())
        labels.add("Option 1");
}

void DraggableIndexedSlider::resized()
{
    DraggableWidget::resized();
}

void DraggableIndexedSlider::mouseDown(const juce::MouseEvent& e)
{
    if (!isResizing)
    {
        updateFromMouse(e.getPosition());
    }
    DraggableWidget::mouseDown(e);
}

void DraggableIndexedSlider::mouseDrag(const juce::MouseEvent& e)
{
    if (!isResizing && !isDragging)
    {
        updateFromMouse(e.getPosition());
    }
    else
    {
        DraggableWidget::mouseDrag(e);
    }
}

void DraggableIndexedSlider::updateFromMouse(const juce::Point<int>& mousePos)
{
    auto bounds = getLocalBounds().reduced(10).toFloat();
    
    // Calcular el indice basado en la posicion X
    float x = juce::jlimit(bounds.getX(), bounds.getRight(), (float)mousePos.x);
    float normalizedX = (x - bounds.getX()) / bounds.getWidth();
    
    int newIndex = (int)(normalizedX * labels.size());
    newIndex = juce::jlimit(0, labels.size() - 1, newIndex);
    
    if (newIndex != currentIndex)
    {
        currentIndex = newIndex;
        repaint();
    }
}

void DraggableIndexedSlider::setIndex(int idx)
{
    currentIndex = juce::jlimit(0, labels.size() - 1, idx);
    repaint();
}

void DraggableIndexedSlider::setLabels(const juce::StringArray& newLabels)
{
    labels = newLabels;
    if (labels.isEmpty())
        labels.add("Option 1");
    currentIndex = juce::jlimit(0, labels.size() - 1, currentIndex);
    repaint();
}

void DraggableIndexedSlider::paintWidget(juce::Graphics& g)
{
    auto bounds = getLocalBounds().reduced(10).toFloat();
    
    // Fondo con gradiente
    juce::ColourGradient gradient(
        style.bgColor.withAlpha(0.3f), bounds.getCentreX(), bounds.getY(),
        style.bgColor.withAlpha(0.6f), bounds.getCentreX(), bounds.getBottom(),
        false
    );
    g.setGradientFill(gradient);
    g.fillRoundedRectangle(bounds, style.cornerRadius);
    
    // Borde
    g.setColour(style.borderColor);
    g.drawRoundedRectangle(bounds, style.cornerRadius, style.borderWidth);
    
    // Dibujar segmentos de fondo
    float segmentWidth = bounds.getWidth() / labels.size();
    for (int i = 0; i < labels.size(); ++i)
    {
        auto segmentBounds = juce::Rectangle<float>(
            bounds.getX() + i * segmentWidth,
            bounds.getY(),
            segmentWidth,
            bounds.getHeight()
        ).reduced(2);
        
        // Resaltar el segmento actual
        if (i == currentIndex)
        {
            g.setColour(style.knobColor.withAlpha(0.3f));
            g.fillRoundedRectangle(segmentBounds, style.cornerRadius * 0.5f);
        }
        
        // Etiqueta
        g.setColour(i == currentIndex ? style.textColor : style.textColor.withAlpha(0.6f));
        g.setFont(juce::Font(i == currentIndex ? 13.0f : 11.0f, i == currentIndex ? juce::Font::bold : juce::Font::plain));
        g.drawText(labels[i], segmentBounds, juce::Justification::centred);
        
        // Separadores
        if (i < labels.size() - 1)
        {
            g.setColour(style.borderColor.withAlpha(0.3f));
            float sepX = bounds.getX() + (i + 1) * segmentWidth;
            g.drawLine(sepX, bounds.getY() + 5, sepX, bounds.getBottom() - 5, 1.0f);
        }
    }
    
    // Indicador animado en la parte inferior
    float indicatorWidth = segmentWidth * 0.8f;
    float indicatorX = bounds.getX() + currentIndex * segmentWidth + (segmentWidth - indicatorWidth) / 2.0f;
    auto indicatorBounds = juce::Rectangle<float>(
        indicatorX,
        bounds.getBottom() - 6,
        indicatorWidth,
        4
    );
    
    g.setColour(style.knobColor);
    g.fillRoundedRectangle(indicatorBounds, 2.0f);
}

void DraggableIndexedSlider::updateFromMidiValue(int value)
{
    // Convertir valor MIDI (0-127) a índice discreto
    if (labels.size() > 0)
    {
        int index = (int)((value / 127.0f) * (labels.size() - 1) + 0.5f);
        setIndex(index);
    }
}

juce::ValueTree DraggableIndexedSlider::toValueTree() const
{
    auto tree = DraggableWidget::toValueTree();
    tree.setProperty("currentIndex", currentIndex, nullptr);
    
    // Guardar labels como string separado por comas
    juce::String labelsString = labels.joinIntoString(",");
    tree.setProperty("labels", labelsString, nullptr);
    
    return tree;
}
