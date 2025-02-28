/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "autogenerateinteractionuiviewmultistaticselectelement.h"
#include "autogenerateui/autogenerateinteractionuiviewoption.h"
#include "autogenerateui/autogenerateinteractionuiviewtext.h"

#include <QJsonArray>
using namespace Qt::Literals::StringLiterals;
AutoGenerateInteractionUiViewMultiStaticSelectElement::AutoGenerateInteractionUiViewMultiStaticSelectElement()
    : AutoGenerateInteractionUiViewActionable()
{
}

AutoGenerateInteractionUiViewMultiStaticSelectElement::~AutoGenerateInteractionUiViewMultiStaticSelectElement()
{
    delete mPlaceHolder;
    qDeleteAll(mOptions);
}

void AutoGenerateInteractionUiViewMultiStaticSelectElement::parse(const QJsonObject &json)
{
    AutoGenerateInteractionUiViewActionable::parse(json);
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

QWidget *AutoGenerateInteractionUiViewMultiStaticSelectElement::generateWidget(QWidget *parent)
{
    // TODO QListWidget ??
    return nullptr;
}

AutoGenerateInteractionUiViewText *AutoGenerateInteractionUiViewMultiStaticSelectElement::placeHolder() const
{
    return mPlaceHolder;
}

void AutoGenerateInteractionUiViewMultiStaticSelectElement::setPlaceHolder(AutoGenerateInteractionUiViewText *newPlaceHolder)
{
    mPlaceHolder = newPlaceHolder;
}

QList<AutoGenerateInteractionUiViewOption *> AutoGenerateInteractionUiViewMultiStaticSelectElement::options() const
{
    return mOptions;
}

void AutoGenerateInteractionUiViewMultiStaticSelectElement::setOptions(const QList<AutoGenerateInteractionUiViewOption *> &newOptions)
{
    mOptions = newOptions;
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewMultiStaticSelectElement &t)
{
    d.space() << "AutoGenerateInteractionUiViewActionable:" << static_cast<AutoGenerateInteractionUiViewActionable>(t);
    if (t.placeHolder()) {
        d.space() << "placeHolder:" << *t.placeHolder();
    }
    d.space() << "options:" << t.options();
    return d;
}

bool AutoGenerateInteractionUiViewMultiStaticSelectElement::operator==(const AutoGenerateInteractionUiViewMultiStaticSelectElement &other) const
{
    // TODO add placeholder too
    return other.options() == options() && AutoGenerateInteractionUiViewActionable::operator==(other);
}

QJsonObject AutoGenerateInteractionUiViewMultiStaticSelectElement::serialize() const
{
    return {};
}
