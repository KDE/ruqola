/*
   SPDX-FileCopyrightText: 2022 Nicolas Fella <nicolas.fella@gmx.de>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "colors.h"

#include <QApplication>

Colors::Colors()
    : QObject()
{
    connect(qApp, &QApplication::paletteChanged, this, [this] {
        mScheme = KColorScheme();
    });
}

Colors &Colors::self()
{
    static Colors c;
    return c;
}

KColorScheme Colors::scheme() const
{
    return mScheme;
}
