/*
   SPDX-FileCopyrightText: 2022 Nicolas Fella <nicolas.fella@gmx.de>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "colors.h"

#include <QApplication>

Colors::Colors()
    : QObject()
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    connect(qApp, &QApplication::paletteChanged, this, [this] {
        mScheme = KColorScheme(QPalette::Active, KColorScheme::View);
    });
#endif
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

bool Colors::event(QEvent *e)
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    if (e->type() == QEvent::ApplicationPaletteChange) {
        mScheme = KColorScheme();
    }
#endif
    return QObject::event(e);
}
