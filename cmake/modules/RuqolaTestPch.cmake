# SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

# Create a precompiled header target for the autotests of the current directory
# and set RUQOLA_TEST_PCH_TARGET to its name, so that the local add_*_test()
# macro can do:
#     target_precompile_headers(${_name} REUSE_FROM ${RUQOLA_TEST_PCH_TARGET})
#
# The PCH target must live in the *same* directory as the tests reusing it.
# Qt::Test injects QT_TESTCASE_BUILDDIR and QT_TESTCASE_SOURCEDIR, whose values
# depend on the directory, and the compiler silently ignores a PCH built with a
# different value ("not used because QT_TESTCASE_BUILDDIR is defined",
# -Winvalid-pch), which brings us back to compiling every test from scratch.
#
# Call it after the add_definitions() of the directory, otherwise the PCH would
# not see the same preprocessor state as the tests.
macro(ruqola_add_widgets_test_pch)
    if(ENABLE_PCH)
        file(
            RELATIVE_PATH
            _ruqola_pch_dir
            ${Ruqola_SOURCE_DIR}
            ${CMAKE_CURRENT_SOURCE_DIR}
        )
        string(
            REGEX REPLACE "[^A-Za-z0-9]"
            "_"
            RUQOLA_TEST_PCH_TARGET
            "${_ruqola_pch_dir}_pch"
        )
        file(GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/empty_pch.cpp CONTENT "/*empty file*/")
        add_library(${RUQOLA_TEST_PCH_TARGET} STATIC ${CMAKE_CURRENT_BINARY_DIR}/empty_pch.cpp)
        target_link_libraries(
            ${RUQOLA_TEST_PCH_TARGET}
            PRIVATE
                Qt::Test
                Qt::Widgets
                libruqolawidgets
        )
        target_precompile_headers(
            ${RUQOLA_TEST_PCH_TARGET}
            PRIVATE
                <QObject>
                <QSignalSpy>
                <QStandardPaths>
                <QTest>
                <QWidget>
        )
    endif()
endmacro()
