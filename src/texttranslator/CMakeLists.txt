# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: none
#add_definitions(-DTRANSLATION_DOMAIN=\"libtexttranslator\")
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KF5TextTranslator")

#configure_package_config_file(
#    "${CMAKE_CURRENT_SOURCE_DIR}/KF5TextTranslatorConfig.cmake.in"
#    "${CMAKE_CURRENT_BINARY_DIR}/KF5TextTranslatorConfig.cmake"
#    INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
#    )


#ecm_setup_version(PROJECT VARIABLE_PREFIX TEXTTRANSLATE
#    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/texttranslator_version.h"
#    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5TextTranslatorConfigVersion.cmake"
#    SOVERSION 5
#    )

#install(FILES
#    "${CMAKE_CURRENT_BINARY_DIR}/KF5TextTranslatorConfig.cmake"
#    "${CMAKE_CURRENT_BINARY_DIR}/KF5TextTranslatorConfigVersion.cmake"
#    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
#    COMPONENT Devel
#    )

#install(EXPORT KF5TextTranslatorTargets DESTINATION "${CMAKECONFIG_INSTALL_DIR}" FILE KF5TextTranslatorTargets.cmake NAMESPACE KF5::)

#install(FILES
#    ${CMAKE_CURRENT_BINARY_DIR}/texttranslator_version.h
#    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/TextTranslator/ COMPONENT Devel
#    )

#if(BUILD_DESIGNERPLUGIN)
#    add_subdirectory(designer)
#endif()

add_library(KF5TextTranslator )
add_library(KF5::TextTranslator ALIAS KF5TextTranslator)
ecm_qt_declare_logging_category(KF5TextTranslator HEADER texttranslator_debug.h
    IDENTIFIER TEXTTRANSLATOR_LOG CATEGORY_NAME org.kde.kf5.texttranslator
        DESCRIPTION "kf5 (TextTranslator)"
        EXPORT KTEXTADDONS
    )

target_sources(KF5TextTranslator PRIVATE
    translator/misc/translatorutil.cpp
    translator/misc/translatorutil.h

    translator/widgets/translatorwidget.cpp
    translator/widgets/translatorwidget.h
    translator/widgets/translatordebugdialog.cpp
    translator/widgets/translatordebugdialog.h
    translator/widgets/translatorconfigurewidget.cpp
    translator/widgets/translatorconfigurewidget.h
    translator/widgets/translatorconfiguredialog.h
    translator/widgets/translatorconfiguredialog.cpp

    translator/widgets/translatorconfigurelistswidget.cpp
    translator/widgets/translatorconfigurelistswidget.h
    translator/widgets/translatorconfigurelanguagelistwidget.cpp
    translator/widgets/translatorconfigurelanguagelistwidget.h
    translator/widgets/translatorconfigurecombowidget.h
    translator/widgets/translatorconfigurecombowidget.cpp

    translator/widgets/translatormenu.cpp
    translator/widgets/translatormenu.h


    translator/networkmanager.cpp
    translator/networkmanager.h
    translator/translatorengineaccessmanager.h
    translator/translatorengineaccessmanager.cpp

    translator/translatorengineplugin.cpp
    translator/translatorengineplugin.h

    translator/translatorengineclient.h
    translator/translatorengineclient.cpp

    translator/translatorengineloader.cpp
    translator/translatorengineloader.h
    )
if (BUILD_TESTING)
    add_subdirectory(translator/autotests)
    add_subdirectory(translator/tests)
    add_subdirectory(translator/widgets/autotests)
endif()
add_subdirectory(translator/plugins)


if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(KF5TextTranslator PROPERTIES UNITY_BUILD ON)
endif()
generate_export_header(KF5TextTranslator BASE_NAME texttranslator)


target_link_libraries(KF5TextTranslator
    PUBLIC
    Qt::Gui
    Qt::Widgets
    Qt::Network
    PRIVATE
    KF5::I18n
    KF5::WidgetsAddons
    KF5::ConfigGui
    )

target_include_directories(KF5TextTranslator INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF}/TextTranslator/>")

set_target_properties(KF5TextTranslator PROPERTIES
    VERSION ${TEXTTRANSLATE_VERSION}
    SOVERSION ${TEXTTRANSLATE_SOVERSION}
    EXPORT_NAME TextTranslator
    )

install(TARGETS
    KF5TextTranslator
    EXPORT KF5TextTranslatorTargets ${KF_INSTALL_TARGETS_DEFAULT_ARGS}
    )

ecm_generate_headers(TextTranslator_Camelcasetranslatorwidgets_HEADERS
    HEADER_NAMES
    TranslatorWidget
    TranslatorConfigureDialog
    TranslatorConfigureLanguageListWidget
    TranslatorConfigureListsWidget
    TranslatorMenu
    REQUIRED_HEADERS TextTranslator_translatorwidgets_HEADERS
    PREFIX TextTranslator
    RELATIVE translator/widgets/
    )

ecm_generate_headers(TextTranslator_Camelcasetranslator_HEADERS
    HEADER_NAMES
    TranslatorEnginePlugin
    TranslatorEngineClient
    TranslatorEngineAccessManager
    TranslatorEngineLoader
    REQUIRED_HEADERS TextTranslator_translator_HEADERS
    PREFIX TextTranslator
    RELATIVE translator
    )

ecm_generate_headers(TextTranslator_Camelcasetranslatormisc_HEADERS
    HEADER_NAMES
    TranslatorUtil
    REQUIRED_HEADERS TextTranslator_translatormisc_HEADERS
    PREFIX TextTranslator
    RELATIVE translator/misc/
    )

#ecm_generate_pri_file(BASE_NAME TextTranslator
#    LIB_NAME KF5TextTranslator
#    DEPS "network" FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR_KF}/TextTranslator
#    )

#install(FILES
#    ${CMAKE_CURRENT_BINARY_DIR}/texttranslator_version.h
#    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/TextTranslator/ COMPONENT Devel
#    )

#install(FILES
#    ${TextTranslator_Camelcasetranslatorwidgets_HEADERS}
#    ${TextTranslator_translatorwidgets_HEADERS}

#    ${TextTranslator_Camelcasetranslatormisc_HEADERS}
#    ${TextTranslator_translatormisc_HEADERS}

#    ${TextTranslator_translator_HEADERS}
#    ${TextTranslator_Camelcasetranslator_HEADERS}
#    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/TextTranslator/TextTranslator
#    COMPONENT Devel
#    )

#install(FILES
#    ${CMAKE_CURRENT_BINARY_DIR}/texttranslator_export.h
#    ${TextTranslator_translator_HEADERS}
#    ${TextTranslator_translatorwidgets_HEADERS}
#    ${TextTranslator_translatormisc_HEADERS}
#    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/TextTranslator/texttranslator
#    COMPONENT Devel
#    )

#install(FILES
#    ${PRI_FILENAME}
#    DESTINATION ${ECM_MKSPECS_INSTALL_DIR})


#if (BUILD_QCH)
#    ecm_add_qch(
#        KF5TextTranslator_QCH
#        NAME KF5TextTranslator
#        BASE_NAME KF5TextTranslator
#        VERSION ${TEXTTRANSLATE_VERSION}
#        ORG_DOMAIN org.kde
#        SOURCES # using only public headers, to cover only public API
#        ${TextTranslator_translator_HEADERS}
#        LINK_QCHS
#            Qt5Core_QCH
#            Qt5Gui_QCH
#            Qt5Widgets_QCH
#        INCLUDE_DIRS
#            ${CMAKE_CURRENT_BINARY_DIR}
#        BLANK_MACROS
#            TEXTTRANSLATE_EXPORT
#        TAGFILE_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
#        QCH_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
#        COMPONENT Devel
#    )
#    ecm_install_qch_export(
#        TARGETS KF5TextTranslator_QCH
#        FILE KF5TextTranslatorQchTargets.cmake
#        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
#        COMPONENT Devel
#    )
#    set(PACKAGE_INCLUDE_QCHTARGETS "include(\"\${CMAKE_CURRENT_LIST_DIR}/KF5TextTranslatorQchTargets.cmake\")")
#endif()
