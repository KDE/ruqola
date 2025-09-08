# SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>
# SPDX-FileCopyrightText: 2025 Andro Ranogajec <ranogaet@gmail.com>
# SPDX-License-Identifier: BSD-3-Clause

add_executable(encryptiontestgui)
target_sources(
    encryptiontestgui
    PRIVATE encryptiontestgui.h encryptiontestgui.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/encryptiontestcli.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/uploaddownloadrsakeypair.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/loginmanager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/envutils.cpp
)

target_link_libraries(encryptiontestgui Qt::Widgets Qt::Gui libruqolacore)
set_target_properties(
    encryptiontestgui
    PROPERTIES DISABLE_PRECOMPILE_HEADERS ON
)

if(USE_E2E_SUPPORT)

add_executable(encryptiontestcli
    ${CMAKE_CURRENT_SOURCE_DIR}/encryptiontestcli.cpp
)
target_include_directories(encryptiontestcli PRIVATE
    ${CMAKE_BINARY_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/src/core/connection
    ${CMAKE_SOURCE_DIR}/src/rocketchatrestapi-qt
    ${CMAKE_SOURCE_DIR}/src/rocketchatrestapi-qt/e2e
    ${CMAKE_SOURCE_DIR}/tests/encryptiontest/
    ${CMAKE_SOURCE_DIR}/src/core/encryption/
)
target_link_libraries(encryptiontestcli
    librocketchatrestapi-qt
    libruqolawidgets
    KF6::Crash
    Qt::Network
    OpenSSL::Crypto
)
endif()
