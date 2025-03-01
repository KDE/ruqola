/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "autogenerateinteractionuiviewcheckboxelement.h"
#include "ruqola_action_buttons_debug.h"

#include <QJsonArray>

using namespace Qt::Literals::StringLiterals;

AutoGenerateInteractionUiViewCheckboxElement::AutoGenerateInteractionUiViewCheckboxElement()
    : AutoGenerateInteractionUiViewActionable()
{
}

AutoGenerateInteractionUiViewCheckboxElement::~AutoGenerateInteractionUiViewCheckboxElement()
{
    qDeleteAll(mOptions);
}

void AutoGenerateInteractionUiViewCheckboxElement::parse(const QJsonObject &json)
{
    AutoGenerateInteractionUiViewActionable::parse(json);
    // Options
    const QJsonArray optionsArray = json["options"_L1].toArray();
    for (const auto &opt : optionsArray) {
        AutoGenerateInteractionUiViewOption *option = new AutoGenerateInteractionUiViewOption;
        option->parse(opt.toObject());
        mOptions.append(option);
    }
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

void AutoGenerateInteractionUiViewCheckboxElement::serializeElement(QJsonObject &o) const
{
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewCheckboxElement &t)
{
    d.space() << "AutoGenerateInteractionUiViewActionable:" << static_cast<const AutoGenerateInteractionUiViewActionable &>(t);
    d.space() << "options:" << t.options();
    return d;
}

#include "moc_autogenerateinteractionuiviewcheckboxelement.cpp"
