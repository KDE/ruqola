# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause
########### CMake Config Files ###########
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KF5PimTextEditTextToSpeech")

#ecm_setup_version(PROJECT VARIABLE_PREFIX KPIMTEXTEDITTEXTTOSPEECH
#    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kpimtextedittexttospeech_version.h"
#    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5PimTextEditTextToSpeechConfigVersion.cmake"
#    SOVERSION 5
#)

add_library(KF5PimTextEditTextToSpeech)
add_library(KF5::PimTextEditTextToSpeech ALIAS KF5PimTextEditTextToSpeech)

if (KPIMTEXTEDIT_TEXT_TO_SPEECH)
    target_sources(KF5PimTextEditTextToSpeech PRIVATE
        texttospeech.cpp
        texttospeechwidget.cpp
        texttospeechconfigwidget.cpp
        texttospeechinterface.cpp
        abstracttexttospeechinterface.cpp
        abstracttexttospeechconfiginterface.cpp
        texttospeechconfiginterface.cpp
        texttospeechconfigdialog.cpp
        texttospeechlanguagecombobox.cpp
        texttospeechactions.cpp
        texttospeech.h
        texttospeechwidget.h
        texttospeechconfigwidget.h
        texttospeechinterface.h
        abstracttexttospeechinterface.h
        abstracttexttospeechconfiginterface.h
        texttospeechconfiginterface.h
        texttospeechconfigdialog.h
        texttospeechlanguagecombobox.h
        texttospeechactions.h
        texttospeechsliderwidget.h
        texttospeechsliderwidget.cpp
        texttospeechutil.h
        texttospeechutil.cpp	
    )
endif()

if (KPIMTEXTEDIT_TEXT_TO_SPEECH)
    target_link_libraries(KF5PimTextEditTextToSpeech
        PRIVATE
        KF5::WidgetsAddons
        KF5::ConfigWidgets
        KF5::XmlGui
        KF5::I18n
        Qt::TextToSpeech
        )
endif()

ecm_qt_declare_logging_category(KF5PimTextEditTextToSpeech HEADER kpimtextedittexttospeech_debug.h
    IDENTIFIER KPIMTEXTEDITTEXTTOSPEECH_LOG CATEGORY_NAME org.kde.pim.kpimtextedit.texttospeech
        DESCRIPTION "kpimtextedit (kpimtextedit text to speech)" EXPORT KPIMTEXTEDIT)

if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(KF5PimTextEditTextToSpeech PROPERTIES UNITY_BUILD ON)
endif()
generate_export_header(KF5PimTextEditTextToSpeech BASE_NAME kpimtextedittexttospeech)

#target_include_directories(KF5PimTextEditTextToSpeech INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF}/KPIMTextEditTextToSpeech;>")
target_include_directories(KF5PimTextEditTextToSpeech PUBLIC "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>")


target_link_libraries(KF5PimTextEditTextToSpeech
    PRIVATE
    KF5::I18n
    )

set_target_properties(KF5PimTextEditTextToSpeech
    PROPERTIES OUTPUT_NAME ruqola-texttospeech VERSION ${RUQOLA_LIB_VERSION} SOVERSION ${RUQOLA_LIB_SOVERSION}
)

install(TARGETS KF5PimTextEditTextToSpeech ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} LIBRARY NAMELINK_SKIP)


#set_target_properties(KF5PimTextEditTextToSpeech PROPERTIES
#    VERSION ${KPIMTEXTEDITTEXTTOSPEECH_VERSION}
#    SOVERSION ${KPIMTEXTEDITTEXTTOSPEECH_SOVERSION}
#    EXPORT_NAME PimTextEditTextToSpeech
#    )

#install(TARGETS KF5PimTextEditTextToSpeech EXPORT KF5PimTextEditTextToSpeechTargets ${KF_INSTALL_TARGETS_DEFAULT_ARGS})

if (KPIMTEXTEDIT_TEXT_TO_SPEECH)
    if(BUILD_TESTING)
        add_subdirectory(autotests)
        add_subdirectory(tests)
    endif()
endif()

if (KPIMTEXTEDIT_TEXT_TO_SPEECH)
    ecm_generate_headers(KPimTextEdit_CamelCasetexttospeechs_HEADERS
        HEADER_NAMES
        TextToSpeech
        TextToSpeechActions
        TextToSpeechInterface
        TextToSpeechWidget
        TextToSpeechConfigWidget
        TextToSpeechConfigDialog
        AbstractTextToSpeechInterface
        AbstractTextToSpeechConfigInterface
        REQUIRED_HEADERS KPimTextEdit_texttospeechs_HEADERS
        PREFIX KPIMTextEditTextToSpeech
    )
endif()

#if (KPIMTEXTEDIT_TEXT_TO_SPEECH)
#    install(FILES
#        ${KPimTextEdit_texttospeechs_HEADERS}
#        ${CMAKE_CURRENT_BINARY_DIR}/kpimtextedittexttospeech_export.h
#        DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/KPIMTextEditTextToSpeech/kpimtextedittexttospeech
#        COMPONENT Devel
#        )
#    install(FILES
#        ${KPimTextEdit_CamelCasetexttospeechs_HEADERS}
#        DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/KPIMTextEditTextToSpeech/KPIMTextEditTextToSpeech/
#        COMPONENT Devel
#        )
#endif()

#configure_package_config_file(
#    "${CMAKE_CURRENT_SOURCE_DIR}/KF5PimTextEditTextToSpeechConfig.cmake.in"
#    "${CMAKE_CURRENT_BINARY_DIR}/KF5PimTextEditTextToSpeechConfig.cmake"
#    INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR}
#    )

#install(FILES
#    "${CMAKE_CURRENT_BINARY_DIR}/KF5PimTextEditTextToSpeechConfig.cmake"
#    "${CMAKE_CURRENT_BINARY_DIR}/KF5PimTextEditTextToSpeechConfigVersion.cmake"
#    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
#    COMPONENT Devel
#    )

#install(EXPORT KF5PimTextEditTextToSpeechTargets
#    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
#    FILE KF5PimTextEditTextToSpeechTargets.cmake
#    NAMESPACE KF5::
#    )

#install(FILES
#    ${CMAKE_CURRENT_BINARY_DIR}/kpimtextedittexttospeech_version.h
#    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/KPIMTextEditTextToSpeech
#    COMPONENT Devel
#    )


#ecm_generate_pri_file(BASE_NAME KPIMTextEditTextToSpeech LIB_NAME KPIMTextEditTextToSpeech DEPS "" FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR_KF}/KPIMTextEdit/)
#install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})

#if (BUILD_QCH)
#    ecm_add_qch(
#        KF5PimTextEditTextToSpeech_QCH
#        NAME KPimTextEditTextToSpeech
#        BASE_NAME KF5PimTextEditTextToSpeech
#        VERSION ${PIM_VERSION}
#        ORG_DOMAIN org.kde
#        SOURCES # using only public headers, to cover only public API
#            ${KPimTextEdit_texttospeechs_HEADERS}
#        LINK_QCHS
#            Qt5Core_QCH
#            Qt5Gui_QCH
#            Qt5Widgets_QCH
#        INCLUDE_DIRS
#            ${CMAKE_CURRENT_BINARY_DIR}
#        BLANK_MACROS
#            KPIMTEXTEDIT_EXPORT
#        TAGFILE_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
#        QCH_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
#        COMPONENT Devel
#    )
#    ecm_install_qch_export(
#        TARGETS KF5PimTextEditTextToSpeech_QCH
#        FILE KF5PimTextEditTextToSpeechQchTargets.cmake
#        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
#        COMPONENT Devel
#    )
#    set(PACKAGE_INCLUDE_QCHTARGETS "include(\"\${CMAKE_CURRENT_LIST_DIR}/KF5PimTextEditTextToSpeechQchTargets.cmake\")")
#endif()
