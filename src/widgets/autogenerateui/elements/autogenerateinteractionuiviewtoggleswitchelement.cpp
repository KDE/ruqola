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

void AutoGenerateInteractionUiViewToggleSwitchElement::serializeElement(QJsonObject &o) const
{
}

QWidget *AutoGenerateInteractionUiViewToggleSwitchElement::generateWidget(QWidget *parent)
{
    return nullptr;
    // TODO
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewToggleSwitchElement &t)
{
    d.space() << "AutoGenerateInteractionUiViewActionable:" << static_cast<const AutoGenerateInteractionUiViewActionable &>(t);
    d.space() << "options:" << t.options();
    return d;
}

bool AutoGenerateInteractionUiViewToggleSwitchElement::operator==(const AutoGenerateInteractionUiViewToggleSwitchElement &other) const
{
    return AutoGenerateInteractionUiViewActionable::operator==(other) && other.options() == options();
}

#include "moc_autogenerateinteractionuiviewtoggleswitchelement.cpp"
