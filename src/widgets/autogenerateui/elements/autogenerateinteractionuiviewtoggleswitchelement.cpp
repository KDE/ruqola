/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "autogenerateinteractionuiviewtoggleswitchelement.h"
#include "ruqola_action_buttons_debug.h"
#include "ruqola_autogenerateui_debug.h"
#include <QJsonArray>
using namespace Qt::Literals::StringLiterals;

AutoGenerateInteractionUiViewToggleSwitchElement::AutoGenerateInteractionUiViewToggleSwitchElement(QObject *parent)
    : AutoGenerateInteractionUiViewActionable(parent)
{
}

AutoGenerateInteractionUiViewToggleSwitchElement::~AutoGenerateInteractionUiViewToggleSwitchElement()
{
    qDeleteAll(mOptions);
}

void AutoGenerateInteractionUiViewToggleSwitchElement::parseElement(const QJsonObject &json)
{
    // Options
    const QJsonArray optionsArray = json["options"_L1].toArray();
    for (const auto &opt : optionsArray) {
        AutoGenerateInteractionUiViewOption *option = new AutoGenerateInteractionUiViewOption;
        option->parse(opt.toObject());
        mOptions.append(option);
    }
}

QList<AutoGenerateInteractionUiViewOption *> AutoGenerateInteractionUiViewToggleSwitchElement::options() const
{
    return mOptions;
}

void AutoGenerateInteractionUiViewToggleSwitchElement::setOptions(const QList<AutoGenerateInteractionUiViewOption *> &newOptions)
{
    mOptions = newOptions;
}

void AutoGenerateInteractionUiViewToggleSwitchElement::serializeElement(QJsonObject &o) const
{
    QJsonArray options;
    for (const auto &r : std::as_const(mOptions)) {
        options.append(r->serialize());
    }
    o["options"_L1] = options;
}

QWidget *AutoGenerateInteractionUiViewToggleSwitchElement::generateWidget(QWidget *parent)
{
    Q_UNUSED(parent)
    return nullptr;
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
