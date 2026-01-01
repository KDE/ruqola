# SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_subdirectory(webshortcut)
if(TARGET KF6::Purpose)
    add_subdirectory(sharetext)
endif()

if(TARGET KF6::TextAutoGenerateText)
    add_subdirectory(aitext)
endif()
