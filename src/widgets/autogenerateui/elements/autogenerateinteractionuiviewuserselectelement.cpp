/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewuserselectelement.h"
#include "autogenerateui/autogenerateinteractionuiviewtext.h"
#include "autogenerateui/widgets/actionelementwidget.h"

#include <QJsonObject>
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

ActionElementWidget *AutoGenerateInteractionUiViewUserSelectElement::generateWidget(QWidget *parent)
{
    // Verify ?
    mPlainTextEdit = new QPlainTextEdit(parent);
    mActionElementWidget = new ActionElementWidget(mPlainTextEdit, actionId(), parent);

    if (mPlaceHolder) {
        mPlainTextEdit->setPlaceholderText(mPlaceHolder->generateText());
    }
    return mActionElementWidget;
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

QVariant AutoGenerateInteractionUiViewUserSelectElement::currentValue() const
{
    return mPlainTextEdit->toPlainText();
}

void AutoGenerateInteractionUiViewUserSelectElement::setCurrentValue(const QVariant &v)
{
    mPlainTextEdit->setPlainText(v.toString());
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
