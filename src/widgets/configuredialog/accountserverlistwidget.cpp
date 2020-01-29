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

#include "accountserverlistwidget.h"
#include "dialogs/createnewaccountdialog.h"
#include <KLocalizedString>

#include <QListWidgetItem>
#include <QPointer>

AccountServerListWidget::AccountServerListWidget(QWidget *parent)
    : QListWidget(parent)
{
    connect(this, &AccountServerListWidget::itemDoubleClicked, this, &AccountServerListWidget::modifyAccountConfig);
}

AccountServerListWidget::~AccountServerListWidget()
{
}

void AccountServerListWidget::load()
{
}

void AccountServerListWidget::save()
{
}

void AccountServerListWidget::modifyAccountConfig()
{
    QListWidgetItem *item = currentItem();
    if (!item) {
        return;
    }

    //AccountServerListWidgetItem *serverSieveListItem = static_cast<AccountServerListWidgetItem *>(item);
    //TODO change title
    QPointer<CreateNewAccountDialog> dlg = new CreateNewAccountDialog(this);
    if (dlg->exec()) {
        //TODO add account
    }
}

void AccountServerListWidget::deleteAccountConfig(QListWidgetItem *item)
{
    //TODO
}

void AccountServerListWidget::addAccountConfig()
{
    QPointer<CreateNewAccountDialog> dlg = new CreateNewAccountDialog(this);
    if (dlg->exec()) {
        //TODO add account
    }
}

AccountServerListWidgetItem::AccountServerListWidgetItem(QListWidget *parent)
    : QListWidgetItem(parent)
{
}

AccountServerListWidgetItem::~AccountServerListWidgetItem()
{
}

