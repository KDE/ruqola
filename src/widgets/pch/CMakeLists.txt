# SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/empty_pch.cpp)
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/empty_pch.cpp "/*empty file*/")
endif()

file(
    WRITE ${CMAKE_CURRENT_BINARY_DIR}/ruqolawidgets_pch_tmp.h
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

# avoid rebuilding if there was no change
execute_process(
    COMMAND
        ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_BINARY_DIR}/ruqolawidgets_pch_tmp.h"
        "${CMAKE_CURRENT_BINARY_DIR}/ruqolawidgets_pch.h"
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
