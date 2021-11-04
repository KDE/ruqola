# SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
set(ruqola_texttospeechtextplugin_SRCS
    texttospeechtextinterface.cpp
    texttospeechtextplugin.cpp
    texttospeechtextinterface.h
    texttospeechtextplugin.h
    )

kcoreaddons_add_plugin(ruqola_texttospeechtextplugin JSON ruqola_texttospeechtextplugin.json SOURCES
    ${ruqola_texttospeechtextplugin_SRCS} INSTALL_NAMESPACE ruqolaplugins/textplugins)

target_link_libraries(ruqola_texttospeechtextplugin
    libruqolawidgets
    Qt5::TextToSpeech
    )
if (BUILD_TESTING)
    add_subdirectory(autotests)
endif()
