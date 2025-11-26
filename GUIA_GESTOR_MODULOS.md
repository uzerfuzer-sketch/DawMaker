# GUÍA DEL GESTOR DE MÓDULOS JUCE

## ¿Qué es el Gestor de Módulos?

El **Gestor de Módulos** es una herramienta visual que te permite seleccionar qué partes del framework JUCE incluir en tu proyecto. En lugar de incluir todo JUCE (lo cual aumentaría el tiempo de compilación), puedes elegir solo los módulos que necesitas.

---

## Estructura de Módulos JUCE

### 📦 Core (Módulos Esenciales)

#### **juce_core** ✅ REQUERIDO
- **Qué incluye**: Clases fundamentales
  - `String`, `StringArray` - Manejo de texto
  - `Array`, `OwnedArray` - Contenedores de datos
  - `File`, `FileInputStream` - Sistema de archivos
  - `Time`, `RelativeTime` - Tiempo y fechas
  - `Thread`, `ThreadPool` - Multi-threading
  - `MemoryBlock`, `MemoryOutputStream` - Manejo de memoria
  - `Random`, `Range` - Utilidades matemáticas

- **Cuándo usarlo**: SIEMPRE (es la base de todo JUCE)

#### **juce_events** ✅ REQUERIDO
- **Qué incluye**:
  - `MessageManager` - Sistema de mensajes
  - `Timer` - Temporizadores
  - `AsyncUpdater` - Actualizaciones asíncronas
  - Callbacks y listeners

- **Cuándo usarlo**: Siempre que uses GUI o necesites eventos

#### **juce_data_structures**
- **Qué incluye**:
  - `ValueTree` - Árboles de datos jerárquicos
  - `UndoManager` - Sistema de deshacer/rehacer
  - `Identifier` - Identificadores únicos

- **Cuándo usarlo**:
  - Proyectos con configuración compleja
  - Sistemas de presets
  - Gestión de estado de la aplicación

---

### 🎨 GUI (Interfaz Gráfica)

#### **juce_graphics**
- **Qué incluye**:
  - `Graphics` - API de renderizado 2D
  - `Colour`, `ColourGradient` - Colores y gradientes
  - `Font` - Fuentes y texto
  - `Image` - Imágenes y bitmaps
  - `Path` - Formas vectoriales
  - `Rectangle`, `Point` - Geometría

- **Cuándo usarlo**: Base para cualquier GUI

#### **juce_gui_basics**
- **Qué incluye**: Componentes básicos
  - `Button`, `TextButton`, `ImageButton`
  - `Slider` - Controles deslizantes
  - `Label` - Etiquetas de texto
  - `TextEditor` - Entrada de texto
  - `ComboBox` - Listas desplegables
  - `ListBox` - Listas
  - `Component` - Clase base de componentes

- **Cuándo usarlo**: Cualquier interfaz de usuario

#### **juce_gui_extra**
- **Qué incluye**: Componentes avanzados
  - `WebBrowserComponent` - Navegador web integrado
  - `CodeEditorComponent` - Editor de código
  - `FileBrowserComponent` - Explorador de archivos
  - `TableListBox` - Tablas
  - `AnimatedAppComponent` - Animaciones

- **Cuándo usarlo**: Interfaces complejas o especializadas

---

### 🔊 Audio (Procesamiento de Audio)

#### **juce_audio_basics**
- **Qué incluye**:
  - `AudioBuffer` - Buffers de audio
  - `AudioSampleBuffer` - Muestras de audio
  - `MidiMessage`, `MidiBuffer` - Mensajes MIDI
  - `AudioProcessor` - Clase base de procesadores
  - Sample rate, canales, formatos

- **Cuándo usarlo**: Cualquier aplicación de audio

#### **juce_audio_devices**
- **Qué incluye**:
  - `AudioDeviceManager` - Gestión de dispositivos
  - `AudioIODevice` - Dispositivos de entrada/salida
  - `MidiInput`, `MidiOutput` - Entrada/salida MIDI
  - Driver selection (ASIO, CoreAudio, etc.)

- **Cuándo usarlo**:
  - Aplicaciones standalone
  - Grabación o reproducción en tiempo real

#### **juce_audio_formats**
- **Qué incluye**: Lectura/escritura de archivos
  - WAV, AIFF - Formatos sin comprimir
  - MP3, OGG, FLAC - Formatos comprimidos
  - `AudioFormatReader`, `AudioFormatWriter`

- **Cuándo usarlo**:
  - Importar/exportar audio
  - Samplers, grabadoras

#### **juce_audio_processors**
- **Qué incluye**: Framework de plugins
  - `AudioProcessor` - Clase base de plugins
  - VST3, AU, AAX wrappers
  - `AudioProcessorEditor` - Editor del plugin
  - Parameter management

- **Cuándo usarlo**: Desarrollo de plugins de audio

#### **juce_audio_utils**
- **Qué incluye**: Utilidades de audio + GUI
  - `AudioAppComponent` - Aplicaciones de audio
  - `MidiKeyboardComponent` - Teclado MIDI visual
  - `AudioThumbnailComponent` - Waveform display
  - Audio file players

- **Cuándo usarlo**: Aplicaciones que combinan audio y GUI

#### **juce_dsp**
- **Qué incluye**: Algoritmos DSP
  - Filtros (IIR, FIR, State Variable)
  - Delays y reverbs
  - Compresores y limiters
  - FFT y análisis de frecuencia
  - Convolución
  - Oscillators y generadores

- **Cuándo usarlo**: Procesamiento avanzado de señales

---

### 🎬 Media y Gráficos Avanzados

#### **juce_opengl**
- **Qué incluye**:
  - `OpenGLContext` - Contexto OpenGL
  - `OpenGLShaderProgram` - Shaders
  - Renderizado acelerado por GPU

- **Cuándo usarlo**:
  - Gráficos 3D
  - Visualizaciones intensivas
  - Efectos visuales complejos

#### **juce_video**
- **Qué incluye**:
  - `VideoComponent` - Reproductor de video
  - Soporte para formatos de video

- **Cuándo usarlo**: Aplicaciones multimedia

---

## Cómo Usar el Gestor

### 1️⃣ Seleccionar Módulos

1. Abre la pestaña **"Modulos"** en el Panel de Gestión de Proyectos
2. Marca los checkboxes de los módulos que necesitas
3. Los módulos en **gris** son REQUERIDOS (no se pueden deseleccionar)
4. Lee las descripciones para entender qué hace cada módulo

**Botones útiles**:
- **?** - Muestra esta guía
- **Todos** - Selecciona todos los módulos
- **Ninguno** - Deselecciona todos (excepto requeridos)

### 2️⃣ Generar Configuración

Una vez seleccionados los módulos:

- **Generar CMakeLists.txt**: Crea un archivo CMake con tus módulos
- **Generar .jucer**: Crea proyecto para Projucer (próximamente)

### 3️⃣ Compilar

El proyecto incluirá automáticamente los módulos seleccionados.

---

## Combinaciones Comunes

### 🎸 Plugin de Audio Simple
```
✅ juce_core (requerido)
✅ juce_events (requerido)
✅ juce_audio_basics
✅ juce_audio_processors
✅ juce_gui_basics
```

**Para qué**: Plugin VST/AU básico con interfaz simple

---

### 🎹 Plugin de Audio Avanzado
```
✅ juce_core
✅ juce_events
✅ juce_data_structures (para presets)
✅ juce_audio_basics
✅ juce_audio_processors
✅ juce_dsp (para procesamiento)
✅ juce_gui_basics
✅ juce_gui_extra (para controles avanzados)
```

**Para qué**: Plugin profesional con procesamiento DSP y GUI compleja

---

### 🎼 DAW o Aplicación Standalone
```
✅ juce_core
✅ juce_events
✅ juce_data_structures
✅ juce_audio_basics
✅ juce_audio_devices
✅ juce_audio_formats
✅ juce_audio_utils
✅ juce_gui_basics
✅ juce_gui_extra
```

**Para qué**: Aplicación completa de audio con grabación/reproducción

---

### 📊 Aplicación Solo GUI
```
✅ juce_core
✅ juce_events
✅ juce_graphics
✅ juce_gui_basics
```

**Para qué**: Aplicación sin funcionalidad de audio

---

### 🎛️ Procesador DSP sin GUI
```
✅ juce_core
✅ juce_audio_basics
✅ juce_dsp
```

**Para qué**: Procesamiento de audio offline o en línea de comandos

---

## Dependencias Automáticas

Algunos módulos requieren otros. El sistema los incluirá automáticamente:

- `juce_gui_basics` → requiere `juce_graphics`
- `juce_graphics` → requiere `juce_core` + `juce_events`
- `juce_audio_utils` → requiere `juce_audio_basics` + `juce_gui_basics`
- `juce_audio_devices` → requiere `juce_audio_basics`

---

## Consejos Prácticos

### ✅ Mejores Prácticas

1. **Empieza Simple**: Solo incluye lo que sabes que necesitas
2. **Agrega Gradualmente**: Siempre puedes agregar más después
3. **Lee las Descripciones**: Cada módulo tiene una explicación
4. **Considera el Tamaño**: Más módulos = mayor tiempo de compilación

### ⚠️ Errores Comunes

1. **Olvidar juce_audio_devices**: Necesario para reproducir audio
2. **No incluir juce_dsp**: Necesario para filtros y efectos
3. **Incluir todo**: Aumenta innecesariamente el tiempo de compilación
4. **Ignorar dependencias**: Algunos módulos requieren otros

---

## Ejemplos Paso a Paso

### Ejemplo 1: Crear un Reverb Simple

**Módulos necesarios**:
```
✅ juce_core
✅ juce_events
✅ juce_audio_basics
✅ juce_audio_processors
✅ juce_dsp (para el reverb)
✅ juce_gui_basics
```

**Por qué**:
- `juce_audio_processors`: Framework de plugin
- `juce_dsp`: Algoritmo de reverb
- `juce_gui_basics`: Controles de parámetros

---

### Ejemplo 2: Sampler con Waveform Display

**Módulos necesarios**:
```
✅ juce_core
✅ juce_events  
✅ juce_audio_basics
✅ juce_audio_formats (leer archivos)
✅ juce_audio_utils (waveform display)
✅ juce_gui_basics
```

**Por qué**:
- `juce_audio_formats`: Cargar WAV/MP3
- `juce_audio_utils`: AudioThumbnailComponent

---

### Ejemplo 3: MIDI Controller Visual

**Módulos necesarios**:
```
✅ juce_core
✅ juce_events
✅ juce_audio_basics (para MidiMessage)
✅ juce_audio_devices (para MidiInput/Output)
✅ juce_audio_utils (para MidiKeyboardComponent)
✅ juce_gui_basics
```

**Por qué**:
- `juce_audio_devices`: Acceso a puertos MIDI
- `juce_audio_utils`: Teclado MIDI visual

---

## Preguntas Frecuentes

### ❓ ¿Puedo cambiar los módulos después?
**Sí**, solo modifica tu selección y regenera el CMakeLists.txt

### ❓ ¿Qué módulos necesito para un plugin VST?
`juce_audio_processors` + `juce_audio_basics` + `juce_gui_basics` (mínimo)

### ❓ ¿Por qué algunos están en gris?
Son **requeridos** por JUCE y no pueden desactivarse

### ❓ ¿Cómo sé si necesito juce_dsp?
Si vas a usar filtros, delays, reverbs, FFT → Sí, lo necesitas

### ❓ ¿juce_opengl es necesario?
Solo para gráficos 3D o visualizaciones GPU-aceleradas

---

## Recursos Adicionales

- **Documentación JUCE**: https://docs.juce.com
- **Tutoriales**: https://juce.com/learn/tutorials
- **Forum**: https://forum.juce.com

---

## Resumen Rápido

| Tipo de Proyecto | Módulos Core | Módulos Adicionales |
|-----------------|--------------|---------------------|
| Plugin Básico | core, events, audio_basics, audio_processors | gui_basics |
| Plugin Avanzado | + data_structures | + dsp, gui_extra |
| DAW/Standalone | + audio_devices, audio_formats | + audio_utils |
| Solo GUI | core, events, graphics | gui_basics |
| DSP/CLI | core, audio_basics | dsp |

---

**¡Listo!** Ahora sabes cómo usar el Gestor de Módulos para optimizar tu proyecto JUCE.
