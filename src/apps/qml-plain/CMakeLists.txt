if (Qt5QuickCompiler_FOUND AND NOT DISABLE_QTQUICK_COMPILER)
    qtquick_compiler_add_resources(RuqolaPlainResources qml/qml.qrc)
else ()
    qt5_add_resources(RuqolaPlainResources qml/qml.qrc)
endif()

# if(NOT WIN32)
#     add_compile_options(-fsanitize=address)
#     add_link_options(-fsanitize=address)
# endif()

# Add QML file as sources so they are available in QtCreator searches
file(GLOB ruqolaplain_qml_files */*.qml)

set(Ruqola_qmlplain_SRCS
    main.cpp

    applicationengine.cpp
    applicationengine.h

    controllers/chatscreencontroller.cpp
    controllers/chatscreencontroller.h
    controllers/emojipopupcontroller.cpp
    controllers/emojipopupcontroller.h
    controllers/loginscreencontroller.cpp
    controllers/loginscreencontroller.h
    controllers/screencontroller.cpp
    controllers/screencontroller.h
    controllers/serverselectorcontroller.cpp
    controllers/serverselectorcontroller.h
    controllers/settingscontroller.cpp
    controllers/settingscontroller.h

    models/rocketchaticonsmodel.cpp
    models/rocketchaticonsmodel.h
    models/roomavatarsmodel.cpp
    models/roomavatarsmodel.h

    ${RuqolaPlainResources}
    ${ruqolaplain_qml_files})

ecm_qt_declare_logging_category(Ruqola_qmlplain_SRCS HEADER ruqolaqmlplain_debug.h IDENTIFIER RUQOLAQMLPLAIN_LOG CATEGORY_NAME org.kde.ruqola.qmlplain)

add_executable(ruqolaqmlplain ${Ruqola_qmlplain_SRCS})

target_link_libraries(ruqolaqmlplain
    libruqolacore
    librocketchatrestapi-qt5
    KF5::Crash
    Qt5::QuickPrivate
)

install(TARGETS ruqolaqmlplain ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
