/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
    if (KMessageBox::Yes
        == KMessageBox::questionYesNo(this,
                                      i18n("Do you want to remove this account \'%1\'?", item->text()),
                                      i18nc("@title:window", "Remove Account"),
                                      KStandardGuiItem::remove(),
                                      KStandardGuiItem::cancel())) {
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
