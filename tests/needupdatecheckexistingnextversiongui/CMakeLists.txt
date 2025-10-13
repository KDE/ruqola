# SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_executable(needupdatecheckexistingnextversiongui)
target_sources(
    needupdatecheckexistingnextversiongui
    PRIVATE
        needupdatecheckexistingnextversiongui.cpp
        needupdatecheckexistingnextversionwidget.h
        needupdatecheckexistingnextversionwidget.cpp
)

target_link_libraries(needupdatecheckexistingnextversiongui libruqolawidgets KF6::TextAddonsWidgets)
set_target_properties(
    needupdatecheckexistingnextversiongui
    PROPERTIES DISABLE_PRECOMPILE_HEADERS ON
)
