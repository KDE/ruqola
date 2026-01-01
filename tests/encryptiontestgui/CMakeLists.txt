# SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>
# SPDX-FileCopyrightText: 2025 Andro Ranogajec <ranogaet@gmail.com>
# SPDX-License-Identifier: BSD-3-Clause

set(TEST_SOURCE
    ${CMAKE_CURRENT_SOURCE_DIR}/uploaddownloadrsakeypair.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/loginmanager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/envutils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/uploaddownloadrsakeypair.h
    ${CMAKE_CURRENT_SOURCE_DIR}/loginmanager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/envutils.h
)

add_executable(encryptiontestgui)
target_sources(
    encryptiontestgui
    PRIVATE
        encryptiontestgui.h
        encryptiontestgui.cpp
        ${TEST_SOURCE}
)

target_link_libraries(
    encryptiontestgui
    Qt::Widgets
    Qt::Gui
    libruqolacore
)
set_target_properties(
    encryptiontestgui
    PROPERTIES
        DISABLE_PRECOMPILE_HEADERS
            ON
)

########
add_executable(encryptiontestcli)
target_sources(
    encryptiontestcli
    PRIVATE
        encryptiontestcli.cpp
        ${TEST_SOURCE}
)

target_link_libraries(
    encryptiontestcli
    librocketchatrestapi-qt
    libruqolawidgets
    KF6::Crash
    Qt::Network
    OpenSSL::Crypto
)
