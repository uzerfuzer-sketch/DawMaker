# Custom DAW - Estación de Trabajo de Audio Digital

Una aplicación DAW (Digital Audio Workstation) profesional construida desde cero con JUCE, con enfoque en personalización completa de la GUI y capacidades robustas de audio/MIDI.

## 🎯 Características Principales

### GUI 100% Personalizable
- **CustomLookAndFeel**: Control total sobre la apariencia visual
  - Paleta de colores personalizable
  - Gradientes y efectos visuales
  - Tipografía y fuentes personalizadas
  - Animaciones y transiciones
- **Widgets Personalizados**:
  - `CustomRotarySlider`: Knobs rotatorios con estilo profesional
  - `CustomButton`: Botones con estados visuales dinámicos
  - Totalmente estilizables sin limitaciones

### Audio/MIDI de Alta Fidelidad
- **AudioEngine**: Motor de audio profesional
  - Soporte para ASIO (Windows), CoreAudio (macOS), ALSA (Linux)
  - Baja latencia optimizada
  - Configuración flexible de sample rate y buffer size
- **AudioDeviceManager**: Gestión completa de dispositivos
  - Detección automática de dispositivos de audio/MIDI
  - Configuración en tiempo real
  - Soporte para múltiples entradas/salidas

### Arquitectura Modular
- Código limpio y bien organizado
- Separación clara entre lógica y presentación
- Diseño extensible para añadir nuevos widgets
- Siguiendo las mejores prácticas de C++ moderno (C++17)

## 📋 Requisitos Previos

### Windows
- **Visual Studio 2019 o superior** (Community Edition es suficiente)
  - Con componentes de desarrollo de C++
  - Windows SDK
- **CMake 3.22 o superior**
- **Git** (para clonar JUCE)

### macOS
- **Xcode 12 o superior**
- **CMake 3.22 o superior**
- **Herramientas de línea de comandos de Xcode**

### Linux
- **GCC 9+ o Clang 10+**
- **CMake 3.22 o superior**
- **Bibliotecas de desarrollo**:
  ```bash
  sudo apt-get install build-essential cmake
  sudo apt-get install libasound2-dev libjack-jackd2-dev \
      ladspa-sdk \
      libcurl4-openssl-dev \
      libfreetype6-dev \
      libx11-dev libxcomposite-dev libxcursor-dev libxcursor-dev \
      libxext-dev libxinerama-dev libxrandr-dev libxrender-dev \
      libwebkit2gtk-4.0-dev \
      libglu1-mesa-dev mesa-common-dev
  ```

## 🚀 Instalación y Configuración

### Paso 1: Clonar JUCE (si aún no lo has hecho)

```powershell
# En PowerShell (Windows) o terminal (macOS/Linux)
git clone https://github.com/juce-framework/JUCE.git
```

### Paso 2: Verificar la Estructura del Proyecto

Tu estructura de directorios debe verse así:

```
freesurface 0.1/
├── JUCE/                    # Framework JUCE clonado
└── DAW_Project/             # Tu proyecto DAW
    ├── CMakeLists.txt
    ├── Include/
    │   ├── AudioEngine.h
    │   ├── CustomButton.h
    │   ├── CustomLookAndFeel.h
    │   ├── CustomRotarySlider.h
    │   ├── MainComponent.h
    │   └── MainWindow.h
    └── Source/
        ├── AudioEngine.cpp
        ├── CustomButton.cpp
        ├── CustomLookAndFeel.cpp
        ├── CustomRotarySlider.cpp
        ├── Main.cpp
        ├── MainComponent.cpp
        └── MainWindow.cpp
```

### Paso 3: Compilar el Proyecto

#### En Windows (PowerShell):

```powershell
# Navegar al directorio del proyecto
cd "DAW_Project"

# Crear directorio de compilación
mkdir build
cd build

# Configurar con CMake (Visual Studio)
cmake .. -G "Visual Studio 17 2022" -A x64

# Compilar (o abrir la solución en Visual Studio)
cmake --build . --config Release

# Ejecutar
.\Release\CustomDAW.exe
```

#### En macOS:

```bash
# Navegar al directorio del proyecto
cd DAW_Project

# Crear directorio de compilación
mkdir build && cd build

# Configurar con CMake
cmake .. -G "Xcode"

# Compilar
cmake --build . --config Release

# Ejecutar
open Release/CustomDAW.app
```

#### En Linux:

```bash
# Navegar al directorio del proyecto
cd DAW_Project

# Crear directorio de compilación
mkdir build && cd build

# Configurar con CMake
cmake .. -G "Unix Makefiles"

# Compilar
make -j$(nproc)

# Ejecutar
./CustomDAW
```

## 🎨 Personalización de la GUI

### Modificar el Look and Feel

El archivo `CustomLookAndFeel.cpp` contiene toda la lógica de personalización visual. Aquí hay algunos ejemplos:

#### Cambiar la Paleta de Colores:

```cpp
// En CustomLookAndFeel::initializeDefaultTheme()
themeColours["background"] = juce::Colour(0xff1a1a2e);      // Fondo principal
themeColours["accent"] = juce::Colour(0xff00adb5);          // Color de acento
themeColours["highlight"] = juce::Colour(0xffe94560);       // Resaltado
```

#### Personalizar un Knob Rotatorio:

```cpp
// En CustomLookAndFeel::drawRotarySlider()
// Cambiar el grosor del arco
g.strokePath(valueArc, juce::PathStrokeType(4.0f, ...));  // Cambia el 4.0f

// Cambiar el radio del knob
auto radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
```

#### Personalizar Botones:

```cpp
// En CustomLookAndFeel::drawButtonBackground()
auto cornerSize = 6.0f;  // Radio de las esquinas redondeadas
g.drawRoundedRectangle(bounds, cornerSize, 2.0f);  // Grosor del borde
```

### Añadir Nuevos Widgets

Para añadir un nuevo widget personalizado:

1. **Crear archivos de header e implementación**:
   ```cpp
   // Include/CustomWaveformDisplay.h
   #pragma once
   #include <JuceHeader.h>
   
   class CustomWaveformDisplay : public juce::Component
   {
   public:
       CustomWaveformDisplay();
       void paint(juce::Graphics& g) override;
       void resized() override;
   private:
       JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomWaveformDisplay)
   };
   ```

2. **Añadir al CMakeLists.txt**:
   ```cmake
   target_sources(CustomDAW PRIVATE
       ...
       Source/CustomWaveformDisplay.cpp
       Include/CustomWaveformDisplay.h
   )
   ```

3. **Usar en MainComponent**:
   ```cpp
   // En MainComponent.h
   CustomWaveformDisplay waveformDisplay;
   
   // En MainComponent.cpp constructor
   addAndMakeVisible(waveformDisplay);
   ```

## 🔊 Configuración de Audio

### Acceder a la Configuración

Cuando ejecutes la aplicación, haz clic en el botón **"Audio Settings"** para:
- Seleccionar dispositivo de audio (ASIO/CoreAudio/ALSA)
- Configurar sample rate (44.1kHz, 48kHz, 96kHz, etc.)
- Ajustar buffer size para optimizar latencia
- Configurar entradas/salidas MIDI

### Procesamiento de Audio Personalizado

Modifica `AudioEngine::getNextAudioBlock()` para implementar tu procesamiento:

```cpp
void AudioEngine::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    auto* leftChannel = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    auto* rightChannel = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);
    
    for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
    {
        // Ejemplo: generar un tono senoidal
        float phase = currentPhase;
        float sineValue = std::sin(phase * juce::MathConstants<float>::twoPi);
        
        leftChannel[sample] = sineValue * 0.1f;
        rightChannel[sample] = sineValue * 0.1f;
        
        currentPhase += phaseIncrement;
        if (currentPhase >= 1.0f)
            currentPhase -= 1.0f;
    }
}
```

## 📚 Recursos Adicionales

### Documentación JUCE
- **API Reference**: https://docs.juce.com/
- **Tutorials**: https://docs.juce.com/master/tutorial_simple_synth_noise.html
- **Forum**: https://forum.juce.com/

### Conceptos Clave

#### LookAndFeel
La clase `LookAndFeel` es el corazón de la personalización visual en JUCE. Permite sobrescribir métodos `draw*` para controlar:
- Formas y geometrías
- Colores y gradientes
- Tipografías
- Animaciones

#### AudioAppComponent
Simplifica la integración de audio al combinar:
- `AudioSource`: Para procesamiento de audio
- `Component`: Para la GUI
- `AudioDeviceManager`: Para gestión de dispositivos

#### Procesamiento de Audio en Tiempo Real
- `prepareToPlay()`: Inicializa recursos (llamado cuando cambia sample rate o buffer size)
- `getNextAudioBlock()`: Procesa cada bloque de audio (llamado continuamente)
- `releaseResources()`: Limpia recursos (llamado al detener audio)

## 🛠️ Próximos Pasos Recomendados

1. **Añadir más widgets personalizados**:
   - Faders verticales
   - Medidores de nivel (VU meters)
   - Display de forma de onda
   - Analizador de espectro

2. **Implementar procesamiento de audio real**:
   - Sintetizadores
   - Efectos de audio (reverb, delay, EQ)
   - Grabación y reproducción de audio

3. **Añadir funcionalidad MIDI**:
   - Piano roll
   - MIDI CC mapping
   - Secuenciador

4. **Mejorar la arquitectura**:
   - Sistema de plugins
   - Proyecto/sesión save/load
   - Automatización de parámetros

## 📝 Notas Importantes

- **Latencia**: Para audio profesional, usa ASIO en Windows (drivers específicos del fabricante)
- **Thread Safety**: El procesamiento de audio ocurre en un thread separado; usa locks apropiados
- **Optimización**: Evita allocaciones de memoria en `getNextAudioBlock()`
- **Look and Feel**: Recuerda llamar a `setLookAndFeel(nullptr)` antes de destruir el objeto CustomLookAndFeel

## 📄 Licencia

Este proyecto es un template educativo. JUCE tiene su propia licencia (GPL/Comercial).
Consulta: https://juce.com/juce-7-licence/

## 🤝 Contribuciones

Este es un proyecto base diseñado para aprendizaje. Siéntete libre de extenderlo y personalizarlo según tus necesidades.

---

**¡Disfruta construyendo tu DAW personalizada!** 🎵🎹🎸
