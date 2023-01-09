# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: none
#add_definitions(-DTRANSLATION_DOMAIN=\"libtextautocorrection\")

install(EXPORT KF5TextAutoCorrectionTargets DESTINATION "${CMAKECONFIG_INSTALL_DIR}" FILE KF5TextAutoCorrectionTargets.cmake NAMESPACE KF5::)

kconfig_add_kcfg_files(textautocorrection_settings_SRCS
    settings/textautocorrectionsetting_base.kcfgc
    )

add_library(KF5TextAutoCorrection )
add_library(KF5::TextAutoCorrection ALIAS KF5TextAutoCorrection)
ki18n_wrap_ui(KF5TextAutoCorrection
    ui/autocorrectionwidget.ui
    )
ecm_qt_declare_logging_category(KF5TextAutoCorrection HEADER textautocorrection_debug.h
	IDENTIFIER TEXTAUTOCORRECTION_LOG CATEGORY_NAME org.kde.kf5.textautocorrection
	DESCRIPTION "KF5 (text AutoCorrection)"
	EXPORT KTEXTADDONS
    )

ecm_qt_declare_logging_category(KF5TextAutoCorrection HEADER textautocorrectionautocorrect_debug.h
	IDENTIFIER TEXTAUTOCORRECTION_AUTOCORRECT_LOG CATEGORY_NAME org.kde.kf5.textautocorrection.autocorrect
	DESCRIPTION "KF5 (AutoCorrection autocorrect text)"
        EXPORT RUQOLA
    )

target_sources(KF5TextAutoCorrection PRIVATE
    ${textautocorrection_settings_SRCS}
    autocorrection.cpp
    autocorrection.h
    autocorrectionutils.h
    autocorrectionutils.cpp
    autocorrectionsettings.h
    autocorrectionsettings.cpp

    import/importlibreofficeautocorrection.cpp
    import/importkmailautocorrection.cpp
    import/importabstractautocorrection.cpp
    import/importkmailautocorrection.h
    import/importlibreofficeautocorrection.h
    import/importabstractautocorrection.h

    export/exportabstractautocorrection.h
    export/exportabstractautocorrection.cpp
    export/exportlibreofficeautocorrection.h
    export/exportlibreofficeautocorrection.cpp

    widgets/autocorrectionwidget.cpp
    widgets/autocorrectionlistwidget.cpp
    widgets/autocorrectiontreewidget.cpp
    widgets/autocorrectionlanguage.cpp
    widgets/autocorrectionlistwidget.h
    widgets/autocorrectionlanguage.h
    widgets/autocorrectionwidget.h
    widgets/autocorrectiontreewidget.h
    widgets/selectspecialchardialog.h
    widgets/selectspecialchardialog.cpp
    widgets/autocorrectiontextedit.cpp
    widgets/autocorrectiontextedit.h
    widgets/autocorrectionlineedit.cpp
    widgets/autocorrectionlineedit.h
    widgets/lineeditcatchreturnkey.h
    widgets/lineeditcatchreturnkey.cpp
    settings/textautocorrectionsettings.h
    settings/textautocorrectionsettings.cpp

    )

if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(KF5TextAutoCorrection PROPERTIES UNITY_BUILD ON)
endif()
generate_export_header(KF5TextAutoCorrection BASE_NAME textautocorrection)


target_link_libraries(KF5TextAutoCorrection
    PUBLIC
    Qt::Gui
    Qt::Widgets
    KF5::ConfigWidgets
    PRIVATE
    KF5::I18n
    KF5::Archive
    )

target_include_directories(KF5TextAutoCorrection INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF}/KTextAutoCorrection/>")

set_target_properties(KF5TextAutoCorrection PROPERTIES
    VERSION ${RUQOLA_LIB_VERSION}
    SOVERSION ${RUQOLA_LIB_SOVERSION}
    EXPORT_NAME TextAutoCorrection
    )

install(TARGETS KF5TextAutoCorrection ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} LIBRARY NAMELINK_SKIP)

install(TARGETS
    KF5TextAutoCorrection
    EXPORT KF5TextAutoCorrectionTargets ${KF_INSTALL_TARGETS_DEFAULT_ARGS}
    )

if (BUILD_TESTING)
    add_subdirectory(autotests)
    add_subdirectory(tests)
endif()


ecm_generate_headers(TextAutoCorrection_Camelcaseautocorrectionwidgets_HEADERS
    HEADER_NAMES
    AutoCorrectionWidget
    AutoCorrectionLanguage
    SelectSpecialCharDialog
    AutoCorrectionTextEdit
    REQUIRED_HEADERS TextAutoCorrection_autocorrectionwidgets_HEADERS
    PREFIX TextAutoCorrection
    RELATIVE widgets/
    )

ecm_generate_headers(TextAutoCorrection_Camelcaseautocorrection_HEADERS
    HEADER_NAMES
    AutoCorrection
    AutoCorrectionSettings
    AutoCorrectionUtils

    REQUIRED_HEADERS TextAutoCorrection_autocorrection_HEADERS
    PREFIX TextAutoCorrection
    )

ecm_generate_headers(TextAutoCorrection_Camelcasesettings_HEADERS
    HEADER_NAMES
    TextAutoCorrectionSettings
    REQUIRED_HEADERS TextAutoCorrection_settings_HEADERS
    PREFIX TextAutoCorrection
    RELATIVE settings
    )

#ecm_generate_pri_file(BASE_NAME TextAutoCorrection
#    LIB_NAME KF5TextAutoCorrection
#    DEPS "KConfigWidgets" FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR_KF}/KTextAutoCorrection
#    )


#install(FILES
#    ${TextAutoCorrection_Camelcaseautocorrection_HEADERS}
#    ${TextAutoCorrection_Camelcaseautocorrectionwidgets_HEADERS}
#    ${TextAutoCorrection_Camelcasesettings_HEADERS}
#    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/KTextAutoCorrection/KTextAutoCorrection
#    COMPONENT Devel
#    )

#install(FILES
#    ${CMAKE_CURRENT_BINARY_DIR}/textautocorrection_export.h
#    ${CMAKE_CURRENT_BINARY_DIR}/textautocorrectionsetting_base.h
#    ${TextAutoCorrection_autocorrection_HEADERS}
#    ${TextAutoCorrection_autocorrectionwidgets_HEADERS}
#    ${TextAutoCorrection_settings_HEADERS}

#    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/KTextAutoCorrection/ktextautocorrection
#    COMPONENT Devel
#    )

#install(FILES
#    ${PRI_FILENAME}
#    DESTINATION ${ECM_MKSPECS_INSTALL_DIR})


#if (BUILD_QCH)
#    ecm_add_qch(
#        KF5TextAutoCorrection_QCH
#        NAME KF5TextAutoCorrection
#        BASE_NAME KF5TextAutoCorrection
#        VERSION ${TEXTAUTOCORRECTION_VERSION}
#        ORG_DOMAIN org.kde
#        SOURCES # using only public headers, to cover only public API
#        ${TextAutoCorrection_autocorrection_HEADERS}
#        ${TextAutoCorrection_autocorrectionwidgets_HEADERS}
#        LINK_QCHS
#            Qt5Core_QCH
#            Qt5Gui_QCH
#            Qt5Widgets_QCH
#        INCLUDE_DIRS
#            ${CMAKE_CURRENT_BINARY_DIR}
#        BLANK_MACROS
#            TEXTAUTOCORRECTION_EXPORT
#        TAGFILE_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
#        QCH_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
#        COMPONENT Devel
#    )
#    ecm_install_qch_export(
#        TARGETS KF5TextAutoCorrection_QCH
#        FILE KF5TextAutoCorrectionQchTargets.cmake
#        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
#        COMPONENT Devel
#    )
#    set(PACKAGE_INCLUDE_QCHTARGETS "include(\"\${CMAKE_CURRENT_LIST_DIR}/KF5TextAutoCorrectionQchTargets.cmake\")")
#endif()
