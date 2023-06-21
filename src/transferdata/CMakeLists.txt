# SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

add_executable(ruqola-transferdata main.cpp)

target_link_libraries(ruqola-transferdata
    KF${KF_MAJOR_VERSION}::XmlGui
    KF${KF_MAJOR_VERSION}::CoreAddons
    KF${KF_MAJOR_VERSION}::Crash
    KF${KF_MAJOR_VERSION}::I18n
)

install(TARGETS ruqola-transferdata ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
