add_subdirectory(password)

if (PLUGINS_AUTHENTICATION_BASED_ON_O2)
    add_subdirectory(google)
endif()
