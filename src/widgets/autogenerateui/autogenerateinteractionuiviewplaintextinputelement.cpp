/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewplaintextinputelement.h"
#include "autogenerateui/autogenerateinteractionuiviewtext.h"

#include <QPlainTextEdit>
using namespace Qt::Literals::StringLiterals;
AutoGenerateInteractionUiViewPlainTextInputElement::AutoGenerateInteractionUiViewPlainTextInputElement()
    : AutoGenerateInteractionUiViewActionable()
{
}

AutoGenerateInteractionUiViewPlainTextInputElement::~AutoGenerateInteractionUiViewPlainTextInputElement()
{
    delete mPlaceHolder;
}

void AutoGenerateInteractionUiViewPlainTextInputElement::parse(const QJsonObject &json)
{
    AutoGenerateInteractionUiViewActionable::parse(json);
    if (json.contains("placeholder"_L1)) {
        mPlaceHolder = new AutoGenerateInteractionUiViewText;
        mPlaceHolder->parse(json["placeholder"_L1].toObject());
    }
    mInitialValue = json["initialValue"_L1].toString();
    mMultiLine = json["multiline"_L1].toBool();
    mMinLength = json["minLength"_L1].toInt(-1);
    mMaxLength = json["maxLength"_L1].toInt(-1);
}

QWidget *AutoGenerateInteractionUiViewPlainTextInputElement::generateWidget(QWidget *parent)
{
    auto plainText = new QPlainTextEdit(parent);
    if (mPlaceHolder) {
        plainText->setPlaceholderText(mPlaceHolder->generateText());
    }
    plainText->setPlainText(mInitialValue);
    if (!mMultiLine) {
        plainText->setMaximumBlockCount(1);
    }
    return plainText;
}

AutoGenerateInteractionUiViewText *AutoGenerateInteractionUiViewPlainTextInputElement::placeHolder() const
{
    return mPlaceHolder;
}

void AutoGenerateInteractionUiViewPlainTextInputElement::setPlaceHolder(AutoGenerateInteractionUiViewText *newPlaceHolder)
{
    mPlaceHolder = newPlaceHolder;
}

QString AutoGenerateInteractionUiViewPlainTextInputElement::initialValue() const
{
    return mInitialValue;
}

void AutoGenerateInteractionUiViewPlainTextInputElement::setInitialValue(const QString &newInitialValue)
{
    mInitialValue = newInitialValue;
}

bool AutoGenerateInteractionUiViewPlainTextInputElement::multiLine() const
{
    return mMultiLine;
}

void AutoGenerateInteractionUiViewPlainTextInputElement::setMultiLine(bool newMultiLine)
{
    mMultiLine = newMultiLine;
}

bool AutoGenerateInteractionUiViewPlainTextInputElement::operator==(const AutoGenerateInteractionUiViewPlainTextInputElement &other) const
{
    return AutoGenerateInteractionUiViewActionable::operator==(other) && other.multiLine() == multiLine() && other.placeHolder() == placeHolder()
        && other.initialValue() == initialValue() && other.minLength() == minLength() && other.maxLength() == maxLength();
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewPlainTextInputElement &t)
{
    if (t.placeHolder()) {
        d.space() << "placeHolder:" << *t.placeHolder();
    }
    d.space() << "initialValue:" << t.initialValue();
    d.space() << "multiLine:" << t.multiLine();
    d.space() << "minLength:" << t.minLength();
    d.space() << "maxLength:" << t.maxLength();
    return d;
}

int AutoGenerateInteractionUiViewPlainTextInputElement::minLength() const
{
    return mMinLength;
}

void AutoGenerateInteractionUiViewPlainTextInputElement::setMinLength(int newMinLength)
{
    mMinLength = newMinLength;
}

int AutoGenerateInteractionUiViewPlainTextInputElement::maxLength() const
{
    return mMaxLength;
}

void AutoGenerateInteractionUiViewPlainTextInputElement::setMaxLength(int newMaxLength)
{
    mMaxLength = newMaxLength;
}
