/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagestylelayoutmenu.h"
#include "ruqolaglobalconfig.h"
#include <KLocalizedString>
#include <QActionGroup>
#include <QMenu>

MessageStyleLayoutMenu::MessageStyleLayoutMenu(QObject *parent)
    : KActionMenu{parent}
    , mActionGroup(new QActionGroup(this))
{
    setText(i18n("Message Style"));
    mActionGroup->setExclusive(true);

    auto action = new QAction(i18nc("Message Style", "Normal"), this);
    action->setData(RuqolaGlobalConfig::EnumMessageStyle::Normal);
    mActionGroup->addAction(action);
    menu()->addAction(action);
    action->setCheckable(true);
    connect(action, &QAction::triggered, this, [this]() {
        RuqolaGlobalConfig::self()->setMessageStyle(RuqolaGlobalConfig::EnumMessageStyle::Normal);
        Q_EMIT styleChanged();
    });

    action = new QAction(i18nc("Message Style", "Compact"), this);
    action->setData(RuqolaGlobalConfig::EnumMessageStyle::Compact);
    connect(action, &QAction::triggered, this, [this]() {
        RuqolaGlobalConfig::self()->setMessageStyle(RuqolaGlobalConfig::EnumMessageStyle::Compact);
        Q_EMIT styleChanged();
    });
    mActionGroup->addAction(action);
    menu()->addAction(action);
    action->setCheckable(true);
}

MessageStyleLayoutMenu::~MessageStyleLayoutMenu() = default;

#include "moc_messagestylelayoutmenu.cpp"
