# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: none
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KF5PimCommonAutoCorrection")

#if (BUILD_QCH)
#    ecm_install_qch_export(
#        TARGETS KF5PimCommonAutoCorrection_QCH
#        FILE KF5PimCommonAutoCorrectionQchTargets.cmake
#        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
#        COMPONENT Devel
#    )
#    set(PACKAGE_INCLUDE_QCHTARGETS "include(\"\${CMAKE_CURRENT_LIST_DIR}/KF5PimCommonAutoCorrectionQchTargets.cmake\")")
#endif()

#configure_package_config_file(
#    "${CMAKE_CURRENT_SOURCE_DIR}/KF5PimCommonAutoCorrectionConfig.cmake.in"
#    "${CMAKE_CURRENT_BINARY_DIR}/KF5PimCommonAutoCorrectionConfig.cmake"
#    INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
#    )


#ecm_setup_version(PROJECT VARIABLE_PREFIX PIMCOMMON
#    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/pimcommonautocorrection_version.h"
#    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5PimCommonAutoCorrectionConfigVersion.cmake"
#    SOVERSION 5
#    )

#install(FILES
#    "${CMAKE_CURRENT_BINARY_DIR}/KF5PimCommonAutoCorrectionConfig.cmake"
#    "${CMAKE_CURRENT_BINARY_DIR}/KF5PimCommonAutoCorrectionConfigVersion.cmake"
#    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
#    COMPONENT Devel
#    )

#install(EXPORT KF5PimCommonAutoCorrectionTargets DESTINATION "${CMAKECONFIG_INSTALL_DIR}" FILE KF5PimCommonAutoCorrectionTargets.cmake NAMESPACE KF5::)

#install(FILES
#    ${CMAKE_CURRENT_BINARY_DIR}/pimcommonautocorrection_version.h
#    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/PimCommonAutoCorrection/ COMPONENT Devel
#    )

kconfig_add_kcfg_files(pimcommonautocorrection_settings_SRCS
    settings/pimcommonautocorrectionsetting_base.kcfgc
    )

add_library(KF5PimCommonAutoCorrection )
add_library(KF5::PimCommonAutoCorrection ALIAS KF5PimCommonAutoCorrection)
ki18n_wrap_ui(KF5PimCommonAutoCorrection
    autocorrection/ui/autocorrectionwidget.ui
    )
ecm_qt_declare_logging_category(KF5PimCommonAutoCorrection HEADER pimcommonautocorrection_debug.h IDENTIFIER PIMCOMMONAUTOCORRECTION_LOG CATEGORY_NAME org.kde.pim.pimcommonautocorrection
        DESCRIPTION "kdepim (pimcommon AutoCorrection)"
        EXPORT PIMCOMMON
    )

target_sources(KF5PimCommonAutoCorrection PRIVATE
    ${pimcommonautocorrection_settings_SRCS}
    autocorrection/autocorrection.cpp
    autocorrection/autocorrection.h
    autocorrection/autocorrectionutils.h
    autocorrection/autocorrectionutils.cpp
    autocorrection/autocorrectionsettings.h
    autocorrection/autocorrectionsettings.cpp

    autocorrection/import/importlibreofficeautocorrection.cpp
    autocorrection/import/importkmailautocorrection.cpp
    autocorrection/import/importabstractautocorrection.cpp
    autocorrection/import/importkmailautocorrection.h
    autocorrection/import/importlibreofficeautocorrection.h
    autocorrection/import/importabstractautocorrection.h

    autocorrection/export/exportabstractautocorrection.h
    autocorrection/export/exportabstractautocorrection.cpp
    autocorrection/export/exportlibreofficeautocorrection.h
    autocorrection/export/exportlibreofficeautocorrection.cpp

    autocorrection/widgets/autocorrectionwidget.cpp
    autocorrection/widgets/autocorrectionlistwidget.cpp
    autocorrection/widgets/autocorrectiontreewidget.cpp
    autocorrection/widgets/autocorrectionlanguage.cpp
    autocorrection/widgets/autocorrectionlistwidget.h
    autocorrection/widgets/autocorrectionlanguage.h
    autocorrection/widgets/autocorrectionwidget.h
    autocorrection/widgets/autocorrectiontreewidget.h
    autocorrection/widgets/selectspecialchardialog.h
    autocorrection/widgets/selectspecialchardialog.cpp
    autocorrection/widgets/autocorrectiontextedit.cpp
    autocorrection/widgets/autocorrectiontextedit.h
    autocorrection/widgets/autocorrectionlineedit.cpp
    autocorrection/widgets/autocorrectionlineedit.h
    settings/pimcommonautocorrectionsettings.h
    settings/pimcommonautocorrectionsettings.cpp

    )

if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(KF5PimCommonAutoCorrection PROPERTIES UNITY_BUILD ON)
endif()
generate_export_header(KF5PimCommonAutoCorrection BASE_NAME pimcommonautocorrection)


target_link_libraries(KF5PimCommonAutoCorrection
    PUBLIC
    Qt::Gui
    Qt::Widgets
    KF5::ConfigWidgets
    PRIVATE
    KF5::I18n
    #KF5::Libkdepim
    KF5::Archive
    )

#target_include_directories(KF5PimCommonAutoCorrection INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF}/PimCommonAutoCorrection/>")

target_include_directories(KF5PimCommonAutoCorrection PUBLIC "$<BUILD_INTERFACE:${ruqola_SOURCE_DIR}/src;${ruqola_BINARY_DIR}/src>")

set_target_properties(KF5PimCommonAutoCorrection PROPERTIES
    VERSION ${PIMCOMMON_VERSION}
    SOVERSION ${PIMCOMMON_SOVERSION}
    EXPORT_NAME PimCommonAutoCorrection
    )

install(TARGETS
    KF5PimCommonAutoCorrection
    EXPORT KF5PimCommonAutoCorrectionTargets ${KF_INSTALL_TARGETS_DEFAULT_ARGS}
    )

if (BUILD_TESTING)
    add_subdirectory(autocorrection/autotests)
    add_subdirectory(autocorrection/tests)
endif()


#ecm_generate_headers(PimCommon_Camelcaseautocorrectionwidgets_HEADERS
#    HEADER_NAMES
#    AutoCorrectionWidget
#    AutoCorrectionLanguage
#    SelectSpecialCharDialog
#    AutoCorrectionTextEdit
#    REQUIRED_HEADERS PimCommon_autocorrectionwidgets_HEADERS
#    PREFIX PimCommonAutoCorrection
#    RELATIVE autocorrection/widgets/
#    )

#ecm_generate_headers(PimCommon_Camelcaseautocorrection_HEADERS
#    HEADER_NAMES
#    AutoCorrection
#    AutoCorrectionSettings
#    AutoCorrectionUtils

#    REQUIRED_HEADERS PimCommon_autocorrection_HEADERS
#    PREFIX PimCommonAutoCorrection
#    RELATIVE autocorrection/
#    )

#ecm_generate_headers(PimCommon_Camelcasesettings_HEADERS
#    HEADER_NAMES
#    PimCommonAutoCorrectionSettings
#    REQUIRED_HEADERS PimCommon_settings_HEADERS
#    PREFIX PimCommonAutoCorrection
#    RELATIVE settings
#    )

#ecm_generate_pri_file(BASE_NAME PimCommonAutoCorrection
#    LIB_NAME KF5PimCommon
#    DEPS "PimCommon" FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR_KF}/PimCommonAutoCorrection
#    )


#install(FILES
#    ${PimCommon_Camelcaseautocorrection_HEADERS}
#    ${PimCommon_Camelcaseautocorrectionwidgets_HEADERS}
#    ${PimCommon_Camelcasesettings_HEADERS}
#    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/PimCommonAutoCorrection/PimCommonAutoCorrection
#    COMPONENT Devel
#    )

#install(FILES
#    ${CMAKE_CURRENT_BINARY_DIR}/pimcommonautocorrection_export.h
#    ${CMAKE_CURRENT_BINARY_DIR}/pimcommonautocorrectionsetting_base.h
#    ${PimCommon_HEADERS}
#    ${PimCommon_autocorrection_HEADERS}
#    ${PimCommon_autocorrectionwidgets_HEADERS}
#    ${PimCommon_settings_HEADERS}

#    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/PimCommonAutoCorrection/pimcommonautocorrection
#    COMPONENT Devel
#    )

#install(FILES
#    ${PRI_FILENAME}
#    DESTINATION ${ECM_MKSPECS_INSTALL_DIR})


#if (BUILD_QCH)
#    ecm_add_qch(
#        KF5PimCommonAutoCorrection_QCH
#        NAME KF5PimCommonAutoCorrection
#        BASE_NAME KF5PimCommonAutoCorrection
#        VERSION ${PIM_VERSION}
#        ORG_DOMAIN org.kde
#        SOURCES # using only public headers, to cover only public API
#        ${PimCommon_autocorrection_HEADERS}
#        ${PimCommon_autocorrectionwidgets_HEADERS}
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
#endif()
