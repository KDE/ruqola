/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "autogenerateinteractionuiviewdatepickerelement.h"
#include "autogenerateui/autogenerateinteractionuiviewtext.h"
#include "autogenerateui/widgets/actionelementwidget.h"

#include <QDateEdit>
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
AutoGenerateInteractionUiViewDatePickerElement::AutoGenerateInteractionUiViewDatePickerElement(QObject *parent)
    : AutoGenerateInteractionUiViewActionable(parent)
{
}

AutoGenerateInteractionUiViewDatePickerElement::~AutoGenerateInteractionUiViewDatePickerElement()
{
    delete mPlaceHolder;
}

bool AutoGenerateInteractionUiViewDatePickerElement::operator==(const AutoGenerateInteractionUiViewDatePickerElement &other) const
{
    return other.initialDate() == initialDate() && AutoGenerateInteractionUiViewActionable::operator==(other);
}

void AutoGenerateInteractionUiViewDatePickerElement::parseElement(const QJsonObject &json)
{
    mInitialDate = json["initialValue"_L1].toString();
    if (json.contains("placeholder"_L1)) {
        mPlaceHolder = new AutoGenerateInteractionUiViewText;
        mPlaceHolder->parse(json["placeholder"_L1].toObject());
    }
}

ActionElementWidget *AutoGenerateInteractionUiViewDatePickerElement::generateWidget(QWidget *parent)
{
    mDateTime = new QDateEdit(parent);
    mActionElementWidget = new ActionElementWidget(mDateTime, actionId(), parent);
    if (!mInitialDate.isEmpty()) {
        mDateTime->setDate(QDate::fromString(mInitialDate, "yyyy-MM-dd"_L1));
    }
    return mActionElementWidget;
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

QVariant AutoGenerateInteractionUiViewDatePickerElement::currentValue() const
{
    return mDateTime->date().toString("yyyy-MM-dd"_L1);
}

void AutoGenerateInteractionUiViewDatePickerElement::setCurrentValue(const QVariant &v)
{
    mDateTime->setDateTime(QDateTime::fromString(v.toString()));
}

void AutoGenerateInteractionUiViewDatePickerElement::serializeElement(QJsonObject &o) const
{
    if (mPlaceHolder) {
        o["placeholder"_L1] = mPlaceHolder->serialize();
    }
    o["initialValue"_L1] = mInitialDate;
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewDatePickerElement &t)
{
    d.space() << "AutoGenerateInteractionUiViewActionable:" << static_cast<const AutoGenerateInteractionUiViewActionable &>(t);
    d.space() << "initialDate:" << t.initialDate();
    if (t.placeHolder()) {
        d.space() << "placeHolder:" << *t.placeHolder();
    }
    return d;
}

#include "moc_autogenerateinteractionuiviewdatepickerelement.cpp"
