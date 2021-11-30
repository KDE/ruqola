#! /usr/bin/env bash
# SPDX-License-Identifier: BSD-3-Clause
# SPDX-FileCopyrightText: 2018 Montel Laurent <montel@kde.org> 

$EXTRACTRC `find . -name "*.ui" -o -name "*.rc"` >> rc.cpp
$XGETTEXT `find . -name "*.cpp"` -o $podir/ruqola.pot
