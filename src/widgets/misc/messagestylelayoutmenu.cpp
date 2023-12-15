/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagestylelayoutmenu.h"
#include <KLocalizedString>
#include <QActionGroup>
#include <QMenu>

MessageStyleLayoutMenu::MessageStyleLayoutMenu(QObject *parent)
    : KActionMenu{parent}
    , mActionGroup(new QActionGroup(this))
{
    setText(i18n("Message Style"));
    mActionGroup->setExclusive(true);

    addMessageStyleAction(i18nc("Message Style", "Normal"), RuqolaGlobalConfig::EnumMessageStyle::Normal);
    addMessageStyleAction(i18nc("Message Style", "Compact"), RuqolaGlobalConfig::EnumMessageStyle::Compact);
    // Disable for the moment as Cozy is not implemented
    // addMessageStyleAction(i18nc("Message Style", "Cozy"), RuqolaGlobalConfig::EnumMessageStyle::Cozy);
}

MessageStyleLayoutMenu::~MessageStyleLayoutMenu() = default;

void MessageStyleLayoutMenu::addMessageStyleAction(const QString &name, RuqolaGlobalConfig::EnumMessageStyle::type status)
{
    auto action = new QAction(name, this);
    action->setData(status);
    action->setChecked(RuqolaGlobalConfig::self()->messageStyle() == status);
    mActionGroup->addAction(action);
    menu()->addAction(action);
    action->setCheckable(true);
    connect(action, &QAction::triggered, this, [this, status]() {
        RuqolaGlobalConfig::self()->setMessageStyle(status);
        RuqolaGlobalConfig::self()->save();
        Q_EMIT styleChanged();
    });
}

#include "moc_messagestylelayoutmenu.cpp"
