# SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
########### CMake Config Files ###########
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KF${KF_MAJOR_VERSION}TextEmoticonsWidgets")

ecm_setup_version(PROJECT VARIABLE_PREFIX TEXTEMOTICONSWIDGETS
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/textemoticonswidgets_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF${KF_MAJOR_VERSION}TextEmoticonsWidgetsConfigVersion.cmake"
    SOVERSION 1
)

add_library(KF${KF_MAJOR_VERSION}TextEmoticonsWidgets)
add_library(KF${KF_MAJOR_VERSION}::TextEmoticonsWidgets ALIAS KF${KF_MAJOR_VERSION}TextEmoticonsWidgets)

target_sources(KF${KF_MAJOR_VERSION}TextEmoticonsWidgets PRIVATE
    emoticontexteditselector.cpp
    emoticontexteditselector.h
    emoticoncategorybuttons.cpp
    emoticoncategorybuttons.h
    emoticonlistview.cpp
    emoticonlistview.h
    textemoticonswidgets_private_export.h
    emoticontexteditaction.h
    emoticontexteditaction.cpp
    emoticoncategorybutton.h
    emoticoncategorybutton.cpp
    )

target_link_libraries(KF${KF_MAJOR_VERSION}TextEmoticonsWidgets
    PUBLIC
    KF${KF_MAJOR_VERSION}::TextEmoticonsCore
    KF${KF_MAJOR_VERSION}::XmlGui
    PRIVATE
    Qt::Widgets
)

ecm_qt_declare_logging_category(KF${KF_MAJOR_VERSION}TextEmoticonsWidgets HEADER textemoticonswidgets_debug.h
    IDENTIFIER TEXTEMOTICONSWIDGETS_LOG CATEGORY_NAME org.kde.kf${KF_MAJOR_VERSION}.textemoticonswidgets
    DESCRIPTION "KF${KF_MAJOR_VERSION} (textaddons widgets)" EXPORT KTEXTADDONS)

if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(KF${KF_MAJOR_VERSION}TextEmoticonsWidgets PROPERTIES UNITY_BUILD ON)
endif()
generate_export_header(KF${KF_MAJOR_VERSION}TextEmoticonsWidgets BASE_NAME TextEmoticonsWidgets)

target_include_directories(KF${KF_MAJOR_VERSION}TextEmoticonsWidgets INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF}/TextEmoticonsWidgets;>")


target_link_libraries(KF${KF_MAJOR_VERSION}TextEmoticonsWidgets
    PRIVATE
    KF${KF_MAJOR_VERSION}::I18n
)

set_target_properties(KF${KF_MAJOR_VERSION}TextEmoticonsWidgets PROPERTIES
    VERSION ${TEXTEMOTICONSWIDGETS_VERSION}
    SOVERSION ${TEXTEMOTICONSWIDGETS_SOVERSION}
    EXPORT_NAME TextEmoticonsWidgets
)

install(TARGETS KF${KF_MAJOR_VERSION}TextEmoticonsWidgets EXPORT KF${KF_MAJOR_VERSION}TextEmoticonsWidgetsTargets ${KF_INSTALL_TARGETS_DEFAULT_ARGS})

if(BUILD_TESTING)
    add_subdirectory(autotests)
    add_subdirectory(tests)
endif()

ecm_generate_headers(TextEmoticonsWidgets_CamelCase_HEADERS
    HEADER_NAMES
    EmoticonTextEditSelector
    EmoticonTextEditAction
    REQUIRED_HEADERS TextEmoticonsWidgets_HEADERS
    PREFIX TextEmoticonsWidgets
)

install(FILES
    ${TextEmoticonsWidgets_HEADERS}
    ${CMAKE_CURRENT_BINARY_DIR}/textemoticonswidgets_export.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/TextEmoticonsWidgets/textemoticonswidgets
    COMPONENT Devel
)
install(FILES
    ${TextEmoticonsWidgets_CamelCase_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/TextEmoticonsWidgets/TextEmoticonsWidgets/
    COMPONENT Devel
)
if (BUILD_QCH)
    ecm_install_qch_export(
        TARGETS KF${KF_MAJOR_VERSION}TextEmoticonsWidgets_QCH
        FILE KF${KF_MAJOR_VERSION}TextEmoticonsWidgetsQchTargets.cmake
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel
    )
set(PACKAGE_INCLUDE_QCHTARGETS "include(\"\${CMAKE_CURRENT_LIST_DIR}/KF${KF_MAJOR_VERSION}TextEmoticonsWidgetsQchTargets.cmake\")")
endif()

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/KFTextEmoticonsWidgetsConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/KF${KF_MAJOR_VERSION}TextEmoticonsWidgetsConfig.cmake"
    INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR}
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/KF${KF_MAJOR_VERSION}TextEmoticonsWidgetsConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/KF${KF_MAJOR_VERSION}TextEmoticonsWidgetsConfigVersion.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    COMPONENT Devel
)

install(EXPORT KF${KF_MAJOR_VERSION}TextEmoticonsWidgetsTargets
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    FILE KF${KF_MAJOR_VERSION}TextEmoticonsWidgetsTargets.cmake
    NAMESPACE KF${KF_MAJOR_VERSION}::
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/textemoticonswidgets_version.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/TextEmoticonsWidgets
    COMPONENT Devel
)


ecm_generate_pri_file(BASE_NAME textemoticonswidgets LIB_NAME TextEmoticonsWidgets DEPS "" FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR
    ${KDE_INSTALL_INCLUDEDIR_KF}/TextEmoticonsWidgets/)
install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})

if (BUILD_QCH)
    ecm_add_qch(
        KF${KF_MAJOR_VERSION}TextEmoticonsWidgets_QCH
        NAME KF${KF_MAJOR_VERSION}TextEmoticonsWidgets
        BASE_NAME KF${KF_MAJOR_VERSION}TextEmoticonsWidgets
        VERSION ${TEXTEMOTICONSWIDGETS_VERSION}
        ORG_DOMAIN org.kde
        SOURCES # using only public headers, to cover only public API
        ${TextEmoticonsWidgets_HEADERS}
        LINK_QCHS
        Qt${QT_MAJOR_VERSION}Core_QCH
        Qt${QT_MAJOR_VERSION}Gui_QCH
        Qt${QT_MAJOR_VERSION}Widgets_QCH
        INCLUDE_DIRS
        ${CMAKE_CURRENT_BINARY_DIR}
        BLANK_MACROS
        TEXTEMOTICONSWidgets_EXPORT
        TAGFILE_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        QCH_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        COMPONENT Devel
    )
endif()
