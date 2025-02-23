/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "autogenerateinteractionuiviewbuttonelement.h"

AutoGenerateInteractionUiViewButtonElement::AutoGenerateInteractionUiViewButtonElement() = default;
AutoGenerateInteractionUiViewButtonElement::~AutoGenerateInteractionUiViewButtonElement() = default;

bool AutoGenerateInteractionUiViewButtonElement::isValid() const
{
    // TODO
    return false;
}

AutoGenerateInteractionUiViewButtonElement::Style AutoGenerateInteractionUiViewButtonElement::style() const
{
    return mStyle;
}

void AutoGenerateInteractionUiViewButtonElement::setStyle(Style newStyle)
{
    mStyle = newStyle;
}

bool AutoGenerateInteractionUiViewButtonElement::operator==(const AutoGenerateInteractionUiViewButtonElement &other) const
{
    return other.style() == style();
}

AutoGenerateInteractionUiViewText AutoGenerateInteractionUiViewButtonElement::text() const
{
    return mText;
}

void AutoGenerateInteractionUiViewButtonElement::setText(const AutoGenerateInteractionUiViewText &newText)
{
    mText = newText;
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewButtonElement &t)
{
    d.space() << "style:" << t.style();
    d.space() << "text:" << t.text();
    return d;
}
