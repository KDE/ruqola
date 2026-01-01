#! /usr/bin/env bash
# SPDX-License-Identifier: BSD-3-Clause
# SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org> 

$EXTRACTRC `find . -name "*.ui" -o -name "*.rc"` >> rc.cpp
$XGETTEXT `find . -name "*.cpp" -o -name "*.h" | grep -v '/cmark-rc/'` -o $podir/ruqola.pot
rm -f rc.cpp
