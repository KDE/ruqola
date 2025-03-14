/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewuserselectelement.h"
#include "autogenerateui/autogenerateinteractionuiviewtext.h"

#include <QLineEdit>
#include <QPlainTextEdit>
using namespace Qt::Literals::StringLiterals;
AutoGenerateInteractionUiViewUserSelectElement::AutoGenerateInteractionUiViewUserSelectElement(QObject *parent)
    : AutoGenerateInteractionUiViewActionable(parent)
{
}

AutoGenerateInteractionUiViewUserSelectElement::~AutoGenerateInteractionUiViewUserSelectElement()
{
    delete mPlaceHolder;
}

void AutoGenerateInteractionUiViewUserSelectElement::parseElement(const QJsonObject &json)
{
    if (json.contains("placeholder"_L1)) {
        mPlaceHolder = new AutoGenerateInteractionUiViewText;
        mPlaceHolder->parse(json["placeholder"_L1].toObject());
    }
}

QWidget *AutoGenerateInteractionUiViewUserSelectElement::generateWidget(QWidget *parent)
{
    // Verify ?
    mPlainTextEdit = new QPlainTextEdit(parent);
    if (mPlaceHolder) {
        mPlainTextEdit->setPlaceholderText(mPlaceHolder->generateText());
    }
    return mPlainTextEdit;
}

AutoGenerateInteractionUiViewText *AutoGenerateInteractionUiViewUserSelectElement::placeHolder() const
{
    return mPlaceHolder;
}

void AutoGenerateInteractionUiViewUserSelectElement::setPlaceHolder(AutoGenerateInteractionUiViewText *newPlaceHolder)
{
    mPlaceHolder = newPlaceHolder;
}

bool AutoGenerateInteractionUiViewUserSelectElement::operator==(const AutoGenerateInteractionUiViewUserSelectElement &other) const
{
    return AutoGenerateInteractionUiViewActionable::operator==(other) && other.placeHolder() == placeHolder();
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewUserSelectElement &t)
{
    d.space() << "AutoGenerateInteractionUiViewActionable:" << static_cast<const AutoGenerateInteractionUiViewActionable &>(t);
    if (t.placeHolder()) {
        d.space() << "placeHolder:" << *t.placeHolder();
    }
    return d;
}

void AutoGenerateInteractionUiViewUserSelectElement::serializeElement(QJsonObject &o) const
{
    if (mPlaceHolder) {
        o["placeholder"_L1] = mPlaceHolder->serialize();
    }
}

#include "moc_autogenerateinteractionuiviewuserselectelement.cpp"
