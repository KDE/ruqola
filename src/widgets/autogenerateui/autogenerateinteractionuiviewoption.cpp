/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewoption.h"
using namespace Qt::Literals::StringLiterals;
AutoGenerateInteractionUiViewOption::AutoGenerateInteractionUiViewOption()
    : AutoGenerateInteractionUiViewElement()
{
}

AutoGenerateInteractionUiViewOption::~AutoGenerateInteractionUiViewOption()
{
}

void AutoGenerateInteractionUiViewOption::parse(const QJsonObject &json)
{
    mValue = json["value"_L1].toString();
    mUrl = json["url"_L1].toString();
    mText.parse(json["text"_L1].toObject());
    // TODO description
}

QString AutoGenerateInteractionUiViewOption::value() const
{
    return mValue;
}

void AutoGenerateInteractionUiViewOption::setValue(const QString &newValue)
{
    mValue = newValue;
}

QString AutoGenerateInteractionUiViewOption::url() const
{
    return mUrl;
}

void AutoGenerateInteractionUiViewOption::setUrl(const QString &newUrl)
{
    mUrl = newUrl;
}

bool AutoGenerateInteractionUiViewOption::operator==(const AutoGenerateInteractionUiViewOption &other) const
{
    return other.url() == url() && other.value() == value() && other.text() == text();
}

AutoGenerateInteractionUiViewText AutoGenerateInteractionUiViewOption::text() const
{
    return mText;
}

void AutoGenerateInteractionUiViewOption::setText(const AutoGenerateInteractionUiViewText &newText)
{
    mText = newText;
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewOption &t)
{
    d.space() << "value:" << t.value();
    d.space() << "url:" << t.url();
    d.space() << "text:" << t.text();
    return d;
}
