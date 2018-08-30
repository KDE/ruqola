if (Qt5QuickCompiler_FOUND)
    qtquick_compiler_add_resources(RuqolaResources qml/qml.qrc)
else ()
    qt5_add_resources(RuqolaResources qml/qml.qrc)
endif()

add_executable(ruqola main.cpp ${RuqolaResources})

target_link_libraries(ruqola libruqolacore librestapi)

install(TARGETS ruqola ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
