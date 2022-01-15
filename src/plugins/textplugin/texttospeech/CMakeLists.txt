# SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

kcoreaddons_add_plugin(ruqola_texttospeechtextplugin JSON ruqola_texttospeechtextplugin.json INSTALL_NAMESPACE ruqolaplugins/textplugins)

target_sources(ruqola_texttospeechtextplugin PRIVATE
    texttospeechtextinterface.cpp
    texttospeechtextplugin.cpp
    texttospeechtextinterface.h
    texttospeechtextplugin.h
    )
target_link_libraries(ruqola_texttospeechtextplugin
    libruqolawidgets
    Qt5::TextToSpeech
    )
if (BUILD_TESTING)
    add_subdirectory(autotests)
endif()
