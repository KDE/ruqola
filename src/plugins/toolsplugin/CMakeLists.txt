# SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_subdirectory(grabscreen)

if(TARGET KF6::TextAutoGenerateText)
    add_subdirectory(autogeneratetext)
    add_subdirectory(aiactions)
endif()
