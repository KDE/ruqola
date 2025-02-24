/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewtext.h"
#include "ruqola_action_buttons_debug.h"
using namespace Qt::Literals::StringLiterals;
AutoGenerateInteractionUiViewText::AutoGenerateInteractionUiViewText() = default;

AutoGenerateInteractionUiViewText::~AutoGenerateInteractionUiViewText() = default;

void AutoGenerateInteractionUiViewText::parse(const QJsonObject &json)
{
    mText = json["text"_L1].toString();
    mEmoji = json["emoji"_L1].toBool();
    mType = convertTypeFromString(json["type"_L1].toString());
}

AutoGenerateInteractionUiViewText::TextType AutoGenerateInteractionUiViewText::convertTypeFromString(const QString &str) const
{
    if (str == "mrkdwn"_L1) {
        return AutoGenerateInteractionUiViewText::TextType::Markdown;
    } else if (str == "mrkdwn"_L1) {
        return AutoGenerateInteractionUiViewText::TextType::PlainText;
    } else {
        qCWarning(RUQOLA_ACTION_BUTTONS_LOG) << "convertTypeFromString Unknown type " << str;
    }
    return AutoGenerateInteractionUiViewText::TextType::Unknown;
}

bool AutoGenerateInteractionUiViewText::operator==(const AutoGenerateInteractionUiViewText &other) const
{
    return other.text() == text() && other.emoji() == emoji() && other.type() == type();
}

bool AutoGenerateInteractionUiViewText::emoji() const
{
    return mEmoji;
}

void AutoGenerateInteractionUiViewText::setEmoji(bool newEmoji)
{
    mEmoji = newEmoji;
}

AutoGenerateInteractionUiViewText::TextType AutoGenerateInteractionUiViewText::type() const
{
    return mType;
}

void AutoGenerateInteractionUiViewText::setType(TextType newType)
{
    mType = newType;
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
    d.space() << "type:" << t.type();
    return d;
}
