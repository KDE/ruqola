/*
   SPDX-FileCopyrightText: 2022 Nicolas Fella <nicolas.fella@gmx.de>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "colorsandmessageviewstyle.h"

ColorsAndMessageViewStyle::ColorsAndMessageViewStyle()
{
    connect(this, &ColorsAndMessageViewStyle::needToUpdateColors, this, &ColorsAndMessageViewStyle::slotUpdateColors);
    slotUpdateColors();
}

ColorsAndMessageViewStyle &ColorsAndMessageViewStyle::self()
{
    static ColorsAndMessageViewStyle c;
    return c;
}

void ColorsAndMessageViewStyle::slotUpdateColors()
{
    mPositiveText = schemeView().foreground(KColorScheme::PositiveText).color().name();
    mPositiveBackground = schemeView().background(KColorScheme::PositiveBackground).color().name();
    mNeutralText = schemeView().foreground(KColorScheme::NeutralText).color().name();
    mNeutralBackground = schemeView().background(KColorScheme::NeutralBackground).color().name();
    mNegativeText = schemeView().foreground(KColorScheme::NegativeText).color().name();
    mNegativeBackground = schemeView().background(KColorScheme::NegativeBackground).color().name();
    const QColor alternateBackgroundColor = schemeView().background(KColorScheme::AlternateBackground).color();
    mDarkTheme = alternateBackgroundColor.lightness() < 128;
    mAlternateBackground = alternateBackgroundColor.name();
    mInactiveText = schemeView().foreground(KColorScheme::InactiveText).color().name();
}

bool ColorsAndMessageViewStyle::darkTheme() const
{
    return mDarkTheme;
}

const QString &ColorsAndMessageViewStyle::inactiveText() const
{
    return mInactiveText;
}

const QString &ColorsAndMessageViewStyle::alternateBackground() const
{
    return mAlternateBackground;
}

const QString &ColorsAndMessageViewStyle::negativeBackground() const
{
    return mNegativeBackground;
}

const QString &ColorsAndMessageViewStyle::negativeText() const
{
    return mNegativeText;
}

const QString &ColorsAndMessageViewStyle::neutralBackground() const
{
    return mNeutralBackground;
}

const QString &ColorsAndMessageViewStyle::neutralText() const
{
    return mNeutralText;
}

const QString &ColorsAndMessageViewStyle::positiveBackground() const
{
    return mPositiveBackground;
}

const QString &ColorsAndMessageViewStyle::positiveText() const
{
    return mPositiveText;
}

#include "moc_colorsandmessageviewstyle.cpp"
