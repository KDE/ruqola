set(ruqola_texttospeechtextplugin_SRCS
    texttospeechtextinterface.cpp
    texttospeechtextplugin.cpp
    )

kcoreaddons_add_plugin(ruqola_texttospeechtextplugin JSON ruqola_texttospeechtextplugin.json SOURCES
    ${ruqola_texttospeechtextplugin_SRCS} INSTALL_NAMESPACE ruqolaplugins/textplugins)

target_link_libraries(ruqola_texttospeechtextplugin
    libruqolawidgets
    Qt5::TextToSpeech
    )
if (BUILD_TESTING)
    add_subdirectory(autotests)
endif()
