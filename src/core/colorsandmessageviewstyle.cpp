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
#else
    qGuiApp->installEventFilter(this);
#endif
    regenerateColorScheme();
}

#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
bool ColorsAndMessageViewStyle::eventFilter(QObject *obj, QEvent *event)
{
    Q_UNUSED(obj);
    if (event->type() == QEvent::ApplicationPaletteChange) {
        regenerateColorScheme();
    }
    return false;
}
#endif

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
    const KColorScheme colorScheme(QPalette::Active, KColorScheme::Window);
    if (!(mSchemeWindow == colorScheme)) {
        mSchemeView = KColorScheme();
        mSchemeWindow = std::move(colorScheme);
        Q_EMIT needToUpdateColors();
    }
}

#include "moc_colorsandmessageviewstyle.cpp"
