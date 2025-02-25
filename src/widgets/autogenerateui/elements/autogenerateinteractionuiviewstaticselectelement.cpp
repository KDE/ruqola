/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "autogenerateinteractionuiviewstaticselectelement.h"
#include "autogenerateui/autogenerateinteractionuiviewtext.h"
#include "ruqola_action_buttons_debug.h"
#include <QComboBox>
using namespace Qt::Literals::StringLiterals;

AutoGenerateInteractionUiViewStaticSelectElement::AutoGenerateInteractionUiViewStaticSelectElement()
    : AutoGenerateInteractionUiViewActionable()
{
}

AutoGenerateInteractionUiViewStaticSelectElement::~AutoGenerateInteractionUiViewStaticSelectElement()
{
    delete mPlaceHolder;
}

void AutoGenerateInteractionUiViewStaticSelectElement::parse(const QJsonObject &json)
{
    AutoGenerateInteractionUiViewActionable::parse(json);
    mInitialValue = json["initialValue"_L1].toString();
    if (json.contains("placeholder"_L1)) {
        mPlaceHolder = new AutoGenerateInteractionUiViewText;
        mPlaceHolder->parse(json["placeholder"_L1].toObject());
    }
}

QWidget *AutoGenerateInteractionUiViewStaticSelectElement::generateWidget(QWidget *parent)
{
    // TODO
    return new QComboBox(parent);
}

AutoGenerateInteractionUiViewText *AutoGenerateInteractionUiViewStaticSelectElement::placeHolder() const
{
    return mPlaceHolder;
}

void AutoGenerateInteractionUiViewStaticSelectElement::setPlaceHolder(AutoGenerateInteractionUiViewText *newPlaceHolder)
{
    mPlaceHolder = newPlaceHolder;
}

QString AutoGenerateInteractionUiViewStaticSelectElement::initialValue() const
{
    return mInitialValue;
}

void AutoGenerateInteractionUiViewStaticSelectElement::setInitialValue(const QString &newInitialValue)
{
    mInitialValue = newInitialValue;
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewStaticSelectElement &t)
{
    d.space() << "AutoGenerateInteractionUiViewActionable:" << static_cast<AutoGenerateInteractionUiViewActionable>(t);
    return d;
}
