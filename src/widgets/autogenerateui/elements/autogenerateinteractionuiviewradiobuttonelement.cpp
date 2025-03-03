/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "autogenerateinteractionuiviewradiobuttonelement.h"
#include "ruqola_action_buttons_debug.h"
#include <QJsonArray>

using namespace Qt::Literals::StringLiterals;

AutoGenerateInteractionUiViewRadioButtonElement::AutoGenerateInteractionUiViewRadioButtonElement()
    : AutoGenerateInteractionUiViewActionable()
{
}

AutoGenerateInteractionUiViewRadioButtonElement::~AutoGenerateInteractionUiViewRadioButtonElement()
{
    qDeleteAll(mOptions);
}

void AutoGenerateInteractionUiViewRadioButtonElement::parseElement(const QJsonObject &json)
{
    // Options
    const QJsonArray optionsArray = json["options"_L1].toArray();
    for (const auto &opt : optionsArray) {
        AutoGenerateInteractionUiViewOption *option = new AutoGenerateInteractionUiViewOption;
        option->parse(opt.toObject());
        mOptions.append(option);
    }
}

QWidget *AutoGenerateInteractionUiViewRadioButtonElement::generateWidget(RocketChatAccount *account, QWidget *parent)
{
    // QCheckBox ?
    Q_UNUSED(account)
    Q_UNUSED(parent)
    return nullptr;
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewRadioButtonElement &t)
{
    d.space() << "AutoGenerateInteractionUiViewActionable:" << static_cast<const AutoGenerateInteractionUiViewActionable &>(t);
    return d;
}

bool AutoGenerateInteractionUiViewRadioButtonElement::operator==(const AutoGenerateInteractionUiViewRadioButtonElement &other) const
{
    return AutoGenerateInteractionUiViewActionable::operator==(other);
}

void AutoGenerateInteractionUiViewRadioButtonElement::serializeElement(QJsonObject &o) const
{
    for (const auto &r : std::as_const(mOptions)) {
        o["options"_L1] = r->serialize();
    }
}

#include "moc_autogenerateinteractionuiviewradiobuttonelement.cpp"
