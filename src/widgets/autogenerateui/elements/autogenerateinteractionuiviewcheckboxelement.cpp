/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "autogenerateinteractionuiviewcheckboxelement.h"
#include "ruqola_action_buttons_debug.h"
using namespace Qt::Literals::StringLiterals;

AutoGenerateInteractionUiViewCheckboxElement::AutoGenerateInteractionUiViewCheckboxElement()
    : AutoGenerateInteractionUiViewActionable()
{
}

AutoGenerateInteractionUiViewCheckboxElement::~AutoGenerateInteractionUiViewCheckboxElement() = default;

void AutoGenerateInteractionUiViewCheckboxElement::parse(const QJsonObject &json)
{
    AutoGenerateInteractionUiViewActionable::parse(json);
}

QWidget *AutoGenerateInteractionUiViewCheckboxElement::generateWidget(QWidget *parent)
{
    // TODO
    return nullptr;
}

bool AutoGenerateInteractionUiViewCheckboxElement::operator==(const AutoGenerateInteractionUiViewCheckboxElement &other) const
{
    return other.options() == options() && AutoGenerateInteractionUiViewActionable::operator==(other);
}

QList<AutoGenerateInteractionUiViewOption *> AutoGenerateInteractionUiViewCheckboxElement::options() const
{
    return mOptions;
}

void AutoGenerateInteractionUiViewCheckboxElement::setOptions(const QList<AutoGenerateInteractionUiViewOption *> &newOptions)
{
    mOptions = newOptions;
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewCheckboxElement &t)
{
    d.space() << "AutoGenerateInteractionUiViewActionable:" << static_cast<AutoGenerateInteractionUiViewActionable>(t);
    d.space() << "options:" << t.options();
    return d;
}

#include "moc_autogenerateinteractionuiviewcheckboxelement.cpp"
