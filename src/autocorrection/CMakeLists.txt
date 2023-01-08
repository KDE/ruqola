# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: none
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KF5TextAutoCorrection")


#configure_package_config_file(
#    "${CMAKE_CURRENT_SOURCE_DIR}/KF5TextAutoCorrectionConfig.cmake.in"
#    "${CMAKE_CURRENT_BINARY_DIR}/KF5TextAutoCorrectionConfig.cmake"
#    INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
#    )


#ecm_setup_version(PROJECT VARIABLE_PREFIX TEXTAUTOCORRECTION
#    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/textautocorrection_version.h"
#    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5TextAutoCorrectionConfigVersion.cmake"
#    SOVERSION 5
#    )

#install(FILES
#    "${CMAKE_CURRENT_BINARY_DIR}/KF5TextAutoCorrectionConfig.cmake"
#    "${CMAKE_CURRENT_BINARY_DIR}/KF5TextAutoCorrectionConfigVersion.cmake"
#    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
#    COMPONENT Devel
#    )

add_subdirectory(src)

#install(FILES
#    ${CMAKE_CURRENT_BINARY_DIR}/textautocorrection_version.h
#    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/TextAutoCorrection/ COMPONENT Devel
#    )
