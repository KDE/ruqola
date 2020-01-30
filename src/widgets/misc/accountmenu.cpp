/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "accountmenu.h"
#include "ruqola.h"
#include "rocketchataccount.h"
#include "model/rocketchataccountmodel.h"
#include "accountmanager.h"
#include <QMenu>
#include <KLocalizedString>

AccountMenu::AccountMenu(QWidget *parent)
    : KActionMenu(parent)
{
    setText(i18n("Account"));
    connect(menu(), &QMenu::aboutToShow, this, &AccountMenu::slotUpdateAccountMenu);
}

AccountMenu::~AccountMenu()
{
}

void AccountMenu::slotUpdateAccountMenu()
{
    menu()->clear();
    RocketChatAccountModel *model = Ruqola::self()->accountManager()->rocketChatAccountModel();
    QActionGroup *group = new QActionGroup(this);
    group->setExclusive(true);
    for (int i = 0; i < model->accountNumber(); ++i) {
        RocketChatAccount *account = model->account(i);
        const QString accountName = account->accountName();
        QAction *action = new QAction(accountName, this);
        action->setCheckable(true);
        group->addAction(action);
        if (Ruqola::self()->accountManager()->currentAccount() == accountName) {
            action->setChecked(true);
        }
        menu()->addAction(action);
        connect(action, &QAction::triggered, this, [this, accountName](){
            Ruqola::self()->accountManager()->setCurrentAccount(accountName);
        });
    }
}
