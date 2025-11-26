# Guia de Personalizacion 100%

## Descripcion General
Este DAW Visual Builder te permite personalizar completamente cada aspecto de tus widgets. Puedes cambiar colores, formas, tamaños y estilos de cualquier elemento.

## Inspector de Propiedades (Panel Derecho)

Cuando seleccionas un widget, el Inspector de Propiedades muestra todas las opciones disponibles:

### 1. Posicion y Tamaño
- **X**: Posicion horizontal (0-2000 px)
- **Y**: Posicion vertical (0-2000 px)
- **Ancho**: Ancho del widget (50-1000 px)
- **Alto**: Alto del widget (50-1000 px)

### 2. Apariencia

#### Colores (12 opciones predefinidas)
- **Color de Fondo**: Color del fondo del widget
  * Azul Oscuro (#2d2d44)
  * Verde Oscuro (#2d442d)
  * Rojo Oscuro (#442d2d)
  * Morado (#442d44)
  * Azul Claro (#4d4d88)
  * Verde Claro (#4d884d)
  * Rojo Claro (#884d4d)
  * Cyan (#00adb5)
  * Gris Oscuro (#3d3d3d)
  * Gris Claro (#7d7d7d)
  * Blanco
  * Negro

- **Color de Borde**: Color del contorno del widget
- **Color de Knob**: Color del knob/perilla (solo para DraggableKnob)
- **Color de Texto**: Color del texto/etiqueta

#### Dimensiones
- **Ancho de Borde**: Grosor del borde (0.5 - 10.0 px)
- **Radio de Esquina**: Redondeo de esquinas (0 - 20 px)
  * 0 = Esquinas cuadradas
  * 20 = Esquinas muy redondeadas

#### Estilo de Knob (solo para DraggableKnob)
- **Knob Relleno**: Toggle para activar/desactivar el relleno del knob
  * Activado: Knob relleno con color solido
  * Desactivado: Solo contorno del knob

- **Forma de Knob**: Selecciona la forma del knob
  * **Circular**: Knob redondo clasico con indicador lineal
  * **Rectangular**: Knob cuadrado con indicador
  * **Diamante**: Knob en forma de rombo con indicador

### 3. Conexiones de Codigo
- **Variable C++**: Nombre de la variable en codigo
- **Callback**: Tipo de evento (onClick, onValueChanged, onDrag)

## Tipos de Widgets Disponibles

### 1. Knob (Perilla Rotatoria)
- Personalizacion completa de forma (Circular/Rectangular/Diamante)
- 4 colores configurables
- Relleno opcional
- Indicador de posicion

### 2. Slider (Deslizador)
- Vertical u horizontal
- Personalizable en color y tamaño

### 3. Button (Boton)
- Colores y bordes personalizables
- Texto configurable

### 4. Label (Etiqueta)
- Color de texto personalizable
- Tamaño y posicion ajustable

### 5. Waveform (Forma de Onda)
- Visualizacion de audio
- Colores personalizables

### 6. Meter (Medidor)
- Nivel de audio
- Colores personalizables

## Funciones Adicionales

### Grid (Cuadricula)
- **Mostrar Grid**: Toggle para mostrar/ocultar cuadricula
- **Snap to Grid**: Ajustar widgets automaticamente a la cuadricula
- **Tamaño de Grid**: 10-50 px

### Guardar/Cargar
- **Guardar Layout**: Guarda todos los widgets con sus estilos en XML
- **Cargar Layout**: Restaura widgets y personalizaciones desde XML
- **Limpiar Canvas**: Elimina todos los widgets

### Menu Contextual (Click Derecho)
- **Configurar**: Abre el inspector de propiedades
- **Asignar MIDI**: Conecta el widget a un controlador MIDI CC
- **Duplicar**: Crea una copia del widget
- **Traer al Frente / Enviar al Fondo**: Controla el orden Z
- **Eliminar**: Borra el widget

### Audio/MIDI
- **Configuracion de Audio**: Selecciona dispositivos de entrada/salida
- **Asignacion MIDI**: 
  * Canal MIDI (1-16)
  * Numero CC (0-127)
  * Asignacion directa a widgets

## Flujo de Trabajo Recomendado

1. **Crear Widget**: Click en el tipo de widget en la paleta izquierda
2. **Posicionar**: Arrastra el widget al canvas
3. **Seleccionar**: Click en el widget para abrir el Inspector
4. **Personalizar Colores**: 
   - Click en "Seleccionar Color" para cada elemento
   - Elige de la paleta de 12 colores
5. **Ajustar Forma** (para Knobs):
   - Selecciona Circular/Rectangular/Diamante
   - Activa/desactiva el relleno
6. **Afinar Detalles**:
   - Ajusta ancho de borde con el slider
   - Cambia radio de esquinas para redondear
7. **Guardar**: Click en "Guardar" para preservar tu diseño

## Ejemplos de Estilos

### Knob Retro
- Forma: Circular
- Color Fondo: Azul Oscuro
- Color Knob: Cyan
- Relleno: Activado
- Borde: 3px
- Esquinas: 10px

### Knob Moderno
- Forma: Rectangular
- Color Fondo: Gris Oscuro
- Color Knob: Blanco
- Relleno: Desactivado
- Borde: 1px
- Esquinas: 2px

### Knob Artistico
- Forma: Diamante
- Color Fondo: Morado
- Color Knob: Cyan
- Relleno: Activado
- Borde: 2px
- Esquinas: 5px

## Shortcuts de Teclado

- **Ctrl+S**: Guardar layout
- **Ctrl+O**: Cargar layout
- **Delete**: Eliminar widget seleccionado
- **Ctrl+D**: Duplicar widget seleccionado

## Persistencia de Datos

Todos los estilos se guardan en formato XML:
```xml
<Widget type="0" name="Knob 1" x="100" y="100" width="80" height="100"
        bgColor="#ff2d2d44" borderColor="#ff00adb5" 
        knobColor="#ff00d4ff" textColor="#ffffffff"
        borderWidth="2.0" cornerRadius="5.0" 
        fillKnob="true" knobStyle="0" value="0.5"/>
```

Esto permite:
- Guardar proyectos completos
- Compartir diseños con otros usuarios
- Versionado de configuraciones
- Backup de estilos personalizados

## Solución de Problemas

**Problema**: Los colores no se actualizan
**Solucion**: Asegurate de seleccionar el widget primero, luego cambiar el color

**Problema**: El knob no muestra la forma correcta
**Solucion**: Cambia la forma en el combo "Forma de Knob" y el widget se repintara automaticamente

**Problema**: Los cambios no se guardan
**Solucion**: Click en "Guardar" en la paleta de widgets antes de cerrar la aplicacion

## Proximas Funciones

- Gradientes personalizados
- Texturas e imagenes de fondo
- Exportacion a codigo C++
- Temas predefinidos
- Mas formas de knob (Hexagonal, Estrella, etc.)
