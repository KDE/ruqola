/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewoption.h"
using namespace Qt::Literals::StringLiterals;
AutoGenerateInteractionUiViewOption::AutoGenerateInteractionUiViewOption()
{
}

AutoGenerateInteractionUiViewOption::~AutoGenerateInteractionUiViewOption()
{
}

void AutoGenerateInteractionUiViewOption::parse(const QJsonObject &json)
{
    mValue = json["value"_L1].toString();
    mUrl = json["url"_L1].toString();
    // TODO
}

QWidget *AutoGenerateInteractionUiViewOption::generateWidget(QWidget *parent)
{
    // TODO
    return nullptr;
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
    return other.url() == url() && other.value() == value();
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewOption &t)
{
    d.space() << "AutoGenerateInteractionUiViewActionable:" << static_cast<AutoGenerateInteractionUiViewActionable>(t);
    d.space() << "value:" << t.value();
    d.space() << "url:" << t.url();
    return d;
}
