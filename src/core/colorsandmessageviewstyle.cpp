/*
   SPDX-FileCopyrightText: 2022 Nicolas Fella <nicolas.fella@gmx.de>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "colorsandmessageviewstyle.h"

#include <QApplication>

ColorsAndMessageViewStyle::ColorsAndMessageViewStyle()
{
#if !HAVE_TEXTUTILS_COLOR_AND_MESSAGE_VIEW_STYLE_SUPPORT
    regenerateColorScheme();
    qGuiApp->installEventFilter(this);
#endif
}

#if !HAVE_TEXTUTILS_COLOR_AND_MESSAGE_VIEW_STYLE_SUPPORT
bool ColorsAndMessageViewStyle::eventFilter([[maybe_unused]] QObject *obj, QEvent *event)
{
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
#if !HAVE_TEXTUTILS_COLOR_AND_MESSAGE_VIEW_STYLE_SUPPORT
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
        mSchemeWindow = colorScheme;
        Q_EMIT needToUpdateColors();
    }
}
#endif

#include "moc_colorsandmessageviewstyle.cpp"
