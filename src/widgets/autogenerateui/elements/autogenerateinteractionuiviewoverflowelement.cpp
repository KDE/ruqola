/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewoverflowelement.h"
#include "autogenerateui/autogenerateinteractionuiviewoption.h"

#include <QJsonArray>
using namespace Qt::Literals::StringLiterals;
AutoGenerateInteractionUiViewOverflowElement::AutoGenerateInteractionUiViewOverflowElement()
    : AutoGenerateInteractionUiViewActionable()
{
}

AutoGenerateInteractionUiViewOverflowElement::~AutoGenerateInteractionUiViewOverflowElement()
{
}

void AutoGenerateInteractionUiViewOverflowElement::parseElement(const QJsonObject &json)
{
    // Options
    const QJsonArray optionsArray = json["options"_L1].toArray();
    for (const auto &opt : optionsArray) {
        AutoGenerateInteractionUiViewOption *option = new AutoGenerateInteractionUiViewOption;
        option->parse(opt.toObject());
        mOptions.append(option);
    }
}

QWidget *AutoGenerateInteractionUiViewOverflowElement::generateWidget(QWidget *parent)
{
    // TODO
    return nullptr;
}

QList<AutoGenerateInteractionUiViewOption *> AutoGenerateInteractionUiViewOverflowElement::options() const
{
    return mOptions;
}

void AutoGenerateInteractionUiViewOverflowElement::setOptions(const QList<AutoGenerateInteractionUiViewOption *> &newOptions)
{
    mOptions = newOptions;
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewOverflowElement &t)
{
    d.space() << "AutoGenerateInteractionUiViewActionable:" << static_cast<const AutoGenerateInteractionUiViewActionable &>(t);
    d.space() << "options:" << t.options();
    return d;
}

bool AutoGenerateInteractionUiViewOverflowElement::operator==(const AutoGenerateInteractionUiViewOverflowElement &other) const
{
    return other.options() == options() && AutoGenerateInteractionUiViewActionable::operator==(other);
}

void AutoGenerateInteractionUiViewOverflowElement::serializeElement(QJsonObject &o) const
{
    for (const auto &r : std::as_const(mOptions)) {
        o["options"_L1] = r->serialize();
    }
}
