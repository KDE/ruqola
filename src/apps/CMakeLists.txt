if (Qt5QuickCompiler_FOUND AND NOT DISABLE_QTQUICK_COMPILER)
    qtquick_compiler_add_resources(RuqolaResources qml/qml.qrc)
else ()
    qt5_add_resources(RuqolaResources qml/qml.qrc)
endif()

add_executable(ruqola main.cpp ${RuqolaResources})

target_link_libraries(ruqola libruqolacore
    librocketchatrestapi-qt5
    KF5::Crash
)

install(TARGETS ruqola ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
