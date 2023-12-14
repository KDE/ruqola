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

    auto action = new QAction(i18nc("Message Style", "Normal"), this);
    mActionGroup->addAction(action);
    menu()->addAction(action);
    action->setCheckable(true);

    action = new QAction(i18nc("Message Style", "Compact"), this);
    mActionGroup->addAction(action);
    menu()->addAction(action);
    action->setCheckable(true);
}

MessageStyleLayoutMenu::~MessageStyleLayoutMenu() = default;

#include "moc_messagestylelayoutmenu.cpp"
