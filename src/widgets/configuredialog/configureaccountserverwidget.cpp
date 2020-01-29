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

#include "configureaccountserverwidget.h"
#include "ui_configureaccountserverwidget.h"

#include <KLocalizedString>
#include <KMessageBox>

ConfigureAccountServerWidget::ConfigureAccountServerWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ConfigureAccountServerWidget)
{
    ui->setupUi(this);
    connect(ui->modifyServer, &QPushButton::clicked, this, &ConfigureAccountServerWidget::slotModifyServer);
    connect(ui->addServer, &QPushButton::clicked, this, &ConfigureAccountServerWidget::slotAddServer);
    connect(ui->removeServer, &QPushButton::clicked, this, &ConfigureAccountServerWidget::slotDeleteServer);
    connect(ui->accountServerListwidget, &AccountServerListWidget::itemSelectionChanged, this, &ConfigureAccountServerWidget::slotItemSelectionChanged);
    slotItemSelectionChanged();
}

ConfigureAccountServerWidget::~ConfigureAccountServerWidget()
{
    delete ui;
}

void ConfigureAccountServerWidget::save()
{
    ui->accountServerListwidget->save();
}

void ConfigureAccountServerWidget::load()
{
    ui->accountServerListwidget->load();
}

void ConfigureAccountServerWidget::slotModifyServer()
{
    ui->accountServerListwidget->modifyAccountConfig();
}

void ConfigureAccountServerWidget::slotAddServer()
{
    ui->accountServerListwidget->addAccountConfig();
}

void ConfigureAccountServerWidget::slotDeleteServer()
{
    QListWidgetItem *item = ui->accountServerListwidget->currentItem();
    if (!item) {
        return;
    }
    if (KMessageBox::Yes == KMessageBox::questionYesNo(this, i18n("Do you want to remove this account \'%1\'?", item->text()),
                                                       i18nc("@title:window", "Remove Account"))) {
        ui->accountServerListwidget->deleteAccountConfig(item);
        delete item;
        slotItemSelectionChanged();
    }
}

void ConfigureAccountServerWidget::slotItemSelectionChanged()
{
    const bool hasItemSelected = ui->accountServerListwidget->currentItem();
    ui->modifyServer->setEnabled(hasItemSelected);
    ui->removeServer->setEnabled(hasItemSelected);
}
