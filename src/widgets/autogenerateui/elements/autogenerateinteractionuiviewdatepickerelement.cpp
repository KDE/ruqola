/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "autogenerateinteractionuiviewdatepickerelement.h"
#include "autogenerateui/autogenerateinteractionuiviewtext.h"

#include <QDateEdit>

using namespace Qt::Literals::StringLiterals;
AutoGenerateInteractionUiViewDatePickerElement::AutoGenerateInteractionUiViewDatePickerElement() = default;

AutoGenerateInteractionUiViewDatePickerElement::~AutoGenerateInteractionUiViewDatePickerElement()
{
    delete mPlaceHolder;
}

bool AutoGenerateInteractionUiViewDatePickerElement::operator==(const AutoGenerateInteractionUiViewDatePickerElement &other) const
{
    return other.initialDate() == initialDate() && AutoGenerateInteractionUiViewActionable::operator==(other);
}

void AutoGenerateInteractionUiViewDatePickerElement::parse(const QJsonObject &json)
{
    AutoGenerateInteractionUiViewActionable::parse(json);
    mInitialDate = json["initialValue"_L1].toString();
    if (json.contains("placeholder"_L1)) {
        mPlaceHolder = new AutoGenerateInteractionUiViewText;
        mPlaceHolder->parse(json["placeholder"_L1].toObject());
    }
    // TODO
}

QWidget *AutoGenerateInteractionUiViewDatePickerElement::generateWidget(QWidget *parent)
{
    auto w = new QDateEdit(parent);
    if (!mInitialDate.isEmpty()) {
        w->setDate(QDate::fromString(mInitialDate, "yyyy-MM-dd"_L1));
    }
    return w;
}

QString AutoGenerateInteractionUiViewDatePickerElement::initialDate() const
{
    return mInitialDate;
}

void AutoGenerateInteractionUiViewDatePickerElement::setInitialDate(const QString &newInitialDate)
{
    mInitialDate = newInitialDate;
}

AutoGenerateInteractionUiViewText *AutoGenerateInteractionUiViewDatePickerElement::placeHolder() const
{
    return mPlaceHolder;
}

void AutoGenerateInteractionUiViewDatePickerElement::setPlaceHolder(AutoGenerateInteractionUiViewText *newPlaceHolder)
{
    mPlaceHolder = newPlaceHolder;
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewDatePickerElement &t)
{
    d.space() << "AutoGenerateInteractionUiViewActionable:" << static_cast<AutoGenerateInteractionUiViewActionable>(t);
    d.space() << "initialDate:" << t.initialDate();
    if (t.placeHolder()) {
        d.space() << "placeHolder:" << *t.placeHolder();
    }
    return d;
}
