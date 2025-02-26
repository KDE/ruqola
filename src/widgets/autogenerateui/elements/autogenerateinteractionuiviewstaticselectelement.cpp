/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "autogenerateinteractionuiviewstaticselectelement.h"
#include "autogenerateui/autogenerateinteractionuiviewoption.h"
#include "autogenerateui/autogenerateinteractionuiviewtext.h"

#include "ruqola_action_buttons_debug.h"
#include <QComboBox>
#include <QJsonArray>
using namespace Qt::Literals::StringLiterals;

AutoGenerateInteractionUiViewStaticSelectElement::AutoGenerateInteractionUiViewStaticSelectElement()
    : AutoGenerateInteractionUiViewActionable()
{
}

AutoGenerateInteractionUiViewStaticSelectElement::~AutoGenerateInteractionUiViewStaticSelectElement()
{
    delete mPlaceHolder;
    qDeleteAll(mOptions);
}

void AutoGenerateInteractionUiViewStaticSelectElement::parse(const QJsonObject &json)
{
    AutoGenerateInteractionUiViewActionable::parse(json);
    mInitialValue = json["initialValue"_L1].toString();
    if (json.contains("placeholder"_L1)) {
        mPlaceHolder = new AutoGenerateInteractionUiViewText;
        mPlaceHolder->parse(json["placeholder"_L1].toObject());
    }
    // Options
    const QJsonArray optionsArray = json["options"_L1].toArray();
    for (const auto &opt : optionsArray) {
        AutoGenerateInteractionUiViewOption *option = new AutoGenerateInteractionUiViewOption;
        option->parse(opt.toObject());
        mOptions.append(option);
    }
}

QWidget *AutoGenerateInteractionUiViewStaticSelectElement::generateWidget(QWidget *parent)
{
    auto comboBox = new QComboBox(parent);
    for (const auto &r : std::as_const(mOptions)) {
        comboBox->addItem(r->value());
    }
    return comboBox;
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

QList<AutoGenerateInteractionUiViewOption *> AutoGenerateInteractionUiViewStaticSelectElement::options() const
{
    return mOptions;
}

void AutoGenerateInteractionUiViewStaticSelectElement::setOptions(const QList<AutoGenerateInteractionUiViewOption *> &newOptions)
{
    mOptions = newOptions;
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewStaticSelectElement &t)
{
    d.space() << "AutoGenerateInteractionUiViewActionable:" << static_cast<AutoGenerateInteractionUiViewActionable>(t);
    d.space() << "initialValue:" << t.initialValue();
    d.space() << "options:" << t.options();
    return d;
}
