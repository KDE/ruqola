/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewoverflowelement.h"
#include "autogenerateui/autogenerateinteractionuiviewoption.h"
#include "autogenerateui/widgets/actionelementwidget.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QMenu>
#include <QToolButton>
using namespace Qt::Literals::StringLiterals;
AutoGenerateInteractionUiViewOverflowElement::AutoGenerateInteractionUiViewOverflowElement(QObject *parent)
    : AutoGenerateInteractionUiViewActionable(parent)
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

ActionElementWidget *AutoGenerateInteractionUiViewOverflowElement::generateWidget(QWidget *parent)
{
    auto toolButton = new QToolButton(parent);
    toolButton->setIcon(QIcon::fromTheme(u"menu_new"_s));
    mActionElementWidget = new ActionElementWidget(toolButton, actionId(), parent);
    toolButton->setPopupMode(QToolButton::InstantPopup);
    auto menu = new QMenu(parent);
    for (const auto &r : std::as_const(mOptions)) {
        auto act = new QAction(menu);
        act->setText(r->text().generateText());
        act->setData(r->value());
        connect(act, &QAction::triggered, this, [this, r]() {
            // qDebug() << "act mBlockId " << mBlockId << " r->value() " << r->value() << " mActionId " << mActionId;
            Q_EMIT actionChanged(mActionId, r->value());
        });
        menu->addAction(act);
    }
    toolButton->setMenu(menu);
    return mActionElementWidget;
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

#include "moc_autogenerateinteractionuiviewoverflowelement.cpp"
