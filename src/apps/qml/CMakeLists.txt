if (Qt5QuickCompiler_FOUND AND NOT DISABLE_QTQUICK_COMPILER)
    qtquick_compiler_add_resources(RuqolaResources qml/qml.qrc)
else ()
    qt5_add_resources(RuqolaResources qml/qml.qrc)
endif()

# Add QML file as sources so they are available in QtCreator searches
file(GLOB ruqola_qml_files */*.qml)

add_executable(ruqolaqml main.cpp ${RuqolaResources} ${ruqola_qml_files})

target_link_libraries(ruqolaqml libruqolacore
    librocketchatrestapi-qt5
    KF5::Crash
)

install(TARGETS ruqolaqml ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
