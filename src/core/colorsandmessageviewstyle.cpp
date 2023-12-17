/*
   SPDX-FileCopyrightText: 2022 Nicolas Fella <nicolas.fella@gmx.de>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "colorsandmessageviewstyle.h"

#include <QApplication>

ColorsAndMessageViewStyle::ColorsAndMessageViewStyle()
    : QObject()
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    connect(qApp, &QApplication::paletteChanged, this, &ColorsAndMessageViewStyle::regenerateColorScheme);
#endif
    regenerateColorScheme();
}

ColorsAndMessageViewStyle &ColorsAndMessageViewStyle::self()
{
    static ColorsAndMessageViewStyle c;
    return c;
}

KColorScheme ColorsAndMessageViewStyle::schemeView() const
{
    return mSchemeView;
}

KColorScheme ColorsAndMessageViewStyle::schemeWindow() const
{
    return mSchemeWindow;
}

void ColorsAndMessageViewStyle::regenerateColorScheme()
{
    mSchemeView = KColorScheme();
    mSchemeWindow = KColorScheme(QPalette::Active, KColorScheme::Window);
    Q_EMIT needToUpdateColors();
}

bool ColorsAndMessageViewStyle::event(QEvent *e)
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    if (e->type() == QEvent::ApplicationPaletteChange) {
        regenerateColorScheme();
    }
#endif
    return QObject::event(e);
}

#include "moc_colorsandmessageviewstyle.cpp"
