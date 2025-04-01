# SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_subdirectory(grabscreen)

if(TARGET KF6::TextAutogenerateText)
    add_subdirectory(autogeneratetext)
endif()
