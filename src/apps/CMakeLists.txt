if (COMPILE_RUQOLA_QML)
    add_subdirectory(qml)
endif()

if (NOT ANDROID)
    add_subdirectory(widget)
endif()
