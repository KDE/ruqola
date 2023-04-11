# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause
########### CMake Config Files ###########
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KF${KF_MAJOR_VERSION}TextEmoticonsCore")

ecm_setup_version(PROJECT VARIABLE_PREFIX TEXTEMOTICONSCORE
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/textemoticonscore_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF${KF_MAJOR_VERSION}TextEmoticonsCoreConfigVersion.cmake"
    SOVERSION 1
)

add_library(KF${KF_MAJOR_VERSION}TextEmoticonsCore)
add_library(KF${KF_MAJOR_VERSION}::TextEmoticonsCore ALIAS KF${KF_MAJOR_VERSION}TextEmoticonsCore)

target_sources(KF${KF_MAJOR_VERSION}TextEmoticonsCore PRIVATE
    emoji.qrc
    emoticoncategory.cpp
    emoticoncategory.h
    emoticonunicodeutils.cpp
    emoticonunicodeutils.h
    unicodeemoticon.cpp
    unicodeemoticon.h
    unicodeemoticonmanager.cpp
    unicodeemoticonmanager.h
    unicodeemoticonparser.cpp
    unicodeemoticonparser.h
    emoticonunicodemodel.cpp
    emoticonunicodemodel.h
    emoticonunicodeproxymodel.cpp
    emoticonunicodeproxymodel.h
    emoticonunicodemodelmanager.cpp
    emoticonunicodemodelmanager.h

)

target_link_libraries(KF${KF_MAJOR_VERSION}TextEmoticonsCore
    PRIVATE
    Qt::Widgets
    KF${KF_MAJOR_VERSION}::ConfigCore
)

ecm_qt_declare_logging_category(KF${KF_MAJOR_VERSION}TextEmoticonsCore HEADER textemoticonscore_debug.h
    IDENTIFIER TEXTEMOTICONSCORE_LOG CATEGORY_NAME org.kde.kf${KF_MAJOR_VERSION}.textemoticonscore
    DESCRIPTION "KF${KF_MAJOR_VERSION} (textaddons widgets)" EXPORT KTEXTADDONS)

if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(KF${KF_MAJOR_VERSION}TextEmoticonsCore PROPERTIES UNITY_BUILD ON)
endif()
generate_export_header(KF${KF_MAJOR_VERSION}TextEmoticonsCore BASE_NAME TextEmoticonsCore)

target_include_directories(KF${KF_MAJOR_VERSION}TextEmoticonsCore INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF}/TextEmoticonsCore;>")


target_link_libraries(KF${KF_MAJOR_VERSION}TextEmoticonsCore
    PRIVATE
    KF${KF_MAJOR_VERSION}::I18n
)

set_target_properties(KF${KF_MAJOR_VERSION}TextEmoticonsCore PROPERTIES
    VERSION ${TEXTEMOTICONSCORE_VERSION}
    SOVERSION ${TEXTEMOTICONSCORE_SOVERSION}
    EXPORT_NAME TextEmoticonsCore
)

install(TARGETS KF${KF_MAJOR_VERSION}TextEmoticonsCore EXPORT KF${KF_MAJOR_VERSION}TextEmoticonsCoreTargets ${KF_INSTALL_TARGETS_DEFAULT_ARGS})

if(BUILD_TESTING)
    add_subdirectory(autotests)
endif()

ecm_generate_headers(TextEmoticonsCore_CamelCase_HEADERS
    HEADER_NAMES
    UnicodeEmoticonManager
    UnicodeEmoticon
    EmoticonCategory
    EmoticonUnicodeUtils
    EmoticonUnicodeModel
    EmoticonUnicodeModelManager
    EmoticonUnicodeProxyModel
    REQUIRED_HEADERS TextEmoticonsCore_HEADERS
    PREFIX TextEmoticonsCore
)

install(FILES
    ${TextEmoticonsCore_HEADERS}
    ${CMAKE_CURRENT_BINARY_DIR}/textemoticonscore_export.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/TextEmoticonsCore/textemoticonscore
    COMPONENT Devel
)
install(FILES
    ${TextEmoticonsCore_CamelCase_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/TextEmoticonsCore/TextEmoticonsCore/
    COMPONENT Devel
)
if (BUILD_QCH)
    ecm_install_qch_export(
        TARGETS KF${KF_MAJOR_VERSION}TextEmoticonsCore_QCH
        FILE KF${KF_MAJOR_VERSION}TextEmoticonsCoreQchTargets.cmake
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel
    )
set(PACKAGE_INCLUDE_QCHTARGETS "include(\"\${CMAKE_CURRENT_LIST_DIR}/KF${KF_MAJOR_VERSION}TextEmoticonsCoreQchTargets.cmake\")")
endif()

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/KFTextEmoticonsCoreConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/KF${KF_MAJOR_VERSION}TextEmoticonsCoreConfig.cmake"
    INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR}
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/KF${KF_MAJOR_VERSION}TextEmoticonsCoreConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/KF${KF_MAJOR_VERSION}TextEmoticonsCoreConfigVersion.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    COMPONENT Devel
)

install(EXPORT KF${KF_MAJOR_VERSION}TextEmoticonsCoreTargets
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    FILE KF${KF_MAJOR_VERSION}TextEmoticonsCoreTargets.cmake
    NAMESPACE KF${KF_MAJOR_VERSION}::
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/textemoticonscore_version.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/TextEmoticonsCore
    COMPONENT Devel
)


ecm_generate_pri_file(BASE_NAME textemoticonscore LIB_NAME TextEmoticonsCore DEPS "" FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR
    ${KDE_INSTALL_INCLUDEDIR_KF}/TextEmoticonsCore/)
install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})

if (BUILD_QCH)
    ecm_add_qch(
        KF${KF_MAJOR_VERSION}TextEmoticonsCore_QCH
        NAME KF${KF_MAJOR_VERSION}TextEmoticonsCore
        BASE_NAME KF${KF_MAJOR_VERSION}TextEmoticonsCore
        VERSION ${TEXTEMOTICONSCORE_VERSION}
        ORG_DOMAIN org.kde
        SOURCES # using only public headers, to cover only public API
        ${TextEmoticonsCore_HEADERS}
        LINK_QCHS
        Qt${QT_MAJOR_VERSION}Core_QCH
        Qt${QT_MAJOR_VERSION}Gui_QCH
        Qt${QT_MAJOR_VERSION}Widgets_QCH
        INCLUDE_DIRS
        ${CMAKE_CURRENT_BINARY_DIR}
        BLANK_MACROS
        TEXTEMOTICONSCore_EXPORT
        TAGFILE_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        QCH_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        COMPONENT Devel
    )
endif()
