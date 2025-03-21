/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "autogenerateinteractionuiviewlinearscaleelement.h"
#include "autogenerateui/widgets/actionelementwidget.h"
#include <QSlider>
using namespace Qt::Literals::StringLiterals;
AutoGenerateInteractionUiViewLinearScaleElement::AutoGenerateInteractionUiViewLinearScaleElement(QObject *parent)
    : AutoGenerateInteractionUiViewActionable(parent)
{
}

AutoGenerateInteractionUiViewLinearScaleElement::~AutoGenerateInteractionUiViewLinearScaleElement() = default;

ActionElementWidget *AutoGenerateInteractionUiViewLinearScaleElement::generateWidget(QWidget *parent)
{
    mSlider = new QSlider(parent);
    mActionElementWidget = new ActionElementWidget(mSlider, actionId(), parent);
    mSlider->setRange(mMinValue, mMaxValue);
    mSlider->setValue(mInitialValue);
    return mActionElementWidget;
}

bool AutoGenerateInteractionUiViewLinearScaleElement::operator==(const AutoGenerateInteractionUiViewLinearScaleElement &other) const
{
    return AutoGenerateInteractionUiViewActionable::operator==(other) && other.maxValue() == maxValue() && other.minValue() == minValue()
        && other.initialValue() == initialValue();
}

void AutoGenerateInteractionUiViewLinearScaleElement::parseElement(const QJsonObject &json)
{
    mMinValue = json["minValue"_L1].toInt(-1);
    mMaxValue = json["maxValue"_L1].toInt(-1);
    mInitialValue = json["initialValue"_L1].toInt(-1);
}

int AutoGenerateInteractionUiViewLinearScaleElement::initialValue() const
{
    return mInitialValue;
}

void AutoGenerateInteractionUiViewLinearScaleElement::setInitialValue(int newInitialValue)
{
    mInitialValue = newInitialValue;
}

QVariant AutoGenerateInteractionUiViewLinearScaleElement::currentValue() const
{
    return QString::number(mSlider->value());
}

void AutoGenerateInteractionUiViewLinearScaleElement::setCurrentValue(const QVariant &v)
{
    mSlider->setValue(v.toString().toInt());
}

int AutoGenerateInteractionUiViewLinearScaleElement::maxValue() const
{
    return mMaxValue;
}

void AutoGenerateInteractionUiViewLinearScaleElement::setMaxValue(int newMaxValue)
{
    mMaxValue = newMaxValue;
}

int AutoGenerateInteractionUiViewLinearScaleElement::minValue() const
{
    return mMinValue;
}

void AutoGenerateInteractionUiViewLinearScaleElement::setMinValue(int newMinValue)
{
    mMinValue = newMinValue;
}

void AutoGenerateInteractionUiViewLinearScaleElement::serializeElement(QJsonObject &o) const
{
    if (mMinValue != -1) {
        o["minValue"_L1] = mMinValue;
    }
    if (mInitialValue != -1) {
        o["initialValue"_L1] = mInitialValue;
    }
    if (mMaxValue != -1) {
        o["maxValue"_L1] = mMaxValue;
    }
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewLinearScaleElement &t)
{
    d.space() << "AutoGenerateInteractionUiViewActionable:" << static_cast<const AutoGenerateInteractionUiViewActionable &>(t);
    d.space() << "minValue:" << t.minValue();
    d.space() << "maxValue:" << t.maxValue();
    d.space() << "initialValue:" << t.initialValue();
    return d;
}

#include "moc_autogenerateinteractionuiviewlinearscaleelement.cpp"
