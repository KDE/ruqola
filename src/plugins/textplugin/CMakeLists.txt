# SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_subdirectory(webshortcut)
find_package(Qt${QT_MAJOR_VERSION} OPTIONAL_COMPONENTS TextToSpeech)
if (NOT Qt${QT_MAJOR_VERSION}TextToSpeech_FOUND)
    message(STATUS "Qt${QT_MAJOR_VERSION}TextToSpeech not found, speech feature will be disabled")
else()
    add_subdirectory(texttospeech)
endif()
add_subdirectory(sharetext)
