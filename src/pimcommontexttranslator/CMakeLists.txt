# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: none
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KF5PimCommonTextTranslator")



#configure_package_config_file(
#    "${CMAKE_CURRENT_SOURCE_DIR}/KF5PimCommonTextTranslatorConfig.cmake.in"
#    "${CMAKE_CURRENT_BINARY_DIR}/KF5PimCommonTextTranslatorConfig.cmake"
#    INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
#    )


#ecm_setup_version(PROJECT VARIABLE_PREFIX PIMCOMMON
#    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/pimcommontexttranslator_version.h"
#    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5PimCommonTextTranslatorConfigVersion.cmake"
#    SOVERSION 5
#    )

#install(FILES
#    "${CMAKE_CURRENT_BINARY_DIR}/KF5PimCommonTextTranslatorConfig.cmake"
#    "${CMAKE_CURRENT_BINARY_DIR}/KF5PimCommonTextTranslatorConfigVersion.cmake"
#    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
#    COMPONENT Devel
#    )

#install(EXPORT KF5PimCommonTextTranslatorTargets DESTINATION "${CMAKECONFIG_INSTALL_DIR}" FILE KF5PimCommonTextTranslatorTargets.cmake NAMESPACE KF5::)

#install(FILES
#    ${CMAKE_CURRENT_BINARY_DIR}/pimcommontexttranslator_version.h
#    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/PimCommonTextTranslator/ COMPONENT Devel
#    )

#if(BUILD_DESIGNERPLUGIN)
#    add_subdirectory(designer)
#endif()

add_library(KF5PimCommonTextTranslator )
add_library(KF5::PimCommonTextTranslator ALIAS KF5PimCommonTextTranslator)
ecm_qt_declare_logging_category(KF5PimCommonTextTranslator HEADER pimcommontexttranslator_debug.h
    IDENTIFIER PIMCOMMONTEXTTRANSLATOR_LOG CATEGORY_NAME org.kde.pim.pimcommontexttranslator
        DESCRIPTION "kdepim (pimcommon TextTranslator)"
        EXPORT PIMCOMMON
    )

target_sources(KF5PimCommonTextTranslator PRIVATE
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
    translator/widgets/translatorconfigureutil.cpp
    translator/widgets/translatorconfigureutil.h
    translator/widgets/translatormenu.cpp
    translator/widgets/translatormenu.h


    translator/networkmanager.cpp
    translator/networkmanager.h
    translator/translatorenginebase.cpp
    translator/translatorenginebase.h
    translator/translatorengineaccessmanager.h
    translator/translatorengineaccessmanager.cpp

    translator/engine/googletranslator.h
    translator/engine/googletranslator.cpp
    translator/engine/bingtranslator.h
    translator/engine/bingtranslator.cpp
    translator/engine/yandextranslator.h
    translator/engine/yandextranslator.cpp
    translator/engine/lingvatranslator.h
    translator/engine/lingvatranslator.cpp
    translator/engine/libretranslatetranslator.h
    translator/engine/libretranslatetranslator.cpp
    translator/engine/deepltranslator.cpp
    translator/engine/deepltranslator.h
    )
if (BUILD_TESTING)
    add_subdirectory(translator/autotests)
    add_subdirectory(translator/tests)
    add_subdirectory(translator/widgets/autotests)
endif()

if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(KF5PimCommonTextTranslator PROPERTIES UNITY_BUILD ON)
endif()
generate_export_header(KF5PimCommonTextTranslator BASE_NAME pimcommontexttranslator)


target_link_libraries(KF5PimCommonTextTranslator
    PUBLIC
    Qt::Gui
    Qt::Widgets
    KF5::KIOCore
    PRIVATE
    Qt::Network
    KF5::I18n
    KF5::WidgetsAddons
    KF5::ItemViews
    KF5::ConfigCore
    )

#target_include_directories(KF5PimCommonTextTranslator INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF}/PimCommonTextTranslator/>")

target_include_directories(KF5PimCommonTextTranslator PUBLIC "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src;${CMAKE_SOURCE_DIR}/src>")

#set_target_properties(KF5PimCommonTextTranslator PROPERTIES
#    VERSION ${PIMCOMMON_VERSION}
#    SOVERSION ${PIMCOMMON_SOVERSION}
#    EXPORT_NAME PimCommonTextTranslator
#    )

set_target_properties(KF5PimCommonTextTranslator
    PROPERTIES OUTPUT_NAME ruqola-texttranslator VERSION ${RUQOLA_LIB_VERSION} SOVERSION ${RUQOLA_LIB_SOVERSION}
)

install(TARGETS KF5PimCommonTextTranslator ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} LIBRARY NAMELINK_SKIP)

#install(TARGETS
#    KF5PimCommonTextTranslator
#    EXPORT KF5PimCommonTextTranslatorTargets ${KF_INSTALL_TARGETS_DEFAULT_ARGS}
#    )

ecm_generate_headers(PimCommonTextTranslator_Camelcasetranslatorwidgets_HEADERS
    HEADER_NAMES
    TranslatorWidget
    TranslatorConfigureDialog
    TranslatorConfigureLanguageListWidget
    TranslatorConfigureListsWidget
    TranslatorMenu
    REQUIRED_HEADERS PimCommonTextTranslator_translatorwidgets_HEADERS
    PREFIX PimCommonTextTranslator
    RELATIVE translator/widgets/
    )

ecm_generate_headers(PimCommonTextTranslator_Camelcasetranslator_HEADERS
    HEADER_NAMES
    TranslatorEngineBase
    REQUIRED_HEADERS PimCommonTextTranslator_translator_HEADERS
    PREFIX PimCommonTextTranslator
    RELATIVE translator
    )

ecm_generate_headers(PimCommonTextTranslator_Camelcasetranslatormisc_HEADERS
    HEADER_NAMES
    TranslatorUtil
    REQUIRED_HEADERS PimCommonTextTranslator_translatormisc_HEADERS
    PREFIX PimCommonTextTranslator
    RELATIVE translator/misc/
    )

#ecm_generate_pri_file(BASE_NAME PimCommonTextTranslator
#    LIB_NAME KF5PimCommonTextTranslator
#    DEPS "" FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR_KF}/PimCommonTextTranslator
#    )

#install(FILES
#    ${CMAKE_CURRENT_BINARY_DIR}/pimcommontexttranslator_version.h
#    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/PimCommonTextTranslator/ COMPONENT Devel
#    )

#install(FILES
#    ${PimCommonTextTranslator_translator_HEADERS}
#    ${PimCommonTextTranslator_Camelcasetranslator_HEADERS}
#    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/PimCommonTextTranslator/PimCommonTextTranslator
#    COMPONENT Devel
#    )

#install(FILES
#    ${CMAKE_CURRENT_BINARY_DIR}/pimcommontexttranslator_export.h
#    ${PimCommonTextTranslator_translator_HEADERS}
#    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/PimCommonTextTranslator/pimcommontexttranslator
#    COMPONENT Devel
#    )

#install(FILES
#    ${PRI_FILENAME}
#    DESTINATION ${ECM_MKSPECS_INSTALL_DIR})


#if (BUILD_QCH)
#    ecm_add_qch(
#        KF5PimCommonTextTranslator_QCH
#        NAME KF5PimCommonTextTranslator
#        BASE_NAME KF5PimCommonTextTranslator
#        VERSION ${PIM_VERSION}
#        ORG_DOMAIN org.kde
#        SOURCES # using only public headers, to cover only public API
#        ${PimCommonTextTranslator_translator_HEADERS}
#        LINK_QCHS
#            Qt5Core_QCH
#            Qt5Gui_QCH
#            Qt5Widgets_QCH
#        INCLUDE_DIRS
#            ${CMAKE_CURRENT_BINARY_DIR}
#        BLANK_MACROS
#            PIMCOMMON_EXPORT
#        TAGFILE_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
#        QCH_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
#        COMPONENT Devel
#    )
#    ecm_install_qch_export(
#        TARGETS KF5PimCommonTextTranslator_QCH
#        FILE KF5PimCommonTextTranslatorQchTargets.cmake
#        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
#        COMPONENT Devel
#    )
#    set(PACKAGE_INCLUDE_QCHTARGETS "include(\"\${CMAKE_CURRENT_LIST_DIR}/KF5PimCommonTextTranslatorQchTargets.cmake\")")
#endif()
