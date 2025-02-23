/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewtext.h"

AutoGenerateInteractionUiViewText::AutoGenerateInteractionUiViewText() = default;

AutoGenerateInteractionUiViewText::~AutoGenerateInteractionUiViewText() = default;

void AutoGenerateInteractionUiViewText::parse(const QJsonObject &json)
{
    // TODO
}

bool AutoGenerateInteractionUiViewText::operator==(const AutoGenerateInteractionUiViewText &other) const
{
    return other.text() == text() && other.emoji() == emoji();
}

bool AutoGenerateInteractionUiViewText::emoji() const
{
    return mEmoji;
}

void AutoGenerateInteractionUiViewText::setEmoji(bool newEmoji)
{
    mEmoji = newEmoji;
}

QString AutoGenerateInteractionUiViewText::text() const
{
    return mText;
}

void AutoGenerateInteractionUiViewText::setText(const QString &newText)
{
    mText = newText;
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewText &t)
{
    d.space() << "text:" << t.text();
    d.space() << "emoji:" << t.emoji();
    return d;
}
