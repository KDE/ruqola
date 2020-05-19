if (COMPILE_RUQOLA_QML)
    add_subdirectory(qml)
endif()

if (COMPILE_RUQOLA_QML)
    add_subdirectory(qml-plain)
endif()

if (NOT ANDROID)
    add_subdirectory(widget)
endif()
