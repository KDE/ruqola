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

QList<AutoGenerateInteractionUiViewOption> AutoGenerateInteractionUiViewToggleSwitchElement::options() const
{
    return mOptions;
}

void AutoGenerateInteractionUiViewToggleSwitchElement::setOptions(const QList<AutoGenerateInteractionUiViewOption> &newOptions)
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
    d.space() << "AutoGenerateInteractionUiViewActionable:" << static_cast<AutoGenerateInteractionUiViewActionable>(t);
    d.space() << "options:" << t.options();
    return d;
}

#include "moc_autogenerateinteractionuiviewtoggleswitchelement.cpp"
