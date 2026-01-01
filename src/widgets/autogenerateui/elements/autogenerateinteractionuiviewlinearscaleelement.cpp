/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "autogenerateinteractionuiviewlinearscaleelement.h"
#include "autogenerateui/widgets/actionelementwidget.h"
#include <QJsonObject>
#include <QSpinBox>

using namespace Qt::Literals::StringLiterals;
AutoGenerateInteractionUiViewLinearScaleElement::AutoGenerateInteractionUiViewLinearScaleElement(QObject *parent)
    : AutoGenerateInteractionUiViewActionable(parent)
{
}

AutoGenerateInteractionUiViewLinearScaleElement::~AutoGenerateInteractionUiViewLinearScaleElement() = default;

ActionElementWidget *AutoGenerateInteractionUiViewLinearScaleElement::generateWidget(QWidget *parent)
{
    mSpinbox = new QSpinBox(parent);
    mActionElementWidget = new ActionElementWidget(mSpinbox, actionId(), parent);
    mSpinbox->setRange(mMinValue, mMaxValue);
    mSpinbox->setValue(mInitialValue);
    mSpinbox->setPrefix(mPreLabel);
    mSpinbox->setSuffix(mPostLabel);
    return mActionElementWidget;
}

bool AutoGenerateInteractionUiViewLinearScaleElement::operator==(const AutoGenerateInteractionUiViewLinearScaleElement &other) const
{
    return AutoGenerateInteractionUiViewActionable::operator==(other) && other.maxValue() == maxValue() && other.minValue() == minValue()
        && other.initialValue() == initialValue() && other.postLabel() == postLabel() && other.preLabel() == preLabel();
}

void AutoGenerateInteractionUiViewLinearScaleElement::parseElement(const QJsonObject &json)
{
    mMinValue = json["minValue"_L1].toInt(-1);
    mMaxValue = json["maxValue"_L1].toInt(-1);
    mInitialValue = json["initialValue"_L1].toInt(-1);
    mPreLabel = json["preLabel"_L1].toString();
    mPostLabel = json["postLabel"_L1].toString();
}

QString AutoGenerateInteractionUiViewLinearScaleElement::postLabel() const
{
    return mPostLabel;
}

void AutoGenerateInteractionUiViewLinearScaleElement::setPostLabel(const QString &newPostLabel)
{
    mPostLabel = newPostLabel;
}

QString AutoGenerateInteractionUiViewLinearScaleElement::preLabel() const
{
    return mPreLabel;
}

void AutoGenerateInteractionUiViewLinearScaleElement::setPreLabel(const QString &newPreLabel)
{
    mPreLabel = newPreLabel;
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
    return QString::number(mSpinbox->value());
}

void AutoGenerateInteractionUiViewLinearScaleElement::setCurrentValue(const QVariant &v)
{
    mSpinbox->setValue(v.toString().toInt());
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
    if (!mPreLabel.isEmpty()) {
        o["preLabel"_L1] = mPreLabel;
    }
    if (!mPostLabel.isEmpty()) {
        o["postLabel"_L1] = mPostLabel;
    }
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewLinearScaleElement &t)
{
    d.space() << "AutoGenerateInteractionUiViewActionable:" << static_cast<const AutoGenerateInteractionUiViewActionable &>(t);
    d.space() << "minValue:" << t.minValue();
    d.space() << "maxValue:" << t.maxValue();
    d.space() << "initialValue:" << t.initialValue();
    d.space() << "postLabel:" << t.postLabel();
    d.space() << "preLabel:" << t.preLabel();
    return d;
}

#include "moc_autogenerateinteractionuiviewlinearscaleelement.cpp"
