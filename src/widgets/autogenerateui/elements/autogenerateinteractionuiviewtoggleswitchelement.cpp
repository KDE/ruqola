/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "autogenerateinteractionuiviewtoggleswitchelement.h"
#include "ruqola_action_buttons_debug.h"
using namespace Qt::Literals::StringLiterals;

AutoGenerateInteractionUiViewToggleSwitchElement::AutoGenerateInteractionUiViewToggleSwitchElement() = default;
AutoGenerateInteractionUiViewToggleSwitchElement::~AutoGenerateInteractionUiViewToggleSwitchElement() = default;

void AutoGenerateInteractionUiViewToggleSwitchElement::parse(const QJsonObject &json)
{
    AutoGenerateInteractionUiViewActionable::parse(json);
}

QList<AutoGenerateInteractionUiViewOptionElement> AutoGenerateInteractionUiViewToggleSwitchElement::options() const
{
    return mOptions;
}

void AutoGenerateInteractionUiViewToggleSwitchElement::setOptions(const QList<AutoGenerateInteractionUiViewOptionElement> &newOptions)
{
    mOptions = newOptions;
}

QWidget *AutoGenerateInteractionUiViewToggleSwitchElement::generateWidget(QWidget *parent)
{
    return nullptr;
    // TODO
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewToggleSwitchElement &t)
{
    // TODO use qDebug with AutoGenerateInteractionUiViewActionable
    return d;
}

#include "moc_autogenerateinteractionuiviewtoggleswitchelement.cpp"
