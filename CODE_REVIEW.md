# Revision de Codigo - Custom DAW Project
**Fecha:** 25 de noviembre de 2025  
**Framework:** JUCE C++17  
**Revisor:** Analisis de Calidad y Mejores Practicas

---

## 1. CALIDAD Y BUENAS PRACTICAS

### [OK] Aspectos Positivos

- **Uso de C++ moderno (C++17)**
  - Se utilizan `std::unique_ptr` y `std::make_unique` correctamente
  - Lambdas modernas para callbacks
  - Inicializacion uniforme en constructores

- **Arquitectura orientada a componentes**
  - Separacion clara entre Include/ y Source/
  - Uso correcto del patron Component de JUCE
  - Encapsulacion adecuada de responsabilidades

- **RAII (Resource Acquisition Is Initialization)**
  - `~MainComponent()` llama a `setLookAndFeel(nullptr)` y `shutdownAudio()`
  - Liberacion automatica de recursos en destructores

### [WARN] Problemas Identificados

#### **Problema 1: Punteros crudos en callbacks modales**
```cpp
// ProjectManager.cpp línea 156
w->enterModalState(true, juce::ModalCallbackFunction::create([this, windowPtr = w.get()](int result)
{
    // windowPtr es un puntero crudo - PELIGROSO
    if (result == 1)
    {
        auto filename = windowPtr->getTextEditorContents("filename");
        // ...
    }
}));

w.release(); // [ERROR] Libera el unique_ptr pero el lambda capturo el puntero crudo
```

**Solución:**
```cpp
// Capturar el shared_ptr para mantener el objeto vivo
auto wShared = std::shared_ptr<juce::AlertWindow>(w.release());
wShared->enterModalState(true, juce::ModalCallbackFunction::create(
    [this, wShared](int result)
    {
        if (result == 1)
        {
            auto filename = wShared->getTextEditorContents("filename");
            // Ahora es seguro - wShared mantiene vivo el objeto
        }
    }));
```

#### **Problema 2: Falta de const-correctness**
```cpp
// DraggableWidget.cpp
void DraggableWidget::paint(juce::Graphics& g)  // [ERROR] Deberia ser const
{
    g.setColour(juce::Colour(0xff2d2d44));
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 5.0f);
}
```

**Solución:**
```cpp
void DraggableWidget::paint(juce::Graphics& g) override
{
    // paint() no modifica el estado del widget, solo dibuja
    // JUCE no permite const aquí, pero internamente debemos evitar mutaciones
}
```

#### **Problema 3: Posible excepción en dynamic_cast no verificada**
```cpp
// ProjectManager.cpp línea 133
juce::File ProjectExplorer::getSelectedFile() const
{
    if (auto* selected = dynamic_cast<FileTreeItem*>(treeView.getSelectedItem(0)))
        return selected->getFile();
    
    return {}; // [OK] Bien - retorna File vacio si falla
}
```
Este caso está bien manejado, pero en otros lugares podría haber problemas.

#### **Problema 4: Variables shadowing (ocultan miembros)**
```cpp
// MainComponent.cpp advertencia C4458
void MainComponent::saveLayout()
{
    // ...
    int flags = juce::FileBrowserComponent::saveMode;  // [ERROR] Oculta Component::flags
}
```

**Solución:**
```cpp
void MainComponent::saveLayout()
{
    const int browserFlags = juce::FileBrowserComponent::saveMode;
    // Usa un nombre diferente para evitar confusión
}
```

#### **Problema 5: Falta de manejo de errores en operaciones de archivo**
```cpp
// ProjectManager.cpp línea 174
auto newFile = projectRoot.getChildFile(filename + extension);
newFile.create();  // [ERROR] No verifica si create() fallo
refreshTree();
```

**Solución:**
```cpp
auto newFile = projectRoot.getChildFile(filename + extension);
if (newFile.create().wasOk())
{
    refreshTree();
}
else
{
    juce::NativeMessageBox::showMessageBoxAsync(
        juce::MessageBoxIconType::WarningIcon,
        "Error",
        "No se pudo crear el archivo: " + newFile.getFullPathName());
}
```

#### **Problema 6: Acceso directo a miembros en lambdas sin validación**
```cpp
// MainComponent.cpp línea 16
toolbar.onAddWidget = [this](DraggableWidget::WidgetType type) {
    addWidget(type, juce::Point<int>(100, 100));
};
```

**Analisis:** Si `toolbar` sobrevive a `MainComponent`, el lambda captura un `this` invalido.  
**Solucion:** JUCE garantiza que los componentes hijos se destruyen antes del padre, asi que esto es seguro. Pero en general, usar `std::weak_ptr` seria mas robusto.

#### **Problema 7: Magic numbers (números mágicos)**
```cpp
// DraggableWidget.cpp línea 11
constrainer.setMinimumSize(50, 50);  // [ERROR] Por que 50?

// MainComponent.cpp linea 7
setSize(1600, 900);  // [ERROR] Valores hardcodeados
```

**Solución:**
```cpp
// En un archivo de constantes
namespace UIConstants
{
    constexpr int MinWidgetSize = 50;
    constexpr int DefaultWindowWidth = 1600;
    constexpr int DefaultWindowHeight = 900;
}

// Uso
constrainer.setMinimumSize(UIConstants::MinWidgetSize, UIConstants::MinWidgetSize);
setSize(UIConstants::DefaultWindowWidth, UIConstants::DefaultWindowHeight);
```

---

## 2. RENDIMIENTO

### Cuellos de Botella Detectados

#### **Cuello de Botella 1: Creación excesiva de objetos temporales**
```cpp
// ProjectManager.cpp línea 24
g.drawText(icon + " " + file.getFileName(), 4, 0, width - 4, height, 
           juce::Justification::centredLeft, true);
```

**Problema:** Se crean 3 juce::String temporales en cada llamada a paintItem().

**Solución optimizada:**
```cpp
void paintItem(juce::Graphics& g, int width, int height) override
{
    // Usar string caching
    if (cachedDisplayName.isEmpty())
    {
        auto icon = file.isDirectory() ? "[D] " : getIconForFile(file);
        cachedDisplayName = icon + file.getFileName();
    }
    
    auto colour = isSelected() ? juce::Colours::lightblue : juce::Colours::white;
    g.setColour(colour);
    g.setFont(juce::FontOptions(14.0f));
    g.drawText(cachedDisplayName, 4, 0, width - 4, height, 
               juce::Justification::centredLeft, true);
}

private:
    juce::String cachedDisplayName;
```

#### **Cuello de Botella 2: Búsqueda de archivos sin caché**
```cpp
// ProjectManager.cpp línea 39
juce::Array<juce::File> files = file.findChildFiles(
    juce::File::findFilesAndDirectories, false);

files.sort();  // [ERROR] Ordenamiento en cada expansion
```

**Problema:** `findChildFiles()` es una operación de I/O lenta. Se ejecuta cada vez que se expande un nodo.

**Solución optimizada:**
```cpp
void refreshSubItems()
{
    clearSubItems();
    
    if (file.isDirectory())
    {
        // Usar caché con timestamp
        if (lastScanTime == file.getLastModificationTime())
        {
            // Usar caché
            for (auto& f : cachedFiles)
                addSubItem(new FileTreeItem(f));
            return;
        }
        
        juce::Array<juce::File> files;
        file.findChildFiles(files, 
                           juce::File::findFilesAndDirectories, 
                           false);
        
        // Ordenar una sola vez
        std::sort(files.begin(), files.end(), 
                 [](const juce::File& a, const juce::File& b) 
                 {
                     // Directorios primero, luego alfabético
                     if (a.isDirectory() != b.isDirectory())
                         return a.isDirectory();
                     return a.getFileName() < b.getFileName();
                 });
        
        cachedFiles = files;
        lastScanTime = file.getLastModificationTime();
        
        for (auto& f : files)
            addSubItem(new FileTreeItem(f));
    }
}

private:
    juce::Array<juce::File> cachedFiles;
    juce::Time lastScanTime;
```

#### **Cuello de Botella 3: Procesamiento de audio ineficiente**
```cpp
// AudioEngine.cpp línea 25
void AudioEngine::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();  // [OK] Correcto para silencio
    
    // Comentario muestra código ineficiente:
    /*
    for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
    {
        leftChannel[sample] = 0.0f;   // [ERROR] Loop escalar lento
        rightChannel[sample] = 0.0f;
    }
    */
}
```

**Solución optimizada para procesamiento real:**
```cpp
void AudioEngine::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Opción 1: Usar operaciones vectorizadas de JUCE
    bufferToFill.buffer->clear(bufferToFill.startSample, bufferToFill.numSamples);
    
    // Opción 2: Para procesamiento complejo, usar SIMD
    auto* leftChannel = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    auto* rightChannel = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);
    
    // Procesar en bloques de 4 samples (SIMD)
    const int numSamples = bufferToFill.numSamples;
    const int numSimdBlocks = numSamples / 4;
    
    for (int i = 0; i < numSimdBlocks * 4; i += 4)
    {
        // Usar juce::dsp::SIMDRegister para procesamiento vectorizado
        // Hasta 4x más rápido que loops escalares
    }
    
    // Procesar samples restantes
    for (int i = numSimdBlocks * 4; i < numSamples; ++i)
    {
        leftChannel[i] = processSample(leftChannel[i]);
        rightChannel[i] = processSample(rightChannel[i]);
    }
}
```

#### **Cuello de Botella 4: Recálculo innecesario en paint()**
```cpp
// MainComponent.cpp línea 92
void MainComponent::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds();
    juce::ColourGradient gradient(
        juce::Colour(0xff1a1a2e), 0.0f, 0.0f,
        juce::Colour(0xff16213e), 0.0f, static_cast<float>(bounds.getHeight()),
        false
    );  // [ERROR] Se recrea el gradiente en cada repintado
    
    g.setGradientFill(gradient);
    g.fillAll();
}
```

**Solución optimizada:**
```cpp
class MainComponent : public juce::AudioAppComponent
{
private:
    juce::ColourGradient backgroundGradient;
    juce::Rectangle<int> lastBounds;
    
    void resized() override
    {
        // Recalcular gradiente solo cuando cambia el tamaño
        auto bounds = getLocalBounds();
        if (bounds != lastBounds)
        {
            backgroundGradient = juce::ColourGradient(
                juce::Colour(0xff1a1a2e), 0.0f, 0.0f,
                juce::Colour(0xff16213e), 0.0f, static_cast<float>(bounds.getHeight()),
                false
            );
            lastBounds = bounds;
        }
        
        // ... resto del layout
    }
    
    void paint(juce::Graphics& g) override
    {
        g.setGradientFill(backgroundGradient);  // [OK] Reutiliza gradiente precalculado
        g.fillAll();
    }
};
```

---

## 3. SEGURIDAD

### Vulnerabilidades Detectadas

#### **Vulnerabilidad 1: Validación insuficiente de nombres de archivo**
```cpp
// ProjectManager.cpp línea 165
auto filename = windowPtr->getTextEditorContents("filename");
auto newFile = projectRoot.getChildFile(filename + extension);
newFile.create();  // ❌ Sin validación
```

**Riesgos:**
• Path traversal: `filename = "../../etc/passwd"`
• Caracteres inválidos: `filename = "con|aux|nul"` (Windows)
• Nombres demasiado largos: > 255 caracteres

**Solución segura:**
```cpp
bool isValidFilename(const juce::String& filename)
{
    // 1. Verificar longitud
    if (filename.length() == 0 || filename.length() > 255)
        return false;
    
    // 2. Rechazar path traversal
    if (filename.contains("..") || filename.contains("/") || filename.contains("\\"))
        return false;
    
    // 3. Rechazar caracteres peligrosos
    const juce::String dangerousChars = "<>:\"|?*";
    for (auto c : filename.toUTF8())
    {
        if (dangerousChars.containsChar(c))
            return false;
    }
    
    // 4. Rechazar nombres reservados de Windows
    static const juce::StringArray reservedNames = 
        { "CON", "PRN", "AUX", "NUL", "COM1", "LPT1" };
    
    if (reservedNames.contains(filename.toUpperCase()))
        return false;
    
    return true;
}

void ProjectExplorer::createNewFile()
{
    auto w = std::make_unique<juce::AlertWindow>(...);
    // ...
    
    w->enterModalState(true, juce::ModalCallbackFunction::create(
        [this, wShared](int result)
        {
            if (result == 1)
            {
                auto filename = wShared->getTextEditorContents("filename");
                
                // [OK] Validar antes de crear
                if (!isValidFilename(filename))
                {
                    juce::NativeMessageBox::showMessageBoxAsync(
                        juce::MessageBoxIconType::WarningIcon,
                        "Nombre inválido",
                        "El nombre contiene caracteres no permitidos.");
                    return;
                }
                
                // [OK] Usar createSafely con verificacion
                auto newFile = projectRoot.getChildFile(filename + extension);
                
                if (newFile.existsAsFile())
                {
                    juce::NativeMessageBox::showMessageBoxAsync(
                        juce::MessageBoxIconType::WarningIcon,
                        "Archivo existe",
                        "Ya existe un archivo con ese nombre.");
                    return;
                }
                
                auto result = newFile.create();
                if (result.wasOk())
                    refreshTree();
                else
                    handleError(result.getErrorMessage());
            }
        }));
}
```

#### **Vulnerabilidad 2: Eliminación de archivos sin confirmación robusta**
```cpp
// ProjectManager.cpp línea 183
if (result == 1)
{
    selected.deleteFile();  // [ERROR] Elimina sin verificar permisos
    refreshTree();
}
```

**Solución segura:**
```cpp
if (result == 1)
{
    // Verificar que el archivo todavía existe
    if (!selected.exists())
    {
        showError("El archivo ya no existe");
        refreshTree();
        return;
    }
    
    // Verificar que está dentro del proyecto (prevenir path traversal)
    if (!selected.isAChildOf(projectRoot))
    {
        showError("No se pueden eliminar archivos fuera del proyecto");
        return;
    }
    
    // Intentar eliminación con manejo de errores
    if (selected.moveToTrash())  // [OK] Usar papelera es mas seguro
    {
        refreshTree();
    }
    else
    {
        // Si moveToTrash() falla, preguntar si forzar
        showConfirmation("No se pudo mover a la papelera. ¿Eliminar permanentemente?",
            [this, selected]() 
            {
                if (selected.deleteFile())
                    refreshTree();
                else
                    showError("No se pudo eliminar el archivo");
            });
    }
}
```

#### **Vulnerabilidad 3: Buffer overrun potencial en conversiones**
```cpp
// DraggableWidget.cpp línea 90
tree.setProperty("type", static_cast<int>(widgetType), nullptr);
```

**Problema:** Si `WidgetType` es un enum class sin valores definidos, podría causar undefined behavior.

**Solución:**
```cpp
// En el header
enum class WidgetType : int32_t  // [OK] Especificar tipo subyacente
{
    Knob = 0,
    Slider = 1,
    Button = 2,
    Label = 3,
    Waveform = 4,
    Meter = 5,
    Unknown = -1
};

// En fromValueTree
auto typeInt = static_cast<int>(tree.getProperty("type", -1));

// [OK] Validar rango antes de convertir
if (typeInt < 0 || typeInt > static_cast<int>(WidgetType::Meter))
{
    juce::Logger::writeToLog("Widget type inválido: " + juce::String(typeInt));
    return nullptr;
}

auto type = static_cast<WidgetType>(typeInt);
```

#### **Vulnerabilidad 4: Race condition en audio engine**
```cpp
// AudioEngine.h
private:
    double currentSampleRate = 0.0;  // [ERROR] No thread-safe
    int currentBufferSize = 0;
```

**Problema:** `prepareToPlay()` y `getNextAudioBlock()` se llaman desde threads diferentes.

**Solución thread-safe:**
```cpp
#include <atomic>

class AudioEngine
{
private:
    std::atomic<double> currentSampleRate{0.0};
    std::atomic<int> currentBufferSize{0};
    
    // O mejor, usar juce::AudioProcessorValueTreeState para thread-safety
    juce::SpinLock audioLock;  // [OK] Lock ligero para audio
    
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override
    {
        const juce::SpinLock::ScopedLockType lock(audioLock);
        currentSampleRate.store(sampleRate, std::memory_order_release);
        currentBufferSize.store(samplesPerBlockExpected, std::memory_order_release);
    }
    
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override
    {
        const juce::SpinLock::ScopedTryLockType lock(audioLock);
        if (!lock.isLocked())
            return;  // Skip si está reconfigurando
        
        auto sampleRate = currentSampleRate.load(std::memory_order_acquire);
        // Procesar audio...
    }
};
```

---

## 4. EXPLICACION DETALLADA

### Funcionamiento del Sistema de Widgets Arrastrables

**Analogia:** Imagina un tablero de corcho donde puedes pegar notas adhesivas. Cada nota puede:
- Moverse (arrastrarse)
- Cambiar de tamano
- Tener contenido diferente (texto, dibujos)
- Eliminarse con un boton X

#### Linea por linea - DraggableWidget.cpp

```cpp
// Linea 7-15: Constructor
DraggableWidget::DraggableWidget(WidgetType type, const juce::String& name)
    : widgetType(type),      // Tipo: Knob, Slider, Button, etc.
      widgetName(name),      // Nombre único para identificar
      resizer(this, &constrainer),  // Componente que permite redimensionar
      deleteButton("X")      // Botón de eliminar
{
    // ANALOGIA: Estamos preparando nuestra "nota adhesiva"
    // con un marco que permite cambiar su tamaño
    
    constrainer.setMinimumSize(50, 50);  
    // No puede ser mas pequeno que 50x50 pixeles
    // (como una nota adhesiva que tiene un tamano minimo util)
```

```cpp
    addAndMakeVisible(resizer);
    // Hacer visible el componente de redimensionamiento
    // Es como poner pequenos "tiradores" en las esquinas
```

```cpp
    deleteButton.setSize(20, 20);
    deleteButton.onClick = [this] { deleteButtonClicked(); };
    // Crear el boton X de 20x20 pixeles
    // Lambda: cuando se hace clic, ejecuta deleteButtonClicked()
    // ANALOGIA: Es como poner una pequena X en la esquina
```

```cpp
// Linea 28-38: paint() - Dibuja el widget
void DraggableWidget::paint(juce::Graphics& g)
{
    g.setColour(juce::Colour(0xff2d2d44));
    // Color de fondo gris oscuro (0xff es opacidad 100%)
    
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 5.0f);
    // Dibujar un rectangulo con esquinas redondeadas (radio 5px)
    // ANALOGIA: Colorear la nota adhesiva
    
    g.setColour(juce::Colour(0xff00adb5));
    // Color cyan para el borde
    
    g.drawRoundedRectangle(getLocalBounds().toFloat().reduced(1), 5.0f, 2.0f);
    // Dibujar el borde (reducido 1px para que no se solape)
    // Grosor de linea: 2.0f
    
    paintWidget(g);
    // Llamar a la funcion virtual que cada widget sobrescribe
    // ANALOGIA: Cada tipo de nota tiene su contenido unico
}
```

```cpp
// Linea 48-63: mouseDown - Detecta inicio de arrastre
void DraggableWidget::mouseDown(const juce::MouseEvent& e)
{
    auto border = 10;  // Zona de 10px considerada "borde"
    auto bounds = getLocalBounds();
    
    // Verificar si el clic está en el borde
    if (e.x < border || e.x > bounds.getWidth() - border ||
        e.y < border || e.y > bounds.getHeight() - border)
    {
        isResizing = true;
        // Si haces clic cerca del borde, activa modo redimensionar
        // ANALOGIA: Agarrar la esquina de la nota para hacerla mas grande/pequena
    }
    else
    {
        isDragging = true;
        dragger.startDraggingComponent(this, e);
        // Si haces clic en el centro, activa modo arrastrar
        // ComponentDragger maneja el movimiento automaticamente
        // ANALOGIA: Agarrar el centro de la nota para moverla
    }
    
    toFront(true);
    // Traer el widget al frente (z-order)
    // ANALOGIA: Cuando tocas una nota, se pone encima de las demas
}
```

```cpp
// Linea 65-71: mouseDrag - Durante el arrastre
void DraggableWidget::mouseDrag(const juce::MouseEvent& e)
{
    if (isDragging)
    {
        dragger.dragComponent(this, e, nullptr);
        // ComponentDragger mueve el widget siguiendo el mouse
        // nullptr = sin restriccion de area
        // ANALOGIA: Mientras mueves el dedo, la nota se mueve contigo
    }
}
```

```cpp
// Linea 86-98: toValueTree - Serializacion (guardar)
juce::ValueTree DraggableWidget::toValueTree() const
{
    // ValueTree es como un JSON de JUCE
    juce::ValueTree tree("Widget");  // Nodo raíz
    
    // Guardar todas las propiedades importantes
    tree.setProperty("type", static_cast<int>(widgetType), nullptr);
    tree.setProperty("name", widgetName, nullptr);
    tree.setProperty("x", getX(), nullptr);
    tree.setProperty("y", getY(), nullptr);
    tree.setProperty("width", getWidth(), nullptr);
    tree.setProperty("height", getHeight(), nullptr);
    
    // ANALOGIA: Tomar una foto de la nota adhesiva:
    // - Que tipo es
    // - Donde esta ubicada
    // - Que tamano tiene
    
    return tree;
}
```

```cpp
// Linea 100-126: fromValueTree - Deserializacion (cargar)
std::unique_ptr<DraggableWidget> DraggableWidget::fromValueTree(const juce::ValueTree& tree)
{
    // Leer las propiedades guardadas
    auto type = static_cast<WidgetType>(int(tree.getProperty("type")));
    auto name = tree.getProperty("name").toString();
    
    std::unique_ptr<DraggableWidget> widget;
    
    // Factory pattern: crear el widget correcto según el tipo
    switch (type)
    {
        case Knob:
            widget = std::make_unique<DraggableKnob>(name);
            break;
        case Slider:
            widget = std::make_unique<DraggableSlider>(name);
            break;
        // ... más tipos
        default:
            return nullptr;  // Tipo desconocido
    }
    
    // Restaurar posicion y tamano
    widget->setBounds(tree.getProperty("x"),
                     tree.getProperty("y"),
                     tree.getProperty("width"),
                     tree.getProperty("height"));
    
    // ANALOGIA: Recrear la nota adhesiva exactamente como estaba
    // usando la informacion de la foto
    
    return widget;
}
```

### Flujo completo de uso:

1. **Usuario hace clic en boton de paleta** -> `MainComponent::addWidget()`
2. **Se crea un nuevo widget** -> Constructor de `DraggableKnob`, etc.
3. **Usuario arrastra el widget** -> `mouseDown()` -> `mouseDrag()` -> `mouseUp()`
4. **Usuario guarda el layout** -> `toValueTree()` -> Escribe XML al disco
5. **Usuario carga el layout** -> Lee XML -> `fromValueTree()` -> Recrea widgets

---

## 5. GENERAR PRUEBAS UNITARIAS (Google Test)

```cpp
// tests/DraggableWidgetTest.cpp
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../Include/DraggableWidget.h"

using namespace testing;

// Fixture para tests de DraggableWidget
class DraggableWidgetTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Crear un widget de prueba
        widget = std::make_unique<DraggableKnob>("TestKnob");
    }
    
    void TearDown() override
    {
        widget.reset();
    }
    
    std::unique_ptr<DraggableWidget> widget;
};

// Test 1: Constructor inicializa correctamente
TEST_F(DraggableWidgetTest, ConstructorInitializesCorrectly)
{
    EXPECT_EQ(widget->getWidgetName(), "TestKnob");
    EXPECT_EQ(widget->getWidgetType(), DraggableWidget::WidgetType::Knob);
    EXPECT_GE(widget->getWidth(), 50);   // Mínimo tamaño
    EXPECT_GE(widget->getHeight(), 50);
}

// Test 2: Serialización a ValueTree
TEST_F(DraggableWidgetTest, SerializesToValueTreeCorrectly)
{
    widget->setBounds(100, 200, 150, 150);
    
    auto tree = widget->toValueTree();
    
    EXPECT_EQ(tree.getType(), juce::Identifier("Widget"));
    EXPECT_EQ(int(tree.getProperty("type")), 
              static_cast<int>(DraggableWidget::WidgetType::Knob));
    EXPECT_EQ(tree.getProperty("name").toString(), "TestKnob");
    EXPECT_EQ(int(tree.getProperty("x")), 100);
    EXPECT_EQ(int(tree.getProperty("y")), 200);
    EXPECT_EQ(int(tree.getProperty("width")), 150);
    EXPECT_EQ(int(tree.getProperty("height")), 150);
}

// Test 3: Deserialización desde ValueTree
TEST_F(DraggableWidgetTest, DeserializesFromValueTreeCorrectly)
{
    juce::ValueTree tree("Widget");
    tree.setProperty("type", static_cast<int>(DraggableWidget::WidgetType::Slider), nullptr);
    tree.setProperty("name", "TestSlider", nullptr);
    tree.setProperty("x", 50, nullptr);
    tree.setProperty("y", 75, nullptr);
    tree.setProperty("width", 200, nullptr);
    tree.setProperty("height", 100, nullptr);
    
    auto deserializedWidget = DraggableWidget::fromValueTree(tree);
    
    ASSERT_NE(deserializedWidget, nullptr);
    EXPECT_EQ(deserializedWidget->getWidgetName(), "TestSlider");
    EXPECT_EQ(deserializedWidget->getWidgetType(), DraggableWidget::WidgetType::Slider);
    EXPECT_EQ(deserializedWidget->getX(), 50);
    EXPECT_EQ(deserializedWidget->getY(), 75);
    EXPECT_EQ(deserializedWidget->getWidth(), 200);
    EXPECT_EQ(deserializedWidget->getHeight(), 100);
}

// Test 4: Casos límite - ValueTree vacío
TEST_F(DraggableWidgetTest, HandlesEmptyValueTree)
{
    juce::ValueTree emptyTree("Widget");
    auto widget = DraggableWidget::fromValueTree(emptyTree);
    
    // Debe retornar nullptr si faltan propiedades esenciales
    EXPECT_EQ(widget, nullptr);
}

// Test 5: Casos límite - Tipo inválido
TEST_F(DraggableWidgetTest, HandlesInvalidWidgetType)
{
    juce::ValueTree tree("Widget");
    tree.setProperty("type", 999, nullptr);  // Tipo inexistente
    tree.setProperty("name", "Invalid", nullptr);
    
    auto widget = DraggableWidget::fromValueTree(tree);
    
    EXPECT_EQ(widget, nullptr);
}

// Test 6: Tamaño mínimo se respeta
TEST_F(DraggableWidgetTest, EnforcesMinimumSize)
{
    widget->setBounds(0, 0, 10, 10);  // Intentar tamaño menor al mínimo
    
    // El constrainer debería forzar el mínimo
    EXPECT_GE(widget->getWidth(), 50);
    EXPECT_GE(widget->getHeight(), 50);
}

// Test 7: Callback de eliminación se dispara
TEST_F(DraggableWidgetTest, DeleteCallbackIsCalled)
{
    bool callbackCalled = false;
    DraggableWidget* deletedWidget = nullptr;
    
    widget->onDeleteRequested = [&](DraggableWidget* w) {
        callbackCalled = true;
        deletedWidget = w;
    };
    
    // Simular clic en botón de eliminar
    widget->deleteButtonClicked();
    
    EXPECT_TRUE(callbackCalled);
    EXPECT_EQ(deletedWidget, widget.get());
}

// Test 8: Serialización round-trip (ida y vuelta)
TEST_F(DraggableWidgetTest, SerializationRoundTrip)
{
    // Configurar widget original
    widget->setBounds(123, 456, 234, 345);
    
    // Serializar
    auto tree = widget->toValueTree();
    
    // Deserializar
    auto restoredWidget = DraggableWidget::fromValueTree(tree);
    
    // Verificar que es idéntico
    ASSERT_NE(restoredWidget, nullptr);
    EXPECT_EQ(restoredWidget->getWidgetName(), widget->getWidgetName());
    EXPECT_EQ(restoredWidget->getWidgetType(), widget->getWidgetType());
    EXPECT_EQ(restoredWidget->getBounds(), widget->getBounds());
}

// Test 9: Mouse drag actualiza posición
TEST_F(DraggableWidgetTest, MouseDragUpdatesPosition)
{
    widget->setBounds(0, 0, 100, 100);
    
    // Simular mouseDown en el centro
    juce::MouseEvent downEvent(
        juce::MouseInputSource(),
        juce::Point<float>(50, 50),
        juce::ModifierKeys(),
        1.0f,  // pressure
        0.0f, 0.0f,  // tilt
        widget,
        widget,
        juce::Time::getCurrentTime(),
        juce::Point<float>(50, 50),
        juce::Time::getCurrentTime(),
        1,  // clicks
        false  // mouse down
    );
    
    widget->mouseDown(downEvent);
    
    // Simular drag
    juce::MouseEvent dragEvent = downEvent.withNewPosition(juce::Point<float>(150, 150));
    widget->mouseDrag(dragEvent);
    
    // Verificar que se movió
    EXPECT_NE(widget->getX(), 0);
    EXPECT_NE(widget->getY(), 0);
}

// Test 10: Múltiples widgets no colisionan
TEST_F(DraggableWidgetTest, MultipleWidgetsIndependent)
{
    auto widget1 = std::make_unique<DraggableKnob>("Widget1");
    auto widget2 = std::make_unique<DraggableSlider>("Widget2");
    
    widget1->setBounds(0, 0, 100, 100);
    widget2->setBounds(200, 200, 100, 100);
    
    // Modificar widget1 no debe afectar widget2
    widget1->setBounds(50, 50, 150, 150);
    
    EXPECT_EQ(widget2->getX(), 200);
    EXPECT_EQ(widget2->getY(), 200);
}

// Test de rendimiento: Serialización de 1000 widgets
TEST(DraggableWidgetPerformanceTest, SerializesManyWidgetsQuickly)
{
    const int numWidgets = 1000;
    std::vector<std::unique_ptr<DraggableWidget>> widgets;
    
    // Crear widgets
    for (int i = 0; i < numWidgets; ++i)
    {
        widgets.push_back(std::make_unique<DraggableKnob>("Widget" + juce::String(i)));
        widgets.back()->setBounds(i * 10, i * 10, 100, 100);
    }
    
    auto startTime = juce::Time::getMillisecondCounterHiRes();
    
    // Serializar todos
    juce::ValueTree root("Widgets");
    for (const auto& w : widgets)
    {
        root.appendChild(w->toValueTree(), nullptr);
    }
    
    auto endTime = juce::Time::getMillisecondCounterHiRes();
    auto elapsedMs = endTime - startTime;
    
    // Debe completar en menos de 100ms
    EXPECT_LT(elapsedMs, 100.0);
    
    std::cout << "Serialización de " << numWidgets 
              << " widgets: " << elapsedMs << "ms" << std::endl;
}

// Mock para tests de interacción
class MockWidgetListener
{
public:
    MOCK_METHOD(void, onWidgetDeleted, (DraggableWidget*));
    MOCK_METHOD(void, onWidgetMoved, (DraggableWidget*, int x, int y));
};

TEST_F(DraggableWidgetTest, NotifiesListenerOnDelete)
{
    MockWidgetListener listener;
    
    widget->onDeleteRequested = [&](DraggableWidget* w) {
        listener.onWidgetDeleted(w);
    };
    
    EXPECT_CALL(listener, onWidgetDeleted(widget.get())).Times(1);
    
    widget->deleteButtonClicked();
}
```

### CMakeLists.txt para tests

```cmake
# tests/CMakeLists.txt
cmake_minimum_required(VERSION 3.22)

# Encontrar Google Test
find_package(GTest REQUIRED)

# Crear ejecutable de tests
add_executable(CustomDAW_Tests
    DraggableWidgetTest.cpp
    AudioEngineTest.cpp
    ProjectManagerTest.cpp
)

# Vincular con Google Test y el código principal
target_link_libraries(CustomDAW_Tests
    PRIVATE
    GTest::GTest
    GTest::Main
    juce::juce_core
    juce::juce_gui_basics
    # Añadir archivos fuente a testear
)

# Habilitar testing
enable_testing()
add_test(NAME CustomDAW_Tests COMMAND CustomDAW_Tests)
```

---

## 6. DOCUMENTACION DOXYGEN

```cpp
/**
 * @file DraggableWidget.h
 * @brief Sistema de widgets arrastrables y redimensionables para GUI builder
 * @author Custom DAW Team
 * @date 2025-11-25
 * @version 1.0.0
 * 
 * Este archivo define la jerarquía de widgets que pueden ser manipulados
 * visualmente en el constructor de GUI. Soporta drag & drop, redimensionamiento,
 * serialización a XML y eliminación dinámica.
 * 
 * @section usage Uso Básico
 * @code{.cpp}
 * // Crear un widget de perilla
 * auto knob = std::make_unique<DraggableKnob>("Volume");
 * knob->setBounds(100, 100, 120, 120);
 * 
 * // Conectar callback de eliminación
 * knob->onDeleteRequested = [](DraggableWidget* w) {
 *     std::cout << "Widget eliminado: " << w->getWidgetName() << std::endl;
 * };
 * 
 * // Serializar a XML
 * auto tree = knob->toValueTree();
 * auto xml = tree.toXmlString();
 * @endcode
 * 
 * @section architecture Arquitectura
 * - DraggableWidget: Clase base abstracta
 * - DraggableKnob: Perilla rotatoria
 * - DraggableSlider: Deslizador vertical/horizontal
 * - DraggableButton: Botón interactivo
 * - DraggableLabel: Etiqueta de texto
 */

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_data_structures/juce_data_structures.h>

/**
 * @class DraggableWidget
 * @brief Clase base para todos los widgets arrastrables del GUI builder
 * 
 * Esta clase proporciona la funcionalidad común para widgets que pueden:
 * - Arrastrarse con el mouse (drag & drop)
 * - Redimensionarse desde los bordes
 * - Serializarse/deserializarse a ValueTree (XML)
 * - Eliminarse mediante un botón integrado
 * 
 * @details
 * La clase utiliza el patrón Template Method, donde las subclases
 * implementan paintWidget() para personalizar la apariencia visual.
 * 
 * @par Thread Safety
 * Esta clase NO es thread-safe. Debe usarse únicamente desde el
 * Message Thread de JUCE.
 * 
 * @par Ownership
 * Los widgets se gestionan mediante std::unique_ptr. Cuando se elimina
 * un widget, dispara el callback onDeleteRequested para notificar
 * al contenedor padre.
 * 
 * @see DraggableKnob, DraggableSlider, DraggableButton
 */
class DraggableWidget : public juce::Component
{
public:
    /**
     * @enum WidgetType
     * @brief Enumeración de todos los tipos de widgets disponibles
     * 
     * Cada valor corresponde a una subclase concreta de DraggableWidget.
     * Se utiliza para la deserialización desde ValueTree.
     */
    enum class WidgetType : int32_t
    {
        Knob = 0,      ///< Perilla rotatoria (DraggableKnob)
        Slider = 1,    ///< Deslizador lineal (DraggableSlider)
        Button = 2,    ///< Botón pulsable (DraggableButton)
        Label = 3,     ///< Etiqueta de texto (DraggableLabel)
        Waveform = 4,  ///< Visualizador de forma de onda (DraggableWaveform)
        Meter = 5      ///< Medidor de nivel (DraggableMeter)
    };
    
    /**
     * @brief Constructor de la clase base DraggableWidget
     * 
     * Inicializa el widget con un tipo y nombre específicos. Configura
     * los componentes internos necesarios para arrastre, redimensionamiento
     * y eliminación.
     * 
     * @param type Tipo de widget (Knob, Slider, etc.)
     * @param name Nombre único del widget, usado para identificación
     * 
     * @post El widget tiene un tamaño por defecto de 100x100 píxeles
     * @post El tamaño mínimo está configurado en 50x50 píxeles
     * @post El botón de eliminar es visible y funcional
     * 
     * @note El nombre debe ser único dentro del contenedor padre
     * 
     * @code{.cpp}
     * // Crear un widget de perilla llamado "Volume"
     * DraggableWidget widget(WidgetType::Knob, "Volume");
     * @endcode
     */
    DraggableWidget(WidgetType type, const juce::String& name);
    
    /**
     * @brief Destructor virtual para permitir polimorfismo
     * 
     * Limpia todos los recursos asociados al widget, incluyendo
     * el componente de redimensionamiento y el botón de eliminar.
     * 
     * @warning No llamar directamente. Usar std::unique_ptr para
     *          gestión automática de memoria.
     */
    virtual ~DraggableWidget() = default;
    
    /**
     * @brief Renderiza el widget en pantalla
     * 
     * Dibuja el fondo, borde y llama a paintWidget() para que
     * la subclase dibuje su contenido específico.
     * 
     * @param g Contexto gráfico de JUCE para operaciones de dibujo
     * 
     * @note Este método se llama automáticamente por JUCE cuando
     *       el widget necesita repintarse. No llamar manualmente.
     * 
     * @par Rendimiento
     * Este método se ejecuta en el Message Thread y debe completar
     * rápidamente (<16ms) para mantener 60 FPS.
     * 
     * @see paintWidget()
     */
    void paint(juce::Graphics& g) override;
    
    /**
     * @brief Callback cuando cambia el tamaño del widget
     * 
     * Reorganiza los componentes internos (botón de eliminar, resizer)
     * cuando el widget es redimensionado.
     * 
     * @post El botón de eliminar se posiciona en la esquina superior derecha
     * @post El resizer cubre todo el borde del widget
     */
    void resized() override;
    
    /**
     * @brief Maneja el evento de mouse presionado
     * 
     * Determina si el usuario quiere arrastrar (clic en el centro)
     * o redimensionar (clic en el borde).
     * 
     * @param e Evento del mouse con información de posición y botón
     * 
     * @details
     * - Si el clic está dentro de 10px del borde → modo redimensionar
     * - Si el clic está en el centro → modo arrastrar
     * - Trae el widget al frente (z-order)
     * 
     * @see mouseDrag(), mouseUp()
     */
    void mouseDown(const juce::MouseEvent& e) override;
    
    /**
     * @brief Maneja el movimiento del mouse mientras está presionado
     * 
     * Actualiza la posición del widget si está en modo arrastre.
     * El redimensionamiento es manejado automáticamente por ResizableBorderComponent.
     * 
     * @param e Evento del mouse con la nueva posición
     * 
     * @pre mouseDown() debe haber sido llamado primero
     * @post La posición del widget se actualiza en tiempo real
     * 
     * @par Optimización
     * Usa ComponentDragger de JUCE para un arrastre suave y optimizado.
     */
    void mouseDrag(const juce::MouseEvent& e) override;
    
    /**
     * @brief Maneja el evento de mouse liberado
     * 
     * Finaliza las operaciones de arrastre o redimensionamiento.
     * 
     * @param e Evento del mouse (generalmente ignorado)
     * 
     * @post isDragging = false
     * @post isResizing = false
     */
    void mouseUp(const juce::MouseEvent& e) override;
    
    /**
     * @brief Serializa el widget a un ValueTree (estructura de datos de JUCE)
     * 
     * Convierte el estado completo del widget a un formato serializable
     * que puede guardarse en XML, JSON o binario.
     * 
     * @return ValueTree conteniendo todas las propiedades del widget
     * 
     * @note Las subclases pueden sobrescribir para añadir propiedades adicionales
     * 
     * @par Propiedades guardadas
     * - type: Tipo de widget (int)
     * - name: Nombre del widget (String)
     * - x: Posición X (int)
     * - y: Posición Y (int)
     * - width: Ancho (int)
     * - height: Alto (int)
     * 
     * @code{.cpp}
     * auto tree = widget->toValueTree();
     * auto xml = tree.toXmlString();
     * File("widget.xml").replaceWithText(xml);
     * @endcode
     * 
     * @see fromValueTree()
     */
    juce::ValueTree toValueTree() const;
    
    /**
     * @brief Deserializa un widget desde un ValueTree
     * 
     * Crea una instancia del widget apropiado leyendo el ValueTree.
     * Utiliza el Factory Pattern para construir el tipo correcto.
     * 
     * @param tree ValueTree conteniendo los datos del widget
     * @return std::unique_ptr al widget creado, o nullptr si falla
     * 
     * @retval nullptr Si el tipo de widget es inválido
     * @retval nullptr Si faltan propiedades requeridas
     * @retval std::unique_ptr<DraggableWidget> Si la deserialización es exitosa
     * 
     * @par Validación
     * El método valida que:
     * - El ValueTree tenga el identificador correcto
     * - El tipo de widget esté en el rango válido
     * - Todas las propiedades requeridas existan
     * 
     * @code{.cpp}
     * auto xml = File("widget.xml").loadFileAsString();
     * auto tree = juce::ValueTree::fromXml(xml);
     * auto widget = DraggableWidget::fromValueTree(tree);
     * 
     * if (widget)
     *     addAndMakeVisible(widget.get());
     * @endcode
     * 
     * @see toValueTree()
     */
    static std::unique_ptr<DraggableWidget> fromValueTree(const juce::ValueTree& tree);
    
    /**
     * @brief Obtiene el tipo de widget
     * @return WidgetType (Knob, Slider, Button, etc.)
     * @note Este método es inline y muy eficiente
     */
    WidgetType getWidgetType() const { return widgetType; }
    
    /**
     * @brief Obtiene el nombre del widget
     * @return Referencia const al nombre (String)
     * @note Devuelve referencia para evitar copia
     */
    const juce::String& getWidgetName() const { return widgetName; }
    
    /**
     * @brief Callback invocado cuando el usuario hace clic en eliminar
     * 
     * Debe ser asignado por el contenedor padre para manejar la eliminación.
     * 
     * @par Ejemplo de uso
     * @code{.cpp}
     * widget->onDeleteRequested = [this](DraggableWidget* w) {
     *     widgets.removeObject(w, true);  // true = eliminar objeto
     *     repaint();
     * };
     * @endcode
     * 
     * @warning El widget NO se elimina automáticamente. El contenedor
     *          padre debe hacerlo explícitamente.
     */
    std::function<void(DraggableWidget*)> onDeleteRequested;

protected:
    /**
     * @brief Método virtual puro para que subclases dibujen su contenido
     * 
     * Las subclases deben implementar este método para renderizar
     * su apariencia específica (perilla, slider, etc.).
     * 
     * @param g Contexto gráfico para dibujar
     * 
     * @note Solo dibujar dentro de getLocalBounds()
     * @note No modificar el estado del widget en este método
     * 
     * @par Ejemplo de implementación
     * @code{.cpp}
     * void DraggableKnob::paintWidget(juce::Graphics& g) override
     * {
     *     auto bounds = getLocalBounds().reduced(20);
     *     
     *     // Dibujar círculo de la perilla
     *     g.setColour(juce::Colours::darkgrey);
     *     g.fillEllipse(bounds.toFloat());
     *     
     *     // Dibujar indicador
     *     // ...
     * }
     * @endcode
     */
    virtual void paintWidget(juce::Graphics& g) = 0;
    
    /**
     * @brief Dispara el callback de eliminación
     * 
     * Llamado internamente cuando se hace clic en el botón X.
     * 
     * @note Método protected para que solo la clase y subclases puedan llamarlo
     */
    void deleteButtonClicked();

private:
    WidgetType widgetType;                          ///< Tipo de widget (Knob, Slider, etc.)
    juce::String widgetName;                        ///< Nombre único del widget
    
    juce::ComponentBoundsConstrainer constrainer;   ///< Limita el tamaño mínimo/máximo
    juce::ResizableBorderComponent resizer;         ///< Componente de redimensionamiento
    juce::ComponentDragger dragger;                 ///< Componente de arrastre
    juce::TextButton deleteButton;                  ///< Botón de eliminar
    
    bool isDragging = false;                        ///< Estado: arrastrando
    bool isResizing = false;                        ///< Estado: redimensionando
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DraggableWidget)
};

/**
 * @class DraggableKnob
 * @brief Widget de perilla rotatoria para controles de parámetros
 * 
 * Implementa una perilla que puede girar 270° para ajustar valores
 * de 0.0 a 1.0. Comúnmente usado para controles de volumen, ganancia,
 * frecuencia, etc.
 * 
 * @par Características
 * - Rotación suave con mouse drag
 * - Rango personalizable
 * - Visualización del valor actual
 * - Etiqueta de nombre integrada
 * 
 * @see DraggableWidget, juce::Slider
 */
class DraggableKnob : public DraggableWidget
{
public:
    /**
     * @brief Constructor de perilla rotatoria
     * 
     * @param name Nombre de la perilla (ejemplo: "Volume", "Cutoff")
     * 
     * @post La perilla tiene un rango de 0.0 a 1.0
     * @post El valor inicial es 0.5 (centro)
     * @post La etiqueta muestra el nombre proporcionado
     */
    explicit DraggableKnob(const juce::String& name);
    
    /**
     * @brief Obtiene el slider interno de JUCE
     * @return Referencia al juce::Slider
     * @note Permite acceso directo para configuración avanzada
     */
    juce::Slider& getSlider() { return knob; }

protected:
    /**
     * @brief Dibuja la perilla rotatoria
     * @param g Contexto gráfico
     */
    void paintWidget(juce::Graphics& g) override;
    
    /**
     * @brief Reorganiza los componentes internos
     */
    void resized() override;

private:
    juce::Slider knob;   ///< Componente slider de JUCE en modo rotatorio
    juce::Label label;   ///< Etiqueta con el nombre de la perilla
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DraggableKnob)
};

// ... (Documentación similar para DraggableSlider, DraggableButton, etc.)

```

---

## RESUMEN EJECUTIVO

### Metricas de Calidad

| Aspecto | Estado | Prioridad de Correccion |
|---------|--------|------------------------|
| **Seguridad** | [WARN] Media | [HIGH] Alta |
| **Rendimiento** | [WARN] Media | [MED] Media |
| **Mantenibilidad** | [OK] Buena | [LOW] Baja |
| **Documentacion** | [WARN] Escasa | [MED] Media |
| **Pruebas** | [ERROR] Ausente | [HIGH] Alta |
| **Estilo C++** | [OK] Bueno | [LOW] Baja |

### Recomendaciones Prioritarias

1. **URGENTE - Seguridad**
   - Implementar validacion de nombres de archivo
   - Usar thread-safe atomics en AudioEngine
   - Capturar shared_ptr en lambdas asincronos

2. **IMPORTANTE - Rendimiento**
   - Cachear strings en paintItem()
   - Implementar cache de arbol de archivos
   - Precalcular gradientes en resized()

3. **RECOMENDADO - Calidad**
   - Eliminar magic numbers
   - Anadir documentacion Doxygen
   - Implementar suite de tests unitarios

### Estimacion de Esfuerzo

- **Correcciones de seguridad:** 4-6 horas
- **Optimizaciones de rendimiento:** 6-8 horas  
- **Documentacion completa:** 8-12 horas
- **Suite de tests:** 12-16 horas

**Total: 30-42 horas** para alcanzar codigo production-ready.
