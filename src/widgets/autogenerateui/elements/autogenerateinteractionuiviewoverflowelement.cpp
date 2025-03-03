/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewoverflowelement.h"
#include "autogenerateui/autogenerateinteractionuiviewoption.h"

#include <QJsonArray>
#include <QMenu>
#include <QToolButton>
using namespace Qt::Literals::StringLiterals;
AutoGenerateInteractionUiViewOverflowElement::AutoGenerateInteractionUiViewOverflowElement()
    : AutoGenerateInteractionUiViewActionable()
{
}

AutoGenerateInteractionUiViewOverflowElement::~AutoGenerateInteractionUiViewOverflowElement()
{
    qDeleteAll(mOptions);
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

QWidget *AutoGenerateInteractionUiViewOverflowElement::generateWidget(RocketChatAccount *account, QWidget *parent)
{
    Q_UNUSED(account)
    auto toolButton = new QToolButton(parent);
    toolButton->setPopupMode(QToolButton::InstantPopup);
    auto menu = new QMenu(parent);
    for (const auto &r : std::as_const(mOptions)) {
        auto act = new QAction(menu);
        act->setText(r->text().generateText());
        act->setData(r->value());
        menu->addAction(act);
    }
    toolButton->setMenu(menu);
    return toolButton;
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
    QJsonArray options;
    for (const auto &r : std::as_const(mOptions)) {
        options.append(r->serialize());
    }
    o["options"_L1] = options;
}
