# SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_executable(serializemessagesgui)
target_sources(
    serializemessagesgui
    PRIVATE
        serializemessagesgui.cpp
        serializemessagesgui.h
        serializemessagewidget.h
        serializemessagewidget.cpp
)

target_link_libraries(serializemessagesgui libruqolawidgets)

set_target_properties(
    serializemessagesgui
    PROPERTIES DISABLE_PRECOMPILE_HEADERS ON
)
