/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewtext.h"
#include "ruqola_autogenerateui_debug.h"

#include <QJsonObject>
#include <QTextDocument>

using namespace Qt::Literals::StringLiterals;
AutoGenerateInteractionUiViewText::AutoGenerateInteractionUiViewText()

{
}

AutoGenerateInteractionUiViewText::~AutoGenerateInteractionUiViewText() = default;

void AutoGenerateInteractionUiViewText::parse(const QJsonObject &json)
{
    mText = json["text"_L1].toString();
    mEmoji = json["emoji"_L1].toBool();
    mType = convertTypeFromString(json["type"_L1].toString());
}

QString AutoGenerateInteractionUiViewText::convertTextTypeToString() const
{
    switch (mType) {
    case TextType::Unknown:
        return {};
    case TextType::PlainText:
        return "plain_text"_L1;
    case TextType::Markdown:
        return "mrkdwn"_L1;
    }
    return {};
}

AutoGenerateInteractionUiViewText::TextType AutoGenerateInteractionUiViewText::convertTypeFromString(const QString &str) const
{
    if (str.isEmpty()) {
        return AutoGenerateInteractionUiViewText::TextType::Unknown;
    } else if (str == "mrkdwn"_L1) {
        return AutoGenerateInteractionUiViewText::TextType::Markdown;
    } else if (str == "plain_text"_L1) {
        return AutoGenerateInteractionUiViewText::TextType::PlainText;
    } else {
        qCWarning(RUQOLA_AUTOGENERATEUI_LOG) << "convertTypeFromString Unknown type " << str;
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

QString AutoGenerateInteractionUiViewText::generateText(bool forcePlainText) const
{
    switch (mType) {
    case TextType::Unknown:
        qCWarning(RUQOLA_AUTOGENERATEUI_LOG) << "generateText unknown type ";
        return mText;
    case TextType::PlainText:
        return mText;
    case TextType::Markdown: {
        if (forcePlainText) {
            return mText;
        }
        QTextDocument doc;
        doc.setMarkdown(mText);
        return doc.toHtml();
    }
    }
    return {};
}

QJsonObject AutoGenerateInteractionUiViewText::serialize() const
{
    QJsonObject o;
    o["type"_L1] = convertTextTypeToString();
    o["text"_L1] = mText;
    o["emoji"_L1] = mEmoji;
    return o;
}

bool AutoGenerateInteractionUiViewText::isEmpty() const
{
    return mText.isEmpty();
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

#include "moc_autogenerateinteractionuiviewtext.cpp"
