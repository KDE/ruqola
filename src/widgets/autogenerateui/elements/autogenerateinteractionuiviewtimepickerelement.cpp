/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "autogenerateinteractionuiviewtimepickerelement.h"
#include "autogenerateui/autogenerateinteractionuiviewtext.h"
#include "autogenerateui/widgets/actionelementwidget.h"

#include <QJsonObject>
#include <QTimeEdit>

using namespace Qt::Literals::StringLiterals;
AutoGenerateInteractionUiViewTimePickerElement::AutoGenerateInteractionUiViewTimePickerElement(QObject *parent)
    : AutoGenerateInteractionUiViewActionable(parent)
{
}

AutoGenerateInteractionUiViewTimePickerElement::~AutoGenerateInteractionUiViewTimePickerElement()
{
    delete mPlaceHolder;
}

bool AutoGenerateInteractionUiViewTimePickerElement::operator==(const AutoGenerateInteractionUiViewTimePickerElement &other) const
{
    return other.initialTime() == initialTime() && AutoGenerateInteractionUiViewActionable::operator==(other);
}

void AutoGenerateInteractionUiViewTimePickerElement::parseElement(const QJsonObject &json)
{
    mInitialTime = json["initialTime"_L1].toString();
    if (json.contains("placeholder"_L1)) {
        mPlaceHolder = new AutoGenerateInteractionUiViewText;
        mPlaceHolder->parse(json["placeholder"_L1].toObject());
    }
}

ActionElementWidget *AutoGenerateInteractionUiViewTimePickerElement::generateWidget(QWidget *parent)
{
    mTimeEdit = new QTimeEdit(parent);
    mActionElementWidget = new ActionElementWidget(mTimeEdit, actionId(), parent);

    if (!mInitialTime.isEmpty()) {
        mTimeEdit->setTime(QTime::fromString(mInitialTime)); // TODO verify ?
    }
    return mActionElementWidget;
}

QString AutoGenerateInteractionUiViewTimePickerElement::initialTime() const
{
    return mInitialTime;
}

void AutoGenerateInteractionUiViewTimePickerElement::setInitialTime(const QString &newInitialDate)
{
    mInitialTime = newInitialDate;
}

AutoGenerateInteractionUiViewText *AutoGenerateInteractionUiViewTimePickerElement::placeHolder() const
{
    return mPlaceHolder;
}

void AutoGenerateInteractionUiViewTimePickerElement::setPlaceHolder(AutoGenerateInteractionUiViewText *newPlaceHolder)
{
    mPlaceHolder = newPlaceHolder;
}

QVariant AutoGenerateInteractionUiViewTimePickerElement::currentValue() const
{
    return mTimeEdit->time().toString();
}

void AutoGenerateInteractionUiViewTimePickerElement::setCurrentValue(const QVariant &v)
{
    mTimeEdit->setTime(QTime::fromString(v.toString()));
}

void AutoGenerateInteractionUiViewTimePickerElement::serializeElement(QJsonObject &o) const
{
    if (mPlaceHolder) {
        o["placeholder"_L1] = mPlaceHolder->serialize();
    }
    o["initialTime"_L1] = mInitialTime;
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewTimePickerElement &t)
{
    d.space() << "AutoGenerateInteractionUiViewActionable:" << static_cast<const AutoGenerateInteractionUiViewActionable &>(t);
    d.space() << "initialTime:" << t.initialTime();
    if (t.placeHolder()) {
        d.space() << "placeHolder:" << *t.placeHolder();
    }
    return d;
}

#include "moc_autogenerateinteractionuiviewtimepickerelement.cpp"
