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
    connect(qApp, &QApplication::paletteChanged, this, &Colors::regenerateColorScheme);
#endif
    regenerateColorScheme();
}

Colors &Colors::self()
{
    static Colors c;
    return c;
}

KColorScheme Colors::schemeView() const
{
    return mSchemeView;
}

KColorScheme Colors::schemeWindow() const
{
    return mSchemeWindow;
}

void Colors::regenerateColorScheme()
{
    mSchemeView = KColorScheme();
    mSchemeWindow = KColorScheme(QPalette::Active, KColorScheme::Window);
    Q_EMIT needToUpdateColors();
}

bool Colors::event(QEvent *e)
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    if (e->type() == QEvent::ApplicationPaletteChange) {
        regenerateColorScheme();
    }
#endif
    return QObject::event(e);
}

#include "moc_colors.cpp"
