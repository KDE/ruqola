# SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_subdirectory(webshortcut)
if (TARGET KF${KF_MAJOR_VERSION}::Purpose)
    add_subdirectory(sharetext)
endif()
