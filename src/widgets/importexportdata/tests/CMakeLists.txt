# SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
######
add_executable(testimportdatawizard_gui testimportdatawizard_gui.cpp)
target_link_libraries(
    testimportdatawizard_gui
    libruqolawidgets
    KF6::I18n
)

######
add_executable(testexportdatawizard_gui testexportdatawizard_gui.cpp)
target_link_libraries(
    testexportdatawizard_gui
    libruqolawidgets
    KF6::I18n
)
