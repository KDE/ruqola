/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "autogenerateinteractionuiviewtimepickerelement.h"
#include "autogenerateui/autogenerateinteractionuiviewtext.h"

#include <QDateEdit>

using namespace Qt::Literals::StringLiterals;
AutoGenerateInteractionUiViewTimePickerElement::AutoGenerateInteractionUiViewTimePickerElement() = default;

AutoGenerateInteractionUiViewTimePickerElement::~AutoGenerateInteractionUiViewTimePickerElement()
{
    delete mPlaceHolder;
}

bool AutoGenerateInteractionUiViewTimePickerElement::operator==(const AutoGenerateInteractionUiViewTimePickerElement &other) const
{
    return other.initialTime() == initialTime() && AutoGenerateInteractionUiViewActionable::operator==(other);
}

void AutoGenerateInteractionUiViewTimePickerElement::parse(const QJsonObject &json)
{
    AutoGenerateInteractionUiViewActionable::parse(json);
    mInitialTime = json["initialTime"_L1].toString();
    if (json.contains("placeholder"_L1)) {
        mPlaceHolder = new AutoGenerateInteractionUiViewText;
        mPlaceHolder->parse(json["placeholder"_L1].toObject());
    }
}

QWidget *AutoGenerateInteractionUiViewTimePickerElement::generateWidget(QWidget *parent)
{
    auto w = new QTimeEdit(parent);
    if (!mInitialTime.isEmpty()) {
        w->setTime(QTime::fromString(mInitialTime)); // TODO verify ?
    }
    return w;
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

void AutoGenerateInteractionUiViewTimePickerElement::serializeElement(QJsonObject &o) const
{
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
