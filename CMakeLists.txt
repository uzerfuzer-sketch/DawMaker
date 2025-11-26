cmake_minimum_required(VERSION 3.22)

project(CustomDAW VERSION 0.1.0)

# Especificar C++17 como mínimo
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Añadir JUCE al proyecto
add_subdirectory(../JUCE ${CMAKE_CURRENT_BINARY_DIR}/JUCE)

# Definir el ejecutable
juce_add_gui_app(CustomDAW
    PRODUCT_NAME "Custom DAW"
    COMPANY_NAME "YourCompany"
    BUNDLE_ID "com.yourcompany.customdaw"
    VERSION 0.1.0
)

# Fuentes del proyecto
target_sources(CustomDAW PRIVATE
    Source/Main.cpp
    Source/MainComponent.cpp
    Source/MainWindow.cpp
    Source/AudioEngine.cpp
    Source/CustomLookAndFeel.cpp
    Source/DraggableWidget.cpp
    Source/DraggableWidgetExtensions.cpp
    Source/WidgetToolbar.cpp
    Source/VisualBuilder.cpp
    Source/ProjectManager.cpp
    Source/PluginExporter.cpp
    Source/ProjectTemplates.cpp
    Source/IntegratedTerminal.cpp
    Source/MidiKeyboardWidget.cpp
    Include/MainComponent.h
    Include/MainWindow.h
    Include/AudioEngine.h
    Include/CustomLookAndFeel.h
    Include/DraggableWidget.h
    Include/AudioMeters.h
    Include/AudioScope.h
    Include/VerticalFader.h
    Include/CustomButtons.h
    Include/CustomTooltip.h
    Include/AudioProcessors.h
    Include/WidgetToolbar.h
    Include/VisualBuilder.h
    Include/ProjectManager.h
    Include/PluginExporter.h
    Include/ProjectTemplates.h
    Include/IntegratedTerminal.h
    Include/MidiKeyboardWidget.h
)

# Directorios de inclusión
target_include_directories(CustomDAW PRIVATE
    Include
)

# Definiciones de compilación
target_compile_definitions(CustomDAW PRIVATE
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:CustomDAW,JUCE_PRODUCT_NAME>"
    JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:CustomDAW,JUCE_VERSION>"
    JUCE_DISPLAY_SPLASH_SCREEN=0
    JUCE_REPORT_APP_USAGE=0
)

# Vincular módulos JUCE necesarios
target_link_libraries(CustomDAW PRIVATE
    juce::juce_audio_basics
    juce::juce_audio_devices
    juce::juce_audio_formats
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_core
    juce::juce_data_structures
    juce::juce_events
    juce::juce_graphics
    juce::juce_gui_basics
    juce::juce_gui_extra
    PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
)
