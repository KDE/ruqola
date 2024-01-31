# SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_subdirectory(webshortcut)
if (TARGET KF6::Purpose)
    add_subdirectory(sharetext)
endif()
if (TARGET KLLMWidgets)
    add_subdirectory(ollama)
endif()
