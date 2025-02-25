/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewoptionelement.h"
using namespace Qt::Literals::StringLiterals;
AutoGenerateInteractionUiViewOptionElement::AutoGenerateInteractionUiViewOptionElement()
{
}

AutoGenerateInteractionUiViewOptionElement::~AutoGenerateInteractionUiViewOptionElement()
{
}

void AutoGenerateInteractionUiViewOptionElement::parse(const QJsonObject &json)
{
    mValue = json["value"_L1].toString();
    mUrl = json["url"_L1].toString();
    // TODO
}

QWidget *AutoGenerateInteractionUiViewOptionElement::generateWidget(QWidget *parent)
{
    // TODO
    return nullptr;
}

QString AutoGenerateInteractionUiViewOptionElement::value() const
{
    return mValue;
}

void AutoGenerateInteractionUiViewOptionElement::setValue(const QString &newValue)
{
    mValue = newValue;
}

QString AutoGenerateInteractionUiViewOptionElement::url() const
{
    return mUrl;
}

void AutoGenerateInteractionUiViewOptionElement::setUrl(const QString &newUrl)
{
    mUrl = newUrl;
}

bool AutoGenerateInteractionUiViewOptionElement::operator==(const AutoGenerateInteractionUiViewOptionElement &other) const
{
    return other.url() == url() && other.value() == value();
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewOptionElement &t)
{
    d.space() << "AutoGenerateInteractionUiViewActionable:" << static_cast<AutoGenerateInteractionUiViewActionable>(t);
    d.space() << "value:" << t.value();
    d.space() << "url:" << t.url();
    return d;
}
