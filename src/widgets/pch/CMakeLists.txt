# SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

file(GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/empty_pch.cpp CONTENT "/*empty file*/")

file(
    GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/ruqolawidgets_pch.h
    CONTENT
        "#pragma once
#include <QObject>
#include <QString>
#include <QByteArray>
#include <QStringList>
#include <QDebug>
#include <QTest>
#include <QWidget>
"
)

add_library(ruqolawidgetspch_tests STATIC ${CMAKE_CURRENT_BINARY_DIR}/empty_pch.cpp)
target_precompile_headers(ruqolawidgetspch_tests PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/ruqolawidgets_pch.h)
target_link_libraries(
    ruqolawidgetspch_tests
    PRIVATE
        Qt::Core
        Qt::Test
        Qt::Widgets
)
