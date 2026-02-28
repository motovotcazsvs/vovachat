cmake_minimum_required(VERSION 3.16)

project(vovachat VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick WebEngineQuick WebChannel TextToSpeech Multimedia)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appvovachat
    main.cpp
)

qt_add_qml_module(appvovachat
    URI vovachat
    VERSION 1.0
    QML_FILES qml/Main.qml
    QML_FILES qml/Chat.qml
    #QML_FILES qml/Settings.qml
    #QML_FILES qml/MainMenu.qml
    #QML_FILES qml/Control.qml
    #SOURCES core/chat.h core/chat.cpp
    #SOURCES core/control.h core/control.cpp
    SOURCES core/speechrecognizer.h core/speechrecognizer.cpp
    SOURCES core/speechsynthesizer.h core/speechsynthesizer.cpp
    #SOURCES core/llm.h core/llm.cpp
    SOURCES core/chatgpt.h core/chatgpt.cpp
    #SOURCES core/gemini.h core/gemini.cpp
    #SOURCES core/grok.h core/grok.cpp
    #SOURCES core/settings.h core/settings.cpp
    #SOURCES core/trayiconmanager.h core/trayiconmanager.cpp
    SOURCES core/recognizervosk.h core/recognizervosk.cpp
    #SOURCES core/recognizergoogle.h core/recognizergoogle.cpp
    #RESOURCES core/SpeechHelper.java
    #SOURCES core/copilot.h core/copilot.cpp
    QML_FILES js/chatgpt.js
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appvovachat PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appvovachat
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Vosk API paths
set(VOSK_DIR ${CMAKE_SOURCE_DIR}/vosk/vosk-win64-0.3.45)
target_include_directories(appvovachat PRIVATE ${VOSK_DIR})

target_link_libraries(appvovachat
    PRIVATE Qt6::Quick Qt6::WebEngineQuick Qt6::WebChannel Qt6::TextToSpeech Qt6::Multimedia
    ${VOSK_DIR}/libvosk.lib
)

include(GNUInstallDirs)
install(TARGETS appvovachat
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
